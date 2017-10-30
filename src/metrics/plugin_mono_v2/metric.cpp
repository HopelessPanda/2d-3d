#include "metric.h"
#include "deep_analysis.h"
#include <iostream>
#include <fstream>
char *plugin_metric_names[] = { "mono_difference", "mono_position", "mono_scale", "mono_confidence",
                                      "mono_brightness", "mono_flat", "object_coef", "del_square", "coef_conf",
                                      "mono_me" };
char *plugin_visualization_names[] = { "mono" };
CMetricProperties plugin_properties = {
    0,			// m_src_backward_num
    0,			// m_src_fwd_num
    10,			// m_metric_num
    0,			// m_vis_num
    0,			// m_avg_vis_num

    // m_name
    "mono to stereo comparison",

    // m_interface_name
    "mono",

    0,			// m_num_generated_data
    false,		// m_using_gpu
    false,		// m_is_configurable
    false,		// m_reusable 
    32,			// m_width_mul
    32,			// m_height_mul                                                          

    // m_metric_units_name
    "MONO"
};

//constructor
CMonoPlugin::CMonoPlugin() : CCustomMetric(plugin_properties, (char**)plugin_metric_names, (char**)plugin_visualization_names) {
}

//this function used to convert found params into perspective transform matrix
cv::Mat CMonoPlugin::getTransformMatrix(aTransform geo, aTransform pos, bool invert)
{
    cv::Mat tozero(3, 3, CV_64F, cv::Scalar(0));
    cv::Mat fromzero(3, 3, CV_64F, cv::Scalar(0));
    cv::Mat transf(3, 3, CV_64F, cv::Scalar(0));
    transf.at<double>(0, 0) = pos.b;
    transf.at<double>(0, 1) = -geo.a;
    transf.at<double>(0, 2) = pos.c;
    transf.at<double>(1, 0) = geo.a;
    transf.at<double>(1, 1) = geo.b;
    transf.at<double>(1, 2) = geo.c;
    transf.at<double>(2, 2) = 1;

    tozero.at<double>(0, 0) = 1;				fromzero.at<double>(0, 0) = 1;
    tozero.at<double>(0, 1) = 0;				fromzero.at<double>(0, 1) = 0;
    tozero.at<double>(0, 2) = -m_width / 2;		fromzero.at<double>(0, 2) = m_width / 2;
    tozero.at<double>(1, 2) = -m_height / 2;	fromzero.at<double>(1, 2) = m_height / 2;
    tozero.at<double>(1, 1) = 1;				fromzero.at<double>(1, 1) = 1;
    tozero.at<double>(1, 0) = 0;				fromzero.at<double>(1, 0) = 0;
    tozero.at<double>(2, 2) = 1;				fromzero.at<double>(2, 2) = 1;

    cv::Mat f_tr = transf;
    if (invert)
        cv::invert(transf, f_tr);
    f_tr = fromzero * f_tr * tozero;
    return f_tr;
}

void CMonoPlugin::InitMetric()
{
    //the original frame is horizontal stack, so divide by 2
    m_width = m_w / 2;
    m_height = m_h;
    //samplehost gives planed RGB images
    m_Input.Init(m_w, m_h, RGB_P);
    m_Left.Init(m_width, m_height, RGB_P);
    //same init params, just copy it
    m_Right = m_Left;
    m_Mono = m_Left;
    t_Mono = m_Mono;
    //we analyse here both stereo and mono
    m_analyser.Init(m_width, m_height);
    m_analyser.InitStereo(&m_Left, &m_Right);
    m_analyser.InitMono(&t_Mono, true);
    //init data arrays
    d_left.frame = &m_Left;
    d_left.inst = m_analyser.GetME(0);
    d_left.field = FIRST;
    d_right.frame = &m_Right;
    d_right.inst = m_analyser.GetME(0);
    d_right.field = SECOND;
    //zero transform matrix
    transform = getTransformMatrix(m_analyser.GetResults(G_LM), m_analyser.GetResults(P_LM), true);
    seg_engine.Init(m_width, m_height);
}

//used to reload input frames
void CMonoPlugin::UpdateSource(IDataServer* data)
{
    if (ME_app) {
        ExtractRightView(data, m_Input.GetData());
        prev_Mono = m_Mono;
        m_Mono = m_Input;
        m_Mono.Crop(0, 0, m_w - m_width, 0);
        m_Mono_BGR = m_Mono;
#pragma omp parallel sections
        {
#pragma omp section
            m_Mono.ConvertToType(YUV_P);
#pragma omp section
            m_Mono_BGR.ConvertToType(BGR_I);
        }
    }

    ExtractLeftView(data, m_Input.GetData());
    //left view contains stacked left and right stereo views
    m_Left = m_Input;
    m_Left.Crop(0, 0, m_w - m_width, 0);
    m_Right = m_Input;
    m_Right.Crop(m_w - m_width, 0, 0, 0);
    ExtractRightView(data, m_Input.GetData());
    //right view has mono image only
    prev_Mono = m_Mono;
    m_Mono = m_Input;
    m_Mono.Crop(0, 0, m_w - m_width, 0);
    m_Mono_BGR = m_Mono;
    //convert color types, ME needs YUV_P type, and OpenCV uses BGR_I
#pragma omp parallel sections
    {
#pragma omp section
        m_Left.ConvertToType(YUV_P);
#pragma omp section
        m_Right.ConvertToType(YUV_P);
#pragma omp section
        m_Mono.ConvertToType(YUV_P);
#pragma omp section
        m_Mono_BGR.ConvertToType(BGR_I);
    }
}

//there were no any mallocs
void CMonoPlugin::Deinit()
{
}

//but we still have to re-write the original descructor
CMonoPlugin::~CMonoPlugin() {
    if (this->IsInited())
        Deinit();
}
// START
extern "C" __declspec(dllexport) void CreateMetric(IMetric** metric) {
    *metric = new CMonoPlugin;
}

extern "C" __declspec(dllexport) void ReleaseMetric(IMetric** metric) {

    delete *metric;
}

//resize and shift the mono frame
bool CMonoPlugin::TransformMono(int isRefine) //0 = get new params 1 = refine new params 2 = old params only
{
    if (isRefine == 1)
        transform *= getTransformMatrix(m_analyser.GetResults(G_LM), m_analyser.GetResults(P_LM), true);
    else if (isRefine == 0)
        transform = getTransformMatrix(m_analyser.GetResults(G_LM), m_analyser.GetResults(P_LM), true);
    m_z = 1 - transform.at<double>(1, 1);
    double v_x = transform.at<double>(1, 2);
    double v_a = transform.at<double>(1, 0);
    double h_z = 1 - transform.at<double>(0, 0);
    double h_x = transform.at<double>(0, 2);
    double h_a = transform.at<double>(0, 1);
    t_Mono = m_Mono_BGR;
    //too small difference -> leave it as is
    if (abs(m_z) + abs(h_z) > 0.001 || abs(h_a) + abs(v_a) > 0.001 || abs(h_x) + abs(v_x) > 0.2)
    {
        t_Mono.Transform(transform);
        return true; //true = we changed it
    }
    else
        return false; //there was no transform
}

//really. it's not used by our system anymore
int CMonoPlugin::CompareFramesLt(IResultsServer* results, int framenum1, int framenum2, int vis_id) {
    return framenum1;
}

//bad confidence, sending blank results
void CMonoPlugin::BlankResult(int framenum, IResultsServer* results_server)
{
    cv::Mat trans;
    trans = getTransformMatrix(m_analyser.GetResults(G_LM), m_analyser.GetResults(P_LM), true);
    double zy = trans.at<double>(1, 1) - 1;
    results_server->SaveResult(metric_ids[0], framenum, -1.0f);
    results_server->SaveResult(metric_ids[1], framenum, -1.0f);
    results_server->SaveResult(metric_ids[2], framenum, float(zy));
    results_server->SaveResult(metric_ids[3], framenum, float(m_confidence));
    results_server->SaveResult(metric_ids[4], framenum, 0.0f);
    results_server->SaveResult(metric_ids[5], framenum, -1.0f);
    results_server->SaveResult(metric_ids[6], framenum, 99999);
	results_server->SaveResult(metric_ids[7], framenum, -1.0f);
	results_server->SaveResult(metric_ids[8], framenum, -1.0f);
	results_server->SaveResult(metric_ids[9], framenum, -1.0f);
}

//save true results
void CMonoPlugin::PostResult(int framenum, IResultsServer* results_server)
{
    results_server->SaveResult(metric_ids[0], framenum, float(m_difference));
    results_server->SaveResult(metric_ids[1], framenum, float(m_position));
    results_server->SaveResult(metric_ids[2], framenum, float(m_z));
    results_server->SaveResult(metric_ids[3], framenum, float(m_confidence));
    results_server->SaveResult(metric_ids[4], framenum, float(m_brightness));
    results_server->SaveResult(metric_ids[5], framenum, float(m_flat));
    results_server->SaveResult(metric_ids[6], framenum, float(obj_coefficient));
    results_server->SaveResult(metric_ids[7], framenum, float(del_square));
    results_server->SaveResult(metric_ids[8], framenum, float(coef_conf));
    results_server->SaveResult(metric_ids[9], framenum, float(me_prev));
}

//check confidence.. too low -> don't waste our time, skip further measure
bool CMonoPlugin::MeasureConf()
{
    double total_conf = m_analyser.GetTotal(G_LM);
    double geom_conf = m_analyser.GetFit(G_LM);
    m_confidence = geom_conf*total_conf * 255;
    if (m_confidence < TR_C)
        return false;
    else
        return true;
}

//if there's any zoom even after pre-transform (or bad conf) -> re-analyse frame
bool CMonoPlugin::Check()
{
    double zoom = abs(m_analyser.GetResults(G_LM).b - 1) + abs(m_analyser.GetResults(P_LM).b - 1);
    if (!MeasureConf() || zoom > 0.01)
        return false;
    return true;
}

//analyse the position by difference analysis
void CMonoPlugin::MultiPosAnalysis()
{
    //ensure everything is YUV_P before OMP processing
    m_Left.ConvertToType(YUV_P);
    m_Right.ConvertToType(YUV_P);
    t_Mono.ConvertToType(YUV_P);

    aTransform tr_pl_ref = m_analyser.GetResults(P_LM);
    aTransform tr_pr_ref = m_analyser.GetResults(P_RM);
    m_flat = 300;
    double pos_l = tr_pl_ref.z * 4;
    double pos_r = 1.0 - tr_pr_ref.z * 4;
    double d_10, d_00, d_05, d_l, d_r;
    double b_10, b_00, b_05, b_l, b_r;
    //calculate all possible position valuss
#pragma omp parallel sections
    {
#pragma omp section
        if (abs(pos_l - 0.0f) >= 0.05 && abs(pos_l - 1.0f) >= 0.05 && abs(pos_l - 0.5f) >= 0.05)
        {
            MiddleGen mgen;
            mgen.Init(m_width, m_height);
            mgen.MakeView(d_left, d_right, pos_l);
            DeepAnalysis(m_analyser, mgen, b_l, d_l);
        }
        else
            b_l = d_l = 1000.0f; //we calculate it in other section
#pragma omp section
        if (abs(pos_r - 0.0f) >= 0.05 && abs(pos_r - 1.0f) >= 0.05 && abs(pos_r - 0.5f) >= 0.05)
        {
            MiddleGen mgen;
            mgen.Init(m_width, m_height);
            mgen.MakeView(d_left, d_right, pos_r);
            DeepAnalysis(m_analyser, mgen, b_r, d_r);
        }
        else
            b_r = d_r = 1000.0f;
#pragma omp section
        {
            MiddleGen mgen;
            mgen.Init(m_width, m_height);
            mgen.MakeView(d_left, d_right, 0.0f);
            DeepAnalysis(m_analyser, mgen, b_00, d_00);
        }
#pragma omp section
        {
            MiddleGen mgen;
            mgen.Init(m_width, m_height);
            mgen.MakeView(d_left, d_right, 0.5f);
            DeepAnalysis(m_analyser, mgen, b_05, d_05);
        }
#pragma omp section
        {
            MiddleGen mgen;
            mgen.Init(m_width, m_height);
            mgen.MakeView(d_left, d_right, 1.0f);
            DeepAnalysis(m_analyser, mgen, b_10, d_10);
        }
    }
    //use the lowest brightness diff
    m_brightness = ABMIN(ABMIN(ABMIN(b_00, b_05), ABMIN(b_10, b_l)), b_r);
    //find the lowest difference values
    if (d_00 <= MIN(MIN(d_05, d_10), MIN(d_l, d_r)))
    {
        m_difference = d_00;
        m_position = 0.0f;
    }
    else if (d_05 <= MIN(MIN(d_00, d_10), MIN(d_l, d_r)))
    {
        m_difference = d_05;
        m_position = 0.5f;
    }
    else if (d_10 <= MIN(MIN(d_00, d_05), MIN(d_l, d_r)))
    {
        m_difference = d_10;
        m_position = 1.0f;
    }
    else if (d_l <= MIN(MIN(d_00, d_05), MIN(d_10, d_r)))
    {
        m_difference = d_l;
        m_position = pos_l;
    }
    else if (d_r <= MIN(MIN(d_00, d_05), MIN(d_l, d_10)))
    {
        m_difference = d_r;
        m_position = pos_r;
    }
    //recognize flat frames
    m_flat = MAX(abs(d_10 - d_05), abs(d_10 - d_00));
}

//t_Mono, m_Left, m_right must be in YUV_P there
//analysis of structure and brightness difference
void CMonoPlugin::DeepAnalysis(StereoAnalyser &analyser, MiddleGen &mgen, double &br_d, double & ss_d)
{
    cv::Mat p_conf(m_height, m_width, CV_8UC1, mgen.GetMask());

    int ssim_x = 250;
    int ssim_y = ssim_x*m_height / m_width;
    cv::Mat grey_mono(m_height, m_width, CV_8UC1, t_Mono.GetData()),
        grey_left(m_height, m_width, CV_8UC1, m_Left.GetData()),
        grey_right(m_height, m_width, CV_8UC1, m_Right.GetData()),
        grey_nmono(m_height, m_width, CV_8UC1, mgen.GetImage()),
        ssim(ssim_y, ssim_x, CV_8UC1, cv::Scalar::all(0));

    cv::Mat grey_m1, grey_m2, grey_m3;
    //#pragma omp parallel sections
    //{
    //#pragma omp section
    FilterLetterbox(grey_mono, p_conf, 4, 1);
    //#pragma omp section
    FilterLetterbox(grey_nmono, p_conf, 4, 1);
    //#pragma omp section
    FilterLetterbox(grey_left, p_conf, 4, 1);
    //#pragma omp section
    FilterLetterbox(grey_right, p_conf, 4, 1);
    //}

#pragma omp parallel sections
    {
#pragma omp section
        MaskBlur(grey_mono, p_conf, grey_mono, 3);
#pragma omp section
        MaskBlur(grey_nmono, p_conf, grey_nmono, 3);
    }

#pragma omp parallel sections
    {
#pragma omp section
        cv::resize(grey_mono, grey_m1, cv::Size(ssim_x, ssim_y), 0, 0, cv::INTER_NEAREST);
#pragma omp section
        cv::resize(grey_nmono, grey_m2, cv::Size(ssim_x, ssim_y), 0, 0, cv::INTER_NEAREST);
#pragma omp section
        cv::resize(p_conf, grey_m3, cv::Size(ssim_x, ssim_y), 0, 0, cv::INTER_NEAREST);
    }
    cv::Mat sobel_x1, sobel_y1, sobel_x2, sobel_y2, sobel_1(ssim_y, ssim_x, CV_8UC1, cv::Scalar::all(0)), sobel_2(ssim_y, ssim_x, CV_8UC1, cv::Scalar::all(0));

#pragma omp parallel sections
    {
#pragma omp section
        {
#pragma omp parallel sections
            {
#pragma omp section
                cv::Scharr(grey_m1, sobel_x1, CV_64F, 1, 0, 3);
#pragma omp section
                cv::Scharr(grey_m1, sobel_y1, CV_64F, 0, 1, 3);
            }
            for (int j = 0; j < ssim_y; j++)
                for (int i = 0; i < ssim_x; i++)
                    sobel_1.at<unsigned char>(j, i) = BYTE(CLIP(sqrt((double)sqr(sobel_x1.at<double>(j, i) / 16) + sqr(sobel_y1.at<double>(j, i) / 10)), 0, 255));
            FilterLetterbox(sobel_1, grey_m3, 3, 0);
        }
#pragma omp section
        {
#pragma omp parallel sections
            {
#pragma omp section
                cv::Scharr(grey_m2, sobel_x2, CV_64F, 1, 0, 3);
#pragma omp section
                cv::Scharr(grey_m2, sobel_y2, CV_64F, 0, 1, 3);
            }
            for (int j = 0; j < ssim_y; j++)
                for (int i = 0; i < ssim_x; i++)
                    sobel_2.at<unsigned char>(j, i) = BYTE(CLIP(sqrt((double)sqr(sobel_x2.at<double>(j, i) / 16) + sqr(sobel_y2.at<double>(j, i) / 10)), 0, 255));
            FilterLetterbox(sobel_2, grey_m3, 3, 0);
        }
    }
    double error = CalculateSSIM(ssim, sobel_1, sobel_2, 3);
    ss_d = error;

    error = 0;
    int count = 0;
    double err = 0;
    for (int j = 0; j < ssim_y; j++)
        for (int i = 0; i < ssim_x; i++)
            if (ssim.at<unsigned char>(j, i) <= 10 && grey_m1.at<unsigned char>(j, i) + grey_m2.at<unsigned char>(j, i) != 0)
            {
                int dif = (grey_m1.at<unsigned char>(j, i) - grey_m2.at<unsigned char>(j, i));
                count++;
                err += dif;
            }
    err = count <= ssim_y*ssim_x*0.01 ? 0 : err /= count;
    br_d = err;

    return;

}
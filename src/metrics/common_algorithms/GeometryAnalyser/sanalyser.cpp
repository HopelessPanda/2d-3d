#include "sanalyser.h"
#include "ransac_ap.h"

void StereoAnalyser::Init(int width, int height)
{
    m_width = width;
    m_height = height;
    m_ssim = (BYTE*)malloc(m_width*m_height*sizeof(BYTE));
}

void StereoAnalyser::InitStereo(PNG_Image * left, PNG_Image * right)
{
    if (m_ssim == NULL)
        Init(left->width(), left->height());
    L_TO_R.Init(m_width, m_height, false);
    isStereoInit = true;
    o_left = left;
    o_right = right;
    TR_G_S.Init(GEOMETRY);
}
void StereoAnalyser::InitMono(PNG_Image * mono, bool disable_stereo)
{
    if(disable_stereo)
        m_process_stereo = false;
    if (m_ssim == NULL)
        Init(mono->width(), mono->height());
    L_TO_M.Init(m_width, m_height, true);
    R_TO_M.Init(m_width, m_height, true);
    L_TO_N.Init(m_width, m_height, true);
    R_TO_N.Init(m_width, m_height, true);
    M_TO_PREV.Init(m_width, m_height, true);
    o_mono = mono;
    isMonoInit = true;
    TR_G_M.Init(GEOMETRY);
    TR_P_ML.Init(POSITION);
    TR_P_MR.Init(POSITION);
}

//init the source frame
void StereoAnalyser::Load(PNG_Image * src, int t)
{
    switch (t)
    {
    case(LEFT) : o_left = src; break;
    case(RIGHT) : o_right = src; break;
    case(MONO) : o_mono = src; break;
    }
}

StereoAnalyser::StereoAnalyser()
{
    m_process_stereo = true;
    isStereoInit = false;
    isMonoInit = false;
    m_width = m_height = 0;
    o_left = o_right = o_mono = NULL;
    m_ssim = NULL;
}
#include <iostream>
void StereoAnalyser::MEonlyMono()
{
    prev_left->ConvertToType(YUV_P);
	o_left->ConvertToType(YUV_P);
    M_TO_PREV.Load(*o_left, *prev_left);
    M_TO_PREV.Process();    
    M_TO_PREV.CalcQuality();
    M_TO_PREV.FilterPairs();
    
}

void StereoAnalyser::MEwithoutTrans()
{
    o_notrans->ConvertToType(YUV_P);
    L_TO_N.Load(*o_left, *o_notrans);
    L_TO_N.Process();
    L_TO_N.CalcQuality();
    L_TO_N.FilterPairs();
   

    R_TO_N.Load(*o_right, *o_notrans);
    R_TO_N.Process();
    R_TO_N.CalcQuality();
    R_TO_N.FilterPairs();

    L_TO_R.Load(*o_left, *o_right);
    L_TO_R.Process();
    L_TO_R.CalcQuality();
    L_TO_R.FilterPairs();
}

void StereoAnalyser::Analyse()
{
    if (!isStereoInit)
        return; //we must have stereo to begin analysis
    if (isStereoInit && isMonoInit)
    {
#pragma omp parallel sections
        {
#pragma omp section
        o_left->ConvertToType(YUV_P); //our ME requires this image type
#pragma omp section
        o_right->ConvertToType(YUV_P);
#pragma omp section
        o_mono->ConvertToType(YUV_P);
        }
#pragma omp parallel sections
        {
#pragma omp section //prepare LR ME data
            {
                L_TO_R.Load(*o_left, *o_right);
                L_TO_R.Process();
                L_TO_R.CalcQuality();
                L_TO_R.FilterPairs();
            }
#pragma omp section //prepare LM ME data
            {
                L_TO_M.Load(*o_left, *o_mono);
                L_TO_M.Process();
                L_TO_M.CalcQuality();
                L_TO_M.FilterPairs();
            }
#pragma omp section //prepare RM ME data
            {
                R_TO_M.Load(*o_right, *o_mono);
                R_TO_M.Process();
                R_TO_M.CalcQuality();
                R_TO_M.FilterPairs();
            }
        }
#pragma omp parallel sections
        {
#pragma omp section //analyse L->M geometry (R->M isn't really necessary)
            {
                TR_G_M.Load(L_TO_M.Pairs());
                TR_G_M.Analyse();
            }
#pragma omp section //analyse stero geometry (if needed)
            if (m_process_stereo)
            {
                TR_G_S.Load(L_TO_R.Pairs());
                TR_G_S.Analyse();
            }
#pragma omp section //analyse L->M position
            {
                L_TO_R.FillMiscX(L_TO_M.Pairs(), FIRST);
                TR_P_ML.Load(L_TO_M.Pairs());
                TR_P_ML.Analyse();
            }
#pragma omp section //analyse R->M position
            {
                L_TO_R.FillMiscX(R_TO_M.Pairs(), SECOND);
                TR_P_MR.Load(R_TO_M.Pairs());
                TR_P_MR.Analyse();
            }
        }
    }
    else if (isStereoInit) //only stereo processing
    {
#pragma omp parallel sections
        {
#pragma omp section
        o_left->ConvertToType(YUV_P); //our ME requires this image type
#pragma omp section
        o_right->ConvertToType(YUV_P);
        }
        L_TO_R.Load(*o_left, *o_right);
        L_TO_R.Process();
        L_TO_R.CalcQuality();
        L_TO_R.FilterPairs();
        TR_G_S.Load(L_TO_R.Pairs());
        TR_G_S.Analyse();
    }
}

StereoAnalyser::~StereoAnalyser()
{
    if (m_ssim != NULL)
        free(m_ssim);
}

aTransform StereoAnalyser::GetResults(int t)
{
    switch (t)
    {
    default: case (G_LR) : return TR_G_S.Result(); break;
    case (G_LM) : return TR_G_M.Result(); break;
    case (P_LM) : return TR_P_ML.Result(); break;
    case (P_RM) : return TR_P_MR.Result(); break;
    }
}

//return percent of total vectors used during approximation
double StereoAnalyser::GetTotal(int t)
{
    Ransac * source;
    switch (t)
    {
    default: case (G_LR) : source = &TR_G_S; break;
    case (G_LM) : source = &TR_G_M; break;
    case (P_LM) : source = &TR_P_ML; break;
    case (P_RM) : source = &TR_P_MR; break;
    }
    int vectors = source->GetTotal();
    int total_amount = (m_width / FI_BLOCK)*(m_height / FI_BLOCK);
    return double(vectors) / total_amount;
}

//returns percent of how many vectors have been fit 
double StereoAnalyser::GetFit(int t)
{
    Ransac * source;
    switch (t)
    {
    default: case (G_LR) : source = &TR_G_S; break;
    case (G_LM) : source = &TR_G_M; break;
    case (P_LM) : source = &TR_P_ML; break;
    case (P_RM) : source = &TR_P_MR; break;
    }
    int vectors_fit = source->GetFit();
    int vectors_total = source->GetTotal();
    if(vectors_total == 0)
        return 0;
    else
        return double(vectors_fit) / vectors_total;
}

ME_Instance *  StereoAnalyser::GetME(int t)
{
    ME_Instance * source;
    switch (t)
    {
    default:
    case (0) : source = &L_TO_R;    break;
    case (1) : source = &L_TO_M;    break;
    case (2) : source = &R_TO_M;    break;

    case (3) : source = &L_TO_N;    break;
    case (4) : source = &R_TO_N;    break;
    case (5) : source = &M_TO_PREV; break;

    }
    return source;
}
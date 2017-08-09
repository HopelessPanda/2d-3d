#include "metric.h"
#include "im_vis.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <algorithm>
#include <iomanip>
#include <utility>
#include "log.h"

#define CONF_THSH 0.2
#define CNTH 5         // threshold for center difference
double accuracy = 5;
using namespace std;
using namespace cv;

enum direction { x = 0, y = 1 };

bool comp_coef(Object a, Object b) {
    return a.coef_x > b.coef_x;
}
bool comp_double(pair<double, double> a, pair<double, double> b);

int CMonoPlugin::Measure(int framenum, CLinkedAnchor& request, IDataServer* data_manager, IResultsServer* results_server, IVisualizationServer* vis_server)
{
    ME_app = true;
    //load new frame
    UpdateSource(data_manager);
    m_analyser.o_notrans = &m_Mono;
    LOG log(true, 1, false);
    log.print("\n", 1);
    //only for me
    if (ME_app) {
        m_analyser.prev_mono = &prev_Mono;
        m_analyser.MEonlyMono();
        cMV* MtoPREV = m_analyser.GetME(5)->Field(0);
        double   data_mtoprev = 0;
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                data_mtoprev += abs(MtoPREV[i*m_width + j].x);
            }
        }
        int total = m_height*m_width;
        me_prev = data_mtoprev / total;
        //PostResult(framenum, results_server);
        //return 0;
    }

    //get ME vectors
    m_analyser.MEwithoutTrans();
    log.print("after ME");

    //BYTE *q_MtoL = m_analyser.L_TO_N.Quality(1);
    BYTE *q_LtoR = m_analyser.L_TO_R.Quality(0);
    BYTE *q_LtoM = m_analyser.L_TO_N.Quality(0);


    cMV* MtoL = m_analyser.GetME(3)->Field(1); // NtoL
    cMV* LtoM = m_analyser.GetME(3)->Field(0); // LtoN
    cMV* LtoR = m_analyser.GetME(0)->Field(0); // LtoR
    cMV* RtoL = m_analyser.GetME(0)->Field(1); // RtoL
    cMV* MtoR = m_analyser.GetME(4)->Field(1); // MtoR

    //find the left-2D-right coefficient 0 - left, 1 - right
    double alpha = GetPosition(log, framenum);

    log.print("left-2D-right coefficient", alpha, 1);
    if (alpha > 1 || alpha < 0) {
        log.print("bad alpha", alpha, 1);
        PostResult(framenum, results_server);
        return 0;
    }

    //segmentize the image                                                                   
    log.print("starting segments");
    BYTE *depth_LtoR = (BYTE*)malloc(m_width*m_height * sizeof(BYTE));
    BYTE *seg_map_LtoR = (BYTE*)malloc(m_width*m_height * sizeof(BYTE));
    m_analyser.L_TO_R.GetDepthMap(depth_LtoR, 0);
    log.vis_segments(depth_LtoR, m_height, m_width, "depth.png");
    int count_seg = seg_engine.GetSegmentationMap(depth_LtoR, seg_map_LtoR);
    if (count_seg < 1) {
        log.print("no objects detected", 1);
        PostResult(framenum, results_server);
        return 0;
    }
    log.print("finished segments");


    /*visualize segments*/ if (log.visualization) {
        stringstream sss;
        sss << framenum;
        string name = "obj/" + sss.str() + "_segments.png";
        log.vis_segments(seg_map_LtoR, m_height, m_width, name);
    }

    //calculate area and depth of each segment
    double *area = new double[count_seg]();
    long long *depth = new long long[count_seg]();
    vector<int> depths;
    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            int index = i*m_width + j;
            area[seg_map_LtoR[index]]++;
            depth[seg_map_LtoR[index]] += depth_LtoR[index];
        }
    }
    double total_area = m_width*m_height;
    for (int i = 0; i < count_seg; i++) {
        area[i] /= total_area;
        depth[i] /= (long long)(area[i] * m_width * m_height);
        if (area[i] < 0.2 || area[i] > 0.8) continue;
        depths.push_back(depth[i]);
    }

    //find the line between object and background
    if (depths.size() < 1) {
        log.print("no objects detected (depths)", 1);
        PostResult(framenum, results_server);
        return 0;
    }
    std::sort(depths.begin(), depths.end());
    int bckgr_threshold = depths.front() + (depths.back() - depths.front()) / 3;
    log.print("depth threshold", bckgr_threshold);

    //create vectors of objects and backgrounds with 0.1 < area < 0.8
    vector<Object> obj;
    vector<Object> bckg;
    for (int i = 0; i < count_seg; i++) {
        if (area[i] > 0.1 && area[i] < 0.8) {
            Object n;
            n.area = area[i];
            n.depth = depth[i];
            n.index = i;
            if (depth[i] > bckgr_threshold) {
                n.type = 1;
                bckg.push_back(n);
            }
            else {
                n.type = 0;
                obj.push_back(n);
            }
        }
    }


    if (obj.size() < 1) {
        log.print("no objects found", 1);
        obj_coefficient = -1;
    }
    if (bckg.size() < 1) {
        log.print("no background found", 1);
        bckg_coefficient = -1;
    }

    //find objects' coordinates
    int x_min = m_width, y_min = m_height, x_max = 0, y_max = 0;
    for (int k = 0; k < obj.size(); k++) {
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                int index = i*m_width + j;
                if ((int)seg_map_LtoR[index] == obj[k].index) {
                    if (j < x_min) x_min = j;
                    if (j > x_max) x_max = j;
                    if (i < y_min) y_min = i;
                    if (i > y_max) y_max = i;
                }
            }
        }
        obj[k].max_x = x_max;
        obj[k].max_y = y_max;
        obj[k].min_x = x_min;
        obj[k].min_y = y_min;
        x_min = m_width;
        y_min = m_height;
        x_max = 0;
        y_max = 0;
    }
    log.print("found coordinates");

    //find the center for each object
    log.print("starting centers");
    double center_conf = 0;
    int *conf_pix_in_use = (int*)malloc(m_width * sizeof(int));
    int *total_pix_in_use = (int*)malloc(m_width * sizeof(int));
    for (int k = 0; k < obj.size(); k++) {
        //obj[k].center_x = GetCenter(seg_map_LtoR, obj[k].index, alpha, direction::x, log, framenum);
        int *centers = (int*)malloc(m_width * sizeof(int));
        for (int i = 0; i < m_width; i++) { centers[i] = 0; conf_pix_in_use[i] = 0; total_pix_in_use[i] = 0; }
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                int index = i*m_width + j;
                if (seg_map_LtoR[index] != obj[k].index) continue;
                //if (dir == direction::x) {
                if (abs(LtoM[index].x - alpha*LtoR[index].x) <= CNTH) {
                    centers[j]++;
                    conf_pix_in_use[j] += (q_LtoM[index] + q_LtoR[index]);
                    total_pix_in_use[j]++;
                }

                //}
            }
        }

        int max = 0;
        int center = -1;
        int pixels_for_center;
        int conf_center;
        //if (dir == direction::x) {
        for (int i = 0; i < m_width; i++) {
            if (centers[i] > max) {
                max = centers[i];
                center = i;
                pixels_for_center = conf_pix_in_use[i];
                conf_center = pixels_for_center / total_pix_in_use[i];
            }
        }
        //}

        free(centers);

        obj[k].center_x = center;
        coef_conf += conf_center;
        log.print("center conf", conf_center, 1);
        // vertical stretch        
        obj[k].center_y = -1; // GetCenter(seg_map_LtoR, obj[i].index, alpha, direction::y, log, framenum);
        log.print("center x", obj[k].center_x);

    }
    for (int k = 0; k < bckg.size(); k++) {
        //obj[k].center_x = GetCenter(seg_map_LtoR, obj[k].index, alpha, direction::x, log, framenum);
        int *centers = (int*)malloc(m_width * sizeof(int));
        for (int i = 0; i < m_width; i++) centers[i] = 0;
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                int index = i*m_width + j;
                if (seg_map_LtoR[index] != bckg[k].index) continue;
                //if (dir == direction::x) {
                if (abs(LtoM[index].x - alpha*LtoR[index].x) <= CNTH) {
                    centers[j]++;
                }

                //}
            }
        }

        int max = 0;
        int center = -1;
        //if (dir == direction::x) {
        for (int i = 0; i < m_width; i++) {
            if (centers[i] > max) {
                max = centers[i];
                center = i;
            }
        }
        //}

        free(centers);

        bckg[k].center_x = center;
        // vertical stretch        
        bckg[k].center_y = -1; // GetCenter(seg_map_LtoR, obj[i].index, alpha, direction::y, log, framenum);
        log.print("center x", bckg[k].center_x);

    }

    //find the coefficient for each object
    log.print("starting coefficients");
    for (auto k = obj.begin(); k != obj.end(); k++) {
        // horizontal vectors
        if (k->center_x == -1) {
            k->coef_x = -1;
            k->coef_y = -1;
            continue;
        }
        k->coef_x = GetCoef(seg_map_LtoR, alpha, k->index, k->center_x, direction::x, log, framenum);

        // vertical vectors
        k->coef_y = -1;//GetCoef(seg_map_LtoR, alpha, i->index, i->center_y, direction::y, log);         
    }
    for (auto k = bckg.begin(); k != bckg.end(); k++) {
        // horizontal vectors
        if (k->center_x == -1) {
            k->coef_x = -1;
            k->coef_y = -1;
            continue;
        }
        k->coef_x = GetCoef(seg_map_LtoR, alpha, k->index, k->center_x, direction::x, log, framenum);

        // vertical vectors
        k->coef_y = -1;//GetCoef(seg_map_LtoR, alpha, i->index, i->center_y, direction::y, log); 
    }

    //find object and background with biggest coefficient
    stringstream objects, bckgr;
    if (obj.size() > 0) {
        sort(obj.begin(), obj.end(), comp_coef);
        obj_coefficient = obj.front().coef_x;
    }
    if (bckg.size() > 0) {
        sort(bckg.begin(), bckg.end(), comp_coef);
        bckg_coefficient = bckg.front().coef_x;
    }

    //FindDeleted(alpha, framenum);
    // чем меньше тем статичнее, тем больше должен быть процент
    double k = 0.4 - ((double)me_prev / 100);
    if (k < 0) k = 0;
    //if (me_prev != 0)
    //    obj_coefficient = obj_coefficient * (1 - k) + prev_coef * k;

    PostResult(framenum, results_server);

    free(depth_LtoR);
    free(seg_map_LtoR);
    prev_coef = obj_coefficient;
    return 0;

}

double CMonoPlugin::GetPosition(LOG log, int framenum) {
    cMV* MtoL = m_analyser.GetME(3)->Field(1); // NtoL
    cMV* LtoM = m_analyser.GetME(3)->Field(0); // LtoN
    cMV* LtoR = m_analyser.GetME(0)->Field(0); // LtoR
    cMV* RtoL = m_analyser.GetME(0)->Field(1); // RtoL
    cMV* MtoR = m_analyser.GetME(4)->Field(1); // MtoR

    int interations = 10;
    vector<double> alphas;
    vector<pair<double, double>> possible_alphas;
    srand(time(NULL));
    for (int k = 0; k < interations; k++) {
        for (int l = 0; l < 200; l++) {
            int random_number_1 = rand() % m_width;
            int random_number_2 = rand() % m_height;
            int i = random_number_1*random_number_2;
            if (i + MtoL[i].x < 0 || i + MtoL[i].x >= m_width - 1) continue;
            alphas.push_back(abs((double)MtoR[i].x) / abs((double)LtoR[i + MtoL[i].x].x));
        }
        sort(alphas.begin(), alphas.end());
        log.print("sorted alphas");
        if (alphas.size() < 2) continue;
        log.print("choosing median alpha");
        double model = alphas.at(alphas.size() / 2);
        if (model < 0 || model > 1) continue;
        double total_error = 0;
        log.print("checking model");
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                int index = i*m_width + j;
                if (j + MtoL[index].x < 0 || j + MtoL[index].x >= m_width - 1) continue;
                total_error += abs(MtoR[index].x - model*LtoR[j + MtoL[index].x].x);
            }
        }
        log.print("pushing to possible alphas");
        possible_alphas.push_back(make_pair(model, total_error));
    }
    if (possible_alphas.size() == 0) {
        log.print("something wrong in GetPosition: candidate count = 0");
        return 0;
    }
    log.print("sorting possible alphas");
    sort(possible_alphas.begin(), possible_alphas.end(), comp_double);

    coef_conf = possible_alphas.front().second / (m_width * m_height); // средн€€ ошибка (чем меньше тем лучше)
    log.print("alpha conf", coef_conf, 1);

    return 1 - possible_alphas.front().first;
}

int CMonoPlugin::GetCenter(BYTE *image, int ind, double alpha, int dir, LOG log, int framenum)
{
    BYTE *q_MtoL = m_analyser.L_TO_N.Quality(1);
    BYTE *q_LtoR = m_analyser.L_TO_R.Quality(0);
    BYTE *q_LtoM = m_analyser.L_TO_N.Quality(0);

    cMV* MtoL = m_analyser.GetME(3)->Field(1); // NtoL
    cMV* LtoM = m_analyser.GetME(3)->Field(0); // LtoN
    cMV* LtoR = m_analyser.GetME(0)->Field(0); // LtoR
    cMV* RtoL = m_analyser.GetME(0)->Field(1); // RtoL
    cMV* MtoR = m_analyser.GetME(4)->Field(1); // MtoR

    /*save the stretch vectors (white - if there's any magnification, black - only displacement) in grey pic*/ if (log.log) {
        BYTE* coupled = (BYTE*)malloc(m_height*m_width * sizeof(BYTE));
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                int index = i*m_width + j;
                coupled[index] = abs(LtoM[index].x - alpha*LtoR[index].x);
            }
        }
        log.vis_grey_image(coupled, m_height, m_width, "_coupledLM(center).png", framenum);
    }

    int *centers = (int*)malloc(m_width * sizeof(int));
    for (int i = 0; i < m_width; i++) centers[i] = 0;

    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            int index = i*m_width + j;
            if (image[index] != ind) continue;
            if (dir == direction::x) {
                if (abs(LtoM[index].x - alpha*LtoR[index].x) <= CNTH) {
                    centers[j]++;
                }

            }
        }
    }

    int max = 0;
    int center = -1;
    if (dir == direction::x) {
        for (int i = 0; i < m_width; i++) {
            if (centers[i] > max) {
                max = centers[i];
                center = i;
            }
        }
    }

    /*visualize center*/ if (log.log) {
        if (dir == direction::x) {
            BYTE *coef_vis = (BYTE*)malloc(m_width*m_height * sizeof(BYTE));
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    int index = i*m_width + j;
                    if (j == center) coef_vis[index] = 255;
                    else if (abs(j - center) < 5) coef_vis[index] = 100;
                    else coef_vis[index] = 0;
                }
            }
            log.vis_grey_image(coef_vis, m_height, m_width, "center vis.png", framenum);
        }
    }

    free(centers);
    return center;
}

bool comp_double(pair<double, double> a, pair<double, double> b)
{
    return (a.second < b.second);
}

double CMonoPlugin::GetCoef(BYTE *image, double alpha, int ind, int center, int dir, LOG log, int framenum)
{
    BYTE *q_MtoL = m_analyser.L_TO_N.Quality(1);
    BYTE *q_MtoR = m_analyser.R_TO_N.Quality(1);
    BYTE *q_LtoR = m_analyser.L_TO_R.Quality(0);
    BYTE *q_LtoM = m_analyser.L_TO_N.Quality(0);

    cMV* MtoL = m_analyser.GetME(3)->Field(1); // NtoL
    cMV* LtoM = m_analyser.GetME(3)->Field(0);
    cMV* LtoR = m_analyser.GetME(0)->Field(0); // LtoR
    cMV *MtoR = m_analyser.GetME(2)->Field(1); // LtoR

    vector<double> coef;
    double min = 10, max = 0;
    int count = 0, total = 0;
    double conf = 0;

    //cout << endl;
    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            int index = i*m_width + j;
            if (image[index] != ind || abs(j - center) < 25) continue;

            if (dir == direction::x) {
                int distance = abs(j - center) + abs(LtoM[index].x);
                double coef_l = ((double)abs(LtoM[index].x - alpha*LtoR[index].x)) / (2 * distance);
                count++;
                coef.push_back(coef_l);
                conf += (q_LtoM[index] + q_LtoR[index]);
                if (coef_l < min) min = coef_l;
                if (coef_l > max) max = coef_l;
            }
            /*
            else {
                if (i + MtoL[index].y < 0 || i + MtoL[index].y > m_height - 1 || MtoL[index].y == 0) continue;
                double coef_l = abs((MtoL[index].y) / ((double)(i - center)) + 1);
                if (q_MtoL[index] < accuracy && q_LtoM[index] < accuracy) {   /// !!!!!!!!
                    coef.push_back(coef_l);
                    count++;
                    if (coef_l < min) min = coef_l;
                    if (coef_l > max) max = coef_l;
                }
                else {
                }
            } */
        }
    }

    double confidence = conf / count;
    /*if (confidence < CONF_THSH) {
        log.print("confidence too low");
        return -1;
    } */
    if (coef.size() == 0) {
        log.print("no candidates for coefficient");
        return -1;
    }
    //coef_conf += conf;
    //log.print("coef conf", conf, 1);

    sort(coef.begin(), coef.end());
    //detect outliers
    double quartile1 = coef[coef.size() / 2];
    double quartile2 = coef[coef.size() / 4];
    double quartile3 = coef[3 * coef.size() / 4];
    double interquartalrange = quartile3 - quartile1;
    double lowTresh = quartile1 - 1.5*interquartalrange;
    double highTresh = quartile3 + 1.5*interquartalrange;
    //cout << quartile1 << ' ' << quartile2 << ' ' << quartile3 << ' ' << interquartalrange << endl;
    log.print("highthresh", highTresh);
    log.print("lowthresh", lowTresh);
    log.print("max", coef.back());
    log.print("median", coef[coef.size() / 2]);
    log.print("min", coef.front());
    return coef[coef.size() / 2] + 1;
}

bool cmp(pair<int, int> a, pair<int, int> b)
{
    return (a.second < b.second);
}


/*misc
    //load new frame
    UpdateSource(data_manager);

    int bright = Brightness_Color_check(&m_Mono, &m_Left);
    return 0;

    m_analyser.o_notrans = &m_Mono;
    m_analyser.MEwithoutTrans();


    //transform mono according to prev results
    //t_Mono.SaveToPNG("t_Mono_no transform.png");
    bool wasdone = 0; TransformMono(2); //apply old params only
    //std::cout << "\n" << wasdone << " - transform with old params only\n";
    //t_Mono.SaveToPNG("t_Mono_old params.png");

    // try to analyse
    t_Mono.ConvertToType(YUV_P);
    m_analyser.Analyse(); // <- finds existing difference and scale

        //bad result? calculate w/o transform
        // Check() <- still zoom or bad confidence

        if (!Check() && wasdone)
        {
            t_Mono = m_Mono;
            t_Mono.ConvertToType(YUV_P);
            m_analyser.Analyse();
            wasdone = TransformMono(0); //get new params
            cout << wasdone << " - transformed with new params\n";
            //t_Mono.SaveToPNG("t_Mono_new_params.png");
        }
        else {
            wasdone = TransformMono(1); //with using old params
            cout << wasdone << " - trasformed with old params\n";
            //t_Mono.SaveToPNG("t_Mono_refined params.png");
        }

        if(!MeasureConf())
        {
            m_position = -1;
            m_flat = -1;
            m_difference = 0;
            m_brightness = 0;
            cout << "bad confindence\n";
        }
        else
        {
            MultiPosAnalysis();
            cout << "multipos analysis\n";
        }

        PostResult(framenum, results_server);

    //t_Mono = m_Mono;
    //t_Mono.ConvertToType(YUV_P);
    //m_analyser.Analyse();


    d_left.frame->SaveToPNG("d_left_frame.png");
    PNG_Image::SaveArrayToPNG(d_left.inst->Quality(0), m_width, m_height, "inst_quality_first.png");
    BYTE *tmp = (BYTE*)malloc(m_width*m_height);
    d_left.inst->GetDepthMap(tmp, 0);
    PNG_Image::SaveArrayToPNG(tmp, m_width, m_height, "inst_depth_map_first.png");




    //d_right.inst->GetDepthMap(tmp, 0);
    //PNG_Image::SaveArrayToPNG(tmp, m_width, m_height, "right_depth_map.png");
    //name = "right";
    //int count_seg = seg_engine.GetSegmentationMap(tmp, seg_map, name);


    //PNG_Image::SaveArrayToPNG(m_analyser.GetME(0)->Quality(0), m_width, m_height, "quality_LtoR.png");
    //PNG_Image::SaveArrayToPNG(m_analyser.GetME(3)->Quality(1), m_width, m_height, "quality_MtoL.png");
    //PNG_Image::SaveArrayToPNG(m_analyser.GetME(2)->Quality(0), m_width, m_height, "quality_RtoM.png");


    //out.seekg (0, ios::end);

    double alpha = GetPosition(m_analyser.GetME(0)->Field(0), m_analyser.GetME(4)->Field(1), m_analyser.GetME(3)->Field(1), 0, 0);

    cout << alpha << endl;
    //PrintME(0, 0, 0);
    ProcessObjects(alpha, framenum);
    /*
        PNG_Image tmp = m_Mono;
            objects[k].center = GetCenter(objects[k].mask, m_analyser.GetME(3)->Field(1) ,
                m_analyser.GetME(4)->Field(1),m_analyser.GetME(0)->Field(0), alpha, tmp.GetData(), objects[k].index);
            cout << objects[k].index << "center = "<< objects[k].center << endl;

            itoa(k, n, 10);
            strcat(n, "_obj.png");

            //PNG_Image::SaveArrayToPNG(objects[k].mask, m_width, m_height, n);
            if(!objects[k].valid) continue;
            objects[k].coef = GetCoef(alpha, m_analyser.GetME(0)->Field(0), m_analyser.GetME(4)->Field(1), m_analyser.GetME(3)->Field(1), objects[k].mask, objects[k].center, objects[k].index);
            cout << "coef " << objects[k].index << " = " << objects[k].coef << endl;

            int offset = 20;
            //if(objects[k].min_x-offset < 0) objects[k].min_x = 0;
            //if(objects[k].min_y-offset < 0) objects[k].min_y = 0;
            //if(objects[k].max_x+offset > m_width - 1) objects[k].max_x = m_width - 1;
            //if(objects[k].max_y+offset > m_height - 1) objects[k].max_y = m_height - 1;

            string ratio;// = " ratio: ";
            stringstream ss;
            ss << objects[k].coef;
            //cout << "STRING " << ss.str() << endl;
            ratio = ss.str();

            string confidence = " confidence: ";
            stringstream s;
            s << objects[k].confidence;
            confidence = s.str();

            stringstream st;
            st << objects[k].max_coef;

            // --- visualize left --- //
            /*
            tmp = m_Left;
            Mat left(m_height, m_width, CV_8UC1, tmp.GetData());

            rectangle(left, Point(objects[k].min_x, objects[k].min_y), Point(objects[k].max_x, objects[k].max_y), cvScalar(255, 255, 0), 2);
            putText(left, string("ratio: ") + ss.str(), cvPoint(0, 25),  FONT_HERSHEY_SIMPLEX, 0.85, cvScalar(255, 255, 0), 2);
            putText(left, string("max coef: ") + st.str(), cvPoint(0, 50),  FONT_HERSHEY_SIMPLEX, 0.85, cvScalar(255, 255, 0), 2);
            putText(left, string("confidence: ") + s.str(), cvPoint(0, 50),  FONT_HERSHEY_SIMPLEX, 0.85, cvScalar(255, 255, 0), 2);
            line(left, Point(objects[k].center, objects[k].min_y), Point(objects[k].center, objects[k].max_y), cvScalar(255, 255, 0), 1);

            stringstream sss;
            sss << k;
            string name = sss.str() + "_final_left.png";
            BYTE *dataMat = left.data;
            PNG_Image g;
            g.Set(m_Left.width(), m_Left.height(), 4, dataMat);
            g.SaveToPNG(name.c_str());

            // --- visualize mono -- //
            tmp = m_Mono;
            Mat mono(m_height, m_width, CV_8UC1, tmp.GetData());

            rectangle(mono, Point(objects[k].min_x, objects[k].min_y), Point(objects[k].max_x, objects[k].max_y), cvScalar(255, 255, 0), 2);
            putText(mono, string("ratio: ") + ss.str(), cvPoint(0, 25),  FONT_HERSHEY_SIMPLEX, 0.85, cvScalar(255, 255, 0), 2);
            putText(mono, string("max coef: ") + st.str(), cvPoint(0, 50),  FONT_HERSHEY_SIMPLEX, 0.85, cvScalar(255, 255, 0), 2);
            putText(mono, string("confidence: ") + s.str(), cvPoint(0, 50),  FONT_HERSHEY_SIMPLEX, 0.85, cvScalar(255, 255, 0), 2);
            line(mono, Point(objects[k].center, objects[k].min_y), Point(objects[k].center, objects[k].max_y), cvScalar(255, 255, 0), 1);

            dataMat = mono.data;
            g.Set(m_Mono.width(), m_Mono.height(), 4, dataMat);
            name = sss.str() + "_final_mono.png";
            g.SaveToPNG(name.c_str());

            //cout << endl;

            if(max_coef < objects[k].coef){
                max_coef = objects[k].coef;
                m = k;
            }
        }


    //FindDeleted(alpha, framenum);

    return 0;


*/
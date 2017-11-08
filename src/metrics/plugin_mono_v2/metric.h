#pragma once
#include <metrics/common/custom_metric.h>
#include <omp.h>
#include <metrics/common_algorithms/GeometryAnalyser/sanalyser.h>
#include "middlegen.h"
#include "Depth_map_segmentation.h"
#include <vector>
#include "log.h"
#include "depth_estimator.hpp"


#define TR_C 0.01*255

#define ABMIN(a,b) (((abs(a)) < (abs(b))) ? (a) : (b))

enum {OBJ = 0, BCKG = 1, DEL = 2};

struct Object
{
    double area;  // площадь в процентах от всего кадра
    double confidence;  // доля векторов с хорошим доверием
    double coef_x;	  // коэффициент увеличения
    double coef_y;
    double max_coef; 
    //BYTE *mask;
    int index;   
    double depth;
    int center_x;
    int center_y;
    bool valid;
    int min_x, min_y, max_x, max_y; // object coordinates
    int type;     // 0 - object, 1 - background
};

class LOG;

class CMonoPlugin: virtual public CCustomMetric {
    private:
        void InitMetric();
        void Deinit();
        bool TransformMono(int isRefine);																// resize and shift 2D for comparison with 3D
        bool CheckPosition(StereoAnalyser &analyser);
        cv::Mat getTransformMatrix(aTransform geo, aTransform pos, bool invert);						// make a cv::mat matrix from aTrasform structures
        void DeepAnalysis(StereoAnalyser &analyser, MiddleGen &mgen, double &br_d, double &ss_d);		
        bool Check();
        bool MeasureConf();
        void BlankResult(int framenum, IResultsServer* results_server);
        void PostResult(int framenum, IResultsServer* results_server);
        void PostAvgResult(int framenum, IResultsServer* results_server);
        void UpdateSource(IDataServer* data);
        void MultiPosAnalysis();

        PNG_Image m_Left, m_Right, m_Mono, m_Mono_BGR, t_Mono, m_Input;
		PNG_Image prev_Left, prev_Right, prev_Mono;
        int m_width, m_height;
        bool ME_app;

        me_data d_left;		
        me_data d_right;

        cv::Mat transform;
        double m_confidence, m_difference, m_brightness, m_position, m_z, m_flat, me_prev;
        double obj_coefficient, bckg_coefficient, coef_conf, del_square;
        double prev_coef;
        string obj_res, bckg_res;
        StereoAnalyser m_analyser;
        DepthSegmentationEngine seg_engine;
		DepthEstimator* depth_estimator;
        
    public:
        ~CMonoPlugin();
        CMonoPlugin();
        int Measure(int framenum, CLinkedAnchor& request, IDataServer* data_manager, IResultsServer* results_server, IVisualizationServer* vis_server);	

        double GetPosition(LOG log, int framenum);
        double GetCoef(BYTE *image, double alpha, int ind, int center, int direction, LOG log, int framenum);
        int GetCenter(BYTE *image, int ind, double alpha, int direction, LOG log, int framenum);
        int Brightness_Color_check(PNG_Image*, PNG_Image*);
        bool FitBrightness(PNG_Image mono, PNG_Image left);
        void Segmentize(BYTE* mask, double alpha, int id);
        double FindDeleted(double alpha, BYTE* depth_map, int framenum);
		bool color_check(PNG_Image * a, PNG_Image * b, const int i, const int j, int x = 0, int y = 0);
		bool depth_check(BYTE* LtoR, BYTE* MtoL, BYTE* MtoR, const int i, const int j, int x = 0, int y = 0);
		double FindDeletedTwoVersions(BYTE* depth_map, int framenum);
		void two_version_comparison(int framenum, IResultsServer* results_server);
        
        // not used 
        int CompareFramesLt(IResultsServer* results, int framenum1, int framenum2, int vis_id);
};
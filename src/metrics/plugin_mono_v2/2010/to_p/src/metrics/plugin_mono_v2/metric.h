#include <metrics/common/custom_metric.h>
#include <omp.h>
#include <metrics/common_algorithms/GeometryAnalyser/sanalyser.h>
#include "middlegen.h"

#define TR_C 0.01*255

#define ABMIN(a,b) (((abs(a)) < (abs(b))) ? (a) : (b))

class CMonoPlugin: virtual public CCustomMetric {
	private:
		void InitMetric();
		void Deinit();
		bool TransformMono(int isRefine);
		bool CheckPosition(StereoAnalyser &analyser);
		cv::Mat getTransformMatrix(aTransform geo, aTransform pos, bool invert);
		void DeepAnalysis(StereoAnalyser &analyser, MiddleGen &mgen, double &br_d, double &ss_d);
		bool Check();
		bool MeasureConf();
		void BlankResult(int framenum, IResultsServer* results_server);
		void PostResult(int framenum, IResultsServer* results_server);
		PNG_Image m_Left, m_Right, /*m_Left_BGR, m_Right_BGR,*/ m_Mono, m_Mono_BGR, t_Mono, m_Input;
		int m_width, m_height;
		me_data d_left;
		cv::Mat transform;
		me_data d_right;
		double m_confidence, m_difference, m_brightness, m_position, m_z, m_flat;
		StereoAnalyser m_analyser;
		void UpdateSource(IDataServer* data);
		void MultiPosAnalysis();
	public:
		~CMonoPlugin();
		CMonoPlugin();
		int Measure(int framenum, CLinkedAnchor& request, IDataServer* data_manager, IResultsServer* results_server, IVisualizationServer* vis_server);		
		int CompareFramesLt(IResultsServer* results, int framenum1, int framenum2, int vis_id);
};
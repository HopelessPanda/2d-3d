#include <opencv2/opencv.hpp>
/*#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>*/
#include <opencv2/ximgproc.hpp>

#ifndef BYTE
typedef unsigned char BYTE;
#endif

enum fgs_type
{
	fgs_uc8, //BYTE
	fgs_sc16, //short
};

class FastGlobalSmoother
{
private:
	int w;
	int h;
	cv::Mat guide_mat;

	cv::Ptr<cv::ximgproc::FastGlobalSmootherFilter> fgsf;
public:
	FastGlobalSmoother(){}
	~FastGlobalSmoother(){}

	void init(BYTE* guide, int _w, int _h, double lambda, double sigma_color, double lambda_attenuation=0.25, int num_iter=3);
	void filter(void* src_map, BYTE* src_conf, void* dst_map, fgs_type type);
};

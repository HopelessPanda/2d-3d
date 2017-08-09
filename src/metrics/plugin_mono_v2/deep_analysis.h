#include <metrics/common_algorithms/PNG_Image/png_image.h>
#define SSIM_TR 1.25
#define Max(a,b)            (((a) > (b)) ? (a) : (b))
#define Min(a,b)            (((a) < (b)) ? (a) : (b))
#define sqr(x) ((x)*(x))
cv::Mat CalculateIntegral (cv::Mat &image);
cv::Mat CalculateMulIntegral (cv::Mat &image, cv::Mat &image2);
cv::Mat CalculateMaskIntegral (cv::Mat &image, cv::Mat &mask);
cv::Vec3b getColorSubpix(const cv::Mat& img, cv::Point2f pt);
inline int getVec3bAbsDiff(cv::Vec3b first, cv::Vec3b second)
{
	return abs(first[0] - second[0]) + abs(first[1] - second[1]) + abs(first[2] - second[2]);
}
int MaskBlur (cv::Mat &image, cv::Mat &mask, cv::Mat &result, int rad);
void FilterLetterbox(cv::Mat &image, cv::Mat &mask, int depth, int type);
double CalculateSSIM(cv::Mat &dst, cv::Mat &src1, cv::Mat &src2, int rad);
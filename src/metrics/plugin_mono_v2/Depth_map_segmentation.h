#include <windows.h>
#include <string>

struct pixel
{
	int x,y;
	pixel(){}
	pixel(int _x,int _y):x(_x),y(_y){}
};

class DepthSegmentationEngine
{
private:
	int w,h;
	int prefiltering_radius;
	bool use_adaptive_strength;
	double strength;

	double* gauss_kernel;
	int* depth_histogram, *extended_depth_histogram;
	double* smoothed_depth_histogram;
	int* histogram_labeling;
	BYTE* filtered_depth_map;
	pixel* flood_fill_list;
	
	int TraceHistogramLocalMaximum(double* hist,int hist_size,int startpos);
	void ExtendHistogram(int* src_hist,int* dst_hist,int hist_size,int border_size);
	void SmoothHistogram(int* src_hist,double* dst_hist,int hist_size,double sigma);
	void LabelHistogram(double* src_hist,int* dst_hist_labeling,int hist_size);
	double GetDepthRange(BYTE* depth_map);
	void FloodFill(BYTE* src,pixel starting_point,int target_label);
	int ConnectedComponentSegmentation(BYTE* segmentation_map);

public:
	void Init(int _w,int _h,int _prefiltering_radius=8,bool _use_adaptive_strength = true,double _static_strength = 0.0);
	void Deinit();
	int GetSegmentationMap(BYTE* src_depth_map,BYTE* dst_segmentation_map);
};
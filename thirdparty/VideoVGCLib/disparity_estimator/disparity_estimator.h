#ifndef VQMT3D_DISPARITY_ESTIMATOR_DISPARITY_ESTIMATOR_H_
#define VQMT3D_DISPARITY_ESTIMATOR_DISPARITY_ESTIMATOR_H_

#include <vector>

#ifndef BYTE
    typedef unsigned char BYTE;
#endif

/** @brief Interface for all disparity map estimation algorithms. 
The interface is designed in a way that temporal-filtering-based 
algorithms are also supported naturally.
*/
class DisparityEstimator
{
public:
	/** @brief Function for processing another frame. Depending
	on the implementation, a certain number of future frames might 
	need to be processed before retrieving the estimated disparity map.

	@param left_view source left view, pixel-interleave RGB24 format.

	@param right_view source right view, pixel-interleave RGB24 format.
	*/
	virtual void ProcessFrame(const std::vector<BYTE>& src_left_view, const std::vector<BYTE>& src_right_view) = 0;

	/** @brief Getter function for the estimated disparity map.
	
	@param frame_num index of the frame that we want to retrieve. The
	indexing starts with zero for the first frame that was passed to the
	processFrame function.

	@param dst_left_map pre-allocated destination buffer for the left-view 
	disparity map. Two channels in plane-interleave format (all horizontal 
	disparities go first, vertical-disparity plane goes second). The 
	disparity values are multiplied by 4 (1 pixel disparity corresponds to 
	the value of 4).

	@param dst_right_map the same as dst_left_map, but for the right view.
	*/
	virtual void GetDisparityMap(std::vector<short>& dst_left_map, std::vector<short>& dst_right_map) = 0;

	/** @brief Getter function for the estimated confidence map.
	
	@param frame_num index of the frame that we want to retrieve. The
	indexing starts with zero for the first frame that was passed to the
	processFrame function.

	@param dst_left_map pre-allocated destination buffer for the left-view 
	confidence map. The value of 255 corresponds to regions where disparity 
	values are most likely to be correct. The value of 0 corresponds to 
	regions where disparity values are totally unreliable.

	@param dst_right_map the same as dst_left_map, but for the right view.
	*/
	virtual void GetConfidenceMap(std::vector<BYTE>& dst_left_map, std::vector<BYTE>& dst_right_map) = 0;

	/** @brief Getter function for the estimated disparity map without filtering.
	
	@param frame_num index of the frame that we want to retrieve. The
	indexing starts with zero for the first frame that was passed to the
	processFrame function.

	@param dst_left_map pre-allocated destination buffer for the left-view 
	disparity map. Two channels in plane-interleave format (all horizontal 
	disparities go first, vertical-disparity plane goes second). The 
	disparity values are multiplied by 4 (1 pixel disparity corresponds to 
	the value of 4).

	@param dst_right_map the same as dst_left_map, but for the right view.
	*/
	virtual void GetRawDisparityMap(std::vector<short>& dst_left_map, std::vector<short>& dst_right_map) = 0;

	/** @brief Getter function for the estimated confidence map without filtering.
	
	@param frame_num index of the frame that we want to retrieve. The
	indexing starts with zero for the first frame that was passed to the
	processFrame function.

	@param dst_left_map pre-allocated destination buffer for the left-view 
	confidence map. The value of 255 corresponds to regions where disparity 
	values are most likely to be correct. The value of 0 corresponds to 
	regions where disparity values are totally unreliable.

	@param dst_right_map the same as dst_left_map, but for the right view.
	*/
	virtual void GetRawConfidenceMap(std::vector<BYTE>& dst_left_map, std::vector<BYTE>& dst_right_map) = 0;

	virtual ~DisparityEstimator();
};

DisparityEstimator* CreateBasicEstimator(int frame_width, int frame_height);
DisparityEstimator* CreateHierEstimator(int frame_width, int frame_height, bool hier_, bool use_fgs_=true, double fgs_lambda_=8000.0, double fgs_sigma_color_=1.5, int me_warming_frame_num_=7);

#endif // VQMT3D_DISPARITY_ESTIMATOR_DISPARITY_ESTIMATOR_H_

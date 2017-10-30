#ifndef VQMT3D_DISPARITY_ESTIMATOR_DISPARITY_ESTIMATOR_HIER_H_
#define VQMT3D_DISPARITY_ESTIMATOR_DISPARITY_ESTIMATOR_HIER_H_

#include "disparity_estimator.h"

#include <vector>
#include <memory>

#include "MEWrapperBase.h"
#include "ConfidenceEstimator.h"

class DisparityEstimatorBasic: public DisparityEstimator
{
private:
	int w_, h_;
	int wh_;

	bool use_fgs_;
	double fgs_lambda_;
	double fgs_sigma_color_;

	bool is_first_disp_;
	int me_warming_frame_num_;

	std::vector<short> disparity_map_left_;
    std::vector<short> disparity_map_right_;
    std::vector<BYTE> confidence_map_left_;
    std::vector<BYTE> confidence_map_right_;

    std::vector<short> raw_disparity_map_left_;
    std::vector<short> raw_disparity_map_right_;
    std::vector<BYTE> raw_confidence_map_left_;
    std::vector<BYTE> raw_confidence_map_right_;

	std::unique_ptr<IMEWrapper> me_stereo_;

    std::unique_ptr<ConfidenceEstimator> confidence_estimator_;

	void InterleaveBGR2planeBGR(std::vector<BYTE>& dst, const std::vector<BYTE>& src);
public:
    DisparityEstimatorBasic(const int& w_, const int& h_, const bool& use_fgs_=true, const double& fgs_lambda_=8000.0, const double& fgs_sigma_color_=1.5, const int& me_warming_frame_num_=7);
	~DisparityEstimatorBasic();

	void ProcessFrame(const std::vector<BYTE>& src_left_view, const std::vector<BYTE>& src_right_view);
	void GetDisparityMap(std::vector<short>& dst_left_map, std::vector<short>& dst_right_map);
	void GetConfidenceMap(std::vector<BYTE>& dst_left_map, std::vector<BYTE>& dst_right_map);
	void GetRawDisparityMap(std::vector<short>& dst_left_map, std::vector<short>& dst_right_map);
	void GetRawConfidenceMap(std::vector<BYTE>& dst_left_map, std::vector<BYTE>& dst_right_map);
};

#endif // VQMT3D_DISPARITY_ESTIMATOR_DISPARITY_ESTIMATOR_HIER_H_

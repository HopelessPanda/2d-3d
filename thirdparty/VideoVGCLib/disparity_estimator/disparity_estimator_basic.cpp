#include "disparity_estimator_basic.h"

#include "VideoVGCLib/FastWMF/FastGlobalSmoother.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

DisparityEstimatorBasic::DisparityEstimatorBasic(const int& w_, const int& h_, const bool& use_fgs_, const double& fgs_lambda_, const double& fgs_sigma_color_, const int& me_warming_frame_num_)
{
	this->w_ = w_;
	this->h_ = h_;
	wh_ = w_ * h_;

	this->use_fgs_ = use_fgs_;
	this->fgs_lambda_ = fgs_lambda_;
	this->fgs_sigma_color_ = fgs_sigma_color_;
	
	is_first_disp_ = true;
	this->me_warming_frame_num_ = me_warming_frame_num_;
	
	disparity_map_left_ = std::vector<short>(2 * wh_, 0);
	disparity_map_right_ = std::vector<short>(2 * wh_, 0);
	confidence_map_left_ = std::vector<BYTE>(wh_, 0);
	confidence_map_right_ = std::vector<BYTE>(wh_, 0);

    raw_disparity_map_left_ = std::vector<short>(2 * wh_, 0);
    raw_disparity_map_right_ = std::vector<short>(2 * wh_, 0);
    raw_confidence_map_left_ = std::vector<BYTE>(wh_, 0);
    raw_confidence_map_right_ = std::vector<BYTE>(wh_, 0);

    me_stereo_.reset(new MEWrapperBase());
	me_stereo_->Init(w_, h_, true);

    confidence_estimator_.reset(new ConfidenceEstimator(w_, h_, 4));
}

DisparityEstimatorBasic::~DisparityEstimatorBasic()
{
	me_stereo_->Deinit();
}

void DisparityEstimatorBasic::InterleaveBGR2planeBGR(std::vector<BYTE>& dst, const std::vector<BYTE>& src)
{
	int red_shift = 2 * wh_;
	int green_shift = wh_;
	#pragma omp parallel for
	for (int y = 0; y < h_; ++y)
	{
		int y_shift_plain      = y * w_;
		int y_shift_interleave = y * 3 * w_;
		for (int x = 0; x < w_; ++x)
		{
			dst[              y_shift_plain + x] = src[y_shift_interleave + 3 * x];
			dst[green_shift + y_shift_plain + x] = src[y_shift_interleave + 3 * x + 1];
			dst[red_shift   + y_shift_plain + x] = src[y_shift_interleave + 3 * x + 2];
		}
	}
}

void DisparityEstimatorBasic::ProcessFrame(const std::vector<BYTE>& src_left_view, const std::vector<BYTE>& src_right_view)
{
	if (is_first_disp_)
	{
        for (int i = 0; i < me_warming_frame_num_ - 1; ++i)
        {
            me_stereo_->GetMVFields(src_left_view, src_right_view, disparity_map_left_, disparity_map_right_);
        }
        me_stereo_->GetMVFields(src_left_view, src_right_view, disparity_map_left_, disparity_map_right_, &confidence_map_left_, &confidence_map_right_,
                                &raw_disparity_map_left_, &raw_disparity_map_right_, &raw_confidence_map_left_, &raw_confidence_map_right_);

		is_first_disp_ = false;
	}
	else
	{
		me_stereo_->GetMVFields(src_left_view, src_right_view, disparity_map_left_, disparity_map_right_, &confidence_map_left_, &confidence_map_right_,
                                &raw_disparity_map_left_, &raw_disparity_map_right_, &raw_confidence_map_left_, &raw_confidence_map_right_);
	}

	if (use_fgs_)
	{
        std::vector<BYTE> left_view_plane(3 * wh_);
        std::vector<BYTE> right_view_plane(3 * wh_);

        InterleaveBGR2planeBGR(left_view_plane, src_left_view);
        InterleaveBGR2planeBGR(right_view_plane, src_right_view);

		std::unique_ptr<FastGlobalSmoother> fgs(new FastGlobalSmoother());
		fgs->init(left_view_plane.data(), w_, h_, fgs_lambda_, fgs_sigma_color_);
		fgs->filter((void*)(disparity_map_left_.data()), confidence_map_left_.data(), (void*)(disparity_map_left_.data()), fgs_sc16);

		fgs.reset(new FastGlobalSmoother());
		fgs->init(right_view_plane.data(), w_, h_, fgs_lambda_, fgs_sigma_color_);
		fgs->filter((void*)(disparity_map_right_.data()), confidence_map_right_.data(), (void*)(disparity_map_right_.data()), fgs_sc16);

        confidence_estimator_->CalculateConfidence(confidence_map_left_, confidence_map_right_, disparity_map_left_, disparity_map_right_, left_view_plane, right_view_plane);
	}
}

void DisparityEstimatorBasic::GetDisparityMap(std::vector<short>& dst_left_map, std::vector<short>& dst_right_map)
{
    dst_left_map = disparity_map_left_;
    dst_right_map = disparity_map_right_;
}

void DisparityEstimatorBasic::GetConfidenceMap(std::vector<BYTE>& dst_left_map, std::vector<BYTE>& dst_right_map)
{
    dst_left_map = confidence_map_left_;
	dst_right_map = confidence_map_right_;
}

void DisparityEstimatorBasic::GetRawDisparityMap(std::vector<short>& dst_left_map, std::vector<short>& dst_right_map)
{
    dst_left_map = raw_disparity_map_left_;
    dst_right_map = raw_disparity_map_right_;
}

void DisparityEstimatorBasic::GetRawConfidenceMap(std::vector<BYTE>& dst_left_map, std::vector<BYTE>& dst_right_map)
{
    dst_left_map = raw_confidence_map_left_;
    dst_right_map = raw_confidence_map_right_;
}

DisparityEstimator* CreateBasicEstimator(int frame_width, int frame_height, bool use_fgs_, double fgs_lambda_, double fgs_sigma_color_, int me_warming_frame_num_)
{
	DisparityEstimator* res = new DisparityEstimatorBasic(frame_width, frame_height, use_fgs_, fgs_lambda_, fgs_sigma_color_, me_warming_frame_num_);
	
	return res;
}

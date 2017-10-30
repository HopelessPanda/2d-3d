#include "ConfidenceEstimator.h"

#include <algorithm>

ConfidenceEstimator::ConfidenceEstimator(int w, int h, int disp_radius) : dispersion_estimator(new DispersionEstimator(w, h, disp_radius))
{
    w_ = w;
    h_ = h;
}

void ConfidenceEstimator::CalculateConfidence(std::vector<BYTE>& left_confidence, std::vector<BYTE>& right_confidence, const std::vector<short>& mv_left, const std::vector<short>& mv_right,
                                              const std::vector<BYTE>& left_view, const std::vector<BYTE>& right_view, gmcMotionEstimator* me_left, gmcMotionEstimator* me_right)
{
    std::vector<BYTE> dispersion_map_left(w_ * h_);
    std::vector<BYTE> dispersion_map_right(w_ * h_);

    if (me_left == NULL && me_right == NULL)
    {
        dispersion_estimator->CalculateDispersion(dispersion_map_left,  left_view);
        dispersion_estimator->CalculateDispersion(dispersion_map_right, right_view);
    }
    else
    {
        dispersion_estimator->CalculateDispersion(dispersion_map_left,  left_view, me_left);
        dispersion_estimator->CalculateDispersion(dispersion_map_right, right_view, me_right);
    }

    const short* mvx_left  = mv_left.data();
    const short* mvy_left  = mv_left.data() + w_ * h_;
    const short* mvx_right = mv_right.data();
    const short* mvy_right = mv_right.data() + w_ * h_;

    int projected_point_x, projected_point_y;
    float diff_x, diff_y;
    int ind, projected_ind;

    #pragma omp parallel for private(projected_point_x) private(projected_point_y) private(ind) private(projected_ind) private(diff_x) private(diff_y)
    for (int i = 0; i < h_; i++)
    {
        for (int j = 0; j < w_; j++)
        {
            ind = i * w_ + j;

            projected_point_x = (int)(j + 0.25 * mvx_left[ind] + 0.5);
            projected_point_y = (int)(i + 0.25 * mvy_left[ind] + 0.5);
            if (projected_point_x < 0 || projected_point_x > w_ - 1 || projected_point_y < 0 || projected_point_y > h_ - 1)
            {
                left_confidence[ind] = 0;
                continue;
            }
            projected_ind = projected_point_y * w_ + projected_point_x;
            diff_x = 0.25f * (mvx_right[projected_ind] + mvx_left[ind]);
            diff_y = 0.25f * (mvy_right[projected_ind] + mvy_left[ind]);

            left_confidence[ind] = (BYTE)std::min((float)std::min(dispersion_map_left[ind], dispersion_map_right[projected_ind]),
                                                  255.0f - CLIP(LRC_coef_ * ((diff_x * diff_x) / w_ + (diff_y * diff_y) / h_), 0.0f, 255.0f));
        }
    }

    #pragma omp parallel for private(projected_point_x) private(projected_point_y) private(ind) private(projected_ind) private(diff_x) private(diff_y)
    for (int i = 0; i < h_; i++)
    {
        for (int j = 0; j < w_; j++)
        {
            ind = i * w_ + j;

            projected_point_x = (int)(j + 0.25 * mvx_right[ind] + 0.5);
            projected_point_y = (int)(i + 0.25 * mvy_right[ind] + 0.5);
            if (projected_point_x < 0 || projected_point_x > w_ - 1 || projected_point_y < 0 || projected_point_y > h_ - 1)
            {
                right_confidence[ind] = 0;
                continue;
            }
            projected_ind = projected_point_y * w_ + projected_point_x;
            diff_x = 0.25f * (mvx_left[projected_ind] + mvx_right[ind]);
            diff_y = 0.25f * (mvy_left[projected_ind] + mvy_right[ind]);

            right_confidence[ind] = (BYTE)std::min((float)std::min(dispersion_map_right[ind], dispersion_map_left[projected_ind]),
                                                   255.0f - CLIP(LRC_coef_ * ((diff_x * diff_x) / w_ + (diff_y * diff_y) / h_), 0.0f, 255.0f));
        }
    }
}

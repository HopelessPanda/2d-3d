#ifndef VQMT3D_DISPARITY_ESTIMATOR_CONFIDENCE_ESTIMATOR_H_
#define VQMT3D_DISPARITY_ESTIMATOR_CONFIDENCE_ESTIMATOR_H_

#include <vector>
#include <memory>

#include "DispersionEstimator.h"

class ConfidenceEstimator
{
private:
    int w_;
    int h_;
    const float LRC_coef_ = 10000;

    std::unique_ptr<DispersionEstimator> dispersion_estimator;
public:
    ConfidenceEstimator(int w, int h, int disp_radius = 0);

    void CalculateConfidence(std::vector<BYTE>& left_confidence, std::vector<BYTE>& right_confidence, const std::vector<short>& mv_left, const std::vector<short>& mv_right,
                             const std::vector<BYTE>& left_view, const std::vector<BYTE>& right_view, gmcMotionEstimator* me_left = NULL, gmcMotionEstimator* me_right = NULL);
};

#endif // VQMT3D_DISPARITY_ESTIMATOR_CONFIDENCE_ESTIMATOR_H_

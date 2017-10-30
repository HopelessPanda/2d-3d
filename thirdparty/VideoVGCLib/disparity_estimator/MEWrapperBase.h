#ifndef VQMT3D_DISPARITY_ESTIMATOR_ME_WRAPPER_BASE_H_
#define VQMT3D_DISPARITY_ESTIMATOR_ME_WRAPPER_BASE_H_

#include "IMEWrapper.h"
#include "ConfidenceEstimator.h"
#include <vector>
#include <memory>

extern "C" {
#include <VideoVGCLib/MEstimation/cMEstimation.h>
#include <VideoGMLib/VideoImage/VideoImage.h>
};

class MEWrapperBase : public IMEWrapper
{
private:
    int w_, h_, wh_;

    std::unique_ptr<cMEParams> meParams_left_;                //!< Params for ME_ instance for left to right compensation.
    std::unique_ptr<gmcMotionEstimator> meEstimator_left_;    //!< Motion estimation instance for left to right compensation.
    std::unique_ptr<cMEParams> meParams_right_;               //!< Params for ME_ instance for right to left compensation.
    std::unique_ptr<gmcMotionEstimator> meEstimator_right_;   //!< Motion estimation instance for right to left compensation.

    std::unique_ptr<ConfidenceEstimator> confidence_estimator_;
public:
    virtual void Init(int w, int h, bool stereo);
    virtual void Deinit();

    virtual void GetMVFields(const std::vector<BYTE>& left, const std::vector<BYTE>& right,
                             std::vector<short>& left_mv, std::vector<short>& right_mv,
                             std::vector<BYTE>* left_confidence = nullptr, std::vector<BYTE>* right_confidence = nullptr,
                             std::vector<short>* raw_left_mv = nullptr, std::vector<short>* raw_right_mv = nullptr,
                             std::vector<BYTE>* raw_left_confidence = nullptr, std::vector<BYTE>* raw_right_confidence = nullptr);
};

#endif // VQMT3D_DISPARITY_ESTIMATOR_ME_WRAPPER_BASE_H_

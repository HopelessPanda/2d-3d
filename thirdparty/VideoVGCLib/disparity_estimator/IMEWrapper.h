#ifndef VQMT3D_DISPARITY_ESTIMATOR_I_ME_WRAPPER_H_
#define VQMT3D_DISPARITY_ESTIMATOR_I_ME_WRAPPER_H_

#include <vector>

#include <VideoGMLib/Common/CommonTypes.h>

class IMEWrapper
{
public:
    virtual ~IMEWrapper() {}

    virtual void Init(int w, int h, bool stereo) = 0;
    virtual void Deinit() = 0;

    virtual void GetMVFields(const std::vector<BYTE>& left, const std::vector<BYTE>& right,
                             std::vector<short>& left_mv, std::vector<short>& right_mv,
                             std::vector<BYTE>* left_confidence = nullptr, std::vector<BYTE>* right_confidence = nullptr,
                             std::vector<short>* raw_left_mv = nullptr, std::vector<short>* raw_right_mv = nullptr,
                             std::vector<BYTE>* raw_left_confidence = nullptr, std::vector<BYTE>* raw_right_confidence = nullptr) = 0;
};

#endif // VQMT3D_DISPARITY_ESTIMATOR_I_ME_WRAPPER_H_


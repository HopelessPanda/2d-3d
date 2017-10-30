#ifndef VQMT3D_DISPARITY_ESTIMATOR_DISPERSION_ESTIMATOR_H_
#define VQMT3D_DISPARITY_ESTIMATOR_DISPERSION_ESTIMATOR_H_

#include <vector>

extern "C" {
#include <VideoVGCLib/MEstimation/cMEstimation.h>
#include <VideoGMLib/VideoImage/VideoImage.h>
};

class DispersionEstimator
{
private:
    int radius_;
    int w_, h_;
    int wh_;
    int pix_num_;
    int prev_x_, prev_y_;
    int pix_sq_sum_[3];
    int pix_sum_[3];

    double GetPixelBlockDispersion(int cur_x, int cur_y, const std::vector<BYTE>& src);
    void GetBlockDispersion(gmcMotionEstimator* ME_, std::vector<BYTE>& block_dispersion_map, const std::vector<BYTE>& src, int w_, int h_, int x, int y, int block_sz);
public:
    DispersionEstimator(int w, int h, int radius = 0);

    void CalculateDispersion(std::vector<BYTE>& dst_disp, const std::vector<BYTE>& src);
    void CalculateDispersion(std::vector<BYTE>& dst_disp, const std::vector<BYTE>& src, gmcMotionEstimator* ME_);
};

#endif // VQMT3D_DISPARITY_ESTIMATOR_DISPERSION_ESTIMATOR_H_

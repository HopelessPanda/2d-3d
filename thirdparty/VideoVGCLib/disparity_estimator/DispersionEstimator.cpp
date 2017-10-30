#include "DispersionEstimator.h"

DispersionEstimator::DispersionEstimator(int w, int h, int radius)
{
    radius_ = radius;
    w_ = w;
    h_ = h;
    wh_ = w_ * h_;
    pix_num_ = (2 * radius_ + 1) * (2 * radius_ + 1);
}

double DispersionEstimator::GetPixelBlockDispersion(int cur_x, int cur_y, const std::vector<BYTE>& src)
{
    int green_shift = wh_;
    int red_shift = 2 * wh_;

    if ((prev_x_ == -1) && (prev_y_ == -1))
    {
        for (int y = (cur_y - radius_); y <= (cur_y + radius_); ++y)
        {
            int ry;
            if (y < 0)
            {
                ry = -y;
            }
            else if (y >= h_)
            {
                ry = 2 * h_ - 2 - y;
            }
            else
            {
                ry = y;
            }

            int y_shift = ry * w_;
            for (int x = (cur_x - radius_); x <= (cur_x + radius_); ++x)
            {
                int rx;
                if (x < 0)
                {
                    rx = -x;
                }
                else if (x >= w_)
                {
                    rx = 2 * w_ - 2 - x;
                }
                else
                {
                    rx = x;
                }

                pix_sum_[0] += src[y_shift + rx];
                pix_sum_[1] += src[green_shift + y_shift + rx];
                pix_sum_[2] += src[red_shift + y_shift + rx];
                pix_sq_sum_[0] += src[y_shift + rx] * src[y_shift + rx];
                pix_sq_sum_[1] += src[green_shift + y_shift + rx] * src[green_shift + y_shift + rx];
                pix_sq_sum_[2] += src[red_shift + y_shift + rx] * src[red_shift + y_shift + rx];
            }
        }
    }
    else
    {
        int dif_x = cur_x - prev_x_;
        int dif_y = cur_y - prev_y_;

        if (dif_y > 0)
        {
            int neg_y = prev_y_ - radius_;
            int pos_y = cur_y + radius_;
            if (neg_y < 0)
            {
                neg_y = -neg_y;
            }
            if (pos_y >= h_)
            {
                pos_y = 2 * h_ - 2 - pos_y;
            }

            int pos_y_shift = pos_y * w_;
            int neg_y_shift = neg_y * w_;
            for (int x = (cur_x - radius_); x <= (cur_x + radius_); ++x)
            {
                int rx;
                if (x < 0)
                {
                    rx = -x;
                }
                else if (x >= w_)
                {
                    rx = 2 * w_ - 2 - x;
                }
                else
                {
                    rx = x;
                }

                pix_sum_[0] += src[pos_y_shift + rx];
                pix_sum_[1] += src[green_shift + pos_y_shift + rx];
                pix_sum_[2] += src[red_shift + pos_y_shift + rx];
                pix_sq_sum_[0] += src[pos_y_shift + rx] * src[pos_y_shift + rx];
                pix_sq_sum_[1] += src[green_shift + pos_y_shift + rx] * src[green_shift + pos_y_shift + rx];
                pix_sq_sum_[2] += src[red_shift + pos_y_shift + rx] * src[red_shift + pos_y_shift + rx];

                pix_sum_[0] -= src[neg_y_shift + rx];
                pix_sum_[1] -= src[green_shift + neg_y_shift + rx];
                pix_sum_[2] -= src[red_shift + neg_y_shift + rx];
                pix_sq_sum_[0] -= src[neg_y_shift + rx] * src[neg_y_shift + rx];
                pix_sq_sum_[1] -= src[green_shift + neg_y_shift + rx] * src[green_shift + neg_y_shift + rx];
                pix_sq_sum_[2] -= src[red_shift + neg_y_shift + rx] * src[red_shift + neg_y_shift + rx];
            }
        }
        else if (dif_x > 0)
        {
            int neg_x = prev_x_ - radius_;
            int pos_x = cur_x + radius_;
            if (neg_x < 0)
            {
                neg_x = -neg_x;
            }
            if (pos_x >= w_)
            {
                pos_x = 2 * w_ - 2 - pos_x;
            }

            for (int y = (cur_y - radius_); y <= (cur_y + radius_); ++y)
            {
                int ry;
                if (y < 0)
                {
                    ry = -y;
                }
                else if (y >= h_)
                {
                    ry = 2 * h_ - 2 - y;
                }
                else
                {
                    ry = y;
                }

                int y_shift = ry * w_;
                pix_sum_[0] += src[y_shift + pos_x];
                pix_sum_[1] += src[green_shift + y_shift + pos_x];
                pix_sum_[2] += src[red_shift + y_shift + pos_x];
                pix_sq_sum_[0] += src[y_shift + pos_x] * src[y_shift + pos_x];
                pix_sq_sum_[1] += src[green_shift + y_shift + pos_x] * src[green_shift + y_shift + pos_x];
                pix_sq_sum_[2] += src[red_shift + y_shift + pos_x] * src[red_shift + y_shift + pos_x];

                pix_sum_[0] -= src[y_shift + neg_x];
                pix_sum_[1] -= src[green_shift + y_shift + neg_x];
                pix_sum_[2] -= src[red_shift + y_shift + neg_x];
                pix_sq_sum_[0] -= src[y_shift + neg_x] * src[y_shift + neg_x];
                pix_sq_sum_[1] -= src[green_shift + y_shift + neg_x] * src[green_shift + y_shift + neg_x];
                pix_sq_sum_[2] -= src[red_shift + y_shift + neg_x] * src[red_shift + y_shift + neg_x];
            }
        }
        else if (dif_x < 0)
        {
            int neg_x = prev_x_ + radius_;
            int pos_x = cur_x - radius_;

            if (neg_x >= w_)
            {
                neg_x = 2 * w_ - 2 - neg_x;
            }
            if (pos_x < 0)
            {
                pos_x = -pos_x;
            }

            for (int y = (cur_y - radius_); y <= (cur_y + radius_); ++y)
            {
                int ry;
                if (y < 0)
                {
                    ry = -y;
                }
                else if (y >= h_)
                {
                    ry = 2 * h_ - 2 - y;
                }
                else
                {
                    ry = y;
                }

                int y_shift = ry * w_;
                pix_sum_[0] += src[y_shift + pos_x];
                pix_sum_[1] += src[green_shift + y_shift + pos_x];
                pix_sum_[2] += src[red_shift + y_shift + pos_x];
                pix_sq_sum_[0] += src[y_shift + pos_x] * src[y_shift + pos_x];
                pix_sq_sum_[1] += src[green_shift + y_shift + pos_x] * src[green_shift + y_shift + pos_x];
                pix_sq_sum_[2] += src[red_shift + y_shift + pos_x] * src[red_shift + y_shift + pos_x];

                pix_sum_[0] -= src[y_shift + neg_x];
                pix_sum_[1] -= src[green_shift + y_shift + neg_x];
                pix_sum_[2] -= src[red_shift + y_shift + neg_x];
                pix_sq_sum_[0] -= src[y_shift + neg_x] * src[y_shift + neg_x];
                pix_sq_sum_[1] -= src[green_shift + y_shift + neg_x] * src[green_shift + y_shift + neg_x];
                pix_sq_sum_[2] -= src[red_shift + y_shift + neg_x] * src[red_shift + y_shift + neg_x];
            }
        }
    }

    prev_x_ = cur_x;
    prev_y_ = cur_y;

    double sumB = double(pix_sum_[0]) / pix_num_;
    double sumG = double(pix_sum_[1]) / pix_num_;
    double sumR = double(pix_sum_[2]) / pix_num_;
    double sum_squaresB = double(pix_sq_sum_[0]) / pix_num_;
    double sum_squaresG = double(pix_sq_sum_[1]) / pix_num_;
    double sum_squaresR = double(pix_sq_sum_[2]) / pix_num_;

    return sum_squaresB - sumB * sumB + sum_squaresG - sumG * sumG + sum_squaresR - sumR * sumR;
}

void DispersionEstimator::CalculateDispersion(std::vector<BYTE>& dst_disp, const std::vector<BYTE>& src)
{
    const double dispersion_coef = 50.0;

    prev_x_ = -1;
    prev_y_ = -1;
    for (int i = 0; i < 3; ++i)
    {
        pix_sq_sum_[i] = 0;
        pix_sum_[i] = 0;
    }
    int dir = 1;
    for (int y = 0; y < h_; ++y)
    {
        int y_shift = y * w_;
        if (dir > 0)
        {
            for (int x = 0; x < w_; ++x)
            {
                dst_disp[y_shift + x] = (BYTE)(CLIP(dispersion_coef * GetPixelBlockDispersion(x, y, src), 0.0, 255.0));
            }

            dir = -1;
        }
        else
        {
            for (int x = w_ - 1; x >= 0; --x)
            {
                dst_disp[y_shift + x] = (BYTE)(CLIP(dispersion_coef * GetPixelBlockDispersion(x, y, src), 0.0, 255.0));
            }

            dir = 1;
        }
    }
}

void DispersionEstimator::GetBlockDispersion(gmcMotionEstimator* ME_, std::vector<BYTE>& block_dispersion_map, const std::vector<BYTE>& src, int w_, int h_, int x, int y, int block_sz)
{
    const double dispersion_coef = 25.0f;

    int motionx, motiony, motion_diff;
    int iCurBlockSize;
    cGetMotionVector(ME_, x, y, &motionx, &motiony, &motion_diff, &iCurBlockSize);
    int green_shift = wh_;
    int red_shift = 2 * wh_;
    if (iCurBlockSize == block_sz)
    {
        double sumR = 0, sum_squaresR = 0;
        double sumG = 0, sum_squaresG = 0;
        double sumB = 0, sum_squaresB = 0;
        int idx;
        for (int i = y; i < y + block_sz; ++i)
        {
            for (int j = x; j < x + block_sz; ++j)
            {
                idx = i * w_ + j;
                sumR += src[idx + red_shift];
                sumG += src[idx + green_shift];
                sumB += src[idx];
                sum_squaresR += src[idx + red_shift] * src[idx + red_shift];
                sum_squaresG += src[idx + green_shift] * src[idx + green_shift];
                sum_squaresB += src[idx] * src[idx];
            }
        }

        int sz = block_sz*block_sz;
        sumR /= sz;
        sumG /= sz;
        sumB /= sz;
        sum_squaresR /= sz;
        sum_squaresG /= sz;
        sum_squaresB /= sz;

        float dispersion = (float)(sum_squaresR - sumR*sumR +
                                   sum_squaresG - sumG*sumG +
                                   sum_squaresB - sumB*sumB);
        for (int i = y; i < y + block_sz; ++i)
        {
            for (int j = x; j < x + block_sz; ++j)
            {
                block_dispersion_map[i*w_ + j] = (BYTE)CLIP(dispersion_coef * dispersion, 0.0f, 255.0f);
            }
        }
    }
    else
    {
        GetBlockDispersion(ME_, block_dispersion_map, src, w_, h_, x, y, block_sz / 2);
        GetBlockDispersion(ME_, block_dispersion_map, src, w_, h_, x + block_sz / 2, y, block_sz / 2);
        GetBlockDispersion(ME_, block_dispersion_map, src, w_, h_, x, y + block_sz / 2, block_sz / 2);
        GetBlockDispersion(ME_, block_dispersion_map, src, w_, h_, x + block_sz / 2, y + block_sz / 2, block_sz / 2);
    }
}

void DispersionEstimator::CalculateDispersion(std::vector<BYTE>& dst_disp, const std::vector<BYTE>& src, gmcMotionEstimator* ME_)
{
    int iCompStep = ME_->m_pParams->m_MaxBlSize;

    #pragma omp parallel for
    for (int by0 = 0; by0 < h_; by0 += iCompStep)
    {
        for (int bx0 = 0; bx0 < w_; bx0 += iCompStep)
        {
            GetBlockDispersion(ME_, dst_disp, src, w_, h_, bx0, by0, iCompStep);
        }
    }
}

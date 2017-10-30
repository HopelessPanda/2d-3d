#include "MEWrapperBase.h"

#include <algorithm>

void MEWrapperBase::Init(int w, int h, bool stereo)
{
    w_ = w;
    h_ = h;
    wh_ = w_ * h_;

    meParams_left_.reset(new cMEParams);
    meEstimator_left_.reset(new gmcMotionEstimator);
    meParams_right_.reset(new cMEParams);
    meEstimator_right_.reset(new gmcMotionEstimator);

    confidence_estimator_.reset(new ConfidenceEstimator(w_, h_));

    cInitMEParams(meParams_left_.get());
    cInitMEParams(meParams_right_.get());
    meParams_left_->m_PrecMX = QuarterPixel;
    meParams_left_->m_PrecMY = QuarterPixel;
    meParams_left_->m_MaxBlSize = cbt_16x16;
    meParams_left_->m_MinBlSize = cbt_4x4;
    meParams_left_->m_MEAlgorithm = MSU;
    if (stereo)
    {
        meParams_left_->m_MEMetricAlgorithm = METRIC_COLORINDEPENDENT;
        meParams_left_->m_iMaxMotHor = std::max(40, (int)(0.2 * w_));
        meParams_left_->m_iMaxMotVert = (int)(0.04 * h_);
    }
    else 
    {
        meParams_left_->m_MEMetricAlgorithm = METRIC_SAD;
        meParams_left_->m_iMaxMotHor = std::max(40, (int)(0.2 * w_));
        meParams_left_->m_iMaxMotVert = std::max(40, (int)(0.2 * h_));
    }
    meParams_left_->m_advMEParams.m_bConsiderChroma = TRUE;
    meParams_left_->m_advMEParams.m_ChrXMod = crNoReduction;
    meParams_left_->m_advMEParams.m_ChrYMod = crNoReduction;
    cCopyMEParams(meParams_right_.get(), meParams_left_.get());

    cInitME(meEstimator_left_.get());
    cInitME(meEstimator_right_.get());
    cStartME(meEstimator_left_.get(), h_, w_, meParams_left_.get());
    cStartME(meEstimator_right_.get(), h_, w_, meParams_right_.get());
}

void MEWrapperBase::Deinit()
{
    cStopME(meEstimator_left_.get());
    cStopME(meEstimator_right_.get());

    meParams_left_.reset(nullptr);
    meEstimator_left_.reset(nullptr);
    meParams_right_.reset(nullptr);
    meEstimator_right_.reset(nullptr);
    confidence_estimator_.reset(nullptr);
}

void MEWrapperBase::GetMVFields(const std::vector<BYTE>& left, const std::vector<BYTE>& right,
                                std::vector<short>& left_mv, std::vector<short>& right_mv,
                                std::vector<BYTE>* left_confidence, std::vector<BYTE>* right_confidence,
                                std::vector<short>* raw_left_mv, std::vector<short>* raw_right_mv,
                                std::vector<BYTE>* raw_left_confidence, std::vector<BYTE>* raw_right_confidence)
{
    int border = meEstimator_left_->m_iBorderSize;
    int ew = w_ + 2 * border;
    int eh = h_ + 2 * border;
    int ewh = ew*eh;

    std::vector<BYTE> src_left_ext(3 * ewh), src_right_ext(3 * ewh);
    std::vector<BYTE> cmp_left_ext(3 * ewh), cmp_right_ext(3 * ewh);

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            gmcVideoImage_RGB24toYUVConvert(left.data(), src_left_ext.data(), src_left_ext.data() + w_ * h_, src_left_ext.data() + 2 * w_ * h_, w_, h_, 3 * w_, w_);
            cAddRepeatedBordersYUV(cmp_left_ext.data(), src_left_ext.data(), w_, h_, border, border, border, border);
            memcpy(src_left_ext.data(), cmp_left_ext.data(), 3 * ewh);
        }
        #pragma omp section
        {
            gmcVideoImage_RGB24toYUVConvert(right.data(), src_right_ext.data(), src_right_ext.data() + w_ * h_, src_right_ext.data() + 2 * w_ * h_, w_, h_, 3 * w_, w_);
            cAddRepeatedBordersYUV(cmp_right_ext.data(), src_right_ext.data(), w_, h_, border, border, border, border);
            memcpy(src_right_ext.data(), cmp_right_ext.data(), 3 * ewh);
        }
    }

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            cEstimate(meEstimator_left_.get(), cmp_left_ext.data(), src_right_ext.data(), TRUE);
        }
        #pragma omp section
        {
            cEstimate(meEstimator_right_.get(), cmp_right_ext.data(), src_left_ext.data(), TRUE);
        }
    }

    int iCompStep = meEstimator_left_->m_pParams->m_MinBlSize;
    int motionx_left;
    int motionx_right;
    int motiony_left;
    int motiony_right;
    int motion_diff;
    int iCurBlockSize;

    short* mvx_left = left_mv.data();
    short* mvy_left = left_mv.data() + w_ * h_;
    short* mvx_right = right_mv.data();
    short* mvy_right = right_mv.data() + w_ * h_;

    for (int by0 = 0; by0 < h_; by0 += iCompStep)
    {
        for (int bx0 = 0; bx0 < w_; bx0 += iCompStep)
        {
            cGetMotionVector(meEstimator_left_.get(), bx0, by0, &motionx_left, &motiony_left, &motion_diff, &iCurBlockSize);
            cGetMotionVector(meEstimator_right_.get(), bx0, by0, &motionx_right, &motiony_right, &motion_diff, &iCurBlockSize);
            for (int y = by0; y < by0 + iCompStep; y++)
            {
                for (int x = bx0; x < bx0 + iCompStep; x++)
                {
                    mvx_left[y * w_ + x] = (short)motionx_left;
                    mvy_left[y * w_ + x] = (short)motiony_left;
                    mvx_right[y * w_ + x] = (short)motionx_right;
                    mvy_right[y * w_ + x] = (short)motiony_right;
                }
            }
        }
    }

    if (left_confidence != nullptr && right_confidence != nullptr)
    {
        confidence_estimator_->CalculateConfidence(*left_confidence, *right_confidence, left_mv, right_mv, left, right, meEstimator_left_.get(), meEstimator_right_.get());
    }
    if (raw_left_mv != nullptr && raw_right_mv != nullptr && raw_left_confidence != nullptr && raw_right_confidence != nullptr)
    {
        *raw_left_mv = left_mv;
        *raw_right_mv = right_mv;
        if (left_confidence != nullptr && right_confidence != nullptr)
        {
            *raw_left_confidence = *left_confidence;
            *raw_right_confidence = *right_confidence;
        }
    }
}

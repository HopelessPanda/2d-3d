#include "disparity_estimator.h"

#include <vector>

extern "C" {
#include <VideoVGCLib/MEstimation/cMEstimation.h>
#include <VideoGMLib/VideoImage/VideoImage.h>
};

#include <VideoGMLib/Common/CommonTypes.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

/** @brief Straightforward ME_-based disparity 
estimation algorithm with no filtering. 
Confidence maps are computed using LRC
coupled with RGB block-wise variance.
*/
class BasicEstimator : public DisparityEstimator
{
public:
    void ProcessFrame(const std::vector<BYTE>& src_left_view, const std::vector<BYTE>& src_right_view);
    void GetDisparityMap(std::vector<short>& dst_left_map, std::vector<short>& dst_right_map);
    void GetConfidenceMap(std::vector<BYTE>& dst_left_map, std::vector<BYTE>& dst_right_map);
    void GetRawDisparityMap(std::vector<short>& dst_left_map, std::vector<short>& dst_right_map);
    void GetRawConfidenceMap(std::vector<BYTE>& dst_left_map, std::vector<BYTE>& dst_right_map);

	BasicEstimator(int _w,int _h);
	~BasicEstimator();
private:
	cMEParams meParams_left_;                //!< Params for ME_ instance for left to right compensation.
	gmcMotionEstimator meEstimator_left_;    //!< Motion estimation instance for left to right compensation.
	cMEParams meParams_right_;               //!< Params for ME_ instance for right to left compensation.
	gmcMotionEstimator meEstimator_right_;   //!< Motion estimation instance for right to left compensation.

	int w_,h_;
	std::vector<short> motion_field_left; 
    std::vector<short> motion_field_right;
    std::vector<BYTE> confidence_map_left_;
    std::vector<BYTE> confidence_map_right_;
};

BasicEstimator::BasicEstimator(int _w,int _h) : motion_field_left(2 * _w * _h),
                                                motion_field_right(2 * _w * _h),
                                                confidence_map_left_(w_ * h_),
                                                confidence_map_right_(w_ * h_)
{
	w_ = _w;
	h_ = _h;

	cInitMEParams(&meParams_left_);
	cInitMEParams(&meParams_right_);
	meParams_left_.m_PrecMX		      = QuarterPixel;
	meParams_left_.m_PrecMY			  = QuarterPixel;
	meParams_left_.m_MaxBlSize		  = cbt_16x16;
	meParams_left_.m_MinBlSize		  = cbt_4x4;
	meParams_left_.m_MEAlgorithm		  = MSU;
	meParams_left_.m_MEMetricAlgorithm = METRIC_COLORINDEPENDENT;
	meParams_left_.m_iMaxMotHor		  = max(40, (int)(0.16*w_));
	meParams_left_.m_iMaxMotVert		  = max(32, (int)(0.08*h_));
	meParams_left_.m_advMEParams.m_bConsiderChroma = TRUE;
	meParams_left_.m_advMEParams.m_ChrXMod = crNoReduction;
	meParams_left_.m_advMEParams.m_ChrYMod = crNoReduction;
	cCopyMEParams(&meParams_right_, &meParams_left_);

	cInitME(&meEstimator_left_);
	cInitME(&meEstimator_right_);
	cStartME(&meEstimator_left_,  h_, w_, &meParams_left_);
	cStartME(&meEstimator_right_, h_, w_, &meParams_right_);
}

BasicEstimator::~BasicEstimator()
{
	cStopME(&meEstimator_left_);
	cStopME(&meEstimator_right_);
}

void getRGB24BlockDispersion(gmcMotionEstimator* ME_, std::vector<float>& block_dispersion_map, const std::vector<BYTE>& src, int w_, int h_, int x, int y, int block_sz)
{
	const float dispersion_coef = 25.0f;

	int motionx, motiony, motion_diff;
	int iCurBlockSize;
	cGetMotionVector(ME_, x, y, &motionx, &motiony, &motion_diff, &iCurBlockSize);
	if(iCurBlockSize==block_sz)
	{
		double sumR = 0,sum_squaresR = 0;
		double sumG = 0,sum_squaresG = 0;
		double sumB = 0,sum_squaresB = 0;
		int idx;
		for(int i=y;i<y+block_sz;i++)
			for(int j=x;j<x+block_sz;j++)
			{
				idx = 3*(i*w_+j);
				sumR += src[idx];
				sumG += src[idx+1];
				sumB += src[idx+2];
				sum_squaresR += src[idx]  *src[idx];
				sum_squaresG += src[idx+1]*src[idx+1];
				sum_squaresB += src[idx+2]*src[idx+2];
			}

		int sz = block_sz*block_sz;
		sumR/=sz;
		sumG/=sz;
		sumB/=sz;
		sum_squaresR/=sz;
		sum_squaresG/=sz;
		sum_squaresB/=sz;
		
		float dispersion = (float)(sum_squaresR - sumR*sumR +
								   sum_squaresG - sumG*sumG +
								   sum_squaresB - sumB*sumB);
		for(int i=y;i<y+block_sz;i++)
			for(int j=x;j<x+block_sz;j++)
				block_dispersion_map[i*w_+j] = CLIP(dispersion_coef*dispersion,0.0f,255.0f);
	}
	else
	{
		getRGB24BlockDispersion(ME_,block_dispersion_map,src,w_,h_,x			,y,			  block_sz/2);
		getRGB24BlockDispersion(ME_,block_dispersion_map,src,w_,h_,x+block_sz/2,y,			  block_sz/2);
		getRGB24BlockDispersion(ME_,block_dispersion_map,src,w_,h_,x,			 y+block_sz/2,block_sz/2);
		getRGB24BlockDispersion(ME_,block_dispersion_map,src,w_,h_,x+block_sz/2,y+block_sz/2,block_sz/2);
	}
}

void BasicEstimator::ProcessFrame(const std::vector<BYTE>& src_left_view, const std::vector<BYTE>& src_right_view)
{
    // Estimate motion.
    int border = meEstimator_left_.m_iBorderSize;
    int ew = w_ + 2 * border;
    int eh = h_ + 2 * border;
    int ewh = ew*eh;

	std::vector<BYTE> src_left_ext(3 * ewh), src_right_ext(3 * ewh);
    std::vector<BYTE> cmp_left_ext(3 * ewh), cmp_right_ext(3 * ewh);

    #pragma omp parallel sections
    {
        #pragma omp section
		{
			gmcVideoImage_RGB24toYUVConvert(src_left_view.data(), src_left_ext.data(), src_left_ext.data() + w_ * h_, src_left_ext.data() + 2 * w_ * h_, w_, h_, 3 * w_, w_);
			cAddRepeatedBordersYUV(cmp_left_ext.data(), src_left_ext.data(), w_, h_, border, border, border, border);
			src_left_ext = cmp_left_ext;
		}
        #pragma omp section
		{
			gmcVideoImage_RGB24toYUVConvert(src_right_view.data(), src_right_ext.data(), src_right_ext.data() + w_ * h_, src_right_ext.data() + 2 * w_ * h_, w_, h_, 3 * w_, w_);
			cAddRepeatedBordersYUV(cmp_right_ext.data(), src_right_ext.data(), w_, h_, border, border, border, border);
	        src_right_ext = cmp_right_ext;
		}
    }

    #pragma omp parallel sections
    {
        #pragma omp section
		{
			cEstimate(&meEstimator_left_,  cmp_left_ext.data(),  src_right_ext.data(), TRUE);
		}
        #pragma omp section
		{
			cEstimate(&meEstimator_right_, cmp_right_ext.data(), src_left_ext.data(),  TRUE);
		}
    }

	// Extract motion fields.
	int iCompStep = meEstimator_left_.m_pParams->m_MinBlSize;
	int motionx_left;
	int motionx_right;
	int motiony_left;
	int motiony_right;
	int motion_diff; 
	int iCurBlockSize;

	short* mvx_left  = motion_field_left.data();
	short* mvy_left  = motion_field_left.data() + w_ * h_;
	short* mvx_right = motion_field_right.data();
	short* mvy_right = motion_field_right.data() + w_ * h_;

	for(int by0 = 0; by0 < h_; by0 += iCompStep)
	{
		for(int bx0 = 0; bx0 < w_; bx0 += iCompStep)
		{
			cGetMotionVector(&meEstimator_left_ , bx0, by0, &motionx_left , &motiony_left , &motion_diff, &iCurBlockSize);
			cGetMotionVector(&meEstimator_right_, bx0, by0, &motionx_right, &motiony_right, &motion_diff, &iCurBlockSize);
			for (int y = by0; y < by0 + iCompStep; y++)
			{
				for (int x = bx0; x < bx0 + iCompStep; x++)
				{
					mvx_left [y*w_ + x] = (short)motionx_left;
					mvy_left [y*w_ + x] = (short)motiony_left;
					mvx_right[y*w_ + x] = (short)motionx_right;
					mvy_right[y*w_ + x] = (short)motiony_right;
				}
			}
		}
	}

	// Compute confidence maps.
	const float LRC_coef_ = 10000;

	std::vector<float> dispersion_map_left(w_ * h_);
    std::vector<float> dispersion_map_right(w_ * h_);

	iCompStep = meEstimator_left_.m_pParams->m_MaxBlSize;

    #pragma omp parallel for
    for (int by0 = 0; by0 < h_; by0 += iCompStep)
    {
        for (int bx0 = 0; bx0 < w_; bx0 += iCompStep)
        {
            getRGB24BlockDispersion(&meEstimator_left_, dispersion_map_left, src_left_view, w_, h_, bx0, by0, iCompStep);
            getRGB24BlockDispersion(&meEstimator_right_, dispersion_map_right, src_right_view, w_, h_, bx0, by0, iCompStep);
        }
    }

	int projected_point_x,projected_point_y;
	float diff_x,diff_y;
	int ind, projected_ind;

    #pragma omp parallel for private(projected_point_x) private(projected_point_y) private(ind) private(projected_ind) private(diff_x) private(diff_y)
    for (int i = 0; i < h_; ++i)
    {
        for (int j = 0; j < w_; ++j)
        {
            ind = i*w_ + j;

            projected_point_x = (int)(j + 0.25 * mvx_left[ind] + 0.5);
            projected_point_y = (int)(i + 0.25 * mvy_left[ind] + 0.5);
            if (projected_point_x < 0 || projected_point_x > w_ - 1 || projected_point_y < 0 || projected_point_y > h_ - 1)
            {
                confidence_map_left_[ind] = 0;
                continue;
            }
            projected_ind = projected_point_y * w_ + projected_point_x;
            diff_x = 0.25f * (mvx_right[projected_ind] + mvx_left[ind]);
            diff_y = 0.25f * (mvy_right[projected_ind] + mvy_left[ind]);

            confidence_map_left_[ind] = (BYTE)min(min(dispersion_map_left[ind], dispersion_map_right[projected_ind]), 255.0f - CLIP(LRC_coef_*((diff_x*diff_x) / w_ + (diff_y*diff_y) / h_), 0.0f, 255.0f));
        }
    }

    #pragma omp parallel for private(projected_point_x) private(projected_point_y) private(ind) private(projected_ind) private(diff_x) private(diff_y)
    for (int i = 0; i < h_; i++)
    {
        for (int j = 0; j < w_; j++)
        {
            ind = i*w_ + j;

            projected_point_x = (int)(j + 0.25 * mvx_right[ind] + 0.5);
            projected_point_y = (int)(i + 0.25 * mvy_right[ind] + 0.5);
            if (projected_point_x < 0 || projected_point_x > w_ - 1 || projected_point_y < 0 || projected_point_y > h_ - 1)
            {
                confidence_map_right_[ind] = 0;
                continue;
            }
            projected_ind = projected_point_y * w_ + projected_point_x;
            diff_x = 0.25f * (mvx_left[projected_ind] + mvx_right[ind]);
            diff_y = 0.25f * (mvy_left[projected_ind] + mvy_right[ind]);

            confidence_map_right_[ind] = (BYTE)min(min(dispersion_map_right[ind], dispersion_map_left[projected_ind]), 255.0f - CLIP(LRC_coef_*((diff_x*diff_x) / w_ + (diff_y*diff_y) / h_), 0.0f, 255.0f));
        }
    }
}

void BasicEstimator::GetDisparityMap(std::vector<short>& dst_left_map, std::vector<short>& dst_right_map)
{
    dst_left_map = motion_field_left;
    dst_right_map = motion_field_right;
}

void BasicEstimator::GetConfidenceMap(std::vector<BYTE>& dst_left_map, std::vector<BYTE>& dst_right_map)
{
    dst_left_map = confidence_map_left_;
    dst_right_map = confidence_map_right_;
}

void BasicEstimator::GetRawDisparityMap(std::vector<short>& dst_left_map, std::vector<short>& dst_right_map)
{
    dst_left_map = motion_field_left;
    dst_right_map = motion_field_right;
}

void BasicEstimator::GetRawConfidenceMap(std::vector<BYTE>& dst_left_map, std::vector<BYTE>& dst_right_map)
{
    dst_left_map = confidence_map_left_;
    dst_right_map = confidence_map_right_;
}

DisparityEstimator* CreateBasicEstimator(int frame_width, int frame_height)
{
	DisparityEstimator* res = new BasicEstimator(frame_width,frame_height);
	return res;
}

DisparityEstimator::~DisparityEstimator() {}

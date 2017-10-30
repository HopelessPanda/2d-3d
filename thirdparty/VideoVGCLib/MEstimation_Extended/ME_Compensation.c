#include "MExt_CoreInlineFunctions.h"

#include "ME_Compensation.h"
#include <VideoGMLib/VideoImage/VideoImage.h>
#include <math.h>

BYTE GetPixelValueByMotionVector(BYTE *buffer, float x, float y, int width, int height)
{
	float sum, horiz_sum_high, horiz_sum_low;
	int x_ind = (int)floorf(x);
	int y_ind = (int)floorf(y);

	int index_x_high = CLIP(x_ind + 1	, 0, width-1); 
	int index_y_high = CLIP(y_ind + 1	, 0, height-1);
	int index_x_low	 = CLIP(x_ind		, 0, width-1); 
	int index_y_low  = CLIP(y_ind		, 0, height-1);

	int index_x_low_y_low	= index_y_low  * width + index_x_low;
	int index_x_high_y_low	= index_y_low  * width + index_x_high;
	int index_x_low_y_high	= index_y_high * width + index_x_low;
	int index_x_high_y_high	= index_y_high * width + index_x_high;

	float weight_x_high		= x - x_ind;
	float weight_x_low		= 1 - weight_x_high;
	float weight_y_high		= y - y_ind;
	float weight_y_low		= 1 - weight_y_high;

	horiz_sum_high	= weight_x_low * buffer[index_x_low_y_high]	+ weight_x_high * buffer[index_x_high_y_high];
	horiz_sum_low	= weight_x_low * buffer[index_x_low_y_low]	+ weight_x_high * buffer[index_x_high_y_low];
	sum = horiz_sum_high * weight_y_high + horiz_sum_low * weight_y_low;

	return (BYTE)CLIP ( sum, 0.0f, 255.0f );

}

void Compensate_Plane_From_MotionField( cMV *motion_vectors, BYTE * frmPrev, BYTE* frmDst, int width, int height, BOOL pixel_precision )
{
	int bx0, by0;

//#pragma omp parallel for
	for( by0 = 0; by0 < height; ++ by0 )
	{
		for( bx0 = 0; bx0 < width; ++ bx0)
		{
			float motionx = 0.0f;
			float motiony = 0.0f;

			// precision of motion vector field is supposed to be quarter pixel
			motionx =  motion_vectors[by0 * width + bx0].x / 4.0f;
			motiony =  motion_vectors[by0 * width + bx0].y / 4.0f;

			if(pixel_precision)
			{
				motionx = floorf(motionx + 0.5f);
				motiony = floorf(motiony + 0.5f);
			}

			frmDst[by0 * width + bx0] = GetPixelValueByMotionVector(frmPrev, bx0 + motionx, by0 + motiony, width, height);
		}
	}

	return;
}

void Motion_Compensation_Simple(cMV *motion_vectors, BYTE *frm_reference, BYTE *dst, int width, int height, BOOL pixel_precision)
{
	Compensate_Plane_From_MotionField(motion_vectors, frm_reference, dst, width, height, pixel_precision);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// cMV version of compensating

cMV GetPixelValueByMotionVector_cMV(cMV *buffer, double x, double y, int width, int height)
{
	double sum_x, sum_y, horiz_sum_high_x, horiz_sum_high_y, horiz_sum_low_x, horiz_sum_low_y;
	cMV res;

	int index_x_high = CLIP((int)x + 1	, 0, width-1); 
	int index_y_high = CLIP((int)y + 1	, 0, height-1);
	int index_x_low	 = CLIP((int)x		, 0, width-1); 
	int index_y_low  = CLIP((int)y		, 0, height-1);

	int index_x_low_y_low	= index_y_low  * width + index_x_low;
	int index_x_high_y_low	= index_y_low  * width + index_x_high;
	int index_x_low_y_high	= index_y_high * width + index_x_low;
	int index_x_high_y_high	= index_y_high * width + index_x_high;

	double weight_x_high	= x - floor(x);
	double weight_x_low		= 1 - weight_x_high;
	double weight_y_high	= y - floor(y);
	double weight_y_low		= 1 - weight_y_high;

	horiz_sum_high_x	= weight_x_low * buffer[index_x_low_y_high].x	+ weight_x_high * buffer[index_x_high_y_high].x;
	horiz_sum_high_y	= weight_x_low * buffer[index_x_low_y_high].y	+ weight_x_high * buffer[index_x_high_y_high].y;
	horiz_sum_low_x		= weight_x_low * buffer[index_x_low_y_low].x	+ weight_x_high * buffer[index_x_high_y_low].x;
	horiz_sum_low_y		= weight_x_low * buffer[index_x_low_y_low].y	+ weight_x_high * buffer[index_x_high_y_low].y;
	
	sum_x = horiz_sum_high_x * weight_y_high + horiz_sum_low_x * weight_y_low;
	sum_y = horiz_sum_high_y * weight_y_high + horiz_sum_low_y * weight_y_low;

	res.x = (int) floor( sum_x + 0.5 );
	res.y = (int) floor( sum_y + 0.5 );
	res.error = 0;

	return res;
}

void Compensate_Plane_From_MotionField_cMV( cMV *motion_vectors, cMV* frmDst, cMV * frmPrev , int width, int height )
{
	int bx0, by0;

#pragma omp parallel for
	for( by0 = 0; by0 < height; ++ by0 )
	{
		for( bx0 = 0; bx0 < width; ++ bx0)
		{
			double motionx;
			double motiony;

			// precision of motion vector field is supposed to be quarter pixel
			motionx =  motion_vectors[by0 * width + bx0].x / 4.0;
			motiony =  motion_vectors[by0 * width + bx0].y / 4.0;

			frmDst[by0 * width + bx0] = GetPixelValueByMotionVector_cMV(frmPrev, bx0 + motionx, by0 + motiony, width, height);
		}
	}

	return;
}

void Motion_Compensation_cMV(cMV *motion_vectors, cMV *src, cMV *dst, int width, int height)
{
	Compensate_Plane_From_MotionField_cMV(motion_vectors, dst, src, width, height);
}

/**
**************************************************************************
* \brief Motion compensation function
* Compensates y-component of the source frame by given ME, previous frame and RefShifts array
*
* \param me_data		    [IN]    -  ME data structure
* \param mvf			    [IN]    -  field of filtered motion vectors
* \param pRefShiftsY        [IN]    -  RefShifts array
* \param frmDst             [OUT]   -  destination frame without borders
* \param frmPrevExt         [IN]    -  frame with borders to compensate
* 
* \return None
*/
/*void CompensatePlane_Smart( gmcMotionEstimator * me_data, BYTE* pRefShiftsY[4], BYTE* frmDst, BYTE * frmPrevExt  )
{
	int bx0, by0;
	BYTE *pDstY = frmDst;
	int iCompStep = me_data->m_pParams->m_MinBlSize;

	#pragma omp parallel for private(bx0)
	for( by0 = 0; by0 < me_data->m_iHeight; by0 += iCompStep)
	{
		int iCurOffset;
		BYTE bly[256];
		BYTE *pPlY;

		for( bx0 = 0; bx0 < me_data->m_iWidth; bx0 += iCompStep)
		{
			int motionx;
			int motiony;
			int motion_dif; 
			int iCurBlockSize;

			cGetMotionVector( me_data, bx0, by0, &motionx, &motiony, &motion_dif, &iCurBlockSize);

			pPlY = frmPrevExt;

			iCurOffset = me_data->m_iFirstRowOffset + by0 * me_data->m_iExtWidth + bx0;

			if( me_data->m_interAlg == SixTap ) 
				GetBlockH264(&bly[0], iCompStep, iCompStep, iCompStep, pRefShiftsY,
				me_data->m_iExtWidth, me_data->m_iFirstRowOffset, bx0, by0, 
				motionx << (2 - me_data->m_PrecMX), motiony << (2 - me_data->m_PrecMY));
			else
				GetBlock(&bly[0], iCompStep, iCompStep, iCompStep, pPlY + iCurOffset, me_data->m_iExtWidth, 
				motionx, me_data->m_PrecMX, motiony, me_data->m_PrecMY);

			Copy_Block(pDstY, bx0, by0, &bly[0], iCompStep, me_data->m_iWidth);
		}
	}

	return;
}*/

/**
**************************************************************************
* \brief Performs shifting of source frame using field of motion vectors
*
* \param me				[IN]  -  motion estimation data
* \param me_shifts		[IN]  -  image shifts data for internal computations
* \param mvf			[IN]  -  filtered field of motion vectors
* \param src			[IN]  -  source image
* \param dst			[OUT] -  destination shofted image
* \param buf_extended	[IN]  -  buffer for internal computations
* \param width			[IN]  -  image width
* \param height			[IN]  -  image height
*
* \return None
*/
/*void Motion_Compensation_Smart(gmcMotionEstimator *me, RefShiftsData *me_shifts, BYTE *src, BYTE *dst, BYTE *buf_extended, int width, int height)
{
	int me_border = cGetBorderSise(me);

	BOOL me_cons_chroma = me->m_bConsiderChroma;
	me->m_bConsiderChroma = FALSE;

	cAddRepeatedBordersY(buf_extended, src, width, height, me_border, me_border, me_border, me_border);

	dmPrepareEstimation(me, me_shifts, buf_extended);
	dmAfterEstimation(me, me_shifts);
	CompensatePlane_Smart(me, me_shifts->m_pRefShiftsY, dst, buf_extended);

	me->m_bConsiderChroma = me_cons_chroma;
}*/


/*void Motion_Compensation_Smart_3planes(gmcMotionEstimator *me, RefShiftsData *me_shifts, BYTE *src, BYTE *dst, BYTE *buf_extended, int width, int height)
{
	int me_border = cGetBorderSise(me);
	int frame_size		= me->m_iWidth		* me->m_iHeight;
	int frame_size_ext	= me->m_iExtWidth	* me->m_iExtHeight;

	BOOL me_cons_chroma = me->m_bConsiderChroma;
	me->m_bConsiderChroma = TRUE;

	cAddRepeatedBordersYUV(buf_extended, src, width, height, me_border, me_border, me_border, me_border);

	dmPrepareEstimation(me, me_shifts, buf_extended);
	dmAfterEstimation(me, me_shifts);

	CompensatePlane_Smart(me, me_shifts->m_pRefShiftsY,		dst,				buf_extended);
	CompensatePlane_Smart(me, me_shifts->m_pRefShiftsChr1,	dst + frame_size,	buf_extended + frame_size_ext);
	CompensatePlane_Smart(me, me_shifts->m_pRefShiftsChr2,	dst + frame_size*2, buf_extended + frame_size_ext*2);

	me->m_bConsiderChroma = me_cons_chroma;
}*/

/*void Motion_Compensation_LR (gmcMotionEstimator * me_forward, gmcMotionEstimator * me_backward, 
							 RefShiftsData * me_shifts, BYTE * src, BYTE * dst, BYTE *buf_extended, 
							 int width, int height)
{
	// first pass forward
	Motion_Compensation_Smart(me_forward,  me_shifts, src, dst, buf_extended, width, height);

	// return in-place - allowed, because dst is used only to fill bif_extended.
	Motion_Compensation_Smart(me_backward, me_shifts, dst, dst, buf_extended, width, height);
}*/

void Compensate_ForwBackw(cMV *motion_field_for_frame, cMV *motion_field_for_reference_frame, BYTE * frame_to_compensate, BYTE * compensation_temp_buf, BYTE * dst_frame, int width, int height )
{
	// compensation from source frame to the reference frame. 
	// We use reference frame vectors and blocks from the source frame to fill compensated buffer
	Motion_Compensation_Simple( motion_field_for_reference_frame,	frame_to_compensate,	compensation_temp_buf,	width, height, TRUE );

	// compensation from previously compensated frame to the source frame.
	// We use source frame motion vectors, blocks from compensated frame and fill destination buffer, achieving LR-compensation
	Motion_Compensation_Simple( motion_field_for_frame,				compensation_temp_buf,	dst_frame,				width, height, TRUE );
}

// uses maximum from buffer in overlay areas
void Shift_Frame_By_MotionField( cMV * motion_field, BYTE * src_buf, BYTE * mask_buf, BYTE * dst_buf, int width, int height)
{
	int ind_x, ind_y;

	memset(mask_buf, 0, width * height);
	memset(dst_buf,  0, width * height);

	for( ind_y = 0; ind_y < height; ++ ind_y )
	{
		for ( ind_x = 0; ind_x < width; ++ ind_x)
		{
			int index = ind_y * width + ind_x;

			double vector_x = motion_field[index].x / 4.0;
			double vector_y = motion_field[index].y / 4.0;

			int dst_x = (int) CLIP ( floor(ind_x + vector_x + 0.5), 0, width  - 1 );
			int dst_y = (int) CLIP ( floor(ind_y + vector_y + 0.5), 0, height - 1 );
			int dst_index = dst_y * width + dst_x;

			if( ! mask_buf [ dst_index ]  ||  dst_buf[ dst_index ] < src_buf[index])
			{
				mask_buf [ dst_index ] = 255;
				dst_buf  [ dst_index ] = src_buf[index];
			}
		}
	}
}

void CompensateFrame_3Planes( BYTE * dst, cMV * source_to_reference_motion, BYTE * reference, int width, int height )
{
	int frame_size = width * height;

	Motion_Compensation_Simple(source_to_reference_motion, reference,					dst,				width, height, FALSE);
	Motion_Compensation_Simple(source_to_reference_motion, reference + frame_size,		dst + frame_size,	width, height, FALSE);
	Motion_Compensation_Simple(source_to_reference_motion, reference + frame_size*2,	dst + frame_size*2, width, height, FALSE);
}
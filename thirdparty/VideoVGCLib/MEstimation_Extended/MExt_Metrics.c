#include "MExt_Metrics.h"
#include "MExt_CoreInlineFunctions.h"
#include "ME_Compensation.h"
#include "HierarchyME.h"

#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

void LeftRightConsistency(cMV * field_for_left_view, cMV * field_for_right_view, 
						  BYTE * source_frame_left, BYTE * source_frame_right,
						  BYTE * dst_mask_left, BYTE * dst_mask_right,
						  BYTE * dst_sad_left, BYTE * dst_sad_right,
						  int width, int height)
{
	float LRC_METRIC_COEF = 2000.0f;

	float norm_koef = 4.0;
	int frame_size = width * height;
	int ind_x, ind_y;
	memset(dst_mask_left,  0, width * height);
	memset(dst_mask_right, 0, width * height);

#pragma omp parallel for private(ind_x)
	for(ind_y = 0; ind_y < height; ++ ind_y)
	{
		for(ind_x = 0; ind_x < width; ++ ind_x)
		{
			int index = ind_y*width + ind_x;

			//////////////////////////////////////////////////////////////////////////
			/// left view field processing
			// forward vector
			float left_vector_x = field_for_left_view[index].x / norm_koef;
			float left_vector_y = field_for_left_view[index].y / norm_koef;

			// coordinate in right vector field
			int right_x = (int) CLIP( floorf(ind_x + left_vector_x + 0.5f), 0, width  - 1);
			int right_y = (int) CLIP( floorf(ind_y + left_vector_y + 0.5f), 0, height - 1);
			int right_index = right_y * width + right_x;

			// return vector
			float right_vector_x = field_for_right_view[right_index].x / norm_koef;
			float right_vector_y = field_for_right_view[right_index].y / norm_koef;

			// coordinate of return vector in left field
			int left_x = (int) CLIP( floorf(right_x + right_vector_x + 0.5f), 0, width  - 1);
			int left_y = (int) CLIP( floorf(right_y + right_vector_y + 0.5f), 0, height - 1);
			int left_index = left_y * width + left_x;

			//dst_mask_left[index] = (BYTE)CLIP( sqr(left_x - ind_x) + sqr(left_y - ind_y), 0, 255 );
			dst_mask_left[index] = (BYTE)CLIP( sqr(left_x - ind_x)/(float)width * LRC_METRIC_COEF + sqr(left_y - ind_y)/(float)height * LRC_METRIC_COEF, 0, 255 );

			if( dst_sad_left  &&  source_frame_left )
			{
				int sad_value = abs( source_frame_left[index]					- source_frame_left[left_index]  ) + 
					abs( source_frame_left[index + frame_size]		- source_frame_left[left_index + frame_size]  ) +
					abs( source_frame_left[index + frame_size*2]	- source_frame_left[left_index + frame_size*2]  );

				dst_sad_left[index] = (BYTE) CLIP (sad_value, 0, 255);
			}	

			//////////////////////////////////////////////////////////////////////////
			/// right view processing
			// forward vector
			right_vector_x = field_for_right_view[index].x / norm_koef;
			right_vector_y = field_for_right_view[index].y / norm_koef;

			// coordinate in left vector field
			left_x = (int) CLIP( floorf(ind_x + right_vector_x + 0.5f), 0, width  - 1);
			left_y = (int) CLIP( floorf(ind_y + right_vector_y + 0.5f), 0, height - 1);
			left_index = left_y * width + left_x;

			// return vector
			left_vector_x = field_for_left_view[left_index].x / norm_koef;
			left_vector_y = field_for_left_view[left_index].y / norm_koef;

			// coordinate of return in right field
			right_x = (int) CLIP( floorf(left_x + left_vector_x + 0.5f), 0, width  - 1);
			right_y = (int) CLIP( floorf(left_y + left_vector_y + 0.5f), 0, height - 1);
			right_index = right_y * width + right_x;

			dst_mask_right[index] = (BYTE)CLIP( sqr(right_x - ind_x)/(float)width * LRC_METRIC_COEF + sqr(right_y - ind_y)/(float)height * LRC_METRIC_COEF, 0, 255 );

			if( dst_sad_right  &&  source_frame_right )
			{
				int sad_value = abs( source_frame_right[index]					- source_frame_right[right_index]  ) + 
					abs( source_frame_right[index + frame_size]		- source_frame_right[right_index + frame_size]  ) +
					abs( source_frame_right[index + frame_size*2]	- source_frame_right[right_index + frame_size*2]  );

				dst_sad_right[index] = (BYTE) CLIP (sad_value, 0, 255);
			}	
		}
	}
}

void BufferStorage_LeftRightConsistency( Buffer_Storage * frame_buffers_left, Buffer_Storage * frame_buffers_right, 
										 cMV_buffer_type motion_field_type, BYTE_buffer_type lrc_result_type )
{
#define GET_L_BYTE(buf_type) Buffer_Storage_GetBuffer_BYTE ( frame_buffers_left , buf_type ) 
#define GET_R_BYTE(buf_type) Buffer_Storage_GetBuffer_BYTE ( frame_buffers_right, buf_type ) 

#define GET_L_cMV(buf_type) Buffer_Storage_GetBuffer_cMV ( frame_buffers_left , buf_type ) 
#define GET_R_cMV(buf_type) Buffer_Storage_GetBuffer_cMV ( frame_buffers_right, buf_type ) 

	LeftRightConsistency (  GET_L_cMV  (motion_field_type), GET_R_cMV  (motion_field_type), NULL, NULL, 
							GET_L_BYTE (lrc_result_type),   GET_R_BYTE (lrc_result_type),   NULL, NULL, 
							frame_buffers_left->width, frame_buffers_left->height );

}

// summarizes existing values in mask with new SAD values
void Calculate_Frames_Difference(BYTE * buffer1, BYTE * buffer2, BYTE * dst_sad_buf, int width, int height)
{
	int index;
	int frame_size = width * height;

	for (index = 0; index < frame_size; ++ index)
	{
		dst_sad_buf[index] = abs( buffer1[index] - buffer2[index] );
	}
}

void Calculate_Frames_Difference_SeveralPlanes ( BYTE * buffer_1, BYTE * buffer_2, BYTE * dst_sad, int width, int height, int plane_num )
{
	int index;
	int frame_size = width * height;

	for ( index = 0; index < plane_num ; ++ index )
	{
		Calculate_Frames_Difference(	buffer_1 + index*frame_size, 
			buffer_2 + index*frame_size, 
			dst_sad  + index*frame_size,
			width, height );
	}
}

// summarizes existing values in mask with new SAD values
void Calculate_Frames_SAD_Sum(BYTE * buffer1, BYTE * buffer2, BYTE * dst_sad_buf, int width, int height)
{
	int index;
	int frame_size = width * height;

#pragma omp parallel for
	for (index = 0; index < frame_size; ++ index)
	{
		dst_sad_buf[index] = CLIP ( dst_sad_buf[index] + abs( buffer1[index] - buffer2[index] ), 0, 255 );
	}
}

void Calculate_Frames_Difference_BlockBased (BYTE * frame_left, BYTE * frame_right, BYTE * dst_sad_mask, int width, int height, int plane_num, int base_block_size)
{
	int frame_size = width * height;
	int plane_index;

	memset(dst_sad_mask,  0, frame_size);

	for( plane_index = 0; plane_index < plane_num; ++ plane_index )
	{
		Calculate_Frames_SAD_Sum(frame_left,  frame_right,  dst_sad_mask,  width, height);
	}

	if (base_block_size)
	{
		int ind_x, ind_y;
		int block_area = base_block_size * base_block_size;
		int frame_size = width * height;

		for (ind_y = 0; ind_y < height; ind_y += base_block_size)
		{
			for (ind_x = 0; ind_x < width; ind_x += base_block_size)
			{
				int index = ind_y * width + ind_x ;

				BYTE * cur_mask_pointer	= dst_sad_mask + index;

				int internal_x, internal_y;
				int block_sum = 0;

				for (internal_y = 0; internal_y < base_block_size; ++internal_y)
				{
					for (internal_x = 0; internal_x < base_block_size; ++ internal_x)
					{
						block_sum += cur_mask_pointer[internal_x];
					}
					cur_mask_pointer += width;
				}

				block_sum /= block_area;

				Set_Block_Value(dst_sad_mask, ind_x, ind_y, width, CLIP(block_sum, 0, 255), base_block_size);
			}
		}
	}
}

void Calculate_SAD_Error(cMV * motion_field, BYTE * src_frame, BYTE * reference_frame, BYTE * dst_sad_error_frame, int width, int height)
{
	//////////////////////////////////////////////////////////////////////////
	// Compensate frame from reference
	Motion_Compensation_Simple(motion_field, reference_frame, dst_sad_error_frame, width, height, FALSE );

	//////////////////////////////////////////////////////////////////////////
	// Calculate SAD
	Calculate_Frames_Difference(dst_sad_error_frame, src_frame, dst_sad_error_frame, width, height);
}

void Calculate_SAD_Error_3planes(cMV * motion_field, BYTE * src_frame, BYTE * reference_frame, BYTE *compensation_temp_buf, BYTE * dst_sad_error_frame, int width, int height)
{
	int plane_index;
	int frame_size = width * height;

	memset(dst_sad_error_frame, 0, frame_size);

	for(plane_index = 0; plane_index < 3; ++plane_index)
	{
		//////////////////////////////////////////////////////////////////////////
		// Compensate frame from reference
		Motion_Compensation_Simple(motion_field, reference_frame, compensation_temp_buf, width, height, FALSE );

		//////////////////////////////////////////////////////////////////////////
		// Calculate SAD
		Calculate_Frames_SAD_Sum(compensation_temp_buf, src_frame, dst_sad_error_frame, width, height);

		reference_frame += frame_size;
		src_frame		+= frame_size;
	}
}

void Create_Refine_Texture_Mask_DepthBased ( BYTE * source_contrast, BYTE * source_dispersion, BYTE* depth_sobel_mask, BYTE * lrc, BYTE * dst_mask_uniform, int width, int height )
{
	int LRC_FULL_TRUST_COEF = 10;
	int frame_size = width * height;
	int index;

	float max_gradient = 0;

	for ( index = 0; index < frame_size; ++ index)
	{
		if(max_gradient < depth_sobel_mask[index] )
			max_gradient = depth_sobel_mask[index];
	}

	// to avoid division by zero
	max_gradient = MAX( max_gradient, 4.0f );

#pragma omp parallel for
	for ( index = 0; index < frame_size; ++ index)
	{
		int contrast_lrc_weight = source_contrast[index] * lrc[index];
		float gradient_weight	= ( depth_sobel_mask[index] + max_gradient/4 ) / ( max_gradient/2 );

		dst_mask_uniform[index] = (BYTE)(255 - CLIP( contrast_lrc_weight * gradient_weight / 128 , 0, 255 ) );
	}
}


void Calculate_GradientMask( BYTE * depth_map, BYTE * dst_gradient_map, ushort width, ushort height, BYTE sobel_radius, BYTE threshold)
{
	ushort ind_x, ind_y;
	BYTE kernel_pitch = 2*sobel_radius + 1;
	BYTE * circle_kernel;

	circle_kernel = GenerateCircleKernel_BYTE( (BYTE)sobel_radius );

	for ( ind_y = 0; ind_y < height; ++ind_y )
	{
		for ( ind_x = 0; ind_x < width; ++ind_x )
		{
			ushort start_internal_ind_x				= MAX( ind_x - sobel_radius, 0);
			ushort internal_ind_x, internal_ind_y	= MAX( ind_y - sobel_radius, 0);
			BYTE kernel_ind_x, start_kernel_ind_x	= start_internal_ind_x - ind_x + sobel_radius;
			BYTE kernel_ind_y						= internal_ind_y - ind_y + sobel_radius;

			ushort	end_internal_ind_x = MIN(ind_x + sobel_radius, width-1 ),
					end_internal_ind_y = MIN(ind_y + sobel_radius, height-1 );

			BYTE max_in_neighborhood = 0;
			BYTE min_in_neighborhood = 0xff;
			BYTE depth_val = depth_map [ ind_y * width + ind_x ];

			BYTE * cur_kernel = circle_kernel + kernel_pitch * kernel_ind_y;
			BYTE * cur_depth  = depth_map	  + width		 * internal_ind_y;

			for( ; internal_ind_y <= end_internal_ind_y ; ++internal_ind_y )
			{
				for( internal_ind_x		= start_internal_ind_x,
					 kernel_ind_x		= start_kernel_ind_x; 
					internal_ind_x <= end_internal_ind_x ;
					++internal_ind_x, ++kernel_ind_x  )
				{
					if( cur_kernel[kernel_ind_x] )
					{
						if( max_in_neighborhood < cur_depth[internal_ind_x] )
							max_in_neighborhood = cur_depth[internal_ind_x];
						if( min_in_neighborhood > cur_depth[internal_ind_x] )
							min_in_neighborhood = cur_depth[internal_ind_x];
					}
				}

				cur_depth  += width;
				cur_kernel += kernel_pitch;
			}
			dst_gradient_map[ ind_y * width + ind_x ] = 
				(max_in_neighborhood - min_in_neighborhood > threshold) ? max_in_neighborhood - min_in_neighborhood : 0;
		}
	}

	DestroyCircleKernel_BYTE( & circle_kernel );
}

void Create_Refine_Texture_Mask ( BYTE * source_contrast, BYTE * source_dispersion, BYTE * lrc, BYTE * dst_mask_uniform, int width, int height )
{
	int LRC_FULL_TRUST_COEF = 10;
	int frame_size = width * height;
	int index;

#pragma omp parallel for
	for ( index = 0; index < frame_size; ++ index)
	{
		dst_mask_uniform[index] = 255 - CLIP( source_contrast[index] * lrc[index] / 128, 0, 255 );
	}
}

void Create_Refine_NonTexture_Mask ( BYTE * source_contrast, BYTE * source_dispersion, BYTE * lrc, BYTE * dst_mask_uniform, int width, int height )
{
	int CONTRAST_REVERSE_VAL = 128;
	int REVERSE_CONTRAST_VAL = 255 - CONTRAST_REVERSE_VAL;

	int frame_size = width * height;
	int index;

#pragma omp parallel for
	for ( index = 0; index < frame_size; ++ index)
	{
		int contrast_val = CLIP ( 255 - source_contrast[index] * 255/REVERSE_CONTRAST_VAL , 0, 255 );

		dst_mask_uniform[index] = 255 - CLIP( 2 * contrast_val * lrc[index] / CONTRAST_REVERSE_VAL, 0, 255 );
	}
}

void Create_Invert_LRC_Mask ( BYTE * lrc, BYTE * dst_mask, int width, int height )
{
	int LRC_FULL_TRUST_COEF = 15;
	int frame_size = width * height;
	int index;

#pragma omp parallel for
	for ( index = 0; index < frame_size; ++ index)
	{
		dst_mask[index] = CLIP(255 - (lrc[index] - LRC_FULL_TRUST_COEF) * 3, 0, 255 );
	}
}

void Create_Refine_Mask( BYTE * source_contrast, BYTE *sad_contrast, BYTE * source_dispersion, BYTE * lrc, BYTE * dst_mask_uniform, int width, int height, int filter_strength )
{
	int LRC_FULL_TRUST_COEF = 10;
	int frame_size = width * height;
	int index;

#pragma omp parallel for
	for ( index = 0; index < frame_size; ++ index)
	{
		int thresh_low_dispersion		= (filter_strength+2) / 3;
		int thresh_low_sad				= (filter_strength+1) / 3;
		int weight_low_dispersion		= Get_Linear_Function_Value(source_dispersion[index], thresh_low_dispersion+1, thresh_low_dispersion + 5);
		int weight_for_sad_dispersion	= Get_Linear_Function_Value(source_dispersion[index], 4, 10);
		int low_sad_weight				= Get_Linear_Function_Value(sad_contrast[index]		, thresh_low_sad+1, thresh_low_sad + 5);

		int low_sad_penalty				= MAX(low_sad_weight, 1) * MAX(weight_for_sad_dispersion, 1);
		int total_penalty				= MAX(weight_low_dispersion * low_sad_penalty, 1) / 255;

		int downgrade_filter_strength	= (filter_strength < 0) ? abs(filter_strength) : 1 ;

		int final_contrast_val	= source_contrast[index] * MIN(total_penalty, 255)/ ( 255 * MAX(filter_strength / 3, 1) );
		int lrc_cur_val			= MAX(lrc[index] , 1);

		int filter_mask_val		= final_contrast_val * final_contrast_val * downgrade_filter_strength / lrc_cur_val ; 

		dst_mask_uniform[index] = CLIP(filter_mask_val, 0, 255);
	}
}

void Calculate_Disparity_From_MotionField( cMV * src_field, BYTE * dst_disparity, int width, int height, double d_mult)
{
	int ind_x, ind_y;

	for (ind_y = 0; ind_y < height; ++ ind_y)
	{
		for (ind_x = 0; ind_x < width; ++ ind_x)
		{
			int index = ind_y * width + ind_x;
			double disparity_value = 128 - src_field[index].x * d_mult;
			//double disparity_value = (src_field[index].x * src_field[index].x + src_field[index].y * src_field[index].y) * d_mult;

			dst_disparity[index] = (BYTE) CLIP( disparity_value, 0, 255 );
		}
	}
}

void Get_Errors_From_MotionField( cMV * src_field, BYTE * dst_error_mask, int width, int height, double norm_koef)
{
	int ind_x, ind_y;

	for (ind_y = 0; ind_y < height; ++ ind_y)
	{
		for (ind_x = 0; ind_x < width; ++ ind_x)
		{
			int index = ind_y * width + ind_x;

			dst_error_mask[index] = (BYTE) CLIP( src_field[index].error / norm_koef, 0, 255 );
		}
	}
}

//cMV Calc_Block_Dispersion(cMacroBlock *cMB)
//{
//	cMV result;
//	int count = 0;
//	long disp = 0;
//
//	int i, j, k;
//
//	cInitMV(&result);
//
//	// one vector - nothing to do, return
//	if (cMB->m_BlockPartition == cbt_16x16)
//	{
//		result = cMB->m_DataMV[0];
//		result.error = 0;
//		return result;
//	}
//	else if (cMB->m_BlockPartition == cbt_8x8)
//	{
//		for( i = 0, k=0; i < 4; i++ , k+=4)
//		{
//			if (cMB->m_SubBlockPartition[i] == cbt_4x4)
//			{
//				for(j = k; j < k+4; j++)
//				{
//					count++;
//					result.x += cMB->m_DataMV[j].x;
//					result.y += cMB->m_DataMV[j].y;
//					disp	 += sqr(cMB->m_DataMV[j].x) + sqr(cMB->m_DataMV[j].y);
//				}
//			}
//			else
//			{
//				count++;
//				result.x += cMB->m_DataMV[k].x;
//				result.y += cMB->m_DataMV[k].y;
//				disp	 += sqr(cMB->m_DataMV[k].x) + sqr(cMB->m_DataMV[k].y);
//			}
//		}
//	}
//
//	result.x /= count;
//	result.y /= count;
//
//	result.error = disp/count - (sqr(result.x) + sqr(result.y));
//
//	return result;
//}
//
// returns TRUE if SAD was too big
// 
#define COLOR_DROP_THRESH 1234
BOOL Check_Block_SAD(BYTE * block_p, int block_size, int stride)
{
    pGetBlockError comp_func = (block_size == 4) ? GetErrorSAD_4x4_MMX : GetErrorSAD_8x8_MMX;
	BYTE mult = (block_size == 4) ? 16 : 4;

	if (comp_func(block_p, block_p + block_size, stride, NULL)*mult > COLOR_DROP_THRESH*5/2 )
	{
		return TRUE;
	}
	if (comp_func(block_p, block_p + block_size*stride, stride, NULL)*mult > COLOR_DROP_THRESH*5/2  )
	{
		return TRUE;
	}
	if (comp_func(block_p, block_p + block_size*stride + block_size, stride, NULL)*mult > COLOR_DROP_THRESH*5/2  )
	{
		return TRUE;
	}
	if (comp_func(block_p + block_size, block_p + block_size*stride, stride, NULL)*mult > COLOR_DROP_THRESH*5/2  )
	{
		return TRUE;
	}
	if (comp_func(block_p + block_size, block_p + block_size*stride + block_size, stride, NULL)*mult > COLOR_DROP_THRESH*5/2  )
	{
		return TRUE;
	}
	if (comp_func(block_p + block_size*stride, block_p + block_size*stride + block_size, stride, NULL)*mult > COLOR_DROP_THRESH*5/2  )
	{
		return TRUE;
	}

	return FALSE;
}
/// version, which considers blocks color
cMV Calc_Block_Dispersion_Color(cMacroBlock *cMB, BYTE *src, int x, int y, int stride)
{
	cMV result;
	int count = 0;
	long disp = 0;
	BOOL color_dep = FALSE;

	BYTE *block_p = &src[y*stride + x];

	int i, j, k;

	cInitMV(&result);

	// one vector - nothing to do, return
	if (cMB->m_BlockPartition == cbt_16x16)
	{
		result = cMB->m_DataMV[0];
		result.error = 0;
		return result;
	}
	else if (cMB->m_BlockPartition == cbt_8x8)
	{
		if(Check_Block_SAD(block_p, 8, stride))
			color_dep = TRUE;

		for( i = 0, k=0; i < 4; i++ , k+=4)
		{
			block_p = &src[ ( y + (i/2)*8 )*stride   +   x+(i%2)*8];

			if (cMB->m_SubBlockPartition[i] == cbt_4x4)
			{
				if( Check_Block_SAD(block_p, 4, stride) )
				{
					color_dep = TRUE;
				}

				for(j = k; j < k+4; j++)
				{
					count++;
					result.x += cMB->m_DataMV[j].x;
					result.y += cMB->m_DataMV[j].y;
					disp	 += sqr(cMB->m_DataMV[j].x) + sqr(cMB->m_DataMV[j].y);
				}
				
			}
			else
			{
				count++;
				result.x += cMB->m_DataMV[k].x;
				result.y += cMB->m_DataMV[k].y;
				disp	 += sqr(cMB->m_DataMV[k].x) + sqr(cMB->m_DataMV[k].y);
			}
		}
	}

	if(count == 0)
	{
		return result;
	}
	result.x /= count;
	result.y /= count;

	if(color_dep)
		result.error = -1;
	else
		result.error = disp/count - (sqr(result.x) + sqr(result.y));

	return result;
}

// returns dispresion threshold
int Calculate_MVDispersion_Map(gmcMotionEstimator *me_inst, BYTE *src, BYTE *map, BOOL consider_color)
{
	int i, j, k, l;
	int disp_radius = 3;
	int color_drop_thresh = COLOR_DROP_THRESH;

	int w =  me_inst->m_iWidth;
	int h =  me_inst->m_iHeight;
	//int size, curBlockIndex;

	int num_blocks_hor = me_inst->m_iMBlocksHor;
	int num_blocks_vert = me_inst->m_iMBlocksVert;

	cMV *block_dispersion_map;
	BYTE *dispersion_map;

	int max_disp, av_disp;

	INIT_MEMORY_C(block_dispersion_map, 0, num_blocks_hor*num_blocks_vert, cMV);
	INIT_MEMORY_C(dispersion_map, 0, num_blocks_hor*num_blocks_vert, BYTE);

	for(i = 0; i < num_blocks_vert; i++)
	{
		for(j = 0; j < num_blocks_hor; j++)
		{
			cMacroBlock* cMB = &me_inst->m_CurMBlocks[i*num_blocks_hor + j];

			//block_dispersion_map[i*num_blocks_hor + j] = Calc_Block_Dispersion(cMB);

			block_dispersion_map[i*num_blocks_hor + j] = 
				Calc_Block_Dispersion_Color(cMB, src, j*ME_MACROBLOCK_SIZE, i*ME_MACROBLOCK_SIZE, w);
		}
	}

	for(i = 0; i < num_blocks_vert; i++)
	{
		for(j = 0; j < num_blocks_hor; j++)
		{
			///*
			BYTE *cur_block = &src[i*ME_MACROBLOCK_SIZE*w + j*ME_MACROBLOCK_SIZE];

			cMV dispersion_vector;
			int count = 0;
			long disp = 0;
			cInitMV(&dispersion_vector);

			for(k = MAX(0, i-disp_radius); k < MIN(num_blocks_vert, i+disp_radius); k++)
			{
				for (l = MAX(0, j-disp_radius); l < MIN(num_blocks_hor, j+disp_radius); l++)
				{
					BYTE *ref_block = &src[k*ME_MACROBLOCK_SIZE*w + l*ME_MACROBLOCK_SIZE];

					cMV cur_vec = block_dispersion_map[k*num_blocks_hor + l];

                    int color_dif = GetErrorSAD_16x16_MMX(cur_block, ref_block, w, NULL);

					if(color_dif > color_drop_thresh)
					{	
						continue;
					}
					if ((color_dif != 0) && (cur_vec.error == -1))
					{
						continue;
					}

					count ++;
					dispersion_vector.x += cur_vec.x;
					dispersion_vector.y += cur_vec.y;
					disp += sqr(cur_vec.x) + sqr(cur_vec.y);

					// consider dispersion of block itself
					disp += cur_vec.error;
				}
			}

			dispersion_vector.x /= count;
			dispersion_vector.y /= count;
			disp /= count;

			dispersion_vector.error = disp - ( sqr(dispersion_vector.x) + sqr(dispersion_vector.y) ); 

			dispersion_map[i*num_blocks_hor + j] = (BYTE)CLIP(sqrt(dispersion_vector.error)*2, 0, 255);

			Set_Block_Value(map, j * ME_MACROBLOCK_SIZE, i * ME_MACROBLOCK_SIZE, w, (int)CLIP(sqrt(dispersion_vector.error)*2, 0, 255), 16);
			//*/

			/*
			BYTE *cur_block = &src[i*ME_MACROBLOCK_SIZE*w + j*ME_MACROBLOCK_SIZE];

			double dispersion_vector_x = 0;
			double dispersion_vector_y = 0;
			double dispersion_vector_error = 0;

			double count = 0;
			double disp = 0;
			//cInitMV(&dispersion_vector);

			for(k = MAX(0, i-disp_radius); k < MIN(num_blocks_vert, i+disp_radius); k++)
			{
				for (l = MAX(0, j-disp_radius); l < MIN(num_blocks_hor, j+disp_radius); l++)
				{
					BYTE *ref_block = &src[k*ME_MACROBLOCK_SIZE*w + l*ME_MACROBLOCK_SIZE];

					cMV cur_vec = block_dispersion_map[k*num_blocks_hor + l];

					double color_dif = GetErrorSAD_16x16_SSE_Intrin(cur_block, ref_block, w, NULL) / 100.0;

					if(color_dif == 0 || color_dif > color_drop_thresh)
					{	
						continue;
					}

					count += 1/color_dif;
					dispersion_vector_x += cur_vec.x / color_dif;
					dispersion_vector_y += cur_vec.y / color_dif;
					disp += ( sqr(cur_vec.x) + sqr(cur_vec.y) ) / color_dif;

					// consider dispersion of block itself
					disp += cur_vec.error / color_dif;
				}
			}

			if(count != 0)
			{
				dispersion_vector_x /= count;
				dispersion_vector_y /= count;
				disp /= count;
			}

			dispersion_vector_error = disp - ( sqr(dispersion_vector_x) + sqr(dispersion_vector_y) ); 

			dispersion_map[i*num_blocks_hor + j] = (BYTE)CLIP(sqrt(dispersion_vector_error)*2, 0, 255);

			Set_Block_Value(map, j * ME_MACROBLOCK_SIZE, i * ME_MACROBLOCK_SIZE, w, (int)CLIP(sqrt(dispersion_vector_error)*2, 0, 255), 16);
			*/
		}
	}

	// maximum and average dispersion calculation
	av_disp = max_disp = 0;
	for(i = 0; i < num_blocks_vert; i++)
	{
		for(j = 0; j < num_blocks_hor; j++)
		{
			int index = i*num_blocks_hor + j;
			if(dispersion_map[index] > max_disp)
				max_disp = dispersion_map[index];

			av_disp += dispersion_map[index];
		}
	}
	av_disp /= num_blocks_hor * num_blocks_vert;

	DEINIT_MEMORY_C(block_dispersion_map);
	DEINIT_MEMORY_C(dispersion_map);

	return MIN(av_disp*5/4, max_disp*3/4);
}

void Calculate_GradientDirections( BYTE * source_image, BYTE * dst_gradient_map, ushort width, ushort height, BYTE sobel_radius, int threshold )
{
	ushort ind_x, ind_y;
	BYTE kernel_pitch = 2*sobel_radius + 1;
	BYTE * circle_kernel;

	circle_kernel = GenerateCircleKernel_BYTE( (BYTE)sobel_radius );

	for ( ind_y = 0; ind_y < height; ++ind_y )
	{
		for ( ind_x = 0; ind_x < width; ++ind_x )
		{
			ushort start_internal_ind_x				= MAX( ind_x - sobel_radius, 0);
			ushort internal_ind_x, internal_ind_y	= MAX( ind_y - sobel_radius, 0);
			BYTE kernel_ind_x, start_kernel_ind_x	= start_internal_ind_x - ind_x + sobel_radius;
			BYTE kernel_ind_y						= internal_ind_y - ind_y + sobel_radius;

			ushort	end_internal_ind_x = MIN(ind_x + sobel_radius, width-1 ),
				end_internal_ind_y = MIN(ind_y + sobel_radius, height-1 );

			BYTE max_in_neighborhood = 0;
			BYTE min_in_neighborhood = 0xff;

			ushort max_x_index, max_y_index;
			ushort min_x_index, min_y_index;

			BYTE * cur_kernel		 = circle_kernel  + kernel_pitch * kernel_ind_y;
			BYTE * cur_source_image  = source_image	  + width		 * internal_ind_y;

			float angle = 0;

			for( ; internal_ind_y <= end_internal_ind_y ; ++internal_ind_y )
			{
				for( internal_ind_x		= start_internal_ind_x,
					kernel_ind_x		= start_kernel_ind_x; 
					internal_ind_x <= end_internal_ind_x ;
				++internal_ind_x, ++kernel_ind_x  )
				{
					if( cur_kernel[kernel_ind_x] )
					{
						if( max_in_neighborhood < cur_source_image[internal_ind_x] )
						{
							max_in_neighborhood = cur_source_image[internal_ind_x];
							max_x_index = internal_ind_x;
							max_y_index = internal_ind_y;
						}
						if( min_in_neighborhood > cur_source_image[internal_ind_x] )
						{
							min_in_neighborhood = cur_source_image[internal_ind_x];
							min_x_index = internal_ind_x;
							min_y_index = internal_ind_y;
						}
					}
				}

				cur_source_image  += width;
				cur_kernel += kernel_pitch;
			}
			if( max_x_index - ind_x == 0 && max_y_index - ind_y  || (max_in_neighborhood - source_image[ ind_y * width + ind_x ] < threshold ) )
				dst_gradient_map[ ind_y * width + ind_x ] = 255;
			else
			{
				angle = atan2f( (float)(max_y_index - ind_y), (float)(max_x_index - ind_x) ) * 180 / (float)M_PI  + 180;
				dst_gradient_map[ ind_y * width + ind_x ] = (BYTE) MIN ( angle, 360 - angle );
			}

		}
	}

	DestroyCircleKernel_BYTE( & circle_kernel );
}

void Calculate_GradientDirections_RGB( BYTE * source_image, BYTE * dst_gradient_map, ushort width, ushort height, BYTE sobel_radius, int threshold )
{
	ushort ind_x, ind_y;
	int plane_pitch = width * height;
	BYTE kernel_pitch = 2*sobel_radius + 1;
	BYTE * circle_kernel;

	circle_kernel = GenerateCircleKernel_BYTE( (BYTE)sobel_radius );

	for ( ind_y = 0; ind_y < height; ++ind_y )
	{
		for ( ind_x = 0; ind_x < width; ++ind_x )
		{
			ushort start_internal_ind_x				= MAX( ind_x - sobel_radius, 0);
			ushort internal_ind_x, internal_ind_y	= MAX( ind_y - sobel_radius, 0);
			BYTE kernel_ind_x, start_kernel_ind_x	= start_internal_ind_x - ind_x + sobel_radius;
			BYTE kernel_ind_y						= internal_ind_y - ind_y + sobel_radius;

			ushort	end_internal_ind_x = MIN(ind_x + sobel_radius, width-1 ),
				end_internal_ind_y = MIN(ind_y + sobel_radius, height-1 );

			BYTE max_in_neighborhood_R = 0;
			BYTE min_in_neighborhood_R = 0xff;
			BYTE max_in_neighborhood_G = 0;
			BYTE min_in_neighborhood_G = 0xff;
			BYTE max_in_neighborhood_B = 0;
			BYTE min_in_neighborhood_B = 0xff;

			ushort max_x_index_r = start_internal_ind_x, max_y_index_r = internal_ind_y;
			ushort min_x_index_r = start_internal_ind_x, min_y_index_r = internal_ind_y;

			ushort max_x_index_g = start_internal_ind_x, max_y_index_g = internal_ind_y;
			ushort min_x_index_g = start_internal_ind_x, min_y_index_g = internal_ind_y;

			ushort max_x_index_b = start_internal_ind_x, max_y_index_b = internal_ind_y;
			ushort min_x_index_b = start_internal_ind_x, min_y_index_b = internal_ind_y;

			BYTE * cur_kernel			= circle_kernel  + kernel_pitch * kernel_ind_y;
			BYTE * cur_source_image_r   = source_image	 + width		 * internal_ind_y;
			BYTE * cur_source_image_g   = source_image	 + width		 * internal_ind_y + plane_pitch;
			BYTE * cur_source_image_b   = source_image	 + width		 * internal_ind_y + plane_pitch * 2;

			for( ; internal_ind_y <= end_internal_ind_y ; ++internal_ind_y )
			{
				for( internal_ind_x		= start_internal_ind_x,
					kernel_ind_x		= start_kernel_ind_x; 
					internal_ind_x <= end_internal_ind_x ;
					++internal_ind_x, ++kernel_ind_x  )
				{
					if( cur_kernel[kernel_ind_x] )
					{
						// first component
						if( max_in_neighborhood_R < cur_source_image_r[internal_ind_x] )
						{
							max_in_neighborhood_R = cur_source_image_r[internal_ind_x];
							max_x_index_r = internal_ind_x;
							max_y_index_r = internal_ind_y;
						}
						if( min_in_neighborhood_R > cur_source_image_r[internal_ind_x] )
						{
							min_in_neighborhood_R = cur_source_image_r[internal_ind_x];
							min_x_index_r = internal_ind_x;
							min_y_index_r = internal_ind_y;
						}

						//second component
						if( max_in_neighborhood_G < cur_source_image_g[internal_ind_x] )
						{
							max_in_neighborhood_G = cur_source_image_g[internal_ind_x];
							max_x_index_g = internal_ind_x;
							max_y_index_g = internal_ind_y;
						}
						if( min_in_neighborhood_G > cur_source_image_g[internal_ind_x] )
						{
							min_in_neighborhood_G = cur_source_image_g[internal_ind_x];
							min_x_index_g = internal_ind_x;
							min_y_index_g = internal_ind_y;
						}

						// third component
						if( max_in_neighborhood_B < cur_source_image_b[internal_ind_x] )
						{
							max_in_neighborhood_B = cur_source_image_b[internal_ind_x];
							max_x_index_b = internal_ind_x;
							max_y_index_b = internal_ind_y;
						}
						if( min_in_neighborhood_B > cur_source_image_b[internal_ind_x] )
						{
							min_in_neighborhood_B = cur_source_image_b[internal_ind_x];
							min_x_index_b = internal_ind_x;
							min_y_index_b = internal_ind_y;
						}
					}
				}

				cur_source_image_r  += width;
				cur_source_image_g  += width;
				cur_source_image_b  += width;
				cur_kernel += kernel_pitch;
			}
			{
				double angle_r = 0;
				double angle_g = 0;
				double angle_b = 0;

				double angle = 0;
				int weight_r = max_in_neighborhood_R - min_in_neighborhood_R;
				int weight_g = max_in_neighborhood_G - min_in_neighborhood_G;
				int weight_b = max_in_neighborhood_B - min_in_neighborhood_B;

				if( max_y_index_r - min_y_index_r != 0 || max_x_index_r - min_x_index_r != 0 )
				{
					angle_r = atan2f( (float)(max_y_index_r - min_y_index_r), (float)(max_x_index_r - min_x_index_r) ) * 180 / (float)M_PI  + 180;
				}
				else
				{
					weight_r = 0;
				}
				
				if( max_y_index_g - min_y_index_g != 0 || max_x_index_g - min_x_index_g != 0 )
				{	
					angle_g = atan2f( (float)(max_y_index_g - min_y_index_g), (float)(max_x_index_g - min_x_index_g) ) * 180 / (float)M_PI  + 180;
				}
				else
				{
					weight_g = 0;
				}
				
				if( max_y_index_b - min_y_index_b != 0 || max_x_index_b - min_x_index_b != 0 )
				{
					angle_b = atan2f( (float)(max_y_index_b - min_y_index_b), (float)(max_x_index_b - min_x_index_b) ) * 180 / (float)M_PI  + 180;
				}
				else
				{
					weight_b = 0;
				}

				if( weight_r + weight_g + weight_b > threshold )
				{
					angle = (angle_r * weight_r + angle_g * weight_g + angle_b * weight_b) /
									  (weight_r +           weight_g +           weight_b);
					
					dst_gradient_map[ ind_y * width + ind_x ] = (BYTE) MIN ( angle, 360 - angle );
				}
				else
					dst_gradient_map[ ind_y * width + ind_x ] = 255;

			}
		}
	}

	DestroyCircleKernel_BYTE( & circle_kernel );
}

void Calculate_GradientMask_3Planes ( BYTE * source_frame, BYTE * dst_gradient_mask, int width, int height, int radius )
{
	/*Calculate_GradientDirections( source_frame,					dst_gradient_mask,					width, height, radius, 4 );
	Calculate_GradientDirections( source_frame + width*height,	dst_gradient_mask + width*height,	width, height, radius, 4 );
	Calculate_GradientDirections( source_frame + width*height*2,dst_gradient_mask + width*height*2, width, height, radius, 4 );*/
	Calculate_GradientDirections_RGB( source_frame, dst_gradient_mask, width, height, 2/*radius*/, 4 );
}

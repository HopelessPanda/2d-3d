#include "MExt_CoreInlineFunctions.h"

#include "HierME_Core.h"
#include "MExt_Metrics.h"

#include "MVF_Contrast_Filter.h"
#include "Distance_Transform.h"

#include "../MEstimation/cGetError.h"

cMV Get_MV_From_Candidates_DepthBased ( MVCBlock *cands_set, BYTE *src_depth_map, BYTE *src_rgb_frame, int vec_num, 
										int x, int y, int width, int height, int base_block_size)
{
	int BLOCK_SIZE = base_block_size;
	int i;
	cMV res;
	double mv_x = 0; 
	double mv_y = 0;
	double sum = 0;

	int frame_size = width * height;

	BYTE *cur_block			= src_rgb_frame	+ y*width + x ;
	BYTE *cur_block_depth	= src_depth_map + y*width + x ;

	cMV average_vector = Calc_Array_Dispersion_MVCBlock(cands_set, vec_num);

	int max_deviation = 0;
	int min_deviation = 1<<18;
	int deviation_thresh;

	// search vector with the biggest deviance
	for(i = 0; i < vec_num; i++)
	{
		int deviation = abs( sqr(cands_set[i].vector.x) + sqr(cands_set[i].vector.y) - (sqr(average_vector.x)   + sqr(average_vector.y)) );
		if(deviation > max_deviation)
		{
			max_deviation = deviation;
		}
		if(deviation < min_deviation)
		{
			min_deviation = deviation;
		}
	}
	deviation_thresh = (max_deviation - min_deviation)/5;

	///////////////////////////////////////////

	for(i = 0; i < vec_num; i++)
	{
		int cur_x = cands_set[i].x;
		int cur_y = cands_set[i].y;

		double diff;
		double koef;
		BYTE *ref_block			= src_rgb_frame	+ cur_y*width + cur_x;
		BYTE *ref_block_depth	= src_depth_map + cur_y*width + cur_x;

		double color_dif;
		double depth_diff;

		// drop vectors, which are very different from others - noise filtering
		int deviation = abs( sqr(cands_set[i].vector.x) + sqr(cands_set[i].vector.y) - (sqr(average_vector.x)   + sqr(average_vector.y)) );
		if(deviation > max_deviation - deviation_thresh)
			continue;

		if(BLOCK_SIZE == 16)
		{
            color_dif = (src_rgb_frame) ? /*sqrt*/( (GetErrorSAD_16x16_MMX(cur_block				, ref_block					, width, NULL)
                                                + GetErrorSAD_16x16_MMX(cur_block+frame_size		, ref_block + frame_size	, width, NULL)
                                                + GetErrorSAD_16x16_MMX(cur_block+frame_size*2	, ref_block + frame_size*2	, width, NULL))/3 ) : 1;

            depth_diff = (src_depth_map) ? GetErrorSAD_16x16_MMX(cur_block_depth, ref_block_depth, width, NULL) : 1;
		}
		else if(BLOCK_SIZE == 8)
		{
            color_dif = (src_rgb_frame) ? /*sqrt*/(( GetErrorSAD_8x8_MMX(cur_block				, ref_block					, width, NULL)
                                                + GetErrorSAD_8x8_MMX(cur_block+frame_size	, ref_block+frame_size		, width, NULL)
                                                + GetErrorSAD_8x8_MMX(cur_block+frame_size*2	, ref_block+frame_size*2	, width, NULL) )/3 ) : 1;

            depth_diff = (src_depth_map) ? GetErrorSAD_8x8_MMX(cur_block_depth, ref_block_depth, width, NULL) : 1 ;
		}
		else if(BLOCK_SIZE == 4)
		{
            color_dif = (src_rgb_frame) ? /*sqrt*/(( GetErrorSAD_4x4_MMX(cur_block				, ref_block					, width, NULL)
                                                + GetErrorSAD_4x4_MMX(cur_block+frame_size	, ref_block+frame_size		, width, NULL)
                                                + GetErrorSAD_4x4_MMX(cur_block+frame_size*2	, ref_block+frame_size*2	, width, NULL) )/3 ) : 1;

            depth_diff = (src_depth_map) ? GetErrorSAD_4x4_MMX(cur_block_depth, ref_block_depth, width, NULL) : 1 ;
		}
		else
		{
			color_dif = 1;
			depth_diff = 1;
		}

		if( fabs(color_dif) < 0.00001 )
			color_dif = 1;

		if( depth_diff == 0 )
			depth_diff = 1;

		// to avoid very small numbers
		depth_diff = sqr(depth_diff / 255.0);

		diff = sqr( (cur_x - x)/BLOCK_SIZE ) + sqr( (cur_y - y)/BLOCK_SIZE ) ;
		koef = 1/(color_dif * diff * depth_diff);

		mv_x += cands_set[i].vector.x * koef;
		mv_y += cands_set[i].vector.y * koef;

		sum += koef;
	}

	res.x = (int)(mv_x / sum);
	res.y = (int)(mv_y / sum);
	res.error = 1000;
	res.field = FT_TOP;

	return res;
}

BYTE Get_Block_Average(BYTE* buf, int x, int y, int stride, int block_size)
{
	int i, j;
	int sum = 0;

	buf = buf + y*stride + x;
	for(i = 0; i < block_size; i++)
	{
		for (j = 0; j < block_size; j++)
		{
			sum += buf[j];
		}
		buf += stride;
	}

	return (sum/(block_size*block_size));
}

// get depth value, using coordinates of blocks
BYTE Get_Depth_From_Candidates(MVCBlock *result, BYTE *src_depth, BYTE *src, int vec_num, int x, int y, int stride, BOOL consider_color, int base_block_size)
{
	int BLOCK_SIZE = base_block_size;
	int i;
	double depth_val = 0;
	double sum = 0;

	BYTE *cur_block = &src[y*stride + x];
	cMV average_vector = Calc_Array_Dispersion_MVCBlock(result, vec_num);

	int max_deviation = 0;
	int min_deviation = 1<<18;
	int deviation_thresh;

	// search vector with the biggest deviance
	for(i = 0; i < vec_num; i++)
	{
		int deviation = abs( sqr(result[i].vector.x) + sqr(result[i].vector.y) - (sqr(average_vector.x)   + sqr(average_vector.y)) );
		if(deviation > max_deviation)
		{
			max_deviation = deviation;
		}
		if(deviation < min_deviation)
		{
			min_deviation = deviation;
		}
	}
	deviation_thresh = (max_deviation - min_deviation)/5;

	///////////////////////////////////////////

	for(i = 0; i < vec_num; i++)
	{
		int cur_x = result[i].x;
		int cur_y = result[i].y;
		
		double diff;
		double koef;
		BYTE *ref_block = &src[cur_y*stride + cur_x];

		double color_dif;

		// drop vectors, which are very different from others - noise filtering
		int deviation = abs( sqr(result[i].vector.x) + sqr(result[i].vector.y) - (sqr(average_vector.x)   + sqr(average_vector.y)) );
		if(deviation > max_deviation - deviation_thresh)
			continue;

		//color_dif = (consider_color) ? sqrt( GetErrorSAD_16x16_C(cur_block, ref_block, stride, NULL) ) : 1;
		color_dif = 1;
		if( fabs(color_dif) < 0.00001 )
			color_dif = 1;

		diff = sqr( (cur_x - x)/BLOCK_SIZE ) + sqr( (cur_y - y)/BLOCK_SIZE ) ;
		koef = 1/(color_dif * diff);
		
		depth_val += Get_Block_Average(src_depth, cur_x, cur_y, stride, BLOCK_SIZE) * koef;

		sum += koef;
	}

	return (BYTE)(depth_val/sum);
}

int Find_MV_Candidates_FromFillMask(cMV * MV_field, int *filling_mask, MVCBlock *result, int vec_num, 
										int x, int y, int w, int h, int base_block_size)
{
	int BLOCK_SIZE = base_block_size;

	int i, j;
	int step = BLOCK_SIZE;
	int sqr_step;
	int index;
	int found_num = 0;

	int original_fill_val = filling_mask [ y*w + x ];

	while(found_num < vec_num)
	{
		// I don't want infinite loops
		if(step > 4444)
			break;

		sqr_step = step*step;
		for(i = -step; i <= step; i += BLOCK_SIZE)
		{
			BOOL border_flag = (i == -step) || (i == step);

			if ( !INSIDE(y+i, 0, h-1) )
				continue;

			for (j = -step; j <= step; j += BLOCK_SIZE)
			{
				// skip those, which are not in circle
				/*if(i*i + j*j - sqr_step != 0)
				{
					continue;
				}*/
				if ( !border_flag &&  abs(j) != step  )
					continue;

				// skip if we went out of frame borders
				if( ! INSIDE(x+j, 0, w-1)  )
					continue;

				index = (y+i) * w + (x+j);

				if( filling_mask[index] < original_fill_val )
				{
					MVCBlock current;

					current.block_contrast = 255;
					current.vector = MV_field[index];
					current.x = x+j;
					current.y = y+i;

					result[found_num] = current;
					found_num += 1;
				}
			}
		}

		step += BLOCK_SIZE;
	}

	return found_num;
}

int Find_MV_Candidates_FromFillMask_WithDepthThreshold (cMV * MV_field, int *filling_mask, BYTE * depth_map, MVCBlock *result, int vec_num, 
															int x, int y, int w, int h, int base_block_size, int depth_thresh )
{
	int BLOCK_SIZE = base_block_size;
	
	int i, j;
	int step = BLOCK_SIZE;
	int sqr_step;
	int index;
	int found_num = 0;

	int original_fill_val = filling_mask [ y*w + x ];
	BYTE cur_depth_val = depth_map [ y*w + x ];

	while(found_num < vec_num)
	{
		// I don't want infinite loops
		if(step > 4444)
			break;

		sqr_step = step*step;
		for(i = -step; i <= step; i += BLOCK_SIZE)
		{
			BOOL border_flag = (i == -step) || (i == step);

			if ( !INSIDE(y+i, 0, h-1) )
				continue;

			for (j = -step; j <= step; j += BLOCK_SIZE)
			{
				// skip those, which are not in circle
				/*if(i*i + j*j - sqr_step != 0)
				{
					continue;
				}*/
				if ( !border_flag &&  abs(j) != step  )
					continue;

				// skip if we went out of frame borders
				if( ! INSIDE(x+j, 0, w-1)  )
					continue;

				index = (y+i) * w + (x+j);

				if( filling_mask[index] < original_fill_val )
				{
					MVCBlock current;

					if ( abs(depth_map[index] - cur_depth_val) > depth_thresh )
						continue;

					current.block_contrast = 255;
					current.vector = MV_field[index];
					current.x = x+j;
					current.y = y+i;

					result[found_num] = current;
					found_num += 1;
				}
			}
		}

		step += BLOCK_SIZE;
	}

	return found_num;
}

void Find_MV_Candidates(gmcMotionEstimator * ME_Inst, BYTE *contrast_map, MVCBlock *result, int vec_num, 
							int x, int y, int w, int h, int base_block_size)
{
	int BLOCK_SIZE = base_block_size;

	int i, j;
	int step = BLOCK_SIZE;
	int sqr_step;
	int index;
	int found_num = 0;

	while(found_num < vec_num)
	{
		// I don't want infinite loops
		if(step > 4444)
			break;

		sqr_step = step*step;
		for(i = -step; i <= step; i += BLOCK_SIZE)
		{
			for (j = -step; j <= step; j += BLOCK_SIZE)
			{
				// skip everything, if we found all samples
				if(found_num == vec_num)
				{
					continue;
				}
				// skip those, which are not in circle
				if(i*i + j*j - sqr_step != 0)
				{
					continue;
				}
				// skip if we went out of frame borders
				if(  !INNER_POINT(y+i, x+j, h, w)  )
					continue;

				index = (y+i) * w + (x+j);

				if(contrast_map[index] == 255)
				{
					MVCBlock current;

					current.block_contrast = 255;
					current.vector = *(cGetMVBlockPointer(ME_Inst, x+j, y+i, &current.block_size));
					current.x = x+j;
					current.y = y+i;

					result[found_num] = current;
					found_num += 1;
				}
			}
		}

		step += BLOCK_SIZE;
	}
}

void Prefill_Refinement_Mask( BYTE * src, BYTE *mask, BYTE * dst, int width, int height, int block_base_size )
{
	int BLOCK_SIZE = block_base_size;

	int ind_x, ind_y;

	for (ind_y = 0; ind_y < height; ind_y += BLOCK_SIZE )
	{
		for (ind_x = 0; ind_x < width; ind_x += BLOCK_SIZE )
		{
			int index = ind_y * width + ind_x;
			int internal_x, internal_y;
			int mask_block_sum = 0;			// this is sum of pixel values inside block in mask

			long src_block_sum = 0;			// this is sum of pixel values inside block. It is used to find fill value
			long src_count = 0;

			BYTE * cur_src_pointer = src + index;	// this is pointer to the current block in src buffer
			BYTE * cur_mask_pointer = mask + index; // this is pointer to the current block in src buffer

			// analyze pass
			for(internal_y = 0; internal_y < BLOCK_SIZE; ++internal_y)
			{
				for (internal_x = 0; internal_x < BLOCK_SIZE; ++internal_x)
				{
					// calculate uncovered area in block
					mask_block_sum += cur_mask_pointer[internal_x];
					src_block_sum  += cur_src_pointer[internal_x] * cur_mask_pointer[internal_x];
					src_count += 1;
				}
				cur_mask_pointer += width;
				cur_src_pointer  += width;
			}
			src_block_sum  /= MAX(mask_block_sum, 1);
			src_block_sum   = CLIP(src_block_sum, 0, 255);

			mask_block_sum /= src_count;

			// setting values in dst and mask buffers.
			Set_Block_Value(dst, ind_x, ind_y, width, src_block_sum, BLOCK_SIZE);
			Set_Block_Value(mask, ind_x, ind_y, width, mask_block_sum, BLOCK_SIZE);
		}
	}
}

void Prefill_Refinement_Mask_cMV( cMV * src, BYTE *mask, cMV * dst, int width, int height, int block_base_size )
{
	int BLOCK_SIZE = block_base_size;

	int ind_x, ind_y;

	if ( src != dst )
	{
		memcpy ( dst, src, width * height * sizeof(cMV) );
	}

	for (ind_y = 0; ind_y < height; ind_y += BLOCK_SIZE )
	{
		for (ind_x = 0; ind_x < width; ind_x += BLOCK_SIZE )
		{
			int index = ind_y * width + ind_x;
			int internal_x, internal_y;
			int mask_block_sum = 0;			// this is sum of pixel values inside block in mask
			int min_mask_value = 256;

			long src_count = 0;
			long src_block_sum_x = 0;		// this is sum of pixel values inside block. It is used to find fill value
			long src_block_sum_y = 0;		

			cMV  * cur_src_pointer = src + index;	// this is pointer to the current block in src buffer
			cMV  * cur_dst_pointer = dst + index;	// this is pointer to the current block in dst buffer
			BYTE * cur_mask_pointer = mask + index; // this is pointer to the current block in mask buffer

			// analyze pass
			for(internal_y = 0; internal_y < BLOCK_SIZE; ++internal_y)
			{
				for (internal_x = 0; internal_x < BLOCK_SIZE; ++internal_x)
				{
					// calculate uncovered area in block
					mask_block_sum += cur_mask_pointer[internal_x];
					src_block_sum_x  += cur_src_pointer[internal_x].x * cur_mask_pointer[internal_x];
					src_block_sum_y  += cur_src_pointer[internal_x].y * cur_mask_pointer[internal_x];
					++ src_count;
				}
				cur_mask_pointer += width;
				cur_src_pointer  += width;
			}
			src_block_sum_x  /= MAX(mask_block_sum, 1);
			src_block_sum_y  /= MAX(mask_block_sum, 1);
			mask_block_sum /= src_count;

			// increase difference between good and bad approximation in 2 times
			mask_block_sum = 255 - CLIP( (255 - mask_block_sum) * 2, 0, 255); 

			//////////////////////////////////////////////////////////////////////////
			// setting values in dst and mask buffers.
			Set_Block_Value(mask, ind_x, ind_y, width, mask_block_sum, BLOCK_SIZE);

			if( mask_block_sum == 255 )
				continue;

			cur_dst_pointer = dst + index;	// this is pointer to the current block in src buffer
			for(internal_y = 0; internal_y < BLOCK_SIZE; ++internal_y)
			{
				for (internal_x = 0; internal_x < BLOCK_SIZE; ++internal_x)
				{
					cur_dst_pointer[internal_x].x = src_block_sum_x;
					cur_dst_pointer[internal_x].y = src_block_sum_y;
				}
				cur_dst_pointer  += width;
			}
		}
	}
}

BOOL Check_ConfidenceMap(BYTE *confidence, int w, int h, int blocksize, int cands_num)
{
	int i, j;
	int val_count = 0;

	for(i = 0; i < h; i+=blocksize)
	{
		for(j = 0; j < w; j+=blocksize)
		{
			if(confidence[i*w + j] == 255)
			{
				val_count++;
			}
		}
	}

	return (val_count > cands_num * 2) ? TRUE : FALSE ;
}

void MV_Depth_Filter( gmcMotionEstimator *ME_inst, BYTE *mask, BYTE * src, BYTE *src_depth, BYTE *dst_depth, int cands_num, int base_block_size )
{
	int BLOCKSIZE = base_block_size;
	int i,j;
	int w = ME_inst->m_iWidth;
	int w_b = w/4;
	int h = ME_inst->m_iHeight;
	int h_b = h/4;

	int index; 

	MVCBlock *cands_list;
	BYTE result;

	memcpy(dst_depth, src_depth, w*h);

	INIT_MEMORY_C(cands_list, 0, cands_num, MVCBlock); 
	memset(cands_list, 0, cands_num * sizeof(MVCBlock)  );

	if( ! Check_ConfidenceMap(mask, w, h, base_block_size, cands_num ))
		return;

	for(i = 0; i < h; i+=BLOCKSIZE)
	{
		for(j = 0; j < w; j+=BLOCKSIZE)
		{
			int x = j;
			int y = i;
			int ind_x, ind_y;
			int filter_block = FALSE;

			index = i*w + j;

			for(ind_y = i; ind_y < i + BLOCKSIZE; ++ ind_y)
			{
				for (ind_x = j; ind_x < j + BLOCKSIZE; ++ ind_x)
				{
					if(mask[ind_y * w + ind_x] != 255)
					{
						filter_block = TRUE;
						break;
					}
				}
			}

			if( filter_block )
			{
				Find_MV_Candidates(ME_inst, mask, cands_list, cands_num, j, i, w, h, BLOCKSIZE);
				result = Get_Depth_From_Candidates(cands_list, src_depth, src, cands_num, j, i, w, TRUE, BLOCKSIZE);

				Set_Block_Value(dst_depth, j, i, w, result, BLOCKSIZE);
			}
		}
	}

	DEINIT_MEMORY_C(cands_list);
}

// returns number of candidates
int Calculate_TrustedVectorsArray ( MVCBlock * dst_array, cMV * src_motion, BYTE * src_mask, int width, int height, int block_size )
{
	int ind_y, ind_x;
	int dst_array_index = 0;

	for (ind_y = 0; ind_y < height; ind_y += block_size)
	{
		for (ind_x = 0; ind_x < width; ind_x += block_size )
		{
			int index = ind_y * width + ind_x;

			if ( src_mask[index] == 255 )
			{
				dst_array[dst_array_index].block_contrast	= src_mask[index];
				dst_array[dst_array_index].block_size		= block_size;
				dst_array[dst_array_index].vector			= src_motion[index];
				dst_array[dst_array_index].x				= ind_x;
				dst_array[dst_array_index].y				= ind_y;
				dst_array_index += 1;
			}
		}
	}

	return dst_array_index;
}

void MV_Filter_Internal ( BYTE * mask, BYTE * source_frame, BYTE * depth_frame, 
						 cMV * src_field, cMV * dst_field, int cands_num, int base_block_size , int width, int height, // usual options
						 BOOL use_depth_threshold_search, int depth_threshold ) // internal options
{
	int BLOCKSIZE = base_block_size;
	int y, x;

	int k_ind, l_ind, index;
	int max_fill_value, iteration;

	int *filling_mask;

	MVCBlock *cands_list;
	cMV result;
	int max_candidates = width * height / (base_block_size * base_block_size);

	INIT_MEMORY_C(filling_mask, 0, width * height, int );

	INIT_MEMORY_C( cands_list, 0, max_candidates,			MVCBlock  ); 
	memset		 ( cands_list, 0, max_candidates * sizeof  (MVCBlock) );

	if( dst_field != src_field )
	{
		memcpy(dst_field, src_field, width*height*sizeof(cMV));
	}

	// if number of trusted blocks is less than number of candidates * 2, do not filter.
	if( ! Check_ConfidenceMap(mask, width, height, BLOCKSIZE, cands_num) )
	{
		DEINIT_MEMORY_C ( filling_mask );
		DEINIT_MEMORY_C ( cands_list );
		return;
	}

	// set priorities of filtering different blocks, depending on their distance from trusted blocks
	max_fill_value = Calculate_DistanceTransformMask(filling_mask, mask, width, height, BLOCKSIZE);

	for(iteration = 1; iteration <= max_fill_value; ++ iteration)
	{
		for(y = 0; y < height; y+=BLOCKSIZE)
		{
			for(x = 0; x < width; x+=BLOCKSIZE)
			{
				index = y*width + x;

				if(filling_mask[index] == iteration)
				{
					int actual_cands_num;
					if( use_depth_threshold_search )
					{
						actual_cands_num = Find_MV_Candidates_FromFillMask_WithDepthThreshold( dst_field, filling_mask, depth_frame, cands_list, 
																								cands_num, x, y, width, height, BLOCKSIZE, depth_threshold);
						
						if ( actual_cands_num == 0 )
							actual_cands_num = Find_MV_Candidates_FromFillMask(dst_field, filling_mask, cands_list, cands_num, x, y, width, height, BLOCKSIZE);
					}
					else
					{
						actual_cands_num = Find_MV_Candidates_FromFillMask(dst_field, filling_mask, cands_list, cands_num, x, y, width, height, BLOCKSIZE);
					}

					result = Get_MV_From_Candidates_DepthBased(cands_list, depth_frame, source_frame, actual_cands_num, x, y, width, height, BLOCKSIZE);

					for(k_ind = y; k_ind < (y+BLOCKSIZE); ++k_ind)
						for(l_ind = x; l_ind < (x+BLOCKSIZE); ++l_ind)
						{
							int ind = k_ind * width + l_ind;

							dst_field[ind].x = (result.x * (255 - mask[index]) + dst_field[ind].x * ( mask[index] ) ) / 255;
							dst_field[ind].y = (result.y * (255 - mask[index]) + dst_field[ind].y * ( mask[index] ) ) / 255;
						}
				}
			}
		}
	}

	DEINIT_MEMORY_C(cands_list);
	DEINIT_MEMORY_C(filling_mask);
}

void MV_Filter ( BYTE * mask, BYTE * source_frame, BYTE * depth_frame, 
				cMV * src_field, cMV * dst_field, int cands_num, int base_block_size , int width, int height)
{
	MV_Filter_Internal ( mask, source_frame, depth_frame, src_field, dst_field, cands_num, base_block_size , width, height, FALSE, 0 );
}

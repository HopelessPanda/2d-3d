#include "OcclEstimation_2D.h"
#include "OcclEstimation_Internal.h"
#include <VideoVGCLib/VGLibCommon/cMorphology.h>
#include "MExt_CoreInlineFunctions.h"
#include "MExt_Metrics.h"
#include "Distance_Transform.h"
#include "MVF_Contrast_Filter.h"
#include "ME_Compensation.h"

void Calculate_DepthGradient_LRC_Intersection (BYTE * lrc, BYTE * depth_gradient_mask, BYTE * checked_area, int x0, int y0, int width, int height, long * lrc_area, long * intersection_area )
{
	int index, x;
	int line_index = y0*width;

	int pos_x_right = x0, pos_x_left = x0;
	for(x = x0; x < width; x++)
	{
		index = line_index + x;
		if( lrc [index] && !checked_area[index] )
		{ 
			checked_area[index] = 255;
			(*lrc_area) += 1;

			if ( depth_gradient_mask [index] )
			{
				(*intersection_area) += 1;
			}

			pos_x_right = width;
		}
		else
		{
			pos_x_right = x;
			break;
		}
	}
	for(x = x0 - 1; x >= 0; --x)
	{
		index = line_index + x;
		if( lrc [index] && !checked_area[index] )
		{ 
			checked_area[index] = 255;
			(*lrc_area) += 1;

			if ( depth_gradient_mask [index] )
			{
				(*intersection_area) += 1;
			}

			pos_x_left = -1;
		}
		else
		{
			pos_x_left = x;
			break;
		}
	}

	// excluding area borders
	pos_x_left  += 1;
	pos_x_right -= 1;

	if(y0 > 0)
	{
		for(x = pos_x_left; x <= pos_x_right; x++)
		{
			Calculate_DepthGradient_LRC_Intersection ( lrc, depth_gradient_mask, checked_area, x, y0-1, width, height, lrc_area, intersection_area );
		}
	}
	if(y0 < height-1)
	{
		for(x = pos_x_left; x <= pos_x_right; x++)
		{
			Calculate_DepthGradient_LRC_Intersection ( lrc, depth_gradient_mask, checked_area, x, y0+1, width, height, lrc_area, intersection_area );
		}
	}

	return;
}


void SetDestinationIntersectionMask (BYTE * lrc, BYTE * dst_mask, BYTE * depth_gradient_mask, int x0, int y0, int width, int height )
{
	int index, x;
	int line_index = y0*width;

	int pos_x_right = x0, pos_x_left = x0;
	for(x = x0; x < width; x++)
	{
		index = line_index + x;
		if( (lrc [index] || depth_gradient_mask[index]) && dst_mask[index] )
		{ 
			dst_mask[index] = 0;
			pos_x_right = width;
		}
		else
		{
			pos_x_right = x;
			break;
		}
	}
	for(x = x0 - 1; x >= 0; --x)
	{
		index = line_index + x;
		if( (lrc [index] || depth_gradient_mask[index]) && dst_mask[index] )
		{ 
			dst_mask[index] = 0;
			pos_x_left = -1;
		}
		else
		{
			pos_x_left = x;
			break;
		}
	}

	// excluding area borders
	pos_x_left  += 1;
	pos_x_right -= 1;

	if(y0 > 0)
	{
		for(x = pos_x_left; x <= pos_x_right; x++)
		{
			SetDestinationIntersectionMask ( lrc, dst_mask, depth_gradient_mask, x, y0-1, width, height );
		}
	}
	if(y0 < height-1)
	{
		for(x = pos_x_left; x <= pos_x_right; x++)
		{
			SetDestinationIntersectionMask ( lrc, dst_mask, depth_gradient_mask, x, y0+1, width, height );
		}
	}

	return;
}

void Make_Dilation_Erosion_Mask ( BYTE * dst_mask, BYTE * depth_gradient_mask, BYTE * lrc, int width, int height )
{
	int blocksize = 4;
	int LRC_FULL_TRUST_COEF = 20;
	int frame_size = width * height;
	int index;
	int ind_x, ind_y ;

	float max_gradient = 0;

	for (index = 0; index < frame_size; ++ index)
	{
		lrc[index] = (lrc[index] > LRC_FULL_TRUST_COEF) ? lrc[index] : 0;
	}
	/*{
		BYTE *mask_buf;
		WindowMask morph_mask;

		INIT_MEMORY_C(mask_buf, 0, width*height, BYTE);

		Morphology_InitWindows();
		morph_mask = Morphology_GetWindowMask(2, TRUE);

		Dilation_Value_BYTE(mask_buf, lrc,		width, height, &morph_mask);
		Dilation_Value_BYTE(lrc, mask_buf, width, height, &morph_mask);

		DEINIT_MEMORY_C(mask_buf);
	}*/
	{
		BYTE * checked_area;

		INIT_MEMORY_C(checked_area, 0, width*height, BYTE);
		memset ( dst_mask, 255, width * height );

		for ( ind_y = 0; ind_y < height; ++ind_y )
		{
			for ( ind_x = 0; ind_x < width; ++ind_x )
			{
				int index = ind_y * width + ind_x;
				long lrc_area = 0, inters_area = 0;
				
				if( ! lrc[index] || checked_area[index] )
					continue;
				
				Calculate_DepthGradient_LRC_Intersection( lrc, depth_gradient_mask, checked_area, ind_x, ind_y, width, height, &lrc_area, &inters_area );
				
				if ( (double)inters_area / (double)lrc_area  > 0.1 )
				{
					SetDestinationIntersectionMask ( lrc, dst_mask, depth_gradient_mask, ind_x, ind_y, width, height );
				}
			}
		}
		for (index = 0; index < frame_size; ++ index)
		{
			dst_mask[index] = ( depth_gradient_mask[index] ) ? 0 : dst_mask[index];
		}

		DEINIT_MEMORY_C(checked_area);
	}
	{
		BYTE *mask_buf;
		WindowMask morph_mask;

		INIT_MEMORY_C(mask_buf, 0, width*height, BYTE);

		Morphology_InitWindows();
		morph_mask = Morphology_GetWindowMask(2, TRUE);

		Erosion_Value_BYTE_Faster(mask_buf, dst_mask,	width, height, &morph_mask);
		Dilation_Value_BYTE_Faster(dst_mask, mask_buf, width, height, &morph_mask);

		//memcpy(dst_mask, mask_buf, width*height);

		DEINIT_MEMORY_C(mask_buf);
	}

	Occlusion_Mask_Refinement(dst_mask, width, height);

	for ( ind_y = 0; ind_y < height; ind_y += blocksize )
	{
		for ( ind_x = 0; ind_x < width; ind_x += blocksize )
		{
			int internal_x, internal_y;
			for (internal_y = ind_y; internal_y < ind_y + blocksize; ++internal_y)
			{
				for (internal_x = ind_x; internal_x < ind_x + blocksize; ++internal_x)
				{
					if ( dst_mask [internal_y * width + internal_x] == 0)
					{
						Set_Block_Value(dst_mask, ind_x, ind_y, width, 0, blocksize );
						break;
					}
				}
			}
		}
	}
}

void DilateErode_MotionField_DepthBased ( cMV * motion_field, cMV * dst_dilated_field, cMV * dst_erosion_field, BYTE * depth_map, BYTE * lrc, int width, int height, int radius )
{
	int ind_x, ind_y;
	int kernel_pitch = 2*radius + 1;
	int * circle_kernel;

	circle_kernel = GenerateCircleKernel( radius );

	memcpy( dst_dilated_field, motion_field, width * height * sizeof(cMV) );
	memcpy( dst_erosion_field, motion_field, width * height * sizeof(cMV) );

	for ( ind_y = 0; ind_y < height; ++ind_y )
	{
		for ( ind_x = 0; ind_x < width; ++ind_x )
		{
			int index = ind_y * width + ind_x;

			int start_internal_ind_x		= MAX( ind_x - radius, 0);
			int start_internal_ind_y		= MAX( ind_y - radius, 0);
			int end_internal_x				= MIN( ind_x + radius, width -1 );
			int end_internal_y				= MIN( ind_y + radius, height-1 );
			int start_internal_index		= start_internal_ind_y * width + start_internal_ind_x;

			int start_kernel_ind_x = start_internal_ind_x - ind_x + radius;
			int start_kernel_ind_y = start_internal_ind_y - ind_y + radius;
			int start_kernel_index = start_kernel_ind_y * kernel_pitch + start_kernel_ind_x;

			int  * cur_kernel = circle_kernel	+ start_kernel_index;
			BYTE * cur_depth  = depth_map		+ start_internal_index;
			BYTE * cur_lrc	  = lrc				+ start_internal_index;
			cMV * cur_motion  = motion_field	+ start_internal_index;

			int internal_ind_x, internal_ind_y;
			int kernel_ind_x;

			int max_in_neighborhood = 0;
			int min_in_neighborhood = 256;
			int max_in_neighborhood_x = 0;
			int max_in_neighborhood_y = 0;
			int min_in_neighborhood_x = 0;
			int min_in_neighborhood_y = 0;

			int max_count = 0, min_count = 0;
			cMV max_sum, min_sum;

			int depth_val = depth_map [ ind_y * width + ind_x ]; 

			//////////////////////////////////////////////////////////////////////////
			// first pass - determine maximum depth
			for( internal_ind_y = start_internal_ind_y; internal_ind_y <= end_internal_y ; ++internal_ind_y )
			{
				for( internal_ind_x	= start_internal_ind_x, kernel_ind_x = 0; 
					internal_ind_x <= end_internal_x;  ++internal_ind_x, ++kernel_ind_x  )
				{
					if( !cur_kernel[kernel_ind_x] )
						continue;

					// kernel_ind_x is used for element obtaining, because cur_depth is already shifted by start_internal_ind_x, 
					// so I just add difference from the start index
					if( max_in_neighborhood < cur_depth[kernel_ind_x] )
					{
						max_in_neighborhood_x = internal_ind_x;
						max_in_neighborhood_y = internal_ind_y;
						max_in_neighborhood   = cur_depth[kernel_ind_x];
					}
					if ( min_in_neighborhood > cur_depth[kernel_ind_x] )
					{
						min_in_neighborhood_x = internal_ind_x;
						min_in_neighborhood_y = internal_ind_y;
						min_in_neighborhood   = cur_depth[kernel_ind_x];
					}
				}
				cur_kernel += kernel_pitch;
				cur_depth  += width;
			}

			//////////////////////////////////////////////////////////////////////////
			// check if, area is flat in depth around here
			if ( max_in_neighborhood - min_in_neighborhood <= 4)
			{
				// switch to next iteration
				continue;
			}
			// restore pointers:
			cur_kernel = circle_kernel + start_kernel_index;
			cur_depth  = depth_map + start_internal_index;
			cInitMV(&min_sum);
			cInitMV(&max_sum);

			//////////////////////////////////////////////////////////////////////////
			// second pass. get motion vector for dilate/erode (depending on min/max)
			for( internal_ind_y = start_internal_ind_y; internal_ind_y <= end_internal_y ; ++internal_ind_y )
			{
				for( internal_ind_x	= start_internal_ind_x, kernel_ind_x = 0; 
					internal_ind_x <= end_internal_x;  ++internal_ind_x, ++kernel_ind_x  )
				{
					if( !cur_kernel[kernel_ind_x] )
						continue;

					if( cur_depth[kernel_ind_x] == max_in_neighborhood )
					{
						max_sum.x += cur_motion[kernel_ind_x].x * (255 - cur_lrc[kernel_ind_x]);
						max_sum.y += cur_motion[kernel_ind_x].y * (255 - cur_lrc[kernel_ind_x]);
						max_count += (255 - cur_lrc[kernel_ind_x]);
					}
					if( cur_depth[kernel_ind_x] == min_in_neighborhood )
					{
						min_sum.x += cur_motion[kernel_ind_x].x * (255 - cur_lrc[kernel_ind_x]);
						min_sum.y += cur_motion[kernel_ind_x].y * (255 - cur_lrc[kernel_ind_x]);
						min_count += (255 - cur_lrc[kernel_ind_x]);
					}
				}
				cur_kernel	+= kernel_pitch;
				cur_depth	+= width;
				cur_lrc		+= width;
				cur_motion	+= width;
			}

			//////////////////////////////////////////////////////////////////////////
			// step 3. Calculate motion vector for dilation/erosion.
			max_count = MAX(max_count, 1);
			min_count = MAX(min_count, 1);

			max_sum.x /= max_count;
			max_sum.y /= max_count;

			min_sum.x /= min_count;
			min_sum.y /= min_count;

			dst_dilated_field[index] = max_sum;
			dst_erosion_field[index] = min_sum;
		}
	}

	DestroyCircleKernel( & circle_kernel );
}

void MV_Filter_OcclusionFilling( BYTE * mask, BYTE * source_frame, BYTE * depth_frame, cMV * src_field, 
						cMV * dst_field, int cands_num, int base_block_size , int width, int height, 
						int depth_threshold )
{
	MV_Filter_Internal(mask, source_frame, depth_frame, src_field, dst_field, cands_num, base_block_size, width, height, TRUE, depth_threshold);
}

void EstimateOcclusions2D ( Buffer_Storage * frame_buffers, BYTE * src_frame, BYTE * reference_frame )
{
	int * distance_transform, * one_way_distance_transform;
	int w = frame_buffers->width, h = frame_buffers->height;
	int max_fill_value = 0;

	INIT_MEMORY_C ( distance_transform,			0, w*h, int );
	INIT_MEMORY_C ( one_way_distance_transform, 0, w*h, int );

	Buffer_Storage_InternalCopy (frame_buffers, BYTE_depth_map, BYTE_depth_map_dilation );
	Buffer_Storage_InternalCopy (frame_buffers, BYTE_depth_map, BYTE_depth_map_erosion  );

	Calculate_GradientMask		( BUF_BYTE(BYTE_depth_map),		 BUF_BYTE(BYTE_depth_gradient_map), w, h , 8, 4 );
	Make_Dilation_Erosion_Mask	( BUF_BYTE(BYTE_occlusion_mask), BUF_BYTE(BYTE_depth_gradient_map), 
								  BUF_BYTE(BYTE_lrc_uniform_filter), w, h );

	max_fill_value = Calculate_DistanceTransformMask( distance_transform, BUF_BYTE(BYTE_occlusion_mask), w, h, 1);

	MakeOneDirectional_DistantTransform_DepthBased ( distance_transform, one_way_distance_transform,
		BUF_BYTE(BYTE_depth_map_dilation), max_fill_value, w, h, 1, distance_transform_depth_dilation);

	MakeOneDirectional_DistantTransform_DepthBased ( distance_transform, one_way_distance_transform,
		BUF_BYTE(BYTE_depth_map_erosion), max_fill_value, w, h, 1, distance_transform_depth_erosion);

	MapIntBufferToByteBuffer ( distance_transform,			BUF_BYTE ( BYTE_distransform ),			w, h );
	MapIntBufferToByteBuffer ( one_way_distance_transform,	BUF_BYTE ( BYTE_distransform_oneway ),	w, h );
	CompensateFrame_3Planes( BUF_BYTE(BYTE3_compensated_frame), BUF_cMV (cMV_field), reference_frame, w, h );

	MV_Filter_OcclusionFilling ( BUF_BYTE(BYTE_occlusion_mask), NULL, BUF_BYTE(BYTE_depth_map_dilation), 
								BUF_cMV(cMV_uniform_filtered), BUF_cMV (cMV_dilated_field), 4, 4, w, h, 9 );
	CompensateFrame_3Planes( BUF_BYTE(BYTE3_compensated_frame_dil), BUF_cMV (cMV_dilated_field), reference_frame, w, h );
	Calculate_Frames_Difference_SeveralPlanes ( src_frame, BUF_BYTE(BYTE3_compensated_frame_dil), BUF_BYTE(BYTE3_frame_difference_dil), w, h, 3 );


	MV_Filter_OcclusionFilling ( BUF_BYTE(BYTE_occlusion_mask), NULL, BUF_BYTE(BYTE_depth_map_erosion), 
								BUF_cMV(cMV_uniform_filtered), BUF_cMV (cMV_eroded_field), 4, 4, w, h, 9 );
	CompensateFrame_3Planes( BUF_BYTE(BYTE3_compensated_frame_ero), BUF_cMV (cMV_eroded_field), reference_frame, w, h );
	Calculate_Frames_Difference_SeveralPlanes ( src_frame, BUF_BYTE(BYTE3_compensated_frame_ero), BUF_BYTE(BYTE3_frame_difference_ero), w, h, 3 );

	DEINIT_MEMORY_C (distance_transform );
	DEINIT_MEMORY_C (one_way_distance_transform);
}
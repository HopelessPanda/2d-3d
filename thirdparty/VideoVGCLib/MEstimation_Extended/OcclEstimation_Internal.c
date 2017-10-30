#include "OcclEstimation_Internal.h"
#include <VideoVGCLib/VGLibCommon/cMorphology.h>

#include "Distance_Transform.h"

#define ONE_MATCH_COUNT 255

void Occlusion_Mask_Label_Occlusions(BYTE *mask, int *label_mask, int x, int y, int width, int height, int label)
{
	int index, i;
	int pos_x_right = x, pos_x_left = x;
	for(i = x; i < width; i++)
	{
		index =  y*width + i;
		if( mask[index] == 0  &&  label_mask[index] == 0)
		{ 
			label_mask[index] = label;
			pos_x_right = width;
		}
		else
		{
			pos_x_right = i;
			break;
		}
	}
	for(i = x-1; i>=0; i--)
	{
		index = y*width + i;
		if( mask[index] == 0  &&  label_mask[index] == 0)
		{
			label_mask[index] = label;
			pos_x_left = -1;
		}
		else
		{
			pos_x_left = i;
			break;
		}
	}

	// excluding area borders
	pos_x_left  += 1;
	pos_x_right -= 1;

	if(y > 0)
	{
		for(i = pos_x_left; i <= pos_x_right; i++)
		{
			Occlusion_Mask_Label_Occlusions(mask, label_mask, i, y-1, width, height, label);
		}
	}
	if(y < height-1)
	{
		for(i = pos_x_left; i <= pos_x_right; i++)
		{
			Occlusion_Mask_Label_Occlusions(mask, label_mask, i, y+1, width, height, label);
		}
	}

	return;
}

#define MAX_GAP_SIZE (3*256)
BOOL Occlusion_Mask_Check_Gap(BYTE *mask, int *label_mask, int x, int y, int width, int height, int label, int * gap_size)
{
	int index, i;
	int pos_x_right = x, pos_x_left = x;
	BOOL result_flag = TRUE;

	if( *gap_size > MAX_GAP_SIZE)
		return FALSE;

	//if( label_mask[y*width + x] == -1 )
	//	return result_flag;

	for(i = x; i < width; i++)
	{
		index =  y*width + i;

		if( label_mask[index] == -2 )
		{
			return FALSE;
		}

		if( mask[index] == 255 && label_mask[index] == 0)
		{ 
			label_mask[index] = -1;
			pos_x_right = width;
			*gap_size += 1;
		}
		else
		{
			pos_x_right = i;

			if(label_mask[index] != -1 && label == 0)
			{
				label = label_mask[index];
			}
			else if( label_mask[index] != -1  &&  label_mask[index] != label)
			{
				result_flag = FALSE;
			}
			break;
		}
	}
	if( mask[y*width + x] == 255 )
	{
		for(i = x-1; i>=0; i--)
		{
			index = y*width + i;

			if( label_mask[index] == -2 )
				return FALSE;

			if( mask[index] == 255 && label_mask[index] == 0)
			{
				label_mask[index] = -1;
				pos_x_left = -1;
				*gap_size += 1;
			}
			else
			{
				pos_x_left = i;

				if(label_mask[index] != -1 && label == 0)
				{
					label = label_mask[index];
				}
				else if( label_mask[index] != -1 &&  label_mask[index] != label )
				{
					result_flag = FALSE;
				}

				break;
			}
		}
	}
	// excluding area borders
	pos_x_left  += 1;
	pos_x_right -= 1;

	if(y > 0)
	{
		for(i = pos_x_left; i <= pos_x_right; i++)
		{
			result_flag = Occlusion_Mask_Check_Gap(mask, label_mask, i, y-1, width, height, label, gap_size) && result_flag;
		}
	}
	if(y < height-1)
	{
		for(i = pos_x_left; i <= pos_x_right; i++)
		{
			result_flag = Occlusion_Mask_Check_Gap(mask, label_mask, i, y+1, width, height, label, gap_size) && result_flag;
		}
	}

	return result_flag;
}

void Occlusion_Mask_Fill_Area(BYTE * mask, int x, int y, int width, int height, int change_from_val, int change_to_val )
{
	int index, i;
	int pos_x_right = x, pos_x_left = x;

	if( mask[y*width +  x] != change_from_val )
	{
		return;
	}

	for(i = x; i < width; i++)
	{
		index =  y*width + i;
		if( mask[index] == change_from_val )
		{ 
			mask[index] = change_to_val;
			pos_x_right = width;
		}
		else
		{
			pos_x_right = i;
			break;
		}
	}
	for(i = x-1; i>=0; i--)
	{
		index = y*width + i;
		if( mask[index] == change_from_val )
		{
			mask[index] = change_to_val;
			pos_x_left = -1;
		}
		else
		{
			pos_x_left = i;
			break;
		}
	}

	// excluding area borders
	pos_x_left  += 1;
	pos_x_right -= 1;

	if(y > 0)
	{
		for(i = pos_x_left; i <= pos_x_right; i++)
		{
			Occlusion_Mask_Fill_Area(mask, i, y-1, width, height, change_from_val, change_to_val);
		}
	}
	if(y < height-1)
	{
		for(i = pos_x_left; i <= pos_x_right; i++)
		{
			Occlusion_Mask_Fill_Area(mask, i, y+1, width, height, change_from_val, change_to_val);
		}
	}

	return;
}

void Occlusion_Mask_Fill_Area_Int(int * mask, int x, int y, int width, int height, int change_from_val, int change_to_val )
{
	int index, i;
	int pos_x_right = x, pos_x_left = x;

	if( mask[y*width +  x] != change_from_val )
	{
		return;
	}

	for(i = x; i < width; i++)
	{
		index =  y*width + i;
		if( mask[index] == change_from_val )
		{ 
			mask[index] = change_to_val;
			pos_x_right = width;
		}
		else
		{
			pos_x_right = i;
			break;
		}
	}
	for(i = x-1; i>=0; i--)
	{
		index = y*width + i;
		if( mask[index] == change_from_val )
		{
			mask[index] = change_to_val;
			pos_x_left = -1;
		}
		else
		{
			pos_x_left = i;
			break;
		}
	}

	// excluding area borders
	pos_x_left  += 1;
	pos_x_right -= 1;

	if(y > 0)
	{
		for(i = pos_x_left; i <= pos_x_right; i++)
		{
			Occlusion_Mask_Fill_Area_Int(mask, i, y-1, width, height, change_from_val, change_to_val);
		}
	}
	if(y < height-1)
	{
		for(i = pos_x_left; i <= pos_x_right; i++)
		{
			Occlusion_Mask_Fill_Area_Int(mask, i, y+1, width, height, change_from_val, change_to_val);
		}
	}

	return;
}

void Occlusion_Mask_Refinement(BYTE * occlusion_mask, int width, int height)
{
	int * label_mask;
	int ind_x, ind_y;
	int label = 1;

	INIT_MEMORY_C( label_mask, 0, width * height, int );

	//////////////////////////////////////////////////////////////////////////
	// 1. Analyze pass. Label connected areas in occlusions mask
	// 
	for(ind_y = 0; ind_y < height; ++ ind_y)
	{
		for(ind_x = 0; ind_x < width; ++ ind_x)
		{
			int index = ind_y * width + ind_x;

			// skip non occlusion areas
			if( occlusion_mask [ index ] == 255  ||  label_mask[index] != 0 )
				continue;

			Occlusion_Mask_Label_Occlusions(occlusion_mask, label_mask, ind_x, ind_y, width, height, label);
			label += 1;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 2. Apply pass. Search gaps in occlusion mask (areas, which are surrounded by occlusions).
	// Consider such gaps as occlusions.
	// 
	for(ind_y = 0; ind_y < height; ++ ind_y)
	{
		for(ind_x = 0; ind_x < width; ++ ind_x)
		{
			int index = ind_y * width + ind_x;
			int gap_size = 0;

			if ( ind_y == 495  &&  ind_x == 1022 )
			{
				ind_y = ind_y;
			}

			// skip non occlusion areas
			if( occlusion_mask [ index ] == 0  ||  label_mask[index] == -1  ||  label_mask[index] == -2)
				continue;

			label = 0;
			if( Occlusion_Mask_Check_Gap(occlusion_mask, label_mask, ind_x, ind_y, width, height, label, &gap_size) )
			{
				Occlusion_Mask_Fill_Area(occlusion_mask, ind_x, ind_y, width, height, 255, 0) ;
			}
			else
			{
				Occlusion_Mask_Fill_Area_Int(label_mask, ind_x, ind_y, width, height, -1, -2);
			}
		}
	}

	DEINIT_MEMORY_C(label_mask);
}

void Accum_Considering_Occlusions(BYTE *src_buf, BYTE *src_buf_compensated, BYTE *occlusions, 
								  BYTE *dst_buf, int width, int height)
{
	int index, size = width * height;

#pragma omp parallel for
	for (index = 0; index < size; ++ index)
	{
		float weight_source = occlusions[index] / 255.0f;
		float weight_compens = 1 - weight_source;

		dst_buf[index] = (BYTE)CLIP( src_buf[index] * weight_source + src_buf_compensated[index] * weight_compens, 0, 255 );
	}
}

void Accum_Considering_Occlusions_cMV(cMV *src_buf, cMV *src_buf_compensated, BYTE *occlusions, 
									  cMV *dst_buf, int width, int height)
{
	int index, size = width * height;

#pragma omp parallel for
	for (index = 0; index < size; ++ index)
	{
		double weight_source = occlusions[index] / 255.0;
		double weight_compens = 1 - weight_source;

		dst_buf[index].x = (int)(src_buf[index].x * weight_source + src_buf_compensated[index].x * weight_compens);
		dst_buf[index].y = (int)(src_buf[index].y * weight_source + src_buf_compensated[index].y * weight_compens);
	}
}


void FillOcclusionsNearestNeighbor( cMV * src_motion_field, cMV * dst_motion_field, BYTE * occlusion_mask, int * occlusions_distance_transform, int width, int height )
{
	int x, y;
	int iteration;
	int check_radius = 1; // radius to search for nearest neighbor
	int min_value_init = -(check_radius + 1);
	int number_of_iterations = 0;

	memcpy( dst_motion_field, src_motion_field, width * height * sizeof(cMV) );

	number_of_iterations = Calculate_DistanceTransformMask( occlusions_distance_transform, occlusion_mask, width, height, 1 );

	for( iteration = 1; iteration <= number_of_iterations; ++ iteration )
	{
		// sequential processing, each iteration fills some amount of occlusion mask.
		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				int index = y * width + x;
				int internal_x, internal_y;

				int min_x = min_value_init; 
				int min_y = min_value_init;

				// drop not occlusion areas (no need in filling)
				if ( occlusions_distance_transform [index] != iteration )
					continue;

				// search for nearest neighbor in small neighborhood
				for ( internal_y = MAX( y - check_radius, 0); internal_y <= MIN( y + check_radius, height-1 ) ; ++ internal_y )
				{
					for ( internal_x = MAX( x - check_radius, 0); internal_x <= MIN( x + check_radius,  width-1 ) ; ++ internal_x )
					{
						int internal_index = internal_y * width + internal_x;

						// drop occlusion areas
						if( occlusions_distance_transform [internal_index] >= iteration )
							continue;

						// find nearest neighbor in small area around pixel. Check distance (should be closest)
						if( abs(internal_x	- x) + abs(internal_y	- y) > 
							abs(min_x		- x) + abs(min_y		- y) )
							continue;
						else
						{
							min_x = internal_x;
							min_y = internal_y;
						}
					}
				}

				// if some neighbor was found increment counter and remember pixel value
				if( min_x >= 0 && min_y >= 0 )
				{
					int index_to_take_from = min_y * width + min_x;

					dst_motion_field[index] = dst_motion_field[index_to_take_from];
				}
			}
		}
		// repeat until all occlusions are filled
	}
}

void Calculate_Rough_Occlusions_From_Reference_Motion ( BYTE * occlusion_mask_source_frame, cMV *reference_to_source_motion, int width, int height )
{
	int ind_y, ind_x, index;

	memset(occlusion_mask_source_frame, 0, width*height);

	// occlusion is an area, where none of motion vectors come
#pragma omp parallel for private(ind_x, index)
	for(ind_y = 0; ind_y < height; ++ ind_y)
	{
		for(ind_x = 0; ind_x < width; ++ ind_x)
		{
			float x, y;
			int b_x_low, b_y_low;
			int b_x_high, b_y_high;

			index = ind_y*width +ind_x;

			x = reference_to_source_motion[index].x / 4.0f;
			y = reference_to_source_motion[index].y / 4.0f;

			b_y_low  = (int)CLIP( floor( ind_y + y )		, 0, height - 1);
			b_y_high = (int)CLIP( floor( ind_y + y + 0.5f )	, 0, height - 1);
			b_x_low  = (int)CLIP( floor( ind_x + x)			, 0, width - 1);
			b_x_high = (int)CLIP( floor( ind_x + x + 0.5f )	, 0, width - 1);

			occlusion_mask_source_frame[b_y_low  * width + b_x_low]  = CLIP(occlusion_mask_source_frame[b_y_low  * width + b_x_low]  + ONE_MATCH_COUNT, 0, 255);
			occlusion_mask_source_frame[b_y_high * width + b_x_low]  = CLIP(occlusion_mask_source_frame[b_y_high * width + b_x_low]  + ONE_MATCH_COUNT, 0, 255);
			occlusion_mask_source_frame[b_y_low  * width + b_x_high] = CLIP(occlusion_mask_source_frame[b_y_low  * width + b_x_high] + ONE_MATCH_COUNT, 0, 255);
			occlusion_mask_source_frame[b_y_high * width + b_x_high] = CLIP(occlusion_mask_source_frame[b_y_high * width + b_x_high] + ONE_MATCH_COUNT, 0, 255);
		}
	}

	//Occlusion_Mask_Refinement(occlusion_mask_source_frame,  width, height);

	{
		BYTE *mask_buf;
		WindowMask morph_mask;

		INIT_MEMORY_C(mask_buf, 0, width*height, BYTE);

		Morphology_InitWindows();
		morph_mask = Morphology_GetWindowMask(1, FALSE);

		//Dilation_Value_BYTE(mask_buf, occlusion_mask_source_frame, width, height, &morph_mask);
		//Dilation_Value_BYTE(occlusion_mask_source_frame, mask_buf, width, height, &morph_mask);
		//Erosion_Value_BYTE(mask_buf, occlusion_mask_source_frame, width, height, &morph_mask);
		//Erosion_Value_BYTE(occlusion_mask_source_frame, mask_buf, width, height, &morph_mask);
		//Erosion_Value_BYTE(mask_buf, occlusion_mask_source_frame, width, height, &morph_mask);
		//Erosion_Value_BYTE(occlusion_mask_source_frame, mask_buf, width, height, &morph_mask);

		DEINIT_MEMORY_C(mask_buf);
	}
}

void Calculate_Rough_Occlusions_From_Source_Motion( BYTE * occlusion_mask_source_frame, cMV *source_to_reference_motion, int width, int height )
{
	int ind_y, ind_x, index;

	memset(occlusion_mask_source_frame, 0, width*height);

	// occlusion is an area, where none of motion vectors come
#pragma omp parallel for private(ind_x, index)
	for(ind_y = 0; ind_y < height; ++ ind_y)
	{
		for(ind_x = 0; ind_x < width; ++ ind_x)
		{
			float x, y;
			int b_x_low, b_y_low;
			int b_x_high, b_y_high;

			index = ind_y*width +ind_x;

			x = source_to_reference_motion[index].x / 4.0f;
			y = source_to_reference_motion[index].y / 4.0f;

			b_y_low  = (int)CLIP( floor( ind_y + y )		, 0, height - 1);
			b_y_high = (int)CLIP( floor( ind_y + y + 0.5f )	, 0, height - 1);
			b_x_low  = (int)CLIP( floor( ind_x + x)			, 0, width - 1);
			b_x_high = (int)CLIP( floor( ind_x + x + 0.5f )	, 0, width - 1);

			occlusion_mask_source_frame[b_y_low  * width + b_x_low]  = CLIP(occlusion_mask_source_frame[b_y_low  * width + b_x_low]  + 128, 0, 255);
		}
	}
}
void Calculate_Rough_Occlusions_Stereo( BYTE * occlusion_mask_left, BYTE * occlusion_mask_right, cMV *field_for_left_view, cMV *field_for_right_view, int width, int height )
{
	int ind_y, ind_x, index;

	memset(occlusion_mask_left, 0, width*height);
	memset(occlusion_mask_right, 0, width*height);

	// occlusion is an area, where none of motion vectors come
#pragma omp parallel for private(ind_x, index)
	for(ind_y = 0; ind_y < height; ++ ind_y)
	{
		for(ind_x = 0; ind_x < width; ++ ind_x)
		{
			float x, y;
			int b_x_low, b_y_low;
			int b_x_high, b_y_high;

			index = ind_y*width +ind_x;

			// left view occlusion mask
			x = field_for_right_view[index].x / 4.0f;
			y = field_for_right_view[index].y / 4.0f;
			b_y_low  = (int)CLIP( floor( ind_y + y )		, 0, height - 1);
			b_y_high = (int)CLIP( floor( ind_y + y + 0.5f )	, 0, height - 1);
			b_x_low  = (int)CLIP( floor( ind_x + x)			, 0, width - 1);
			b_x_high = (int)CLIP( floor( ind_x + x + 0.5f )	, 0, width - 1);

			occlusion_mask_left[b_y_low  * width + b_x_low]  = CLIP(occlusion_mask_left[b_y_low  * width + b_x_low]  + ONE_MATCH_COUNT, 0, 255);
			occlusion_mask_left[b_y_high * width + b_x_low]  = CLIP(occlusion_mask_left[b_y_high * width + b_x_low]  + ONE_MATCH_COUNT, 0, 255);
			occlusion_mask_left[b_y_low  * width + b_x_high] = CLIP(occlusion_mask_left[b_y_low  * width + b_x_high] + ONE_MATCH_COUNT, 0, 255);
			occlusion_mask_left[b_y_high * width + b_x_high] = CLIP(occlusion_mask_left[b_y_high * width + b_x_high] + ONE_MATCH_COUNT, 0, 255);

			// right view occlusion mask
			x = field_for_left_view[index].x / 4.0f;
			y = field_for_left_view[index].y / 4.0f;

			/*b_y_low = (int)CLIP( floor( ind_y + y + 0.5f ), 0, height - 1);
			b_x_low = (int)CLIP( floor( ind_x + x + 0.5f ), 0, width - 1);
			data->occlusion_mask_right[b_y_low * width + b_x_low] = 255;*/
			b_y_low  = (int)CLIP( floor( ind_y + y )		, 0, height - 1);
			b_y_high = (int)CLIP( floor( ind_y + y + 0.5f )	, 0, height - 1);
			b_x_low  = (int)CLIP( floor( ind_x + x)			, 0, width - 1);
			b_x_high = (int)CLIP( floor( ind_x + x + 0.5f )	, 0, width - 1);

			occlusion_mask_right[b_y_low  * width + b_x_low]  = CLIP(occlusion_mask_right[b_y_low  * width + b_x_low]  + ONE_MATCH_COUNT, 0, 255);
			occlusion_mask_right[b_y_high * width + b_x_low]  = CLIP(occlusion_mask_right[b_y_high * width + b_x_low]  + ONE_MATCH_COUNT, 0, 255);
			occlusion_mask_right[b_y_low  * width + b_x_high] = CLIP(occlusion_mask_right[b_y_low  * width + b_x_high] + ONE_MATCH_COUNT, 0, 255);
			occlusion_mask_right[b_y_high * width + b_x_high] = CLIP(occlusion_mask_right[b_y_high * width + b_x_high] + ONE_MATCH_COUNT, 0, 255);
		}
	}

	//memcpy(data->occlusion_mask_right, data->occlusion_mask_left, width * height);
	Occlusion_Mask_Refinement(occlusion_mask_left,  width, height);
	Occlusion_Mask_Refinement(occlusion_mask_right, width, height);

	/*#pragma omp parallel
	{
		#pragma omp sections
		{
			#pragma omp section
			{			
				BYTE *mask_buf;
				WindowMask morph_mask;

				INIT_MEMORY_C(mask_buf, 0, width*height, BYTE);
				
				Morphology_InitWindows();
				morph_mask = Morphology_GetWindowMask(1, FALSE);

				// dst and src remain after two calls
				Dilation_Value_BYTE_Faster(mask_buf, occlusion_mask_left, width, height, &morph_mask);
				//Dilation_Value_BYTE_Faster(occlusion_mask_left, mask_buf, width, height, &morph_mask);

				// apply morphology to occlusion mask
				//Erosion_Value_BYTE_Faster(mask_buf, occlusion_mask_left, width, height, &morph_mask);
				Erosion_Value_BYTE_Faster(occlusion_mask_left, mask_buf, width, height, &morph_mask);
				Erosion_Value_BYTE_Faster(mask_buf, occlusion_mask_left, width, height, &morph_mask);
				Erosion_Value_BYTE_Faster(occlusion_mask_left, mask_buf, width, height, &morph_mask);

				DEINIT_MEMORY_C(mask_buf);
			}
			#pragma omp section
			{
				BYTE *mask_buf;
				WindowMask morph_mask;

				INIT_MEMORY_C(mask_buf, 0, width*height, BYTE);

				Morphology_InitWindows();
				morph_mask = Morphology_GetWindowMask(1, FALSE);

				// dst and src remain after two calls
				Dilation_Value_BYTE_Faster(mask_buf, occlusion_mask_right, width, height, &morph_mask);
				//Dilation_Value_BYTE_Faster(occlusion_mask_right, mask_buf, width, height, &morph_mask);

				// apply morphology to occlusion mask
				//Erosion_Value_BYTE_Faster(mask_buf, occlusion_mask_right, width, height, &morph_mask);
				Erosion_Value_BYTE_Faster(occlusion_mask_right, mask_buf, width, height, &morph_mask);
				Erosion_Value_BYTE_Faster(mask_buf, occlusion_mask_right, width, height, &morph_mask);
				Erosion_Value_BYTE_Faster(occlusion_mask_right, mask_buf, width, height, &morph_mask);

				DEINIT_MEMORY_C(mask_buf);
	*		}
		}
	}*/
}

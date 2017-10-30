#include <VideoGMLib/Common/CommonTypes.h>
#include "Distance_Transform.h"
#include "HierarchyME.h"
#include "MExt_CoreInlineFunctions.h"

int * GenerateDistanceKernel ( int radius )
{
	int index_y, index_x;
	int pitch = 2*radius + 1;

	int *kernel ;

	INIT_MEMORY_C ( kernel, 0, pitch * pitch, int );

	// initializing
	for ( index_y = -radius; index_y <= radius; ++ index_y )
	{
		for ( index_x = -radius; index_x <= radius; ++index_x )
		{
			int index = (index_y+radius) * pitch + index_x + radius;
			kernel[index] = abs(index_y) + abs(index_x);
		}
	}

	return kernel;
}

void DestroyDistanceKernel ( int ** kernel )
{
	DEINIT_MEMORY_C ( *kernel );
}

int Calculate_DistanceTransformMask(int * filling_mask, BYTE * confidence_map, int width, int height, int blocksize)
{
	int ind_x, ind_y;
	int distance_radius = 2;
	int step = blocksize * distance_radius;
	int step_line_offset = step * width;
	int line_shift = width * blocksize;

	int max_fill = 0;

	// indicates, that mask is filled with values
	int changed_in_iteration;

	BYTE * internal_confidence;
	int * distance_kernel;
	int distance_kernel_pitch = distance_radius * 2 + 1;

	INIT_MEMORY_C(internal_confidence, 0, width*height, BYTE);
	memcpy(internal_confidence, confidence_map, width*height);

	memset(filling_mask, 0, width * height * sizeof(int) );

	distance_kernel = GenerateDistanceKernel ( distance_radius );

	do
	{
		BYTE * confidence_map_backup		= confidence_map;
		BYTE * internal_confidence_backup	= internal_confidence;
		int  * filling_mask_backup			= filling_mask;

		changed_in_iteration = 0;

		for(ind_y = 0; ind_y < height; ind_y += blocksize)
		{
			for(ind_x = 0; ind_x < width; ind_x += blocksize)
			{
				if( confidence_map_backup[ind_x] == 255 )
				{
					continue;
				}
				else
				{
					int start_internal_x = MAX(0, ind_x - step) /*- ind_x*/, 
						start_internal_y = MAX(0, ind_y - step) - ind_y;
					int end_internal_x = MIN(width-1, ind_x + step) /*- ind_x*/,
						end_internal_y = MIN(height-1, ind_y + step) - ind_y;

					int internal_x, internal_y;
					int kernel_ind_x;
					int start_kernel_ind_x = (start_internal_x - ind_x) / blocksize + distance_radius ;
					int start_kernel_ind_y = (start_internal_y)			/ blocksize + distance_radius ;

					BYTE * cur_internal_confidence  = internal_confidence_backup	+ width					* start_internal_y;
					int  * cur_filling_mask			= filling_mask_backup			+ width					* start_internal_y;
					int  * cur_distance_kernel		= distance_kernel				+ distance_kernel_pitch * start_kernel_ind_y ;

					for(internal_y = start_internal_y; internal_y <= end_internal_y; internal_y += blocksize)
					{
						for (internal_x = start_internal_x, kernel_ind_x = start_kernel_ind_x; internal_x <= end_internal_x; internal_x += blocksize, ++kernel_ind_x )
						{
							// this value represents shift in blocks from the current block to the nearest neighbor with 100% confidence
							int change_val = cur_distance_kernel[kernel_ind_x] ;
							// priority of block[ind_y, ind_x], according to block [internal_y, internal_x]
							int fill_value = cur_filling_mask[internal_x] + change_val;

							if(cur_internal_confidence[internal_x] == 255)
							{
								// filling priority is minimum from existing priority and found priority.
								if( filling_mask_backup[ind_x] == 0  ||  filling_mask_backup[ind_x] > fill_value  )
								{
									filling_mask_backup[ind_x] = fill_value;
									++ changed_in_iteration;
								}
							}
						}
						cur_internal_confidence += line_shift;
						cur_filling_mask		+= line_shift;
						cur_distance_kernel		+= distance_kernel_pitch;
					}
				}
			}
			filling_mask_backup			+= line_shift;
			internal_confidence_backup	+= line_shift;
			confidence_map_backup		+= line_shift;
		}

		for(ind_y = 0; ind_y < height; ind_y += blocksize)
		{
			for(ind_x = 0; ind_x < width; ind_x += blocksize)
			{
				int index = ind_y * width + ind_x;
				if(internal_confidence[index] == 255)
					continue;
				if(filling_mask[index] != 0)
				{
					if(max_fill < filling_mask[index])
						max_fill = filling_mask[index];

					internal_confidence[index] = 255;
				}
			}
		}
	}
	while ( changed_in_iteration != 0  );

	for(ind_y = 0; ind_y < height; ind_y += blocksize)
	{
		for(ind_x = 0; ind_x < width; ind_x += blocksize)
		{
			Set_Block_Value_Int ( filling_mask, ind_x, ind_y, width, filling_mask[ind_y * width + ind_x], blocksize );
		}
	}

	DEINIT_MEMORY_C(internal_confidence);
	DestroyDistanceKernel ( &distance_kernel );

	return max_fill;
}

void MakeOneDirectional_DistantTransform_DepthBased (int * filling_mask, int * dst_filling_mask, BYTE * depth_map, int max_fill_value, 
													 int width, int height, int blocksize, one_dir_distance_transform_type dis_type )
{
	int ind_x, ind_y;
	int iteration; // current fill value
	int changed_in_iteration = 0;

	// step 1. Fill transform with depth map
	for ( iteration = 1; iteration <= max_fill_value; ++ iteration )
	{
		for ( ind_y = 0; ind_y < height; ind_y += blocksize )
		{
			for (ind_x = 0; ind_x < width; ind_x += blocksize )
			{
				int index = ind_y * width + ind_x;
				int internal_ind_x, internal_ind_y;

				int max_depth_value = -1;
				int min_depth_value = 256;
				
				if ( filling_mask[index] != iteration )
					continue;
				
				// compare 4 nearest neighbors depth values. Choose maximum from the previous distant transform iteration
				for ( internal_ind_y = MAX(0, ind_y - blocksize) ; internal_ind_y <= MIN (height-blocksize, ind_y + blocksize); internal_ind_y += blocksize )
				{
					for ( internal_ind_x = MAX(0, ind_x - blocksize) ; internal_ind_x <= MIN (width-blocksize, ind_x + blocksize); internal_ind_x += blocksize )
					{
						int internal_index;
						if ( abs(internal_ind_y - ind_y) + abs(internal_ind_x - ind_x) != blocksize )
						{
							continue;
						}

						internal_index = internal_ind_y * width + internal_ind_x;
						if ( filling_mask[internal_index] != iteration-1 )
							continue;

						if( max_depth_value < depth_map[internal_index] )
						{
							max_depth_value = depth_map[internal_index];
						}
						if( min_depth_value > depth_map[internal_index] )
						{
							min_depth_value = depth_map[internal_index];
						}
					}
				}

				depth_map[index] =	(dis_type == distance_transform_depth_dilation) ? max_depth_value : 
								  ( (dis_type == distance_transform_depth_erosion)  ? min_depth_value : depth_map[index]) ;
			}
		}
	}

	// step 1.5. Maximize local maximums from their neighbors.
	for ( ind_y = 0; ind_y < height; ind_y += blocksize )
	{
		for (ind_x = 0; ind_x < width; ind_x += blocksize )
		{
			int index = ind_y * width + ind_x;
			int internal_ind_x, internal_ind_y;

			int max_depth_value = depth_map[index];
			int min_depth_value = depth_map[index];

			BOOL non_local_maximum = FALSE;

			if ( filling_mask[index] == 0 )
				continue;

			// local maximum is maximum 2 pixels dimension. So it can be processed by the same cycle
			for ( internal_ind_y = MAX(0, ind_y - blocksize) ; internal_ind_y <= MIN (height-blocksize, ind_y + blocksize); internal_ind_y += blocksize )
			{
				for ( internal_ind_x = MAX(0, ind_x - blocksize) ; internal_ind_x <= MIN (width-blocksize, ind_x + blocksize); internal_ind_x += blocksize )
				{
					int internal_index;
					if ( abs(internal_ind_y - ind_y) + abs(internal_ind_x - ind_x) != blocksize )
					{
						continue;
					}

					internal_index = internal_ind_y * width + internal_ind_x;

					// if there is value bigger, than current, break
					if ( filling_mask[internal_index] > filling_mask [index] )
					{
						non_local_maximum = TRUE;
						break;
					}

					// choose maximum from all local maximums
					if ( filling_mask[internal_index] == filling_mask [index] )
					{
						if( max_depth_value < depth_map[internal_index] )
						{
							max_depth_value = depth_map[internal_index];
						}
						if( min_depth_value > depth_map[internal_index] )
						{
							min_depth_value = depth_map[internal_index];
						}
					}
				}
			}
			// skip if pixel is not local maximum
			if ( non_local_maximum )
				continue;

			depth_map[index] = 	(dis_type == distance_transform_depth_dilation) ? max_depth_value : 
							  ( (dis_type == distance_transform_depth_erosion)  ? min_depth_value : depth_map[index]) ;
		}
	}

	memcpy(dst_filling_mask, filling_mask, width * height * sizeof(int) );

	//////////////////////////////////////////////////////////////////////////
	// step 2. Propagation of distance transform, using depth map.
	for ( iteration = max_fill_value-1; iteration > 0; -- iteration )
	{
		for ( ind_y = 0; ind_y < height; ind_y += blocksize )
		{
			for (ind_x = 0; ind_x < width; ind_x += blocksize )
			{
				int depth_threshold = 4;
				int index = ind_y * width + ind_x;
				int internal_ind_x, internal_ind_y;
				
				if ( filling_mask[index] != iteration )
					continue;

				// compare 4 nearest neighbors depth values. Choose maximum from the previous distant transform iteration
				for ( internal_ind_y = MAX(0, ind_y - blocksize) ; internal_ind_y <= MIN (height-blocksize, ind_y + blocksize); internal_ind_y += blocksize )
				{
					for ( internal_ind_x = MAX(0, ind_x - blocksize) ; internal_ind_x <= MIN (width-blocksize, ind_x + blocksize); internal_ind_x += blocksize )
					{
						int internal_index;
						// excluding diagonal neighbors
						/*if ( abs(internal_ind_y - ind_y) + abs(internal_ind_x - ind_x) != blocksize )
						{
							continue;
						}*/

						internal_index = internal_ind_y * width + internal_ind_x;
						// excluding not subsequent iterations
						if ( filling_mask[internal_index] != iteration+1 )
							continue;

						// propagating maximum depth from neighbors
						if( dis_type == distance_transform_depth_dilation )
						{
							if( depth_map[index] < depth_map[internal_index]  &&  abs ( depth_map[index] - depth_map[internal_index] ) > depth_threshold  )
							{
								depth_map[index]		= depth_map[internal_index];
								dst_filling_mask[index] = dst_filling_mask[internal_index] + 1;
							}
						}
						else if ( dis_type == distance_transform_depth_erosion )
						{
							if( depth_map[index] > depth_map[internal_index]  &&  abs ( depth_map[index] - depth_map[internal_index] ) > depth_threshold  )
							{
								depth_map[index]		= depth_map[internal_index];
								dst_filling_mask[index] = dst_filling_mask[internal_index] + 1;
							}
						}
					}
				}
			}
		}
	}

	return;
	//////////////////////////////////////////////////////////////////////////
	// step 3. Correction of distance transform, using neighbor distance minimization.
	do
	{
		changed_in_iteration = 0;

		for(ind_y = 0; ind_y < height; ind_y += blocksize)
		{
			for(ind_x = 0; ind_x < width; ind_x += blocksize)
			{
				int index = ind_y * width + ind_x;
				int internal_ind_x, internal_ind_y;

				if ( dst_filling_mask[index] == 0 )
					continue;

				for ( internal_ind_y = MAX(0, ind_y - blocksize) ; internal_ind_y <= MIN (height-blocksize, ind_y + blocksize); internal_ind_y += blocksize )
				{
					for ( internal_ind_x = MAX(0, ind_x - blocksize) ; internal_ind_x <= MIN (width-blocksize, ind_x + blocksize); internal_ind_x += blocksize )
					{
						int internal_index;
						if ( abs(internal_ind_y - ind_y) + abs(internal_ind_x - ind_x) != blocksize )
						{
							continue;
						}

						internal_index = internal_ind_y * width + internal_ind_x;
						if ( dst_filling_mask[internal_index] == 0 )
							continue;

						if ( dst_filling_mask[index] > dst_filling_mask[internal_index] + 1 )
						{
							dst_filling_mask[index] = dst_filling_mask[internal_index] + 1;
							changed_in_iteration += 1;
						}
					}
				}
			}
		}
	}
	while ( changed_in_iteration != 0  );
}

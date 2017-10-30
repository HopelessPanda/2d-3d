#pragma once 

typedef enum
{
	distance_transform_depth_erosion,
	distance_transform_depth_dilation
} one_dir_distance_transform_type;

int Calculate_DistanceTransformMask(int * filling_mask, BYTE * confidence_map, int width, int height, int blocksize);
void MakeOneDirectional_DistantTransform_DepthBased (int * filling_mask, int * dst_filling_mask, BYTE * depth_map, int max_fill_value, 
													 int width, int height, int blocksize, one_dir_distance_transform_type dis_type );

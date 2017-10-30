#pragma once

#include <VideoGMLib/Common/CommonTypes.h>
#include <VideoVGCLib/MEstimation/cMEstimation.h>

#include "Calculation_Buffers.h"

void Create_Refine_Mask(BYTE * source_contrast, BYTE *sad_contrast, BYTE * source_dispersion, BYTE * lrc, BYTE * dst_mask_uniform, int width, int height, int filter_strength);
void Create_Refine_Texture_Mask_DepthBased ( BYTE * source_contrast, BYTE * source_dispersion, BYTE* depth_sobel_mask, BYTE * lrc, BYTE * dst_mask_uniform, int width, int height );
void Create_Refine_Texture_Mask ( BYTE * source_contrast, BYTE * source_dispersion, BYTE * lrc, BYTE * dst_mask_uniform, int width, int height );
void Create_Refine_NonTexture_Mask ( BYTE * source_contrast, BYTE * source_dispersion, BYTE * lrc, BYTE * dst_mask_uniform, int width, int height );
void Create_Invert_LRC_Mask ( BYTE * lrc, BYTE * dst_mask, int width, int height );

void Get_Errors_From_MotionField( cMV * src_field, BYTE * dst_error_mask, int width, int height, double norm_koef);

void Calculate_Disparity_From_MotionField( cMV * src_field, BYTE * dst_disparity, int width, int height, double d_mult);

int  Calculate_MVDispersion_Map(gmcMotionEstimator *me_inst, BYTE *src, BYTE *map, BOOL consider_color);

void LeftRightConsistency(cMV * field_for_left_view, cMV * field_for_right_view, 
						  BYTE * source_frame_left, BYTE * source_frame_right,
						  BYTE * dst_mask_left, BYTE * dst_mask_right,
						  BYTE * dst_sad_left, BYTE * dst_sad_right,
						  int width, int height);

void BufferStorage_LeftRightConsistency( Buffer_Storage * frame_buffers_left, Buffer_Storage * frame_buffers_right, 
										cMV_buffer_type motion_field_type, BYTE_buffer_type lrc_result_type );

void Calculate_Frames_Difference_BlockBased (BYTE * frame_left, BYTE * frame_right, BYTE * dst_sad_mask, int width, int height, int plane_num, int base_block_size);

void Calculate_Frames_Difference(BYTE * buffer1, BYTE * buffer2, BYTE * dst_sad_buf, int width, int height);
void Calculate_Frames_Difference_SeveralPlanes ( BYTE * buffer_1, BYTE * buffer_2, BYTE * dst_sad, int width, int height, int plane_num );

void Calculate_SAD_Error_3planes(cMV * motion_field, BYTE * src_frame, BYTE * reference_frame, BYTE *compensation_temp_buf, BYTE * dst_sad_error_frame, int width, int height);

void Calculate_GradientMask( BYTE * depth_map, BYTE * dst_gradient_map, ushort width, ushort height, BYTE sobel_radius, BYTE threshold);
void Calculate_GradientMask_3Planes ( BYTE * source_frame, BYTE * dst_gradient_mask, int width, int height, int radius );
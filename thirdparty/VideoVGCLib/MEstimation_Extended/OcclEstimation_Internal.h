#pragma once
#include <VideoGMLib/Common/CommonTypes.h>
#include <VideoVGCLib/MEstimation/cMV.h>

void Occlusion_Mask_Refinement(BYTE * occlusion_mask, int width, int height);

void Accum_Considering_Occlusions(BYTE *src_buf, BYTE *src_buf_compensated, BYTE *occlusions, 
								  BYTE *dst_buf, int width, int height);

void Accum_Considering_Occlusions_cMV(cMV *src_buf, cMV *src_buf_compensated, BYTE *occlusions, 
								  cMV *dst_buf, int width, int height);

void FillOcclusionsNearestNeighbor( cMV * src_motion_field, cMV * dst_motion_field, BYTE * occlusion_mask, int * occlusions_distance_transform, int width, int height );

void Calculate_Rough_Occlusions_From_Reference_Motion ( BYTE * occlusion_mask_source_frame, cMV *reference_to_source_motion, int width, int height );
void Calculate_Rough_Occlusions_From_Source_Motion( BYTE * occlusion_mask_source_frame, cMV *source_to_reference_motion, int width, int height );
void Calculate_Rough_Occlusions_Stereo( BYTE * occlusion_mask_left, BYTE * occlusion_mask_right, cMV *field_for_left_view, cMV *field_for_right_view, int width, int height );

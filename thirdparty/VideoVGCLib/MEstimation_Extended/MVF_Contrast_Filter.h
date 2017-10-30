#pragma once

#include "HierarchyME.h"

void MV_Filter ( BYTE * mask, BYTE * source_frame, BYTE * depth_frame, 
				cMV * src_field, cMV * dst_field, int cands_num, int base_block_size , int width, int height);

void MV_Filter_Internal ( BYTE * mask, BYTE * source_frame, BYTE * depth_frame, 
						 cMV * src_field, cMV * dst_field, int cands_num, int base_block_size , int width, int height, // usual options
						 BOOL use_depth_threshold_search, int depth_threshold ); // internal options

void MV_Depth_Filter	( gmcMotionEstimator *ME_inst, BYTE *mask, BYTE * src, BYTE *src_depth, BYTE *dst_depth, int cands_num, int base_block_size);

void Prefill_Refinement_Mask(BYTE * src, BYTE *mask, BYTE * dst, int width, int height, int block_base_size);
void Prefill_Refinement_Mask_cMV( cMV * src, BYTE *mask, cMV * dst, int width, int height, int block_base_size );
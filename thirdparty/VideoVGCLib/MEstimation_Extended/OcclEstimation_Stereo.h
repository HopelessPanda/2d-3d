#pragma once

#include <VideoVGCLib/MEstimation/cMV.h>
#include <VideoGMLib/Common/CommonTypes.h>

#include "Calculation_Buffers.h"

typedef struct 
{
	int width;
	int height;

	BOOL need_invert;

	Buffer_Storage * frame_buffers[2];

	/*BYTE * occlusion_mask_left;
	BYTE * occlusion_mask_right;

	BYTE *compensated_buf_left;
	BYTE *compensated_buf_right;

	BYTE *LRC_buf_left;
	BYTE *LRC_buf_right;

	BYTE *LR_compensated_frame_left;
	BYTE *LR_compensated_frame_right;

	BYTE *shift_mask_left;
	BYTE *shift_mask_right;

	BYTE *zinger_filled_left;
	BYTE *zinger_filled_right;

	BYTE *difference_mask_left;
	BYTE *difference_mask_right;

	cMV *mv_compensated_buf_left;
	cMV *mv_compensated_buf_right;

	cMV *motion_for_left_view;
	cMV *motion_for_right_view;*/

} DisparityOcclData;

#define BUF_L_BYTE(buf_type) Buffer_Storage_GetBuffer_BYTE ( occl_data->frame_buffers[0], buf_type )
#define BUF_R_BYTE(buf_type) Buffer_Storage_GetBuffer_BYTE ( occl_data->frame_buffers[1], buf_type )

#define BUF_L_cMV(buf_type) Buffer_Storage_GetBuffer_cMV ( occl_data->frame_buffers[0], buf_type )
#define BUF_R_cMV(buf_type) Buffer_Storage_GetBuffer_cMV ( occl_data->frame_buffers[1], buf_type )

void DisparityOcclData_Init(DisparityOcclData * data, int width, int height, Buffer_Storage * storage_left, Buffer_Storage * storage_right);
void DisparityOcclData_Deinit(DisparityOcclData * data);

void Process_Occlusions_Upd( DisparityOcclData *data, BYTE *src_left, BYTE *src_right, BYTE *dst_left, BYTE *dst_right, cMV_buffer_type motion_field_type );

void Correct_MVOcclusions_From_Disparity(cMV * dst, BYTE * occlusion_mask, BYTE *disparity, int width, int height);
#include "OcclEstimation_Internal.h"
#include "OcclEstimation_Stereo.h"

#include "ME_Compensation.h"
#include "Zinger_Inpainting.h"
#include "MExt_Metrics.h"

/// returns average value of difference
// after calculation zinger contains mask of  new refinemment.
void Combine_Zinger_Compensated(BYTE * zinger_filled, BYTE * compensation_filled, BYTE * occlusion_mask, BYTE * difference_mask, int width, int height)
{
#define FILL_DIFFERENCE 8

	int index;
	int frame_size = height*width;
	long sum = 0;
	int min_val = 256;
	int max_val = 0;
	int cur_val;
	long sum_count = 0;

	for(index = 0; index < frame_size; ++ index)
	{
		if(occlusion_mask[index] == 255)
		{
			difference_mask[index] = 255;
			continue;
		}

		cur_val = abs( compensation_filled[index] - zinger_filled[index] );
		sum += cur_val;
		++ sum_count;
		difference_mask[index] = /*255 - */cur_val/* * FILL_DIFFERENCE*/;

		if(cur_val < min_val) min_val = cur_val;
		if(cur_val > max_val) max_val = cur_val;
	}

	if(sum_count == 0) sum_count = 1;

	sum /= sum_count;

	for(index = 0; index < frame_size; ++ index)
	{
		if(occlusion_mask[index] == 255)
			continue;

		if(difference_mask[index] < sum )
		{
			difference_mask[index] = 255;
		}
		else if( compensation_filled[index] < zinger_filled[index] )
		{
			difference_mask[index] = 255;
		}
		else
		{
			difference_mask[index] = 0;
		}
	}
}

void DisparityOcclData_Init(DisparityOcclData * data, int width, int height, 
							Buffer_Storage * storage_left, Buffer_Storage * storage_right)
{
	data->width = width;
	data->height = height;

	data->need_invert = TRUE;

	data->frame_buffers[0] = storage_left;
	data->frame_buffers[1] = storage_right;

	//data->occlusion_mask_left  = Buffer_Storage_GetBuffer_BYTE(storage_left, BYTE_occlusion_mask);
	//data->occlusion_mask_right = Buffer_Storage_GetBuffer_BYTE(storage_right, BYTE_occlusion_mask);

	//data->LRC_buf_left = Buffer_Storage_GetBuffer_BYTE(storage_left, BYTE_lrc_mask);
	//data->LRC_buf_right = Buffer_Storage_GetBuffer_BYTE(storage_right, BYTE_lrc_mask);

	//data->zinger_filled_left  = Buffer_Storage_GetBuffer_BYTE(storage_left,  BYTE_zinger_fill);
	//data->zinger_filled_right = Buffer_Storage_GetBuffer_BYTE(storage_right, BYTE_zinger_fill);

	//data->compensated_buf_left  = Buffer_Storage_GetBuffer_BYTE(storage_left,  BYTE_compensated_frame);
	//data->compensated_buf_right = Buffer_Storage_GetBuffer_BYTE(storage_right, BYTE_compensated_frame);

	//data->difference_mask_left  = Buffer_Storage_GetBuffer_BYTE(storage_left,  BYTE_difference_fill);
	//data->difference_mask_right = Buffer_Storage_GetBuffer_BYTE(storage_right, BYTE_difference_fill);

	//data->LR_compensated_frame_left  = Buffer_Storage_GetBuffer_BYTE(storage_left,  BYTE_LR_compensation);
	//data->LR_compensated_frame_right = Buffer_Storage_GetBuffer_BYTE(storage_right, BYTE_LR_compensation);

	//Set_Internal_MotionFields(data,	Buffer_Storage_GetBuffer_cMV(storage_left,  cMV_field), 
	//								Buffer_Storage_GetBuffer_cMV(storage_right, cMV_field) );

	//INIT_MEMORY_C(data->mv_compensated_buf_left, 0, width*height, cMV);
	//INIT_MEMORY_C(data->mv_compensated_buf_right, 0, width*height, cMV);
}

void DisparityOcclData_Deinit(DisparityOcclData * data)
{
	//DEINIT_MEMORY_C(data->mv_compensated_buf_left);
	//DEINIT_MEMORY_C(data->mv_compensated_buf_right);
}


void Process_Occlusions_Upd( DisparityOcclData *occl_data, BYTE *src_left, BYTE *src_right, BYTE *dst_left, BYTE *dst_right, cMV_buffer_type motion_field_type )
{
	int w = occl_data->width;
	int h = occl_data->height;

	Calculate_Rough_Occlusions_Stereo ( BUF_L_BYTE(BYTE_occlusion_mask), BUF_R_BYTE(BYTE_occlusion_mask), BUF_L_cMV(motion_field_type), BUF_R_cMV(motion_field_type), w, h );

	Motion_Compensation_Simple ( BUF_L_cMV(motion_field_type), src_right, BUF_L_BYTE(BYTE_compensated_frame), w, h, FALSE);
	Motion_Compensation_Simple ( BUF_R_cMV(motion_field_type), src_left,  BUF_R_BYTE(BYTE_compensated_frame), w, h, FALSE);

	memcpy ( BUF_L_BYTE(BYTE_zinger_fill), src_left,  w*h);
	memcpy ( BUF_R_BYTE(BYTE_zinger_fill), src_right, w*h);

	if(occl_data->need_invert)
	{
		int size = w*h, i;
		BYTE * compensated_buf_left = BUF_L_BYTE(BYTE_compensated_frame);
		BYTE * zinger_filled_right  = BUF_R_BYTE(BYTE_zinger_fill);

		#pragma omp parallel for
		for(i = 0; i < size; i++)
		{
			compensated_buf_left[i] = 255 - compensated_buf_left[i];
			zinger_filled_right [i]	= 255 - zinger_filled_right [i];
		}
	}

	// in-place processing is possible, if user provided NULL pointers
	if(dst_left == NULL)
		dst_left = src_left;
	if(dst_right == NULL)
		dst_right = src_right;

	ZingerInpainting_1plane ( BUF_L_BYTE(BYTE_zinger_fill), BUF_L_BYTE(BYTE_occlusion_mask), src_left,  w, h, choose_maximum );
	ZingerInpainting_1plane ( BUF_R_BYTE(BYTE_zinger_fill), BUF_R_BYTE(BYTE_occlusion_mask), src_right, w, h, choose_minimum );

	Combine_Zinger_Compensated ( BUF_L_BYTE(BYTE_zinger_fill),		BUF_L_BYTE(BYTE_compensated_frame),  
								 BUF_L_BYTE(BYTE_occlusion_mask),	BUF_L_BYTE(BYTE_difference_fill),  w, h);

	Combine_Zinger_Compensated ( BUF_R_BYTE(BYTE_zinger_fill),		BUF_R_BYTE(BYTE_compensated_frame),  
								 BUF_R_BYTE(BYTE_occlusion_mask),	BUF_R_BYTE(BYTE_difference_fill),  w, h);

	ZingerInpainting_1plane ( BUF_L_BYTE(BYTE_compensated_frame),  BUF_L_BYTE(BYTE_difference_fill),  BUF_L_BYTE(BYTE_compensated_frame), w, h,  choose_all );
	ZingerInpainting_1plane ( BUF_R_BYTE(BYTE_compensated_frame),  BUF_R_BYTE(BYTE_difference_fill),  BUF_R_BYTE(BYTE_compensated_frame), w, h , choose_all );
	
	if(occl_data->need_invert)
	{
		int size = w*h, i;
		BYTE * compensated_buf_right = BUF_R_BYTE(BYTE_compensated_frame);

		#pragma omp parallel for
		for(i = 0; i < size; i++)
		{
			compensated_buf_right [i]	= 255 - compensated_buf_right [i];
		}
	}

	Accum_Considering_Occlusions(src_left,  BUF_L_BYTE(BYTE_compensated_frame), BUF_L_BYTE(BYTE_occlusion_mask), dst_left,  w, h);
	Accum_Considering_Occlusions(src_right, BUF_R_BYTE(BYTE_compensated_frame), BUF_L_BYTE(BYTE_occlusion_mask), dst_right, w, h);
}

void Correct_MVOcclusions_From_Disparity(cMV * dst, BYTE * occlusion_mask, BYTE *disparity, int width, int height)
{
	int index;
	int frame_size = width * height;
	for(index = 0; index < frame_size; ++ index)
	{
		if(occlusion_mask[index] != 255)
		{	
			// disparity here has pixel precision
			dst[index].x = (128 - disparity[index]) * 4;
		}
	}
}

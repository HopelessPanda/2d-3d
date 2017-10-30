#include "HierME_Core.h"

#include "MExt_CoreInlineFunctions.h"
#include "MExt_FrameTransform.h"
#include "MExt_Metrics.h"
#include "ContrastCalculation.h"
//#include "../FRCCore/FRCBase.h"

__inline void cSetMotionVector(gmcMotionEstimator* cMEStruct, int x, int y, int motionx, int motiony, int motion_dif, int block_size)
{

	cMacroBlock* cMB = cMEStruct->m_CurMBlocks + (y/ME_MACROBLOCK_SIZE)*cMEStruct->m_iMBlocksHor + (x/ME_MACROBLOCK_SIZE);

	if (block_size == cbt_16x16)
	{
		cMB->m_BlockPartition	= cbt_16x16;
		cMB->m_DataMV[0].x		= motionx;
		cMB->m_DataMV[0].y		= motiony;
		cMB->m_DataMV[0].error	= motion_dif;
	}
	else if (block_size == cbt_8x8)
	{
		int iIndex = ((y&ME_MACROBLOCK_SIZE_M1)/cMB->m_BlockPartition) * (ME_MACROBLOCK_SIZE/cMB->m_BlockPartition) +
			((x&ME_MACROBLOCK_SIZE_M1)/cMB->m_BlockPartition);
		if ( block_size == cbt_8x8)
		{
			cMB->m_SubBlockPartition[iIndex] = cbt_8x8;
			cMB->m_DataMV[iIndex*4].x		= motionx;
			cMB->m_DataMV[iIndex*4].y		= motiony;
			cMB->m_DataMV[iIndex*4].error	= motion_dif >> 2;
		}
		else if ( block_size == cbt_4x4)
		{
			iIndex *= 4;
			iIndex += ((y&7)/cbt_4x4) * 2 +
				((x&7)/cbt_4x4);

			cMB->m_SubBlockPartition[iIndex] = cbt_4x4;
			cMB->m_DataMV[iIndex].x			= motionx;
			cMB->m_DataMV[iIndex].y			= motiony;
			cMB->m_DataMV[iIndex].error		= motion_dif >> 4;
		}
	}
	return;
}

void MergeVectors(Hierarchic_ME* MEstruct, cMV *result_field)
{
//	int i, j, scale_count;
//	BYTE level_threshold = 128;
//	int scale = MEstruct->hierarchy_parameters.scale_num;
//
//	Size ali_dims = Get_Frame_Dims(&MEstruct->analyzed_frames[0], align);
//	Size dims = Get_Frame_Dims(&MEstruct->analyzed_frames[0], orig);
//
//	int w = dims.m_iW, h = dims.m_iH;
//
//	BYTE ** contrast_buffers = MEstruct->contrast_buffers;
//	cMV  ** MV_buffers = MEstruct->MV_buffers;
//
//	// no sense in merging one field only
//	if(scale == 1)
//		return;
//	
//	for(i = 0; i < scale; i++)
//	{
//		cGetMotionVectorField(MEstruct, MV_buffers[i], i, TRUE);
//		Get_Contrast_Map(MEstruct, contrast_buffers[i], i, TRUE);
//	}
//
//	for(i = 0; i < h; i++)
//	{
//		for (j = 0; j < w; j++)
//		{
//			int index = i*ali_dims.m_iW + j;
//			scale_count = 0;
//			while( (scale_count < scale) && (contrast_buffers[scale_count][index] < level_threshold) )
//			{
//				scale_count++;
//			}
//			if(scale_count == scale)
//				scale_count--;
//
//			result_field [i*w + j] = MV_buffers[scale_count][index];
//		}
//	}
}


void MergeVectors_ToDepth(Hierarchic_ME* MEstruct, BYTE *result_field)
{
//	int i, j, scale_count;
//	BYTE level_threshold = MEstruct->hierarchy_parameters.local_contrast_threshold;
//
//	int scale = MEstruct->hierarchy_parameters.scale_num;
//
//	Size ali_dims = Get_Frame_Dims(&MEstruct->analyzed_frames[0], align);
//	Size dims = Get_Frame_Dims(&MEstruct->analyzed_frames[0], orig);
//
//	int w = dims.m_iW, h = dims.m_iH;
//
//	BYTE  ** contrast_buffers = MEstruct->contrast_buffers;
//	BYTE  ** Depth_Buffers = MEstruct->depth_buffers;
//
//	// no sense in merging one field only
//	if(scale == 1)
//	{
//		cGetMotionVectorField_Depth(MEstruct, result_field, 0, TRUE);
//		return;
//	}
//
//	for(i = 0; i < scale; i++)
//	{
//		cGetMotionVectorField_Depth(MEstruct, Depth_Buffers[i], i, TRUE);
//		Get_Contrast_Map(MEstruct, contrast_buffers[i], i, TRUE);
//	}
//
//	for(i = 0; i < h; i++)
//	{
//		for (j = 0; j < w; j++)
//		{
//			int index = i*ali_dims.m_iW + j;
//			scale_count = 0;
//			while( (scale_count < scale) && (contrast_buffers[scale_count][index] < level_threshold) )
//			{
//				scale_count++;
//			}
//			if(scale_count == scale)
//				scale_count--;
//
//			result_field [i*w + j] = MEstruct->depth_buffers[scale_count][index];
//		}
//	}
}

cMV Calc_Array_Dispersion_MVCBlock ( MVCBlock *array, int number )
{
	int i;
	int count = 0; 
	long disp = 0;
	cMV result;
	cInitMV(&result);

	for(i = 0; i < number; i++)
	{
		result.x += array[i].vector.x;
		result.y += array[i].vector.y;
		count ++;
		disp += sqr(array[i].vector.x) + sqr(array[i].vector.y);
	}

	result.x /= count;
	result.y /= count;

	result.error =   disp/count - (sqr(result.x) + sqr(result.y)) ;

	return result;
}

void Hierarchy_ME_Calculate_Disparity(Hierarchic_ME *MEstruct, BYTE *dst, int scale, BOOL expand)
{

	int i, j, k, l;

	BOOL *blocks_filled;

	int interest_w;
	int interest_h;

	int out_scale;
	int block_mult;

	int real_w ;
	int real_h ;

	// ME info
	gmcMotionEstimator *cur_ME;
	int x, y, error, size;

	double d_mult = MEstruct->hierarchy_parameters.MV_mult_koef / 4;

	// precision multipliers
	EstimationPrecision prec_x, prec_y;
	int prec_x_mult; 
	int prec_y_mult;

	if(scale > MEstruct->hierarchy_parameters.scale_num)
		return;

	interest_w = ME_Frame_Get_Width(&MEstruct->left_source_frames[scale], align);
	interest_h = ME_Frame_Get_Height(&MEstruct->left_source_frames[scale], align);

	out_scale = (expand) ? 0: scale;
	block_mult = 1 << (scale - out_scale);

	real_w = ME_Frame_Get_Width(&MEstruct->left_source_frames[out_scale], align);
	real_h = ME_Frame_Get_Height(&MEstruct->left_source_frames[out_scale], align);

	// ME info
	cur_ME = &MEstruct->left_frame_ME_instances[scale];

	// precision multipliers
	prec_x = (EstimationPrecision)cur_ME->m_PrecMX;
	prec_y = (EstimationPrecision)cur_ME->m_PrecMY;

	prec_x_mult = (2 - prec_x) * 2; 
	prec_y_mult = (2 - prec_y) * 2;

	if(prec_x_mult == 0) prec_x_mult = 1;
	if(prec_y_mult == 0) prec_y_mult = 1;

	INIT_MEMORY_C(blocks_filled, FALSE, interest_h*interest_w, BOOL);

	for(i = 0; i < interest_h; i++)
	{
		for(j = 0; j < interest_w; j++)
		{
			if(blocks_filled[i*interest_w + j])
				continue;

			cGetMotionVector(cur_ME, j, i, &x, &y, &error, &size);

			if(block_mult == 1)
			{
				Set_Block_Value(dst, j, i, real_w, (int)CLIP(128 - x * d_mult, 0, 255), size);
			}
			else
			{
				for(k = i*block_mult; k < MIN((i+size)*block_mult, real_h); k++)
				{
					for(l = j*block_mult; l < MIN((j+size)*block_mult, real_w); l++)
					{	
						dst[k*real_w + l] = (BYTE)CLIP(128 - x * block_mult * d_mult, 0, 255);
					}
				}
			}

			for(k = i; k < (i+size); k++)
			{
				for(l = j; l < (j+size); l++)
				{	
					blocks_filled[k * interest_w + l] = TRUE;
				}
			}
		}
	}

	DEINIT_MEMORY_C(blocks_filled);
}

void cGetSimpleAlgMotionVectorField ( Hierarchic_ME *MEstruct, cMV * dst, int scale, frame_id frame )
{
	gmcMotionEstimator * pMEstruct = (frame == left) ? &MEstruct->left_frame_ME_instances[scale] : &MEstruct->right_frame_ME_instances[scale];
	int x, y, x_bl, y_bl;

	for ( y = 0; y < pMEstruct->m_iHeight; ++y )
	{
		for ( x = 0; x < pMEstruct->m_iWidth; ++x )
		{
			int index = y * pMEstruct->m_iWidth + x;
			x_bl = x / pMEstruct->m_iBlockSize;
			y_bl = y / pMEstruct->m_iBlockSize;

			dst[index] = pMEstruct->MVectors[ y_bl * pMEstruct->m_iBlocksHor + x_bl ];
		}
	}
}

void cGetMotionVectorField(Hierarchic_ME *MEstruct, cMV *dst, int scale, BOOL expand)
{
	int i, j, k, l;

	BOOL *blocks_filled;

	int interest_w;
	int interest_h;

	int out_scale;
	int block_mult;

	int real_w ;
	int real_h ;

	// ME info
	gmcMotionEstimator *cur_ME;
	int size;
	cMV m_vec;

	// precision multipliers
	EstimationPrecision prec_x, prec_y;
	int prec_x_mult; 
	int prec_y_mult;

	if(scale > MEstruct->hierarchy_parameters.scale_num)
		return;

	interest_w = ME_Frame_Get_Width(&MEstruct->left_source_frames[scale], align);
	interest_h = ME_Frame_Get_Height(&MEstruct->left_source_frames[scale], align);

	out_scale = (expand) ? 0: scale;
	block_mult = 1 << (scale - out_scale);

	real_w = ME_Frame_Get_Width(&MEstruct->left_source_frames[out_scale], align);
	real_h = ME_Frame_Get_Height(&MEstruct->left_source_frames[out_scale], align);

	// ME info
	cur_ME = &MEstruct->left_frame_ME_instances[scale];

	// precision multipliers
	prec_x = (EstimationPrecision)cur_ME->m_PrecMX;
	prec_y = (EstimationPrecision)cur_ME->m_PrecMY;

	prec_x_mult = (2 - prec_x) * 2; 
	prec_y_mult = (2 - prec_y) * 2;

	if(prec_x_mult == 0) prec_x_mult = 1;
	if(prec_y_mult == 0) prec_y_mult = 1;

	INIT_MEMORY_C(blocks_filled, FALSE, interest_h*interest_w, BOOL);

	for(i = 0; i < interest_h; i++)
	{
		for(j = 0; j < interest_w; j++)
		{
			if(blocks_filled[i*interest_w + j])
				continue;

			cGetMotionVector(cur_ME, j, i, &m_vec.x, &m_vec.y, &m_vec.error, &size);
			
			m_vec.x *= prec_x_mult * block_mult;
			m_vec.y *= prec_y_mult * block_mult;

			for(k = i*block_mult; k < MIN((i+size)*block_mult, real_h); k++)
			{
				for(l = j*block_mult; l < MIN((j+size)*block_mult, real_w); l++)
				{	
					dst[k*real_w + l] = m_vec;
				}
			}

			for(k = i; k < MIN(i+size, interest_h); k++)
			{
				for(l = j; l < MIN(j+size, interest_w); l++)
				{	
					blocks_filled[k * interest_w + l] = TRUE;
				}
			}
		}
	}

	DEINIT_MEMORY_C(blocks_filled);
}

void cCalculateAdditionalBuffer(Hierarchic_ME *MEstruct, void *dst, frame_id frame, 
								 int output_type, int scale, BOOL expand)
{
	Buffer_Storage *storage = (frame == left) ? &MEstruct->left_frame_buffers[scale] : &MEstruct->right_frame_buffers[scale];

	int i, j, k, l;

	BOOL *blocks_filled;

	int interest_w;
	int interest_h;
	int real_w ;
	int real_h ;
	int out_scale;
	int block_mult;
	BOOL need_upscale;

	BYTE *dst_type_byte = NULL;
	cMV  *dst_type_cMV = NULL;

	BYTE *dst_small_byte = (BYTE*)dst;
	cMV  *dst_small_cMV	 =  (cMV*)dst;

	// ME info
	BYTE *src_frame;
	gmcMotionEstimator *cur_ME;
	int x, y, error, size;

	double d_mult = MEstruct->hierarchy_parameters.MV_mult_koef / 4;

	// precision multipliers
	EstimationPrecision prec_x, prec_y;
	int prec_x_mult; 
	int prec_y_mult;

	// obvious errors processing
	//////////////////////////////////////////////////////////////////////////
	if(scale > MEstruct->hierarchy_parameters.scale_num  ||  scale < 0)
		return; // scale is lower, than processing level.

	if(output_type == cMV_field_filtered			||  
	/*	output_type == BYTE_disparity_filtered		||  
		output_type == BYTE_filter_mask				||
		output_type == BYTE_occlusion_mask			||
		output_type == BYTE_lrc_mask				||
		output_type == BYTE_compensated_lrc			*/
		output_type > BYTE_contrast  &&  output_type < cMV_buffer_type_shift	)
		return; // function does not support filtering, due to filtering dependence on other buffers

	//////////////////////////////////////////////////////////////////////////
	// switching types, depending on output calculation
	switch(output_type)
	{
	case BYTE_confidence:
	case BYTE_disparity:
	case BYTE_disparity_filtered:
	case BYTE_contrast:
	case BYTE_filter_mask:
	case BYTE_MVdispersion:
		dst_type_byte = (BYTE*) dst;
		break;
	case cMV_field:
	case cMV_field_filtered:
		dst_type_cMV = (cMV *)dst;
		break;
	default:
		return; // wrong output type for this function.
	}
	//////////////////////////////////////////////////////////////////////////
	// getting dimensions of buffers
	interest_w = ME_Frame_Get_Width(&MEstruct->left_source_frames[scale],  align);
	interest_h = ME_Frame_Get_Height(&MEstruct->left_source_frames[scale], align);

	out_scale = (expand) ? 0: scale;
	block_mult = 1 << (scale - out_scale);
	need_upscale = (scale < out_scale);

	real_w = ME_Frame_Get_Width(&MEstruct->left_source_frames[out_scale],  align);
	real_h = ME_Frame_Get_Height(&MEstruct->left_source_frames[out_scale], align);

	//////////////////////////////////////////////////////////////////////////
	// ME info
	cur_ME		= (frame == left) ? &MEstruct->left_frame_ME_instances[scale] : &MEstruct->right_frame_ME_instances[scale];
	src_frame	= (frame == left)	? ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->left_source_frames[scale],  align) 
									: ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->right_source_frames[scale], align); 

	//////////////////////////////////////////////////////////////////////////
	// precision multipliers
	prec_x = (EstimationPrecision)cur_ME->m_PrecMX;
	prec_y = (EstimationPrecision)cur_ME->m_PrecMY;

	prec_x_mult = (2 - prec_x) * 2; 
	prec_y_mult = (2 - prec_y) * 2;

	if(prec_x_mult == 0) prec_x_mult = 1;
	if(prec_y_mult == 0) prec_y_mult = 1;
	//////////////////////////////////////////////////////////////////////////
	// initializing of small buffer for result store
	if(need_upscale)
	{
		if(dst_type_byte)
		{
			INIT_MEMORY_C(dst_small_byte, 0, interest_h*interest_w, BYTE);
		}
		else if(dst_type_cMV)
		{
			INIT_MEMORY_C(dst_small_cMV, 0, interest_w*interest_h, cMV);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// main calculation choice is in this switch
	switch (output_type)
	{
	case BYTE_confidence:
		// frame confidence is separate function. Simple copy-paste
		//if(frame == left)
	//		FRC_Get_Confidence(&MEstruct->confidence_data[scale], dst_small_byte, NULL, interest_w, interest_h);
	//	else
	//		FRC_Get_Confidence(&MEstruct->confidence_data[scale], NULL, dst_small_byte, interest_w, interest_h);

		break;
	case BYTE_MVdispersion:
		// Dispersion is in separate function
		MEstruct->dispersion_threshold[frame] = 
			Calculate_MVDispersion_Map(cur_ME, src_frame, dst_small_byte, TRUE);
		break;
	case BYTE_contrast:
		// contrast is separate function
		Calculate_Contrast_Map(MEstruct, dst_small_byte, frame, scale );
		
		//////////////////////////////////////////////////////////////////////////
		// TODO : Don't forget to remove!!
		//Correct_Contrast_Map(src_frame, dst_small_byte, interest_w, interest_h, 16);

		break;
	case BYTE_disparity:
	case BYTE_disparity_filtered:
	case cMV_field:
	case cMV_field_filtered:
		{
			// here anyway there's a need in rough disparity or vector field.
			INIT_MEMORY_C(blocks_filled, FALSE, interest_h*interest_w, BOOL);

			for(i = 0; i < interest_h; i++)
			{
				for(j = 0; j < interest_w; j++)
				{
					if(blocks_filled[i*interest_w + j])
						continue;

					cGetMotionVector(cur_ME, j, i, &x, &y, &error, &size);
					
					// equalization for precision difference
					x *= prec_x_mult;
					y *= prec_y_mult;

					if(dst_type_byte)
					{
						Set_Block_Value(dst_small_byte, j, i, interest_w, (int)CLIP(128 - x * d_mult, 0, 255), size);
					}
					else if(dst_type_cMV)
					{
						int x_ind, y_ind;
						for(y_ind = i; y_ind < i + size; ++y_ind)
						{
							for (x_ind = j; x_ind < j+size; ++x_ind)
							{
								dst_small_cMV[y_ind * interest_w + x_ind].error = error;
								dst_small_cMV[y_ind * interest_w + x_ind].field = FT_TOP;
								dst_small_cMV[y_ind * interest_w + x_ind].x = x;
								dst_small_cMV[y_ind * interest_w + x_ind].y = y;
							}
						}
					}

					for(k = i; k < (i+size); k++)
					{
						for(l = j; l < (j+size); l++)
						{	
							blocks_filled[k * interest_w + l] = TRUE;
						}
					}
				}
			}

			DEINIT_MEMORY_C(blocks_filled);
		}
		break;
	} // switch
	
	if(need_upscale)
	{
		if(dst_type_byte)
		{
			UpScale_frame_by_factor(dst_small_byte, dst_type_byte, interest_w, interest_h, block_mult);
			DEINIT_MEMORY_C(dst_small_byte);
		}
		else if(dst_type_cMV)
		{
			UpScale_frame_by_factor_MV(dst_small_cMV, dst_type_cMV, interest_w, interest_h, block_mult);
			DEINIT_MEMORY_C(dst_small_cMV);
		}
	}
}

void Merge_Filtering_Mask(Hierarchic_ME * MEstruct, frame_id direction, int scale)
{
	int i, j;
	
	Buffer_Storage *storage = (direction == left) ? &MEstruct->left_frame_buffers[scale] : &MEstruct->right_frame_buffers[scale];
	
	Size ali_dim = ME_Frame_Get_Dims(&MEstruct->left_source_frames[scale], align);
	int w = ali_dim.m_iW;
	int h = ali_dim.m_iH;

	int dispersion_threshold = MEstruct->dispersion_threshold[direction];
	
	BYTE *dispersion	= Buffer_Storage_GetBuffer_BYTE(storage, BYTE_MVdispersion);
	BYTE *contrast_buf	= Buffer_Storage_GetBuffer_BYTE(storage, BYTE_contrast);
	BYTE *merge_mask	= Buffer_Storage_GetBuffer_BYTE(storage, BYTE_filter_mask);

	for(i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			int index = i*w + j;

			// dispersion has more correlation with depth map
			if(dispersion[index] == 0)
			{
				merge_mask[index] = 255;
			}
			/// let's add serious numbers for serious comparison
			else if(contrast_buf[index] < 42)
			{
				merge_mask[index] = 0;
			}
			else if(dispersion[index] > dispersion_threshold)
			{
				merge_mask[index] = 0;
			}
			else 
			{
				merge_mask[index] = 255;
			}
		}
	}
}

// simply returns pointer
void * Hierarchy_ME_GetPrecalculatedBufPointer(Hierarchic_ME *MEstruct, int type_of_output, frame_type frame, int scale)
{
	if (frame != left  &&  frame != right)
	{
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	// shorter block for all  BYTE buffers
	if(type_of_output < BYTE_buffer_type_capacity)
	{
		return (frame == left)? (void *)GETSTORAGEBUF_LEFT_BYTE(scale,  type_of_output) :
								(void *)GETSTORAGEBUF_RIGHT_BYTE(scale, type_of_output);
	}
	else if ( type_of_output >= BYTE3_buffer_type_shift  &&  type_of_output < BYTE3_buffer_type_shift + BYTE3_buffer_type_capacity )
	{
		return (frame == left)? (void *)GETSTORAGEBUF_LEFT_BYTE(scale,  type_of_output) :
								(void *)GETSTORAGEBUF_RIGHT_BYTE(scale, type_of_output);
	}
	else if( type_of_output >= cMV_buffer_type_shift  &&  type_of_output < cMV_buffer_type_shift + cMV_buffer_type_capacity )
	{
		return (frame == left)? (void *)GETSTORAGEBUF_LEFT_cMV(scale,  type_of_output) :
								(void *)GETSTORAGEBUF_RIGHT_cMV(scale, type_of_output);
	}
	else
		return NULL;
}



// filtering_cands - array for storing found candidates
// x_ind, y_ind - positions of the current block.
// radius - in pixels, depends on the size of current block
// iteration_number is used to avoid duplicating of vectors in filtering_cands
//
// filtering_cands has size of (2*filter_radius+1) * 16, for case of 16x16 block and surrounding 4x4 blocks
//
void Find_Filtering_Candidates(gmcMotionEstimator *ME_instance, int *temp_filtering_buf, BYTE *confidence_map,
							   MVCBlock *filtering_cands, int *cands_num,
							   int x_ind, int y_ind,
							   int filter_radius, int iteration_number)
{
	int min_size = ME_instance->m_pParams->m_MinBlSize;
	int y, x;
	int internal_radius = filter_radius;
	int width  = ME_instance->m_iWidth;
	int height = ME_instance->m_iHeight;

	MVCBlock current;

	cGetMotionVector(ME_instance, x_ind, y_ind, &current.vector.x, &current.vector.y, &current.vector.error, &current.block_size);
	internal_radius *= current.block_size;

	*cands_num = 0;

	for (y = MAX(0, y_ind - internal_radius); y <= MIN(height-1, y_ind + internal_radius); y += min_size)
	{
		for (x = MAX(0, x_ind - internal_radius); x <= MIN(width-1, x_ind + internal_radius); x += min_size)
		{
			int pixel_index = y*width + x;
			int temp_buf_pixel_index = (y/min_size)*(width/min_size) + x/min_size;

			//////////////////////////////////////////////////////////////////////////
			// Check, whether we already took this block and its motion vector
			if(temp_filtering_buf[temp_buf_pixel_index] == iteration_number)
				continue;

			//////////////////////////////////////////////////////////////////////////
			// get block information and store it in local variable
			cGetMotionVector(ME_instance, x, y, &current.vector.x, &current.vector.y, &current.vector.error, &current.block_size);
			// coordinate is center of the block. Mods are subtracted to get top left corner of the block first.
			current.x = x - x%current.block_size + current.block_size / 2;
			current.y = y - y%current.block_size + current.block_size / 2;
			current.block_contrast = confidence_map[pixel_index];
			
			//////////////////////////////////////////////////////////////////////////
			// dump it to global variable
			filtering_cands[*cands_num] = current;
			(*cands_num) += 1;

			//////////////////////////////////////////////////////////////////////////
			// store in temp buffer, that we have already been here
			Set_Block_Value_Int_Secure(temp_filtering_buf, (x - x%current.block_size)/min_size, 
														   (y - y%current.block_size)/min_size, 
															width/min_size,	height/min_size, 
															iteration_number, current.block_size/min_size);	
		}
	}
}

BYTE Get_Updated_Value(double weighted_value, int original_value, int original_confidence)
{
	double TRUSTED_CONFIDENCE = 180.0;

	double weight = MIN(original_confidence, TRUSTED_CONFIDENCE) / TRUSTED_CONFIDENCE;
	double upd_value = original_value * weight + weighted_value * (1-weight);

	return (BYTE)CLIP(upd_value, 0, 255);
}

int Get_Updated_Value_int(double weighted_value, int original_value, int original_confidence)
{
	double TRUSTED_CONFIDENCE = 240.0;

	double weight = MAX ( 0, MIN( original_confidence *3/2 - TRUSTED_CONFIDENCE , TRUSTED_CONFIDENCE ) ) / TRUSTED_CONFIDENCE;
	double upd_value = original_value * weight + weighted_value * (1-weight);

	return (int)upd_value;
}

void ConfidenceBased_Disparity_Filter(gmcMotionEstimator *ME_instance, BYTE * disparity_map, BYTE * confidence_map, BYTE *dst_map, int width, int height, int filter_radius)
{
	int x_ind, y_ind;
	int MIN_BLOCK_SIZE = ME_instance->m_pParams->m_MinBlSize;
	int MAX_BLOCK_SIZE = ME_instance->m_pParams->m_MaxBlSize;

	int  *temp_filtering_buf;
	int *temp_fill_buf;

	MVCBlock *filtering_cands;
	int filter_cands_number;
	int iteration = 1;

	INIT_MEMORY_C( temp_filtering_buf,	0, (width / MIN_BLOCK_SIZE) * (height / MIN_BLOCK_SIZE), int );
	INIT_MEMORY_C( temp_fill_buf,		0, (width / MIN_BLOCK_SIZE) * (height / MIN_BLOCK_SIZE), int );

	INIT_MEMORY_C( filtering_cands, 0, (2*filter_radius + 1)*(2*filter_radius + 1)*(MAX_BLOCK_SIZE/MIN_BLOCK_SIZE)*(MAX_BLOCK_SIZE/MIN_BLOCK_SIZE), MVCBlock);

	for(y_ind = 0; y_ind < height; y_ind += MIN_BLOCK_SIZE)
	{
		for(x_ind = 0; x_ind < width; x_ind += MIN_BLOCK_SIZE)
		{
			int pixel_index = y_ind*width + x_ind;
			int temp_buf_pixel_index = (y_ind/MIN_BLOCK_SIZE)*(width/MIN_BLOCK_SIZE) + x_ind/MIN_BLOCK_SIZE;

			int internal_ind = 0;
			double weight_sum = 0.0;
			double value_sum  = 0.0;
			double value_weighted_sum = 0.0;

			MVCBlock current;
			int original_x, original_y;

			//////////////////////////////////////////////////////////////////////////
			// skip already filtered block
			if(temp_fill_buf[temp_buf_pixel_index])
				continue;

			//////////////////////////////////////////////////////////////////////////
			cGetMotionVector(ME_instance, x_ind, y_ind, &current.vector.x, &current.vector.y, &current.vector.error, &current.block_size);
			original_x = x_ind + current.block_size / 2;
			original_y = y_ind + current.block_size / 2;

			// Find candidates to filter with.
			Find_Filtering_Candidates(ME_instance, temp_filtering_buf, confidence_map, filtering_cands, &filter_cands_number, x_ind, y_ind, filter_radius, iteration);
			
			//////////////////////////////////////////////////////////////////////////
			// Apply filter
			for (internal_ind = 0; internal_ind < filter_cands_number; internal_ind ++)
			{
				int x = filtering_cands[internal_ind].x;
				int y = filtering_cands[internal_ind].y;
				int confidence_val = filtering_cands[internal_ind].block_contrast;
				int disparity_val = disparity_map[y*width + x];

				value_weighted_sum  += confidence_val * disparity_val;
				weight_sum			+= confidence_val;
				value_sum			+= disparity_val;
			}

			if(weight_sum == 0) 
				value_weighted_sum = value_sum / filter_cands_number;
			else
				value_weighted_sum /= weight_sum;

			Set_Block_Value_Secure(dst_map, x_ind, y_ind, width, height, 
									Get_Updated_Value(value_weighted_sum, disparity_map[pixel_index], confidence_map[pixel_index] ), 
									current.block_size );

			//////////////////////////////////////////////////////////////////////////
			// set block value in temp fill buffer to avoid repeated processing
			Set_Block_Value_Int_Secure(temp_fill_buf, x_ind/MIN_BLOCK_SIZE, y_ind/MIN_BLOCK_SIZE, 
													width/MIN_BLOCK_SIZE, height/MIN_BLOCK_SIZE, TRUE, current.block_size/MIN_BLOCK_SIZE);	
			
			// switch to next iteration
			++ iteration;
		}
	}

	DEINIT_MEMORY_C(temp_filtering_buf);
	DEINIT_MEMORY_C(temp_fill_buf);
	DEINIT_MEMORY_C(filtering_cands);
}

void DepthBased_Motion_Filter ( gmcMotionEstimator *ME_instance, cMV * dst_motion, BYTE * depth_map, BYTE * lrc_confidence, int width, int height, int filter_radius )
{
	int x_ind, y_ind;
	int MIN_BLOCK_SIZE = ME_instance->m_pParams->m_MinBlSize;
	int MAX_BLOCK_SIZE = ME_instance->m_pParams->m_MaxBlSize;

	int  *temp_filtering_buf;
	int *temp_fill_buf;

	MVCBlock *filtering_cands;
	int filter_cands_number;
	int iteration = 1;

	INIT_MEMORY_C( temp_filtering_buf,	0, (width / MIN_BLOCK_SIZE) * (height / MIN_BLOCK_SIZE), int );
	INIT_MEMORY_C( temp_fill_buf,		0, (width / MIN_BLOCK_SIZE) * (height / MIN_BLOCK_SIZE), int );

	INIT_MEMORY_C( filtering_cands, 0, (2*filter_radius + 1)*(2*filter_radius + 1)*(MAX_BLOCK_SIZE/MIN_BLOCK_SIZE)*(MAX_BLOCK_SIZE/MIN_BLOCK_SIZE), MVCBlock);

	for(y_ind = 0; y_ind < height; y_ind += MIN_BLOCK_SIZE)
	{
		for(x_ind = 0; x_ind < width; x_ind += MIN_BLOCK_SIZE)
		{
			int pixel_index = y_ind*width + x_ind;
			int temp_buf_pixel_index = (y_ind/MIN_BLOCK_SIZE)*(width/MIN_BLOCK_SIZE) + x_ind/MIN_BLOCK_SIZE;

			int current_depth = depth_map [ pixel_index ];

			int internal_ind = 0;
			double weight_sum = 0.0;
			double value_weighted_sum_x = 0.0;
			double value_weighted_sum_y = 0.0;

			int k_ind, l_ind;

			MVCBlock current;
			int original_x, original_y;

			//////////////////////////////////////////////////////////////////////////
			// skip already filtered block
			if(temp_fill_buf[temp_buf_pixel_index])
				continue;

			//////////////////////////////////////////////////////////////////////////
			cGetMotionVector(ME_instance, x_ind, y_ind, &current.vector.x, &current.vector.y, &current.vector.error, &current.block_size);
			original_x = x_ind + current.block_size / 2;
			original_y = y_ind + current.block_size / 2;
			//////////////////////////////////////////////////////////////////////////

			// Find candidates to filter with.
			Find_Filtering_Candidates(ME_instance, temp_filtering_buf, depth_map, filtering_cands, &filter_cands_number, x_ind, y_ind, filter_radius, iteration);

			//////////////////////////////////////////////////////////////////////////
			// Apply filter
			for (internal_ind = 0; internal_ind < filter_cands_number; internal_ind ++)
			{
				int x			= filtering_cands[internal_ind].x;
				int y			= filtering_cands[internal_ind].y;

				int depth_val	= 1 / MAX( sqr( abs ( filtering_cands[internal_ind].block_contrast - current_depth ) ) , 
											1); // square from absolute difference in depth

				int mvector_x	= filtering_cands[internal_ind].vector.x;
				int mvector_y	= filtering_cands[internal_ind].vector.y;

				value_weighted_sum_x	+= depth_val * mvector_x;
				value_weighted_sum_y	+= depth_val * mvector_y;
				weight_sum				+= depth_val;
			}

			value_weighted_sum_x = Get_Updated_Value_int( value_weighted_sum_x / weight_sum, current.vector.x, 255 - lrc_confidence[pixel_index] );
			value_weighted_sum_y = Get_Updated_Value_int( value_weighted_sum_y / weight_sum, current.vector.y, 255 - lrc_confidence[pixel_index] );

			for(k_ind = y_ind; k_ind < (y_ind + current.block_size); ++k_ind)
				for(l_ind = x_ind; l_ind < (x_ind + current.block_size); ++l_ind)
				{
					int ind = k_ind * width + l_ind;

					dst_motion[ind].x = (int)value_weighted_sum_x;
					dst_motion[ind].y = (int)value_weighted_sum_y;
				}

			//////////////////////////////////////////////////////////////////////////
			// set block value in temp fill buffer to avoid repeated processing
			Set_Block_Value_Int_Secure(temp_fill_buf, x_ind/MIN_BLOCK_SIZE, y_ind/MIN_BLOCK_SIZE, 
				width/MIN_BLOCK_SIZE, height/MIN_BLOCK_SIZE, TRUE, current.block_size/MIN_BLOCK_SIZE);	

			// switch to next iteration
			++ iteration;
		}
	}

	DEINIT_MEMORY_C(temp_filtering_buf);
	DEINIT_MEMORY_C(temp_fill_buf);
	DEINIT_MEMORY_C(filtering_cands);
	
}

void ConfidenceBased_ME_Filter(gmcMotionEstimator *ME_instance, BYTE * confidence_map, int filter_radius)
{
	int width = ME_instance->m_iWidth;
	int height = ME_instance->m_iHeight;

	int x_ind, y_ind;
	int MIN_BLOCK_SIZE = ME_instance->m_pParams->m_MinBlSize;
	int MAX_BLOCK_SIZE = ME_instance->m_pParams->m_MaxBlSize;

	int  *temp_filtering_buf;
	int *temp_fill_buf;

	MVCBlock *filtering_cands;
	int filter_cands_number;
	int iteration = 1;

	INIT_MEMORY_C( temp_filtering_buf,	0, (width / MIN_BLOCK_SIZE) * (height / MIN_BLOCK_SIZE), int );
	INIT_MEMORY_C( temp_fill_buf,		0, (width / MIN_BLOCK_SIZE) * (height / MIN_BLOCK_SIZE), int );

	INIT_MEMORY_C( filtering_cands, 0, (2*filter_radius + 1)*(2*filter_radius + 1)*(MAX_BLOCK_SIZE/MIN_BLOCK_SIZE)*(MAX_BLOCK_SIZE/MIN_BLOCK_SIZE), MVCBlock);

	for(y_ind = 0; y_ind < height; y_ind += MIN_BLOCK_SIZE)
	{
		for(x_ind = 0; x_ind < width; x_ind += MIN_BLOCK_SIZE)
		{
			int pixel_index = y_ind*width + x_ind;
			int temp_buf_pixel_index = (y_ind/MIN_BLOCK_SIZE)*(width/MIN_BLOCK_SIZE) + x_ind/MIN_BLOCK_SIZE;

			int internal_ind = 0;
			double weight_sum = 0.0;

			double vector_x_weighted_sum = 0.0;
			double vector_y_weighted_sum = 0.0;
			double vector_x_sum = 0.0;
			double vector_y_sum = 0.0;

			MVCBlock current;
			int original_x, original_y;

			//////////////////////////////////////////////////////////////////////////
			// skip already filtered block
			if(temp_fill_buf[temp_buf_pixel_index])
				continue;

			//////////////////////////////////////////////////////////////////////////
			cGetMotionVector(ME_instance, x_ind, y_ind, &current.vector.x, &current.vector.y, &current.vector.error, &current.block_size);
			original_x = x_ind + current.block_size / 2;
			original_y = y_ind + current.block_size / 2;

			// Find candidates to filter with.
			Find_Filtering_Candidates(ME_instance, temp_filtering_buf, confidence_map, filtering_cands, &filter_cands_number, x_ind, y_ind, filter_radius, iteration);

			//////////////////////////////////////////////////////////////////////////
			// Apply filter
			for (internal_ind = 0; internal_ind < filter_cands_number; internal_ind ++)
			{
				int x = filtering_cands[internal_ind].x;
				int y = filtering_cands[internal_ind].y;
				int confidence_val = filtering_cands[internal_ind].block_contrast;
				cMV current_vector = filtering_cands[internal_ind].vector;

				vector_x_weighted_sum	+= confidence_val * current_vector.x;
				vector_y_weighted_sum	+= confidence_val * current_vector.y;
				weight_sum				+= confidence_val;
				vector_x_sum			+= current_vector.x;
				vector_y_sum			+= current_vector.y;
			}

			if(weight_sum == 0) 
			{
				vector_x_weighted_sum = vector_x_sum / filter_cands_number;
				vector_y_weighted_sum = vector_y_sum / filter_cands_number;
			}
			else
			{
				vector_x_weighted_sum /= weight_sum;
				vector_y_weighted_sum /= weight_sum;
			}

			cSetMotionVector(ME_instance, x_ind, y_ind, 
							Get_Updated_Value_int(vector_x_weighted_sum, current.vector.x, confidence_map[pixel_index] ), 
							Get_Updated_Value_int(vector_y_weighted_sum, current.vector.y, confidence_map[pixel_index] ), 
							current.vector.error,
							current.block_size );

			//////////////////////////////////////////////////////////////////////////
			// set block value in temp fill buffer to avoid repeated processing
			Set_Block_Value_Int_Secure(temp_fill_buf, x_ind/MIN_BLOCK_SIZE, y_ind/MIN_BLOCK_SIZE, 
				width/MIN_BLOCK_SIZE, height/MIN_BLOCK_SIZE, TRUE, current.block_size/MIN_BLOCK_SIZE);	

			// switch to next iteration
			++ iteration;
		}
	}

	DEINIT_MEMORY_C(temp_filtering_buf);
	DEINIT_MEMORY_C(temp_fill_buf);
	DEINIT_MEMORY_C(filtering_cands);
}



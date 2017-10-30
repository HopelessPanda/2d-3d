#include "MExt_CoreInlineFunctions.h"
#include "ContrastCalculation.h"
#include "HierME_Core.h"

#include <VideoGMLib/VideoImage/VideoImage.h>

static _inline int GetContrast_InPixel(int **contrast_map, int i, int j, int stride, MEBlockSize bsn)
{
	int ib = i / GetBlockWidth(bsn);
	int jb = j / GetBlockWidth(bsn);

	int wb = stride / GetBlockWidth(bsn);
	int index = ib * wb + jb;
	return contrast_map[bsn][index];
}

///  linear clipping contrast conversion 
BYTE Contrast_Convert_sq(int contrast_val)
{
	int crop_values = 96;
	int val = MAX(contrast_val*2-crop_values, 0);
	return (BYTE)CLIP(val*val, 0, 255);
}

BYTE Contrast_Convert(int contrast_val)
{
	int division_coef = 2;
	return (BYTE)CLIP(contrast_val / division_coef, 0, 255);
	//return (contrast_val*3/2 >= 255) ? 255 : 0;
}

/**
**************************************************************************
* \brief Calculates contrast value for block
*
*  This function calculate the contrast value for given block.
*
* \param block     [IN] - pointer to the block
* \param shift     [IN] - value of shift for SAD calculation
* \param stride    [IN] - offset between lines in image data
* \param bsn       [IN] - block size
* 
* \return Contrast for the specified block
*/
int CalculateBlockContrast(pGetBlockError *error_funcs_array, BYTE *block, int shift, int stride, MEBlockSize bsn)
{
	pGetBlockError GetBlockError = error_funcs_array[bsn];
	int contrast;

	contrast = 4;
	contrast += GetBlockError(block, block + shift, stride, NULL); 
	contrast += GetBlockError(block, block - shift, stride, NULL); 
	contrast += GetBlockError(block, block + shift * stride, stride, NULL); 
	contrast += GetBlockError(block, block - shift * stride, stride, NULL); 
	contrast = contrast >> 2;

	return contrast;
}

void CalculateFrameContrast_1plane(BYTE *frame, int **contrast_map, int w, int h, int shift, 
							pGetBlockError *error_funcs_array, int max_block_size, int min_block_size)
{
	int ib, jb, lvl, mult;
	MEBlockSize bsnum;

	int bs, nBlVert, nBlHor;
	int block_norm;
	int cur_shift = shift << (max_block_size - min_block_size);

	BYTE *extended, *cur_place;
	int we = w+2*cur_shift, he = h+2*cur_shift;
	int pitch = we*he;

	INIT_MEMORY_C(extended, 0, we*he, BYTE);
	cAddRepeatedBordersY(extended, frame, w, h, cur_shift, cur_shift, cur_shift, cur_shift);

	cur_place = extended + we*cur_shift + cur_shift;

	for(lvl = 0; lvl <= max_block_size - min_block_size; lvl++)
	{
		bsnum = (MEBlockSize)lvl;
		bs = 4 << bsnum;
		block_norm = (bs >> 2) * (bs >> 2);

		nBlVert = h / bs;
		nBlHor = w / bs;
		mult = bs * we;

		for(ib = 0; ib < nBlVert; ib++)
		{
			int index = ib * nBlHor;
			BYTE *block = cur_place  + ib * mult;
			BYTE *u_block = block + pitch;
			BYTE *v_block = block + 2*pitch;

			for(jb = 0; jb < nBlHor; jb++)
			{
				int contrast = CalculateBlockContrast(error_funcs_array, block, shift, we, bsnum) / block_norm / 2 ;

				contrast_map[bsnum][index] = contrast;

				block += bs;
				u_block += bs;
				v_block += bs;

				index++;
			}
		}

		shift = shift << 1;
	}

	DEINIT_MEMORY_C(extended);
}

//// calculates frame contrast field for merging ME fields
// frame is all 3 planes
void CalculateFrameContrast(BYTE *frame, int **contrast_map, int w, int h, int shift, 
							pGetBlockError *error_funcs_array, int max_block_size, int min_block_size,
							long *average_errors)
{
	int ib, jb, lvl, mult;
	MEBlockSize bsnum;

	int bs, nBlVert, nBlHor;
	int block_norm;
	int cur_shift = shift << (max_block_size - min_block_size);

	BYTE *extended, *cur_place;
	int we = w+2*cur_shift, he = h+2*cur_shift;
	int pitch = we*he;

	INIT_MEMORY_C(extended, 0, we*he*3, BYTE);
	cAddRepeatedBordersYUV(extended, frame, w, h, cur_shift, cur_shift, cur_shift, cur_shift);

	cur_place = extended + we*cur_shift + cur_shift;

	for(lvl = 0; lvl <= max_block_size - min_block_size; lvl++)
	{
		bsnum = (MEBlockSize)lvl;
		bs = 4 << bsnum;
		block_norm = (bs >> 2) * (bs >> 2);

		nBlVert = h / bs;
		nBlHor = w / bs;
		mult = bs * we;
		
		average_errors[lvl] = 0;

		for(ib = 0; ib < nBlVert; ib++)
		{
			int index = ib * nBlHor;
			BYTE *block = cur_place  + ib * mult;
			BYTE *u_block = block + pitch;
			BYTE *v_block = block + 2*pitch;

			for(jb = 0; jb < nBlHor; jb++)
			{
				int contrast = (  CalculateBlockContrast(error_funcs_array, block, shift, we, bsnum) + 
												CalculateBlockContrast(error_funcs_array, u_block, shift, we, bsnum) +
												CalculateBlockContrast(error_funcs_array, v_block, shift, we, bsnum) ) / block_norm / 2 / 3 ;
				
				average_errors[lvl] += contrast;

				contrast_map[bsnum][index] = contrast;

				block += bs;
				u_block += bs;
				v_block += bs;

				index++;
			}
		}

		average_errors[lvl] /= nBlHor*nBlVert;

		shift = shift << 1;
	}
	
	//for(lvl = 0; lvl <= max_block_size - min_block_size; lvl++)
	//{
	//	// dumping to global memory for use in threshold function
	//	average_frame_contrast_errors[lvl] = average_errors[lvl];
	//}

	DEINIT_MEMORY_C(extended);
}

void Hierarchy_ME_Calc_Frame_Contrast( Hierarchic_ME *ME_struct, frame_id direction, int scale, int shift )
{
	if(direction == left)
	{
		Size dims = ME_Frame_Get_Dims(&ME_struct->left_source_frames[scale], align);
		CalculateFrameContrast(  ME_Frame_Get_Frame_Buf_Pointer(&ME_struct->left_source_frames[scale], align), 
								ME_struct->left_frame_contrast[scale], dims.m_iW, dims.m_iH,	
								shift,	ME_struct->GetContrastErrorFuncs, B_32x32, B_4x4, ME_struct->left_frame_average_contrast);
	}
	else if(direction == right)
	{
		Size dims = ME_Frame_Get_Dims(&ME_struct->right_source_frames[scale], align);
		CalculateFrameContrast(  ME_Frame_Get_Frame_Buf_Pointer(&ME_struct->right_source_frames[scale], align), 
			ME_struct->right_frame_contrast[scale], dims.m_iW, dims.m_iH,	
			shift,	ME_struct->GetContrastErrorFuncs, B_32x32, B_4x4, ME_struct->right_frame_average_contrast);
	}
	else
	{
		return;
	}
}

void Hierarchy_ME_Calc_Frame_Contrast_External( Hierarchic_ME *ME_struct, 
										  BYTE * buffer_left, BYTE * buffer_right, 
										  BYTE * dst_contrast_left, BYTE * dst_contrast_right, 
										  int scale, int shift )
{
#pragma omp parallel num_threads(2)
	#pragma omp sections
	{
		#pragma omp section
		{
			Size dims = ME_Frame_Get_Dims(&ME_struct->left_source_frames[scale], align);
			CalculateFrameContrast_1plane(  buffer_left, 
				ME_struct->left_frame_contrast[scale], dims.m_iW, dims.m_iH,	
				shift,	ME_struct->GetContrastErrorFuncs, B_32x32, B_4x4);

			Calculate_Contrast_Map(ME_struct, dst_contrast_left, left, scale);
		}
		#pragma omp section
		{
			Size dims = ME_Frame_Get_Dims(&ME_struct->right_source_frames[scale], align);
			CalculateFrameContrast_1plane(  buffer_right, 
				ME_struct->right_frame_contrast[scale], dims.m_iW, dims.m_iH,	
				shift,	ME_struct->GetContrastErrorFuncs, B_32x32, B_4x4);

			Calculate_Contrast_Map(ME_struct, dst_contrast_right, right, scale);
		}
	}
}

/// rewrite contrast field to external frame buffer
void Calculate_Contrast_Map(Hierarchic_ME *ME_struct, BYTE *map, frame_id direction, int scale)
{
	int BLOCKSIZE = 4;
	int i, j;	
	int size, error, x, y;
	int contrast_value;

	Size align_dims ;

	int ** contrast_map;
	gmcMotionEstimator *ME_inst;

	contrast_conversion get_contrast_func = ME_struct->ConvertContrastFunc;

	//BYTE *extended_contrast_map, *small_contrast_map;

	if(scale > ME_struct->hierarchy_parameters.scale_num)
		return;

	align_dims = ME_Frame_Get_Dims(&ME_struct->left_source_frames[scale], align);

	contrast_map = (direction == left) ? ME_struct->left_frame_contrast[scale] : ME_struct->right_frame_contrast[scale];
	ME_inst = (direction == left) ? &ME_struct->left_frame_ME_instances[scale] : &ME_struct->right_frame_ME_instances[scale];

	// filling aligned buffer
	for (i = 0; i < align_dims.m_iH; i+=BLOCKSIZE)
	{
		for(j = 0; j < align_dims.m_iW; j+=BLOCKSIZE)
		{
			cGetMotionVector(ME_inst, j, i, &x, &y, &error, &size);
			contrast_value  = 0;
			contrast_value += GetContrast_InPixel(contrast_map, i, j, align_dims.m_iW, GetBlocksize(/*size*/b_4x4));
			contrast_value += GetContrast_InPixel(contrast_map, i, j, align_dims.m_iW, GetBlocksize(/*size*/b_8x8));
			contrast_value += GetContrast_InPixel(contrast_map, i, j, align_dims.m_iW, GetBlocksize(/*size*/b_16x16));
			contrast_value += GetContrast_InPixel(contrast_map, i, j, align_dims.m_iW, GetBlocksize(		b_32x32));
			
			Set_Block_Value(map, j,i, align_dims.m_iW, get_contrast_func(contrast_value), BLOCKSIZE);
		}
	}

} 

void Calculate_FrameColorDispersion(BYTE * src, BYTE *dst_contrast_map, int width, int height, int base_block_size)
{
	int ind_x, ind_y;
	int block_area = base_block_size * base_block_size;
	int frame_size = width * height;

	for (ind_y = 0; ind_y < height; ind_y += base_block_size)
	{
		for (ind_x = 0; ind_x < width; ind_x += base_block_size)
		{
			int index = ind_y * width + ind_x ;

			BYTE * cur_src_pointer_R		= src + index;
			BYTE * cur_src_pointer_G		= src + index + frame_size;
			BYTE * cur_src_pointer_B		= src + index + frame_size*2;

			int internal_x, internal_y;
			float cur_value_R, cur_value_G, cur_value_B;
			float block_sum_R = 0, block_sum_G = 0, block_sum_B = 0;
			float block_dispersion_R = 0, block_dispersion_G = 0, block_dispersion_B = 0;

			float block_sum = 0, block_dispersion = 0;

			for (internal_y = 0; internal_y < base_block_size; ++internal_y)
			{
				for (internal_x = 0; internal_x < base_block_size; ++internal_x)
				{
					cur_value_R = cur_src_pointer_R[internal_x] /*/ 16.0f*/;
					cur_value_G = cur_src_pointer_G[internal_x] /*/ 16.0f*/;
					cur_value_B = cur_src_pointer_B[internal_x] /*/ 16.0f*/;

					block_sum_R += cur_value_R;
					block_sum_G += cur_value_G;
					block_sum_B += cur_value_B;

					block_dispersion_R += cur_value_R * cur_value_R;
					block_dispersion_G += cur_value_G * cur_value_G;
					block_dispersion_B += cur_value_B * cur_value_B;
				}
				cur_src_pointer_R += width;
				cur_src_pointer_G += width;
				cur_src_pointer_B += width;
			}

			block_sum_R /= block_area;
			block_sum_G /= block_area;
			block_sum_B /= block_area;
			block_sum	= (block_sum_R + block_sum_G + block_sum_B) / 3;

			block_dispersion_R = block_dispersion_R / block_area - block_sum_R * block_sum_R;
			block_dispersion_G = block_dispersion_G / block_area - block_sum_G * block_sum_G;
			block_dispersion_B = block_dispersion_B / block_area - block_sum_B * block_sum_B;
			
			block_dispersion = (block_dispersion_R  + block_dispersion_G + block_dispersion_B) ;

			Set_Block_Value(dst_contrast_map, ind_x, ind_y, width, (int)MIN(block_dispersion , 255), base_block_size);
		}
	}
}

void Calculate_FrameColorDispersion_PixelBased (BYTE * src, BYTE *dst_contrast_map, int width, int height, int base_block_size )
{

	int ind_x, ind_y;
	int frame_size = width * height;

	for (ind_y = 0; ind_y < height; ind_y ++)
	{
		for (ind_x = 0; ind_x < width; ind_x ++)
		{
			int index = ind_y * width + ind_x ;

			int start_y = MAX ( ind_y - base_block_size, 0 );
			int start_x	= MAX ( ind_x - base_block_size, 0 );
			int end_y	= MIN ( ind_y + base_block_size, height-1 );
			int end_x	= MIN ( ind_x + base_block_size, width -1 );
			int block_area = (end_y - start_y) * (end_x - start_x);

			int start_index = start_y * width + start_x;

			BYTE * cur_src_pointer_R		= src + start_index;
			BYTE * cur_src_pointer_G		= src + start_index + frame_size;
			BYTE * cur_src_pointer_B		= src + start_index + frame_size*2;

			int internal_x, internal_y;
			float cur_value_R, cur_value_G, cur_value_B;
			float block_sum_R = 0, block_sum_G = 0, block_sum_B = 0;
			float block_dispersion_R = 0, block_dispersion_G = 0, block_dispersion_B = 0;

			float block_sum = 0, block_dispersion = 0;

			for (internal_y = 0; internal_y < end_y-start_y; ++internal_y)
			{
				for (internal_x = 0; internal_x < end_x-start_x; ++internal_x)
				{
					cur_value_R = cur_src_pointer_R[internal_x] / 2.0f;
					cur_value_G = cur_src_pointer_G[internal_x] / 2.0f;
					cur_value_B = cur_src_pointer_B[internal_x] / 2.0f;

					block_sum_R += cur_value_R;
					block_sum_G += cur_value_G;
					block_sum_B += cur_value_B;

					block_dispersion_R += cur_value_R * cur_value_R;
					block_dispersion_G += cur_value_G * cur_value_G;
					block_dispersion_B += cur_value_B * cur_value_B;
				}
				cur_src_pointer_R += width;
				cur_src_pointer_G += width;
				cur_src_pointer_B += width;
			}

			block_sum_R /= block_area;
			block_sum_G /= block_area;
			block_sum_B /= block_area;
			block_sum	= (block_sum_R + block_sum_G + block_sum_B) / 3;

			block_dispersion_R = block_dispersion_R / block_area - block_sum_R * block_sum_R;
			block_dispersion_G = block_dispersion_G / block_area - block_sum_G * block_sum_G;
			block_dispersion_B = block_dispersion_B / block_area - block_sum_B * block_sum_B;

			block_dispersion = (block_dispersion_R  + block_dispersion_G + block_dispersion_B) ;

			dst_contrast_map[index] = (int)MIN(block_dispersion , 255);
		}
	}
}

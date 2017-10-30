#pragma once

#ifndef ___CONTRAST_CALCULATION___
#define ___CONTRAST_CALCULATION___

#include "HierarchyME.h"

// coordinates are for pixels 
BYTE Contrast_Convert(int contrast_val);
BYTE Contrast_Convert_sq(int contrast_val);

_inline int GetContrast_InPixel(int **contrast_map, int i, int j, int stride, MEBlockSize bsn);

_inline int GetContrast_ForBlock(int **contrast_map, int ib, int jb, int strideb, MEBlockSize bsn)
{
	int wb = strideb;
	int index = ib * wb + jb;
	return contrast_map[bsn][index];
}

void Calculate_Contrast_Map(Hierarchic_ME *ME_struct, BYTE *map, frame_id direction, int scale);

void Hierarchy_ME_Calc_Frame_Contrast(Hierarchic_ME *ME_struct, frame_id direction, int scale, int shift );
void Hierarchy_ME_Calc_Frame_Contrast_External( Hierarchic_ME *ME_struct, BYTE * buffer_left, BYTE * buffer_right, 
										  BYTE * dst_contrast_left, BYTE * dst_contrast_right, int scale, int shift );

void Calculate_FrameColorDispersion(BYTE * src, BYTE *contrast_map, int width, int height, int base_block_size);
void Calculate_FrameColorDispersion_PixelBased (BYTE * src, BYTE *dst_contrast_map, int width, int height, int base_block_size );

#endif
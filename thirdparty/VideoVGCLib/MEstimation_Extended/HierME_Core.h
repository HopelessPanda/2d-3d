#ifndef HIERME_CORE
#define HIERME_CORE

#include "HierarchyME.h"

void Hierarchy_ME_Calculate_Disparity(Hierarchic_ME *MEstruct, BYTE *dst, int scale, BOOL expand);
void cGetSimpleAlgMotionVectorField ( Hierarchic_ME *MEstruct, cMV * dst, int scale, frame_id frame );

void cCalculateAdditionalBuffer(Hierarchic_ME *MEstruct, void *dst, frame_id frame, 
								int output_type, int scale, BOOL expand);
//void cGetMotionVectorField(Hierarchic_ME *MEstruct, cMV *dst, int scale, BOOL expand);

//void MergeVectors_ToDepth(Hierarchic_ME* MEstruct, BYTE *result_field);

void Merge_Filtering_Mask(Hierarchic_ME * MEstruct, frame_id direction, int scale);

void * Hierarchy_ME_GetPrecalculatedBufPointer(Hierarchic_ME *me_struct, int type_of_output, frame_type frame, int scale);

cMV  Calc_Array_Dispersion_MVCBlock ( MVCBlock *array, int number );

void ConfidenceBased_Disparity_Filter(gmcMotionEstimator *ME_instance, BYTE * disparity_map, BYTE * confidence_map, BYTE *dst_map, int width, int height, int filter_radius);
void ConfidenceBased_ME_Filter(gmcMotionEstimator *ME_instance, BYTE * confidence_map, int filter_radius);
void DepthBased_Motion_Filter ( gmcMotionEstimator *ME_instance, cMV * dst_motion, BYTE * depth_map, BYTE * lrc_confidence, int width, int height, int filter_radius );

void cSetMotionVector(gmcMotionEstimator* cMEStruct, int x, int y, int motionx, int motiony, int motion_dif, int block_size);

// inline short functions below
//
static __inline MEBlockSize GetBlocksize(int block_width)
{
	return (block_width == 32) ? B_32x32 : ( (block_width == 16) ? B_16x16 : ( (block_width == 8) ? B_8x8 : B_4x4 ) );
}

static __inline int GetBlockWidth(MEBlockSize bsn)
{
	return 1 << (bsn+2);
}

static __inline int GetBlockArea(MEBlockSize bsn)
{
	int w = 1 << (bsn+2);
	return w*w;
}

static __inline int ME_GetBlockSize(gmcMotionEstimator *me, int x, int y)
{
	int curBlockIndex = (y / me->m_iBlocksVert) * me->m_iMBlocksHor + x / me->m_iMBlocksHor;
	return  me->m_CurMBlocks[curBlockIndex].m_BlockPartition;
}

static __inline cMV * cGetMVBlockPointer(gmcMotionEstimator* cMEStruct, int x, int y, int *block_size)
{
	cMacroBlock* cMB = cMEStruct->m_CurMBlocks + (y/ME_MACROBLOCK_SIZE)*cMEStruct->m_iMBlocksHor + (x/ME_MACROBLOCK_SIZE);
	if (cMB->m_BlockPartition == cbt_16x16)
	{
		*block_size = 16;
		return &cMB->m_DataMV[0];
	}
	else if (cMB->m_BlockPartition == cbt_8x8)
	{
		int iIndex = ((y&ME_MACROBLOCK_SIZE_M1)/cMB->m_BlockPartition) * (ME_MACROBLOCK_SIZE/cMB->m_BlockPartition) +
			((x&ME_MACROBLOCK_SIZE_M1)/cMB->m_BlockPartition);
		if (cMB->m_SubBlockPartition[iIndex] == cbt_8x8)
		{
			*block_size = 8;
			return &cMB->m_DataMV[iIndex*4];
		}
		else if (cMB->m_SubBlockPartition[iIndex] == cbt_4x4)
		{
			iIndex *= 4;
			iIndex += ((y&7)/cbt_4x4) * 2 +
				((x&7)/cbt_4x4);

			*block_size = 4;
			return &cMB->m_DataMV[iIndex];
		}
		else
		{
			assert(FALSE);
			return NULL;
		}
	}
	else
	{
		assert(FALSE);
		return NULL;
	}
}

#endif

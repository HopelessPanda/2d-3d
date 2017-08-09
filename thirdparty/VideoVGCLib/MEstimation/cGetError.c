/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cGetError.c
*  \brief File with calculation of block difference functions
*
*  This file contains implementation of the functions that performs
*  calculations of difference between blocks.
*
*  $Id: cGetError.c,v 1.13.10.4.2.1 2008/08/01 13:35:27 10a_moi Exp $
*  $Log: cGetError.c,v $
*  Revision 1.13.10.4.2.1  2008/08/01 13:35:27  10a_moi
*  d
*  @
*  text
*  @a240 20
*  /**
*  ***********
*  Revision 1.13.10.5  2008/07/24 18:13:21  10i_tsv
*  EBUG m*
*  Revision 1.13.10.3  2008/07/17 10:01:12  10i_tsv
*  EBUG macro code removed
*  @
*  text
*  @d17 1
*  a17 1
*  *  $Id: cGetError.c,v 1.13.10.4.2.1 2008/08/01 13:35:27 10a_moi Exp $
*  a18 3
*  *  Revision 1.13*
*  Revision 1.13.10.2  2008/07/12 18:09:05  10i_tsv
*  EBUG macro code removed
*  @
*  text
*  @d17 1
*  a17 1
*  *  $Id: cGetError.c,v 1.13 20*
*  Revision 1.13  2007/06/28 08:22:45  05s_gri
*  Headers are changed on YUVsoft headers
*
*  Revision 1.12  2007/06/04 14:40:40  05s_gri
*  Doxygen completed. Doxygen warnings are not fixed.
*
*  Revision 1.11  2007/04/22 20:15:47  05s_put
*  Add/fix doxygen comments
*
*  Revision 1.10  2007/04/12 07:31:48  05s_gri
*  MMX version of 4x4 block error function added.
*
*  Revision 1.9  2007/03/05 14:19:07  05s_put
*  - interface refactoring
*  - memory fix
*
*  Revision 1.5  2006/12/20 17:36:07  05s_put
*  - doxygen comments fix
*  - small code cleanup
*
*  Revision 1.4  2006/12/20 16:56:51  05s_gri
*  Code doxygenning is continued
*
*  Revision 1.3  2006/11/14 16:12:54  09k_sim
*  GetErrorSAD_l_16x8_MMX was added.
*  Null-pointer bugs fixed.
*
*  Revision 1.2  2006/11/13 10:24:53  05s_put
*  New version of compensation
*
*  Revision 1.4  2006/09/09 13:24:57  09k_sim
*  Began to add codec support to the library.
*  MPEG2 ME is supported.
*
*  Revision 1.3  2006/07/19 23:52:26  09k_sim
*  Bugs fixed: incorrect return values in some functions
*
*  Revision 1.2  2006/07/13 07:51:30  05s_put
*  Fixed error
*
*/
#include "cGetError.h"
#include "cMEStaff.h"

/**
**************************************************************************
* \brief Calculates SAD between 32x32 blocks
*
*  This function calculates sum of absolute differences between 
*  16x16 blocks of unsigned chars, using MMX. This blocks are 
*  situated in two frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSAD_32x32_MMX(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	int iVertOff = stride*16;
	return GetErrorSAD_16x16_MMX(block1,             block2,             stride, pMInfo)+
		GetErrorSAD_16x16_MMX(block1+16,          block2+16,          stride, pMInfo)+
		GetErrorSAD_16x16_MMX(block1+iVertOff,    block2+iVertOff,    stride, pMInfo)+
		GetErrorSAD_16x16_MMX(block1+iVertOff+16, block2+iVertOff+16, stride, pMInfo);
}

/**
**************************************************************************
* \brief Calculates smoothness of motion vector field
*
*  This function calculates smoothness of motion vector field in case of
*  using given motion vector as motion vector for current block (variant 3)
*
* \param pMInfo           [IN] -  pointer to the structure with all needed info
* 
* \return smoothness of obtained vector field
*/
static __inline int MVSmooth3(const MInfoStruct* pMInfo)
{
	return MVABS(&pMInfo->m_CurMV, &pMInfo->m_MeanMV) / pMInfo->m_MeanMV.error;
}

/**
**************************************************************************
* \brief Calculates smoothness of motion vector field
*
*  This function calculates smoothness of motion vector field in case of
*  using given motion vector as motion vector for current block (variant 2)
*
* \param pMInfo           [IN] -  pointer to the structure with all needed info
* 
* \return smoothness of obtained vector field
*/
static __inline int MVSmooth2(const MInfoStruct* pMInfo)
{
	int iResult = 0;
	const int iCurMVOff = pMInfo->m_iBlY * pMInfo->m_iBlocksHor + pMInfo->m_iBlX;
	if (pMInfo->m_iBlY > 0)
	{
		iResult += MVABS(&pMInfo->m_CurMV, &pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor]);
		if (pMInfo->m_iBlX > 0)
		{
			iResult += MVABS(&pMInfo->m_CurMV, &pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor - 1]);
		}
		if (pMInfo->m_iBlX < pMInfo->m_iBlocksHor - 1)
		{
			iResult += MVABS(&pMInfo->m_CurMV, &pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor + 1]);
		}
	}
	if (pMInfo->m_iBlX > 0)
	{
		iResult += MVABS(&pMInfo->m_CurMV, &pMInfo->pCurMVs[iCurMVOff - 1]);
	}

	return iResult<<2;
}

/**
**************************************************************************
* \brief Calculates smoothness of motion vector field
*
*  This function calculates smoothness of motion vector field in case of
*  using given motion vector as motion vector for current block (variant 1)
*
* \param pMInfo           [IN] -  pointer to the structure with all needed info
* 
* \return smoothness of obtained vector field
*/
static __inline int MVSmooth(const MInfoStruct* pMInfo)
{
	cMV MeanMV;
	int iResult = 0;
	int iDisp = 0;
	const int iCurMVOff = pMInfo->m_iBlY * pMInfo->m_iBlocksHor + pMInfo->m_iBlX;
	if (pMInfo->m_iBlY > 0 && pMInfo->m_iBlY < pMInfo->m_iBlocksVert - 1 && pMInfo->m_iBlX > 0 && pMInfo->m_iBlX < pMInfo->m_iBlocksHor - 1)
	{
		MeanMV.x = (pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor].x     + pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor - 1].x +
			pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor + 1].x + pMInfo->pCurMVs[iCurMVOff - 1].x /*+
																																																 pMInfo->pPrvMVs[iCurMVOff - pMInfo->m_iBlocksHor - 1].x + pMInfo->pPrvMVs[iCurMVOff - pMInfo->m_iBlocksHor].x +
																																																 pMInfo->pPrvMVs[iCurMVOff - pMInfo->m_iBlocksHor + 1].x +
																																																 pMInfo->pPrvMVs[iCurMVOff - 1].x                        + pMInfo->pPrvMVs[iCurMVOff].x +
																																																 pMInfo->pPrvMVs[iCurMVOff + 1].x                        +
																																																 pMInfo->pPrvMVs[iCurMVOff + pMInfo->m_iBlocksHor - 1].x + pMInfo->pPrvMVs[iCurMVOff + pMInfo->m_iBlocksHor].x +
																																																 pMInfo->pPrvMVs[iCurMVOff + pMInfo->m_iBlocksHor + 1].x*/
																																																 ) / 4;
		MeanMV.y = (pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor].y     + pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor - 1].y +
			pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor + 1].y + pMInfo->pCurMVs[iCurMVOff - 1].y /*+
																																																 pMInfo->pPrvMVs[iCurMVOff - pMInfo->m_iBlocksHor - 1].y + pMInfo->pPrvMVs[iCurMVOff - pMInfo->m_iBlocksHor].y +
																																																 pMInfo->pPrvMVs[iCurMVOff - pMInfo->m_iBlocksHor + 1].y +
																																																 pMInfo->pPrvMVs[iCurMVOff - 1].y                        + pMInfo->pPrvMVs[iCurMVOff].y +
																																																 pMInfo->pPrvMVs[iCurMVOff + 1].y                        +
																																																 pMInfo->pPrvMVs[iCurMVOff + pMInfo->m_iBlocksHor - 1].y + pMInfo->pPrvMVs[iCurMVOff + pMInfo->m_iBlocksHor].y +
																																																 pMInfo->pPrvMVs[iCurMVOff + pMInfo->m_iBlocksHor + 1].y*/
																																																 ) / 4;
		iDisp += MVABS(&MeanMV, &pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor - 1]);
		iDisp += MVABS(&MeanMV, &pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor    ]);
		iDisp += MVABS(&MeanMV, &pMInfo->pCurMVs[iCurMVOff - pMInfo->m_iBlocksHor + 1]);
		iDisp += MVABS(&MeanMV, &pMInfo->pCurMVs[iCurMVOff - 1]);

		//		iDisp += MVABS(MeanMV, pMInfo->pPrvMVs[iCurMVOff - pMInfo->m_iBlocksHor - 1]);
		//		iDisp += MVABS(MeanMV, pMInfo->pPrvMVs[iCurMVOff - pMInfo->m_iBlocksHor    ]);
		//		iDisp += MVABS(MeanMV, pMInfo->pPrvMVs[iCurMVOff - pMInfo->m_iBlocksHor + 1]);
		//
		//		iDisp += MVABS(MeanMV, pMInfo->pPrvMVs[iCurMVOff - 1]);
		//		iDisp += MVABS(MeanMV, pMInfo->pPrvMVs[iCurMVOff    ]);
		//		iDisp += MVABS(MeanMV, pMInfo->pPrvMVs[iCurMVOff + 1]);
		//
		//		iDisp += MVABS(MeanMV, pMInfo->pPrvMVs[iCurMVOff + pMInfo->m_iBlocksHor - 1]);
		//		iDisp += MVABS(MeanMV, pMInfo->pPrvMVs[iCurMVOff + pMInfo->m_iBlocksHor    ]);
		//		iDisp += MVABS(MeanMV, pMInfo->pPrvMVs[iCurMVOff + pMInfo->m_iBlocksHor + 1]);

		iDisp /= 4;
		if (iDisp) iResult = MVABS(&pMInfo->m_CurMV, &MeanMV) / iDisp;
		else iResult = MVABS(&pMInfo->m_CurMV, &MeanMV) * 4;
	}

	return iResult<<4;
}

/**
**************************************************************************
* \brief Calculates SAD between 32x32 blocks, smoothing vector field
*
*  This function calculates sum of absolute differences between 
*  32x32 blocks of unsigned chars, using MMX and adds a value, responsible
*  for smoothness of motion vector field. This blocks are situated in two
*  frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return difference between two blocks based on SAD and vector field smoothing function
*/
long GetErrorSADS_32x32_MMX(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo)
{
	return (pMInfo?(MVSmooth3(pMInfo)<<4):0) + GetErrorSAD_32x32_MMX(block1, block2, stride, pMInfo);
}

/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks, smoothing vector field
*
*  This function calculates sum of absolute differences between 
*  16x16 blocks of unsigned chars, using MMX and adds a value, responsible
*  for smoothness of motion vector field. This blocks are situated in two
*  frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return difference between two blocks based on SAD and vector field smoothing function
*/
long GetErrorSADS_16x16_MMX(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo)
{
	return (pMInfo?(MVSmooth3(pMInfo)<<2):0) + GetErrorSAD_16x16_MMX(block1, block2, stride, pMInfo);
}

/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks, smoothing vector field
*
*  This function calculates sum of absolute differences between 
*  8x8 blocks of unsigned chars, using MMX and adds a value, responsible
*  for smoothness of motion vector field. This blocks are situated in two
*  frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return difference between two blocks based on SAD and vector field smoothing function
*/
long GetErrorSADS_8x8_MMX(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	return (pMInfo?MVSmooth3(pMInfo):0) + GetErrorSAD_8x8_MMX(block1, block2, stride, pMInfo);
}


/**
**************************************************************************
* \brief Calculates SAD between 32x32 blocks
*
*  This function calculates sum of absolute differences between two
*  16x16 blocks of unsigned chars, not using MMX. This blocks are 
*  situated in two frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSAD_32x32_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	int i, j;
	const BYTE *pBl1, *pBl2;
	long Result;

	pBl1 = block1;
	pBl2 = block2;
	Result = 0;

	for (i=0; i<32; i++)
	{
		for (j=0; j<32; j++)
		{
			Result += abs(*pBl1 - *pBl2);
			pBl1++;
			pBl2++;
		}
		pBl1 += stride-32;
		pBl2 += stride-32;
	}

	if(pMInfo)
	{
		pMInfo->m_fFrameAverageSADsPerPixel += 1024;
	}
	return Result;
}

/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks
*
*  This function calculates sum of absolute differences between two
*  16x16 blocks of unsigned chars, not using MMX. This blocks are 
*  situated in two frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSAD_16x16_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	int i, j;
	const BYTE *pBl1, *pBl2;
	long Result;

	pBl1 = block1;
	pBl2 = block2;
	Result = 0;

	for (i=0; i<16; i++)
	{
		for (j=0; j<16; j++)
		{
			Result += abs(*pBl1 - *pBl2);
			pBl1++;
			pBl2++;
		}
		pBl1 += stride-16;
		pBl2 += stride-16;
	}

	if(pMInfo)
	{
		pMInfo->m_fFrameAverageSADsPerPixel += 256;
	}
	return Result;
}

/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks
*
*  This function calculates sum of absolute differences between two
*  8x8 blocks of unsigned chars, not using MMX. This blocks are 
*  situated in two frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSAD_8x8_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	int i, j;
	const BYTE *pBl1, *pBl2;
	long Result;

	pBl1 = block1;
	pBl2 = block2;
	Result = 0;

	for (i=0; i<8; i++)
	{
		for (j=0; j<8; j++)
		{
			Result += abs(*pBl1 - *pBl2);
			pBl1++;
			pBl2++;
		}
		pBl1 += stride-8;
		pBl2 += stride-8;
	}

	if(pMInfo)
	{
		pMInfo->m_fFrameAverageSADsPerPixel += 64;
	}
	return Result;
}

/**
**************************************************************************
* \brief Calculates SAD between 4x4 blocks
*
*  This function calculates sum of absolute differences between two
*  4x4 blocks of unsigned chars, not using MMX. This blocks are 
*  situated in two frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSAD_4x4_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	int i;
	const BYTE *pBl1, *pBl2;
	long Result;

	pBl1 = block1;
	pBl2 = block2;
	Result = 0;

	for (i=0; i<4; i++)
	{
		Result += abs(pBl1[0] - pBl2[0]);
		Result += abs(pBl1[1] - pBl2[1]);
		Result += abs(pBl1[2] - pBl2[2]);
		Result += abs(pBl1[3] - pBl2[3]);
		pBl1 += stride;
		pBl2 += stride;
	}

	if(pMInfo)
	{
		pMInfo->m_fFrameAverageSADsPerPixel += 16;
	}
	return Result;
}

/**
**************************************************************************
* \brief Calculates SAD between 2x2 blocks
*
*  This function calculates sum of absolute differences between two
*  2x2 blocks of unsigned chars, not using MMX. This blocks are 
*  situated in two frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSAD_2x2_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long res1, res2;

	res1 = abs(block1[0] - block2[0]);
	res2 = abs(block1[1] - block2[1]);
	res1 += abs(block1[stride] - block2[stride]);
	res2 += abs(block1[stride + 1] - block2[stride + 1]);

	if(pMInfo)
	{
		pMInfo->m_fFrameAverageSADsPerPixel += 4;
	}
	return res1 + res2;
}

/**
**************************************************************************
* \brief Calculates SAD between 2x2 blocks
*
*  This function calculates sum of absolute differences between 
*  2x2 blocks of unsigned chars. One of the blocks will be calculated
*  on fly in the case of quarter-pixel shift.
*
* \param blockRef         [IN] -  pointer to the reference block in the frame
* \param strideRef        [IN] -  reference block stride
* \param pSrcShifts       [IN] -  array of pointers to the planes with half-pixel shifts
* \param iFirstRowOffset  [IN] -  offset of the first row in plane
* \param iStride          [IN] -  planes stride
* \param iX               [IN] -  X coordinate of the current block
* \param iY               [IN] -  Y coordinate of the current block
* \param iMX              [IN] -  motion vector horizontal component
* \param iMY              [IN] -  motion vector vertical component
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSAD_2x2_H264_C(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY)
{
	int sum;

	int iCorrection = H264_CORRECTION(iMX, iMY);

	BYTE* pSrc1 = pSrcShifts[H264_SRC1_INDEX(iMX, iMY, iCorrection)] +
		H264_SRC1_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

	if (H264_BLEND(iMX, iMY))
	{
		BYTE* pSrc2 = pSrcShifts[H264_SRC2_INDEX(iMX, iMY, iCorrection)] +
			H264_SRC2_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

		sum = abs(((pSrc1[0] + pSrc2[0] + 1) >> 1) - blockRef[0]);
		sum += abs(((pSrc1[1] + pSrc2[1] + 1) >> 1) - blockRef[1]);
		sum += abs(((pSrc1[iStride] + pSrc2[iStride] + 1) >> 1) - blockRef[strideRef]);
		sum += abs(((pSrc1[iStride + 1] + pSrc2[iStride + 1] + 1) >> 1) - blockRef[strideRef + 1]);
	}
	else {
		sum = abs(pSrc1[0] - blockRef[0]);
		sum += abs(pSrc1[1] - blockRef[1]);
		sum += abs(pSrc1[iStride] - blockRef[strideRef]);
		sum += abs(pSrc1[iStride + 1] - blockRef[strideRef + 1]);
	}
	
	return sum;
}

/**
**************************************************************************
* \brief Calculates SM between 16x16 blocks
*
*  This function calculates smooth measure of two 16x16 blocks
*  residual. This blocks are situated inside two frames of
*  unsigned chars with similar sizes.
*
* \param block1              [IN] -  pointer to the first block
* \param block2              [IN] -  pointer to the second block
* \param stride              [IN] -  length of frame line in bytes
* \param pMInfo              [IN/OUT] - pointer to the structure with motion information about current block
* 
* \return smooth measure of two blocks residual
*/
long GetErrorSM_16x16_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	int resid[256];
	long res;
	int i, j;

	for (i=0; i<16; i++)
		for (j=0; j<16; j++)
			resid[i*16 + j] = abs(block1[i*stride + j] - block2[i*stride + j]);

	res = 0;
	for (i=0; i<16; i+=2)
		for (j=0; j<8; j++)
		{
			res += abs(resid[i*16 + j] - resid[(i+1)*16 + j]);
			res += abs(resid[i*16 + j+8] - resid[(i+1)*16 + j+8]);
			res += abs(resid[i*16 + j] - resid[i*16 + j+1]);
			res += abs(resid[(i+1)*16 + j+7] - resid[(i+1)*16 + j+8]);
		}

		return res;
}

/**
**************************************************************************
* \brief Calculates SM between 8x8 blocks
*
*  This function calculates smooth measure of two 8x8 blocks
*  residual. This blocks are situated inside two frames of
*  unsigned chars with similar sizes.
*
* \param block1              [IN] -  pointer to the first block
* \param block2              [IN] -  pointer to the second block
* \param stride              [IN] -  length of frame line in bytes
* \param pMInfo              [IN/OUT] - pointer to the structure with motion information about current block
* 
* \return smooth measure of two blocks residual
*/
long GetErrorSM_8x8_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	int resid[64];
	long res;
	int i, j;

	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
			resid[i*8 + j] = abs(block1[i*stride + j] - block2[i*stride + j]);

	res = 0;
	for (i=0; i<8; i+=2)
		for (j=0; j<4; j++)
		{
			res += abs(resid[i*8 + j] - resid[(i+1)*8 + j]);
			res += abs(resid[i*8 + j+4] - resid[(i+1)*8 + j+4]);
			res += abs(resid[i*8 + j] - resid[i*8 + j+1]);
			res += abs(resid[(i+1)*8 + j+3] - resid[(i+1)*8 + j+4]);
		}

		//for (i=0; i<8; i+=4)
		//	for (j=0; j<4; j++)
		//	{
		//		res += abs(resid[i*8 + j] - resid[(i+1)*8 + j]);
		//		res += abs(resid[(i+2)*8 + j+4] - resid[(i+3)*8 + j+4]);
		//		res += abs(resid[i*8 + j] - resid[i*8 + j+1]);
		//		res += abs(resid[(i+2)*8 + j+3] - resid[(i+2)*8 + j+4]);
		//	}


		return res;
}


/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks, using logo mask
*
*  This function calculates sum of absolute differences between two
*  16x16 blocks of unsigned chars, using logo mask. This blocks are 
*  situated in two frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMask            [IN] -  pointer to the logo mask
* \param MaskW            [IN] -  width of logo mask
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSADLogo_16x16_C(const BYTE* block1, const BYTE* block2, const int stride, const BYTE* pMask, int MaskW)
{
	int i, j;
	const BYTE *pBl1, *pBl2;
	const BYTE *pCurMask;
	long Result;
	int PixelCount;

	pBl1 = block1;
	pBl2 = block2;
	pCurMask = pMask;
	Result = 0;
	PixelCount = 0;

	for (i=0; i<16; i++)
	{
		for (j=0; j<16; j++)
		{
			if (*pCurMask!=1)
			{
				Result += abs(*pBl1 - *pBl2);
				//PixelCount++;
			}
			pCurMask++;
			pBl1++;
			pBl2++;
		}
		pCurMask += MaskW-16;
		pBl1 += stride-16;
		pBl2 += stride-16;
	}

	return Result;
}

/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks, using logo mask
*
*  This function calculates sum of absolute differences between two
*  16x16 blocks of unsigned chars, using logo mask. This blocks are 
*  situated in two frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMask            [IN] -  pointer to the logo mask
* \param MaskW            [IN] -  width of logo mask
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSADLogo_8x8_C(const BYTE* block1, const BYTE* block2, const int stride, const BYTE* pMask, int MaskW)
{
	int i, j;
	const BYTE *pBl1, *pBl2;
	const BYTE *pCurMask;
	long Result;

	pBl1 = block1;
	pBl2 = block2;
	pCurMask = pMask;
	Result = 0;

	for (i=0; i<8; i++)
	{
		for (j=0; j<8; j++)
		{
			Result += (*pCurMask!=1)*abs(*pBl1 - *pBl2);
			pMask++;
			pBl1++;
			pBl2++;
		}
		pMask += MaskW-8;
		pBl1 += stride-8;
		pBl2 += stride-8;
	}

	return Result;
}

/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks for color components, using logo mask
*
*  This function calculates sum of absolute differences between two
*  8x8 blocks of unsigned chars, using logo mask. This blocks are 
*  situated in two frames with similar sizes, color components of YV12
*  colorspace
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMask            [IN] -  pointer to the logo mask
* \param MaskW            [IN] -  width of logo mask
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSADLogoColor_8x8_C(const BYTE* block1, const BYTE* block2, const int stride, const BYTE* pMask, int MaskW)
{
	int i, j;
	const BYTE *pBl1, *pBl2;
	const BYTE *pCurMask;
	long Result;

	pBl1 = block1;
	pBl2 = block2;
	pCurMask = pMask;
	Result = 0;

	for (i=0; i<8; i++)
	{
		for (j=0; j<8; j++)
		{
			Result += (*pCurMask!=1 && pCurMask[1]!=1 && pCurMask[MaskW]!=1 && pCurMask[MaskW+1]!=1)*abs(*pBl1 - *pBl2);
			pMask++;
			pBl1++;
			pBl2++;
		}
		pMask += MaskW-8;
		pBl1 += stride-8;
		pBl2 += stride-8;
	}

	return Result;
}


/**
**************************************************************************
* \brief Calculates SAD between 32x32 interlaced blocks
*
*  This function calculates sum of absolute differences between 
*  32x32 blocks of unsigned chars, using MMX. These blocks are situated 
*  in two interlaced frames with similar sizes. Sum of absolute
*  differences actually is calculated between lines of one field
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSAD_Int_32x32_MMX(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	int iVertOff = stride*16;
	return GetErrorSAD_Int_16x16_MMX(block1,             block2,             stride, pMInfo)+
		GetErrorSAD_Int_16x16_MMX(block1+16,          block2+16,          stride, pMInfo)+
		GetErrorSAD_Int_16x16_MMX(block1+iVertOff,    block2+iVertOff,    stride, pMInfo)+
		GetErrorSAD_Int_16x16_MMX(block1+iVertOff+16, block2+iVertOff+16, stride, pMInfo);
}


/**
**************************************************************************
* \brief Calculates SAD between 16x16 interlaced blocks
*
*  This function calculates sum of absolute differences between
*  16x16 blocks of unsigned chars, implemented in C code. This
*  blocks are situated in two interlaced frames with similar sizes.
*  Sum of absolute differences actually is calculated between lines
*  of one field
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSAD_Int_16x16_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	int i, j;
	const BYTE *pBl1, *pBl2;
	long Result;
	const int stridetimes2 = stride * 2;

	pBl1 = block1 + (pMInfo->m_bBotBase?stride:0);
	pBl2 = block2 + (pMInfo->m_bBotBase?stride:0);
	Result = 0;

	for (i=0; i<16; i++)
	{
		for (j=0; j<16; j++)
		{
			Result += abs(*pBl1 - *pBl2);
			pBl1++;
			pBl2++;
		}
		pBl1 += stridetimes2-16;
		pBl2 += stridetimes2-16;
	}

	return Result;
}

/**
**************************************************************************
* \brief Calculates SAD between 8x8 interlaced blocks
*
*  This function calculates sum of absolute differences between
*  8x8 blocks of unsigned chars, implemented in C code. This
*  blocks are situated in two interlaced frames with similar sizes.
*  Sum of absolute differences actually is calculated between lines
*  of one field
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSAD_Int_8x8_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	int i, j;
	const BYTE *pBl1, *pBl2;
	long Result;
	const int stridetimes2 = stride * 2;

	pBl1 = block1 + (pMInfo->m_bBotBase?stride:0);
	pBl2 = block2 + (pMInfo->m_bBotBase?stride:0);
	Result = 0;

	for (i=0; i<8; i++)
	{
		for (j=0; j<8; j++)
		{
			Result += abs(*pBl1 - *pBl2);
			pBl1++;
			pBl2++;
		}
		pBl1 += stridetimes2-8;
		pBl2 += stridetimes2-8;
	}

	return Result;
}


/**
**************************************************************************
* \brief Calculates SAD between blocks of different sizes
*
*  This function calculates sum of absolute differences between blocks of
*  unsigned chars, implemented in C code.
*
* \param pBlock1             [IN] -  pointer to the first block
* \param pBlock2             [IN] -  pointer to the second block
* \param iStride1            [IN] -  length of frame line of the first block frame in bytes
* \param iStride2            [IN] -  length of frame line of the second block frame in bytes
* \param BlockSize           [IN] -  specifies size of block (iBlockSize x iBlockSize)
* \param pMInfo              [IN] -  additional information about motion
* 
* \return sum of absolute differences between two blocks
*/
long GetErrorSAD_C(const BYTE* pBlock1, const BYTE* pBlock2, const int iStride1, const int iStride2, cMacroBlockPartitions BlockSize, MInfoStruct* pMInfo)
{
	int i, j;
	long lRetValue = 0;

	for (i=0; i<MBheight[BlockSize]; i++)
		for (j=0; j<MBwidth[BlockSize]; j++)
		{
			lRetValue += abs(pBlock1[i*iStride1 + j] - pBlock2[i*iStride2 + j]);
		}

	if(pMInfo)
	{
		pMInfo->m_fFrameAverageSADsPerPixel += MBwidth[BlockSize]*MBheight[BlockSize];
	}

	return lRetValue;
}
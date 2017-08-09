#include "cGetError.h"
#include "cMEStaff.h"

/**
**************************************************************************
* 
* This file is excluded from build for x32 platform using configuration settings
* 
**************************************************************************
**/

extern long GetErrorSAD_16x16_MMX64_asm(const BYTE* block1, const BYTE* block2, const int stride);
extern long GetErrorSAD_16x16_H264_MMX64_asm(const BYTE* pSrc1, const BYTE* pSrc2, const BYTE* blockRef, const int strideRef, int iStride, int h264_blend);

/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks
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

long GetErrorSAD_16x16_MMX64(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum = GetErrorSAD_16x16_MMX64_asm(block1, block2, stride);	

	if(pMInfo)
	{
		pMInfo->m_fFrameAverageSADsPerPixel += 256;
	}
	return sum;
}

/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks
*
*  This function calculates sum of absolute differences between 
*  16x16 blocks of unsigned chars, using MMX. One of the blocks will be
*  calculated on fly in the case of quarter-pixel shift.
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

long GetErrorSAD_16x16_H264_MMX64(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY)
{
	long sum;
	short fourOnes[4] = { 1, 1, 1, 1 };
	BYTE eightOnes[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

	int iCorrection = H264_CORRECTION(iMX, iMY);

	BYTE* pSrc1 = pSrcShifts[H264_SRC1_INDEX(iMX, iMY, iCorrection)] +
		H264_SRC1_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

	if (H264_BLEND(iMX, iMY))
	{
		BYTE* pSrc2 = pSrcShifts[H264_SRC2_INDEX(iMX, iMY, iCorrection)] +
			H264_SRC2_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

		sum = GetErrorSAD_16x16_H264_MMX64_asm(pSrc1, pSrc2, blockRef, strideRef, iStride, 1);
	}
	else{
		sum = GetErrorSAD_16x16_H264_MMX64_asm(pSrc1, NULL, blockRef, strideRef, iStride, 0);
	}

	return sum;
}

/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks
*
*  This function calculates sum of absolute differences between 
*  8x8 blocks of unsigned chars, using MMX. This blocks are situated 
*  in two frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/

extern long GetErrorSAD_8x8_MMX64_asm(const BYTE* block1, const BYTE* block2, const int stride);

long GetErrorSAD_8x8_MMX64(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum = GetErrorSAD_8x8_MMX64_asm(block1, block2, stride);

	if(pMInfo)
	{
		pMInfo->m_fFrameAverageSADsPerPixel += 64;
	}
	return sum;
}

/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks
*
*  This function calculates sum of absolute differences between 
*  8x8 blocks of unsigned chars, using MMX. One of the blocks will be
*  calculated on fly in the case of quarter-pixel shift.
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

extern long GetErrorSAD_8x8_H264_MMX64_asm(const BYTE* pSrc1, const BYTE* pSrc2, const BYTE* blockRef, const int strideRef, int iStride, int h264_blend);

long GetErrorSAD_8x8_H264_MMX64(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY)
{
	long sum;

	int iCorrection = H264_CORRECTION(iMX, iMY);

	BYTE* pSrc1 = pSrcShifts[H264_SRC1_INDEX(iMX, iMY, iCorrection)] +
		H264_SRC1_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

	if (H264_BLEND(iMX, iMY))
	{
		BYTE* pSrc2 = pSrcShifts[H264_SRC2_INDEX(iMX, iMY, iCorrection)] +
			H264_SRC2_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

		sum = GetErrorSAD_8x8_H264_MMX64_asm(pSrc1, pSrc2, blockRef, strideRef, iStride, 1);
	}
	else{
		sum = GetErrorSAD_8x8_H264_MMX64_asm(pSrc1, NULL, blockRef, strideRef, iStride, 0);
	}

	return sum;
}

/**
**************************************************************************
* \brief Calculates SAD between 4x4 blocks
*
*  This function calculates sum of absolute differences between 
*  4x4 blocks of unsigned chars, using MMX. This blocks are situated 
*  in two frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/

extern long GetErrorSAD_4x4_MMX64_asm(const BYTE* block1, const BYTE* block2, const int stride);

long GetErrorSAD_4x4_MMX64(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum = GetErrorSAD_4x4_MMX64_asm(block1, block2, stride);

	if(pMInfo)
	{

		pMInfo->m_fFrameAverageSADsPerPixel += 16;
	}
	return sum;
}


/**
**************************************************************************
* \brief Calculates SAD between 4x4 blocks
*
*  This function calculates sum of absolute differences between 
*  4x4 blocks of unsigned chars, using MMX. One of the blocks will be
*  calculated on fly in the case of quarter-pixel shift.
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

extern long GetErrorSAD_4x4_H264_MMX64_asm(const BYTE* pSrc1, const BYTE* pSrc2, const BYTE* blockRef, const int strideRef, int iStride, int h264_blend);

long GetErrorSAD_4x4_H264_MMX64(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY)
{
	long sum;

	int iCorrection = H264_CORRECTION(iMX, iMY);

	BYTE* pSrc1 = pSrcShifts[H264_SRC1_INDEX(iMX, iMY, iCorrection)] +
		H264_SRC1_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

	if (H264_BLEND(iMX, iMY))
	{
		BYTE* pSrc2 = pSrcShifts[H264_SRC2_INDEX(iMX, iMY, iCorrection)] +
			H264_SRC2_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

		sum = GetErrorSAD_4x4_H264_MMX64_asm(pSrc1, pSrc2, blockRef, strideRef, iStride, 1);
	}
	else{
		sum = GetErrorSAD_4x4_H264_MMX64_asm(pSrc1, NULL, blockRef, strideRef, iStride, 0);
	}

	return sum;
}

/**
**************************************************************************
* \brief Calculates SAD between 16x16 interlaced blocks
*
*  This function calculates sum of absolute differences between 
*  16x16 blocks of unsigned chars, using MMX. These blocks are situated 
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

extern long GetErrorSAD_Int_16x16_MMX64_asm(const BYTE* block1, const BYTE* block2, const int stride);

long GetErrorSAD_Int_16x16_MMX64(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum;
	const BYTE* b1 = block1 + (pMInfo->m_bBotBase?stride:0);
	const BYTE* b2 = block2 + (pMInfo->m_bBotBase?stride:0);

	sum = GetErrorSAD_Int_16x16_MMX64_asm(b1, b2, stride);

	return sum;
}

/**
**************************************************************************
* \brief Calculates SAD between 16x8 interlaced blocks
*
*  This function calculates sum of absolute differences between 
*  16x8 blocks of unsigned chars, using MMX. These blocks are situated 
*  in two interlaced frame pictures (fields are interleaved) with similar sizes.
*  Is used in field-in-frame ME, MPEG-2 standard.
*
* \param block1           [IN] -  pointer to the first block (from the current frame)
* \param block2           [IN] -  pointer to the second block (compensated from the previous frame)
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/
/*long GetErrorSAD_Int_16x8_MMX(const BYTE* block1, const BYTE* block2, const int stride, const MInfoStruct* pMInfo)
{
	long sum = 0;
	short m[4] = {1,1,1,1};
	//const BYTE* b1 = block1 + (pMInfo->m_bBotBase?stride:0);
	//const BYTE* b2 = block2 + (pMInfo->m_bBotBase?stride:0);
	const BYTE* b1 = block1 + (pMInfo->m_curField?stride:0);
	const BYTE* b2 = block2 + (pMInfo->m_refField?stride:0);
	__asm
	{
		push edi
			push esi

			mov esi, b1//[esp + 8 + 4]	; Current MB (Estimated MB)
			mov edi, b2//[esp + 8 + 12]	; reference MB
			mov eax, stride//[esp + 8 + 20]	; stride
			add eax, stride

			pxor mm7, mm7			; mm7 = sum = 0
			pxor mm6, mm6			; mm6 = 0

			movq mm1, [esi]			; 8 pixels for current MB
			movq mm3, [esi+8]		; another 8 pixels in the same row of current MB

			mov	ecx, 8

sad16_mmx_loop:
		movq mm0, [edi]			; 8 pixels for reference MB
			movq mm2, [edi+8]		; another 8 pixels in the same row of reference MB

			movq mm4, mm1
			movq mm5, mm3

			psubusb mm1, mm0
			psubusb mm3, mm2

			add	esi, eax
			add	edi, eax

			psubusb mm0, mm4
			psubusb mm2, mm5

			por mm0, mm1			; mm0 = |cur - ref|
			por mm2, mm3			; mm2 = |*(cur+8) - *(ref+8)|

			movq mm1,mm0
			movq mm3,mm2

			punpcklbw mm0,mm6
			punpckhbw mm1,mm6

			punpcklbw mm2,mm6
			punpckhbw mm3,mm6

			paddusw mm0,mm1
			paddusw mm2,mm3		

			paddusw mm7,mm0		; sum += mm01
			movq mm1, [esi]		; 8 pixels for current MB

			paddusw mm7,mm2		; sum += mm23
			movq mm3, [esi+8]	; another 8 pixels in the same row of current MB

			;// start next row's processing
		movq mm0, [edi]			; 8 pixels for reference MB
			movq mm2, [edi+8]		; another 8 pixels in the same row of reference MB

			movq mm4, mm1
			movq mm5, mm3

			psubusb mm1, mm0
			psubusb mm3, mm2

			add	esi, eax
			add	edi, eax

			psubusb mm0, mm4
			psubusb mm2, mm5

			por mm0, mm1			; mm0 = |cur - ref|
			por mm2, mm3			; mm2 = |*(cur+8) - *(ref+8)|

			movq mm1,mm0
			movq mm3,mm2

			punpcklbw mm0,mm6
			punpckhbw mm1,mm6

			punpcklbw mm2,mm6
			punpckhbw mm3,mm6

			paddusw mm0,mm1
			paddusw mm2,mm3		

			paddusw mm7,mm0		; sum += mm01
			movq mm1, [esi]		; 8 pixels for current MB

			paddusw mm7,mm2		; sum += mm23
			sub		ecx, 2		; unlooped two rows' processing

			movq mm3, [esi+8]	; another 8 pixels in the same row of current MB

			jnz	sad16_mmx_loop

			pmaddwd mm7, m	; merge sum
			pop 	esi

			movq mm0, mm7
			pop 	edi
			psrlq mm7, 32 

			paddd mm0, mm7
			movd sum, mm0

			emms
	}
	return sum;
}*/

/**
**************************************************************************
* \brief Calculates SAD between 8x8 interlaced blocks
*
*  This function calculates sum of absolute differences between 
*  8x8 blocks of unsigned chars, using MMX. These blocks are situated 
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

extern long GetErrorSAD_Int_8x8_MMX64_asm(const BYTE* block1, const BYTE* block2, const int stride);

long GetErrorSAD_Int_8x8_MMX64(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum;
	const BYTE* b1 = block1 + (pMInfo->m_bBotBase?stride:0);
	const BYTE* b2 = block2 + (pMInfo->m_bBotBase?stride:0);

	sum = GetErrorSAD_Int_8x8_MMX64_asm(b1, b2, stride);

	return sum;
}

/**
**************************************************************************
* \brief Calculates SAD between 16x8 blocks
*
*  This function calculates sum of absolute differences between 
*  16x8 blocks of unsigned chars, using MMX. This blocks are 
*  situated in two frames with similar sizes.
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
* 
* \return sum of absolute differences between two blocks
*/

extern long GetErrorSAD_16x8_MMX64_asm(const BYTE* block1, const BYTE* block2, const int stride);

long GetErrorSAD_16x8_MMX64(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum = GetErrorSAD_16x8_MMX64_asm(block1, block2, stride);

	if(pMInfo)
	{
		pMInfo->m_fFrameAverageSADsPerPixel += 128;
	}
	return sum;
}

/**
**************************************************************************
* \brief Calculates SAD between 16x8 blocks
*
*  This function calculates sum of absolute differences between 
*  16x8 blocks of unsigned chars, using MMX. One of the blocks will be
*  calculated on fly in the case of quarter-pixel shift.
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

extern long GetErrorSAD_16x8_H264_MMX64_asm(const BYTE* pSrc1, const BYTE* pSrc2, const BYTE* blockRef, const int strideRef, int iStride, int h264_blend);

long GetErrorSAD_16x8_H264_MMX64(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY)
{
	long sum;

	int iCorrection = H264_CORRECTION(iMX, iMY);

	BYTE* pSrc1 = pSrcShifts[H264_SRC1_INDEX(iMX, iMY, iCorrection)] +
		H264_SRC1_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

	if (H264_BLEND(iMX, iMY))
	{
		BYTE* pSrc2 = pSrcShifts[H264_SRC2_INDEX(iMX, iMY, iCorrection)] +
			H264_SRC2_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

		sum = GetErrorSAD_16x8_H264_MMX64_asm(pSrc1, pSrc2, blockRef, strideRef, iStride, 1);
	}
	else{
		sum = GetErrorSAD_16x8_H264_MMX64_asm(pSrc1, NULL, blockRef, strideRef, iStride, 0);
	}

	return sum;
}

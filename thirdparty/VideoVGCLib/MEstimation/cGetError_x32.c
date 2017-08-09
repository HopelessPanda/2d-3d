#include "cGetError.h"
#include "cMEStaff.h"

/**
**************************************************************************
* 
* This file is excluded from build for x64 platform using configuration settings
* 
**************************************************************************
**/


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
long GetErrorSAD_16x16_MMX32(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum = 0;
	short m[4] = {1,1,1,1};
	const BYTE* b1 = block1;
	const BYTE* b2 = block2;

	__asm
	{
		push edi
			push esi

			mov esi, b1//[esp + 8 + 4]	; Current MB (Estimated MB)
			mov edi, b2//[esp + 8 + 12]	; reference MB
			mov eax, stride//[esp + 8 + 20]	; stride

			pxor mm7, mm7			; mm7 = sum = 0
			pxor mm6, mm6			; mm6 = 0

			movq mm1, [esi]			; 8 pixels for current MB
			movq mm3, [esi+8]		; another 8 pixels in the same row of current MB

			mov	ecx, 16
			//mov	ecx, 8

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
long GetErrorSAD_16x16_H264_MMX32(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY)
{
	long sum = 0;
	short fourOnes[4] = { 1, 1, 1, 1 };
	BYTE eightOnes[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

	int iCorrection = H264_CORRECTION(iMX, iMY);

	BYTE* pSrc1 = pSrcShifts[H264_SRC1_INDEX(iMX, iMY, iCorrection)] +
		H264_SRC1_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

	if (H264_BLEND(iMX, iMY))
	{
		BYTE* pSrc2 = pSrcShifts[H264_SRC2_INDEX(iMX, iMY, iCorrection)] +
			H264_SRC2_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

		__asm
		{
			push edi
				push esi

				mov esi, pSrc1
				mov ebx, pSrc2
				mov edi, blockRef
				mov eax, strideRef
				mov edx, iStride

				movq mm6, eightOnes
				pxor mm7, mm7

				mov	ecx, 16

lineLoopComplex:
			movq mm1, [esi]			// 8 pixels for current MB
			movq mm3, [esi + 8]		// another 8 pixels in the same row of current MB
			add	esi, edx

				movq mm0, [ebx]
			movq mm2, [ebx + 8]
			add	ebx, edx

				movq mm4, mm1
				movq mm5, mm3

				por mm4, mm0
				por mm5, mm2

				pand mm4, mm6
				pand mm5, mm6

				por mm1, mm6
				por mm3, mm6

				psrlq mm1, 1
				psrlq mm3, 1

				por mm0, mm6
				por mm2, mm6

				psrlq mm0, 1
				psrlq mm2, 1

				pxor mm6, mm6

				paddb mm1, mm0
				paddb mm3, mm2

				paddb mm1, mm4
				paddb mm3, mm5

				movq mm0, [edi]			// 8 pixels for reference MB
			movq mm2, [edi + 8]		// another 8 pixels in the same row of reference MB
			add	edi, eax

				movq mm4, mm1
				movq mm5, mm3

				psubusb mm1, mm0
				psubusb mm3, mm2

				psubusb mm0, mm4
				psubusb mm2, mm5

				por mm0, mm1			// mm0 = |cur - ref|
				por mm2, mm3			// mm2 = |*(cur+8) - *(ref+8)|

				movq mm1, mm0
				movq mm3, mm2

				punpcklbw mm0, mm6
				punpckhbw mm1, mm6

				punpcklbw mm2, mm6
				punpckhbw mm3, mm6

				movq mm6, eightOnes

				paddusw mm0, mm1
				paddusw mm2, mm3		

				paddusw mm7, mm0		// sum += mm01
				paddusw mm7, mm2		// sum += mm23

				sub ecx, 1

				jnz	lineLoopComplex

				pmaddwd mm7, fourOnes	// merge sum

				movq mm0, mm7
				psrlq mm7, 32 

				paddd mm0, mm7
				movd sum, mm0

				emms

				pop 	esi
				pop 	edi
		}
	}
	else {
		__asm
		{
			push edi
				push esi

				mov esi, pSrc1
				mov edi, blockRef
				mov eax, strideRef
				mov edx, iStride

				pxor mm6, mm6
				pxor mm7, mm7

				mov	ecx, 16

lineLoopSimple:
			movq mm1, [esi]			// 8 pixels for current MB
			movq mm3, [esi + 8]		// another 8 pixels in the same row of current MB
			add	esi, edx

				movq mm0, [edi]			// 8 pixels for reference MB
			movq mm2, [edi + 8]		// another 8 pixels in the same row of reference MB
			add	edi, eax

				movq mm4, mm1
				movq mm5, mm3

				psubusb mm1, mm0
				psubusb mm3, mm2

				psubusb mm0, mm4
				psubusb mm2, mm5

				por mm0, mm1			// mm0 = |cur - ref|
				por mm2, mm3			// mm2 = |*(cur+8) - *(ref+8)|

				movq mm1, mm0
				movq mm3, mm2

				punpcklbw mm0, mm6
				punpckhbw mm1, mm6

				punpcklbw mm2, mm6
				punpckhbw mm3, mm6

				paddusw mm0, mm1
				paddusw mm2, mm3		

				paddusw mm7, mm0		// sum += mm01
				paddusw mm7, mm2		// sum += mm23

				sub ecx, 1

				jnz	lineLoopSimple

				pmaddwd mm7, fourOnes	// merge sum

				movq mm0, mm7
				psrlq mm7, 32 

				paddd mm0, mm7
				movd sum, mm0

				emms

				pop 	esi
				pop 	edi
		}
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
long GetErrorSAD_8x8_MMX32(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum = 0;
	short m[4] = {1,1,1,1};
	const BYTE* b1 = block1;
	const BYTE* b2 = block2;

	__asm
	{
		push edi
			push esi

			mov esi, b1//[esp + 8 + 4]	; Current MB (Estimated MB)
			mov edi, b2//[esp + 8 + 12]	; reference MB
			mov eax, stride//[esp + 8 + 20]	; stride

			pxor	mm6, mm6			; mm6 = sum = 0

			pxor	mm7, mm7			; mm7 = 0
			mov		ecx, 4

sad8_mmx_lp:

		movq mm0, [esi]	; ref
			movq mm1, [edi]	; cur

			add	esi, eax
			add	edi, eax

			movq mm2, [esi]	; ref2
			movq mm3, [edi]	; cur2

			movq mm4, mm0 
			movq mm5, mm2

			psubusb mm0, mm1
			psubusb mm2, mm3

			psubusb mm1, mm4
			psubusb mm3, mm5

			por mm0, mm1			; mm0 = |ref - cur|
			por mm2, mm3			; mm2 = |*(ref+stride) - *(cur+stride)|

			movq mm1,mm0
			movq mm3,mm2

			punpcklbw mm0,mm7
			punpcklbw mm2,mm7

			punpckhbw mm1,mm7
			punpckhbw mm3,mm7

			paddusw mm0,mm1
			paddusw mm2,mm3

			paddusw mm6,mm0		; sum += mm01
			add	esi, eax

			add	edi, eax
			paddusw mm6,mm2		; sum += mm23

			dec	ecx
			jnz	sad8_mmx_lp

			pmaddwd mm6, m	; merge sum
			pop 	esi

			movq mm7, mm6
			pop 	edi

			psrlq mm7, 32 

			paddd mm6, mm7

			movd sum, mm6

			emms
	}

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
long GetErrorSAD_8x8_H264_MMX32(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY)
{
	long sum = 0;
	short fourOnes[4] = { 1, 1, 1, 1 };
	BYTE eightOnes[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

	int iCorrection = H264_CORRECTION(iMX, iMY);

	BYTE* pSrc1 = pSrcShifts[H264_SRC1_INDEX(iMX, iMY, iCorrection)] +
		H264_SRC1_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

	if (H264_BLEND(iMX, iMY))
	{
		BYTE* pSrc2 = pSrcShifts[H264_SRC2_INDEX(iMX, iMY, iCorrection)] +
			H264_SRC2_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

		__asm
		{
			push edi
				push esi

				mov esi, pSrc1
				mov ebx, pSrc2
				mov edi, blockRef
				mov eax, strideRef
				mov edx, iStride

				movq mm6, eightOnes
				pxor mm7, mm7

				mov	ecx, 8

lineLoopComplex:
			movq mm1, [esi]			// 8 pixels for current MB
			movq mm3, [esi + edx]	// another 8 pixels in the same row of current MB
			lea	esi, [esi + 2 * edx]

			movq mm0, [ebx]
			movq mm2, [ebx + edx]
			lea	ebx, [ebx + 2 * edx]

			movq mm4, mm1
				movq mm5, mm3

				por mm4, mm0
				por mm5, mm2

				pand mm4, mm6
				pand mm5, mm6

				por mm1, mm6
				por mm3, mm6

				psrlq mm1, 1
				psrlq mm3, 1

				por mm0, mm6
				por mm2, mm6

				psrlq mm0, 1
				psrlq mm2, 1

				pxor mm6, mm6

				paddb mm1, mm0
				paddb mm3, mm2

				paddb mm1, mm4
				paddb mm3, mm5

				movq mm0, [edi]			// 8 pixels for reference MB
			movq mm2, [edi + eax]	// another 8 pixels in the same row of reference MB
			lea	edi, [edi + 2 * eax]

			movq mm4, mm1
				movq mm5, mm3

				psubusb mm1, mm0
				psubusb mm3, mm2

				psubusb mm0, mm4
				psubusb mm2, mm5

				por mm0, mm1			// mm0 = |cur - ref|
				por mm2, mm3			// mm2 = |*(cur+8) - *(ref+8)|

				movq mm1, mm0
				movq mm3, mm2

				punpcklbw mm0, mm6
				punpckhbw mm1, mm6

				punpcklbw mm2, mm6
				punpckhbw mm3, mm6

				movq mm6, eightOnes

				paddusw mm0, mm1
				paddusw mm2, mm3		

				paddusw mm7, mm0		// sum += mm01
				paddusw mm7, mm2		// sum += mm23

				sub ecx, 2

				jnz	lineLoopComplex

				pmaddwd mm7, fourOnes	// merge sum

				movq mm0, mm7
				psrlq mm7, 32 

				paddd mm0, mm7
				movd sum, mm0

				emms

				pop 	esi
				pop 	edi
		}
	}
	else {
		__asm
		{
			push edi
				push esi

				mov esi, pSrc1
				mov edi, blockRef
				mov eax, strideRef
				mov edx, iStride

				pxor mm6, mm6
				pxor mm7, mm7

				mov	ecx, 8

lineLoopSimple:
			movq mm1, [esi]			// 8 pixels for current MB
			movq mm3, [esi + edx]	// another 8 pixels in the same row of current MB
			lea	esi, [esi + 2 * edx]

			movq mm0, [edi]			// 8 pixels for reference MB
			movq mm2, [edi + eax]	// another 8 pixels in the same row of reference MB
			lea	edi, [edi + 2 * eax]

			movq mm4, mm1
				movq mm5, mm3

				psubusb mm1, mm0
				psubusb mm3, mm2

				psubusb mm0, mm4
				psubusb mm2, mm5

				por mm0, mm1			// mm0 = |cur - ref|
				por mm2, mm3			// mm2 = |*(cur+8) - *(ref+8)|

				movq mm1, mm0
				movq mm3, mm2

				punpcklbw mm0, mm6
				punpckhbw mm1, mm6

				punpcklbw mm2, mm6
				punpckhbw mm3, mm6

				paddusw mm0, mm1
				paddusw mm2, mm3		

				paddusw mm7, mm0		// sum += mm01
				paddusw mm7, mm2		// sum += mm23

				sub ecx, 2

				jnz	lineLoopSimple

				pmaddwd mm7, fourOnes	// merge sum

				movq mm0, mm7
				psrlq mm7, 32 

				paddd mm0, mm7
				movd sum, mm0

				emms

				pop 	esi
				pop 	edi
		}
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
long GetErrorSAD_4x4_MMX32(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum = 0;
	short m[4] = {1,1,1,1};
	const BYTE* b1 = block1;
	const BYTE* b2 = block2;

	__asm
	{
		push edi
			push esi

			mov esi, b1//[esp + 8 + 4]	; Current MB (Estimated MB)
			mov edi, b2//[esp + 8 + 12]	; reference MB
			mov eax, stride//[esp + 8 + 20]	; stride

			pxor	mm6, mm6			; mm6 = sum = 0
			pxor	mm7, mm7			; mm7 = 0
			mov		ecx, 2

sad4_mmx_lp:

		movd mm0, [esi]	; ref
			movd mm1, [edi]	; cur

			add	esi, eax
			add	edi, eax

			movd mm2, [esi]	; ref2
			movd mm3, [edi]	; cur2

			movq mm4, mm0 
			movq mm5, mm2

			psubusb mm0, mm1
			psubusb mm2, mm3

			psubusb mm1, mm4
			psubusb mm3, mm5

			por mm0, mm1			; mm0 = |ref - cur|
			por mm2, mm3			; mm2 = |*(ref+stride) - *(cur+stride)|

			punpcklbw mm0,mm7
			punpcklbw mm2,mm7

			paddusw mm6,mm0		; sum += mm01
			paddusw mm6,mm2		; sum += mm23

			add	esi, eax
			add	edi, eax

			dec	ecx
			jnz	sad4_mmx_lp

			pmaddwd mm6, m	; merge sum
			pop 	esi

			movq mm7, mm6
			pop 	edi

			psrlq mm7, 32 

			paddd mm6, mm7

			movd sum, mm6

			emms
	}

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
long GetErrorSAD_4x4_H264_MMX32(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY)
{
	long sum = 0;
	short fourOnes[4] = { 1, 1, 1, 1 };
	BYTE eightOnes[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

	int iCorrection = H264_CORRECTION(iMX, iMY);

	BYTE* pSrc1 = pSrcShifts[H264_SRC1_INDEX(iMX, iMY, iCorrection)] +
		H264_SRC1_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

	if (H264_BLEND(iMX, iMY))
	{
		BYTE* pSrc2 = pSrcShifts[H264_SRC2_INDEX(iMX, iMY, iCorrection)] +
			H264_SRC2_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

		__asm
		{
			push edi
				push esi

				mov esi, pSrc1
				mov ebx, pSrc2
				mov edi, blockRef
				mov eax, strideRef
				mov edx, iStride

				movq mm6, eightOnes
				pxor mm7, mm7

				movd mm1, [esi]
			movd mm3, [esi + 2 * edx]
			add esi, edx

				psllq mm1, 32
				psllq mm3, 32

				movd mm4, [esi]
			movd mm5, [esi + 2 * edx]

			por mm1, mm4
				por mm3, mm5

				movd mm0, [ebx]
			movd mm2, [ebx + 2 * edx]
			add ebx, edx

				psllq mm0, 32
				psllq mm2, 32

				movd mm4, [ebx]
			movd mm5, [ebx + 2 * edx]

			por mm0, mm4
				por mm2, mm5

				movq mm4, mm1
				movq mm5, mm3

				por mm4, mm0
				por mm5, mm2

				pand mm4, mm6
				pand mm5, mm6

				por mm1, mm6
				por mm3, mm6

				psrlq mm1, 1
				psrlq mm3, 1

				por mm0, mm6
				por mm2, mm6

				psrlq mm0, 1
				psrlq mm2, 1

				pxor mm6, mm6

				paddb mm1, mm0
				paddb mm3, mm2

				paddb mm1, mm4
				paddb mm3, mm5

				movd mm0, [edi]
			movd mm2, [edi + 2 * eax]
			add edi, eax

				psllq mm0, 32
				psllq mm2, 32

				movd mm4, [edi]
			movd mm5, [edi + 2 * eax]

			por mm0, mm4
				por mm2, mm5

				movq mm4, mm1
				movq mm5, mm3

				psubusb mm1, mm0
				psubusb mm3, mm2

				psubusb mm0, mm4
				psubusb mm2, mm5

				por mm0, mm1			// mm0 = |cur - ref|
				por mm2, mm3			// mm2 = |*(cur+8) - *(ref+8)|

				movq mm1, mm0
				movq mm3, mm2

				punpcklbw mm0, mm6
				punpckhbw mm1, mm6

				punpcklbw mm2, mm6
				punpckhbw mm3, mm6

				paddusw mm0, mm1
				paddusw mm2, mm3		

				paddusw mm7, mm0		// sum += mm01
				paddusw mm7, mm2		// sum += mm23

				pmaddwd mm7, fourOnes	// merge sum

				movq mm0, mm7
				psrlq mm7, 32 

				paddd mm0, mm7
				movd sum, mm0

				emms

				pop 	esi
				pop 	edi
		}
	}
	else {
		__asm
		{
			push edi
				push esi

				mov esi, pSrc1
				mov edi, blockRef
				mov eax, strideRef
				mov edx, iStride

				pxor mm6, mm6
				pxor mm7, mm7

				movd mm1, [esi]
			movd mm3, [esi + 2 * edx]
			add esi, edx

				psllq mm1, 32
				psllq mm3, 32

				movd mm4, [esi]
			movd mm5, [esi + 2 * edx]

			por mm1, mm4
				por mm3, mm5

				movd mm0, [edi]
			movd mm2, [edi + 2 * eax]
			add edi, eax

				psllq mm0, 32
				psllq mm2, 32

				movd mm4, [edi]
			movd mm5, [edi + 2 * eax]

			por mm0, mm4
				por mm2, mm5

				movq mm4, mm1
				movq mm5, mm3

				psubusb mm1, mm0
				psubusb mm3, mm2

				psubusb mm0, mm4
				psubusb mm2, mm5

				por mm0, mm1			// mm0 = |cur - ref|
				por mm2, mm3			// mm2 = |*(cur+8) - *(ref+8)|

				movq mm1, mm0
				movq mm3, mm2

				punpcklbw mm0, mm6
				punpckhbw mm1, mm6

				punpcklbw mm2, mm6
				punpckhbw mm3, mm6

				paddusw mm0, mm1
				paddusw mm2, mm3		

				paddusw mm7, mm0		// sum += mm01
				paddusw mm7, mm2		// sum += mm23

				pmaddwd mm7, fourOnes	// merge sum

				movq mm0, mm7
				psrlq mm7, 32 

				paddd mm0, mm7
				movd sum, mm0

				emms

				pop 	esi
				pop 	edi
		}
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
long GetErrorSAD_Int_16x16_MMX32(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum = 0;
	short m[4] = {1,1,1,1};
	const BYTE* b1 = block1 + (pMInfo->m_bBotBase?stride:0);
	const BYTE* b2 = block2 + (pMInfo->m_bBotBase?stride:0);
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

			mov	ecx, 16
			//mov	ecx, 8

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
long GetErrorSAD_Int_8x8_MMX32(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum = 0;
	short m[4] = {1,1,1,1};
	const BYTE* b1 = block1 + (pMInfo->m_bBotBase?stride:0);
	const BYTE* b2 = block2 + (pMInfo->m_bBotBase?stride:0);
	__asm
	{
		push edi
			push esi

			mov esi, b1//[esp + 8 + 4]	; Current MB (Estimated MB)
			mov edi, b2//[esp + 8 + 12]	; reference MB
			mov eax, stride//[esp + 8 + 20]	; stride
			add eax, stride

			pxor	mm6, mm6			; mm6 = sum = 0

			pxor	mm7, mm7			; mm7 = 0
			mov		ecx, 4

sad8_mmx_lp:

		movq mm0, [esi]	; ref
			movq mm1, [edi]	; cur

			add	esi, eax
			add	edi, eax

			movq mm2, [esi]	; ref2
			movq mm3, [edi]	; cur2

			movq mm4, mm0 
			movq mm5, mm2

			psubusb mm0, mm1
			psubusb mm2, mm3

			psubusb mm1, mm4
			psubusb mm3, mm5

			por mm0, mm1			; mm0 = |ref - cur|
			por mm2, mm3			; mm2 = |*(ref+stride) - *(cur+stride)|

			movq mm1,mm0
			movq mm3,mm2

			punpcklbw mm0,mm7
			punpcklbw mm2,mm7

			punpckhbw mm1,mm7
			punpckhbw mm3,mm7

			paddusw mm0,mm1
			paddusw mm2,mm3

			paddusw mm6,mm0			; sum += mm01
			add	esi, eax

			add	edi, eax
			paddusw mm6,mm2			; sum += mm23

			dec	ecx
			jnz	sad8_mmx_lp

			pmaddwd mm6, m	; merge sum
			pop 	esi

			movq mm7, mm6
			pop 	edi

			psrlq mm7, 32 

			paddd mm6, mm7

			movd sum, mm6

			emms
	}
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
long GetErrorSAD_16x8_MMX32(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	long sum = 0;
	short m[4] = {1,1,1,1};
	const BYTE* b1 = block1;
	const BYTE* b2 = block2;

	__asm
	{
		push edi
			push esi

			mov esi, b1//[esp + 8 + 4]	; Current MB (Estimated MB)
			mov edi, b2//[esp + 8 + 12]	; reference MB
			mov eax, stride//[esp + 8 + 20]	; stride

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
long GetErrorSAD_16x8_H264_MMX32(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY)
{
	long sum = 0;
	short fourOnes[4] = { 1, 1, 1, 1 };
	BYTE eightOnes[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

	int iCorrection = H264_CORRECTION(iMX, iMY);

	BYTE* pSrc1 = pSrcShifts[H264_SRC1_INDEX(iMX, iMY, iCorrection)] +
		H264_SRC1_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

	if (H264_BLEND(iMX, iMY))
	{
		BYTE* pSrc2 = pSrcShifts[H264_SRC2_INDEX(iMX, iMY, iCorrection)] +
			H264_SRC2_OFFSET(iX, iY, iStride, iMX, iMY, iCorrection) + iFirstRowOffset;

		__asm
		{
			push edi
				push esi

				mov esi, pSrc1
				mov ebx, pSrc2
				mov edi, blockRef
				mov eax, strideRef
				mov edx, iStride

				movq mm6, eightOnes
				pxor mm7, mm7

				mov	ecx, 8

lineLoopComplex:
			movq mm1, [esi]			// 8 pixels for current MB
			movq mm3, [esi + 8]		// another 8 pixels in the same row of current MB
			add	esi, edx

				movq mm0, [ebx]
			movq mm2, [ebx + 8]
			add	ebx, edx

				movq mm4, mm1
				movq mm5, mm3

				por mm4, mm0
				por mm5, mm2

				pand mm4, mm6
				pand mm5, mm6

				por mm1, mm6
				por mm3, mm6

				psrlq mm1, 1
				psrlq mm3, 1

				por mm0, mm6
				por mm2, mm6

				psrlq mm0, 1
				psrlq mm2, 1

				pxor mm6, mm6

				paddb mm1, mm0
				paddb mm3, mm2

				paddb mm1, mm4
				paddb mm3, mm5

				movq mm0, [edi]			// 8 pixels for reference MB
			movq mm2, [edi + 8]		// another 8 pixels in the same row of reference MB
			add	edi, eax

				movq mm4, mm1
				movq mm5, mm3

				psubusb mm1, mm0
				psubusb mm3, mm2

				psubusb mm0, mm4
				psubusb mm2, mm5

				por mm0, mm1			// mm0 = |cur - ref|
				por mm2, mm3			// mm2 = |*(cur+8) - *(ref+8)|

				movq mm1, mm0
				movq mm3, mm2

				punpcklbw mm0, mm6
				punpckhbw mm1, mm6

				punpcklbw mm2, mm6
				punpckhbw mm3, mm6

				movq mm6, eightOnes

				paddusw mm0, mm1
				paddusw mm2, mm3		

				paddusw mm7, mm0		// sum += mm01
				paddusw mm7, mm2		// sum += mm23

				sub ecx, 1

				jnz	lineLoopComplex

				pmaddwd mm7, fourOnes	// merge sum

				movq mm0, mm7
				psrlq mm7, 32 

				paddd mm0, mm7
				movd sum, mm0

				emms

				pop 	esi
				pop 	edi
		}
	}
	else {
		__asm
		{
			push edi
				push esi

				mov esi, pSrc1
				mov edi, blockRef
				mov eax, strideRef
				mov edx, iStride

				pxor mm6, mm6
				pxor mm7, mm7

				mov	ecx, 8

lineLoopSimple:
			movq mm1, [esi]			// 8 pixels for current MB
			movq mm3, [esi + 8]		// another 8 pixels in the same row of current MB
			add	esi, edx

				movq mm0, [edi]			// 8 pixels for reference MB
			movq mm2, [edi + 8]		// another 8 pixels in the same row of reference MB
			add	edi, eax

				movq mm4, mm1
				movq mm5, mm3

				psubusb mm1, mm0
				psubusb mm3, mm2

				psubusb mm0, mm4
				psubusb mm2, mm5

				por mm0, mm1			// mm0 = |cur - ref|
				por mm2, mm3			// mm2 = |*(cur+8) - *(ref+8)|

				movq mm1, mm0
				movq mm3, mm2

				punpcklbw mm0, mm6
				punpckhbw mm1, mm6

				punpcklbw mm2, mm6
				punpckhbw mm3, mm6

				paddusw mm0, mm1
				paddusw mm2, mm3		

				paddusw mm7, mm0		// sum += mm01
				paddusw mm7, mm2		// sum += mm23

				sub ecx, 1

				jnz	lineLoopSimple

				pmaddwd mm7, fourOnes	// merge sum

				movq mm0, mm7
				psrlq mm7, 32 

				paddd mm0, mm7
				movd sum, mm0

				emms

				pop 	esi
				pop 	edi
		}
	}

	return sum;
}

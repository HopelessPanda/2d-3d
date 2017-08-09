/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cMEStaff.c
*  \brief File with support functions for motion estimation
*
*  This file contains implementation of the functions needed to
*  perform motion estimation, such as half- and quarter-pixel
*  shifts, error calculations e.t.c.
*
*  $Id: cMEStaff.c,v 1.16.4.6 2008/07/24 18:13:21 10i_tsv Exp $
*  $Log: cMEStaff.c,v $
*  Revision 1.16.4.6  2008/07/24 18:13:21  10i_tsv
*  CVS lo*
*  Revision 1.16.4.4  2008/07/17 10:01:12  10i_tsv
*  CVS log removed
*  @
*  text
*  @d18 40
*  @
*
*
*  1.15
*  log
*  @Doxygen completed. Doxygen warnings are not fixed.
*  @
*  text
*  @d3 1
*  a3 2
*  (c) MSU Video*
*  Revision 1.16.4.3  2008/07/14 06:13:11  10i_tsv
*  CVS log removed
*  @
*  text
*  @d18 40
*  @
*
*
*  1.15
*  log
*  @Doxygen completed. Doxygen warnings are not fixed.*
*  Revision 1.16.4.2  2008/07/12 18:09:05  10i_tsv
*  CVS log removed
*  @
*  text
*  @d18 40
*  @
*
*
*  1.15
*  log
*  @Doxygen completed. Doxygen w*
*  Revision 1.15  2007/06/04 14:40:40  05s_gri
*  Doxygen completed. Doxygen warnings are not fixed.
*
*  Revision 1.14  2007/04/22 20:15:47  05s_put
*  Add/fix doxygen comments
*
*  Revision 1.13  2007/03/05 14:19:07  05s_put
*  - interface refactoring
*  - memory fix
*
*  Revision 1.6  2006/12/18 13:46:12  05s_put
*  - doxygen comments fixed
*
*  Revision 1.5  2006/11/23 10:59:59  05s_put
*  - fixed cSetPrvMVs for 4x4 blocks
*  - made function, that makes possible give ME externally made half-pixel shifts
*  - fixed negative error values
*  - fixed warning with unreferenced parameter
*
*  Revision 1.4  2006/11/23 08:43:27  05s_gri
*  CVS versions conflicts repaired
*
*  Revision 1.2  2006/11/13 10:24:53  05s_put
*  New version of compensation
*
*  Revision 1.4  2006/09/09 13:24:57  09k_sim
*  Began to add codec support to the library.
*  MPEG2 ME is supported.
*
*  Revision 1.3  2006/08/01 12:53:01  09k_sim
*  SaveBMP function added for debug purposes.
*
*  Revision 1.2  2006/07/13 07:51:30  05s_put
*  Fixed error
*
*  Revision 1.4  2006/07/11 16:16:49  05s_put
*  test commit of cvs comments
*
*/

#include "cMEStaff.h"

#include <mmintrin.h>

#pragma warning(disable:4996)

/**
*  \var me_staff_cliping
*  \brief array used for clipping
*
*  This variable is array used for clipping values from interval
*  [-200; 455] to interval [0; 255]
*/
const BYTE me_staff_cliping[200+256+200] =
{
//1 2 3 4 5 6 7 8 91011121314151617181920
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//1
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//2
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//3
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//4
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//5
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//6
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//7
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//8
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//9
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//10

	  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
	 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,

	100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
	120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,
	140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,
	180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,
	
	200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,
	220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,
	
//1 2 3 4 5 6 7 8 91011121314151617181920
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,//1
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,//2
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,//3
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,//4
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,//5
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,//6
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,//7
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,//8
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,//9
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255 //10
};

/**
**************************************************************************
* \brief Clip value to the [0; 255] interval
*
*  This function clips value from interval [-200; 455] into interval [0; 255]
*
* \param iVal                [IN] -  value that is should be clipped
* 
* \return Result of the value clipping
*/
__inline BYTE ClipBYTE(int iVal)
{
	return me_staff_cliping[iVal + 200];
}

/**
**************************************************************************
* \brief Shift filter horizontal core
*
*  This function applies horizontal core of half pixel shift
*
* \param pSrc                [IN] -  pointer to the left pixel during shift
* 
* \return Result of the shift core calculation
*/
__inline int HorShiftHalf(const BYTE* pSrc)
{
	return pSrc[-2] + pSrc[3] - 5 * (pSrc[-1] + pSrc[2]) + 20 * (pSrc[0] + pSrc[1]);
}

/**
**************************************************************************
* \brief Shift filter vertical core
*
*  This function applies vertical core of half pixel shift
*
* \param pSrc                [IN] -  pointer to the upper pixel during shift
* \param iStride             [IN] -  stride of the image
* 
* \return Result of the shift core calculation
*/
__inline int VerShiftHalf(const BYTE* pSrc, const int iStride)
{
	return pSrc[-(iStride<<1)] + pSrc[3*iStride] - 5 * (pSrc[-iStride] + pSrc[iStride<<1]) + 20 * (pSrc[0] + pSrc[iStride]);
}

/**
**************************************************************************
* \brief Makes half-pixel shifted planes
*
*  This function makes half-pixel shifted planes using H.264 shifts
*  formula
*
* \param pSrc                [IN/OUT] - array of pointers to the planes
* \param iFirstRowOffset     [IN] -  first row offset in the planes
* \param iWidth              [IN] -  width of the plane
* \param iHeight             [IN] -  height of the plane
* \param iStride             [IN] -  stride of the plane
* 
* \return none
*/
void MakePlanes(BYTE** pSrc, int iFirstRowOffset, int iWidth, int iHeight, int iStride)
{
	int i, j;
	int iLine[6];

	BYTE* pCurLine0 = pSrc[0] + iFirstRowOffset;
	BYTE* pCurLine1 = pSrc[1] + iFirstRowOffset;
	BYTE* pCurLine2 = pSrc[2] + iFirstRowOffset;
	BYTE* pCurLine3 = pSrc[3] + iFirstRowOffset;

	for (i = 0; i < iHeight; i++)
	{
		iLine[0] = VerShiftHalf(pCurLine0 - 2, iStride);
		iLine[1] = VerShiftHalf(pCurLine0 - 1, iStride);
		iLine[2] = VerShiftHalf(pCurLine0    , iStride);
		iLine[3] = VerShiftHalf(pCurLine0 + 1, iStride);
		iLine[4] = VerShiftHalf(pCurLine0 + 2, iStride);

		for (j = 0; j < iWidth; j++)
		{
			iLine[5] = VerShiftHalf(pCurLine0 + j + 3, iStride);

			pCurLine1[j] = ClipBYTE( (HorShiftHalf(pCurLine0 + j) + 16) >> 5 );
			pCurLine2[j] = ClipBYTE( (iLine[2] + 16) >> 5 );
			pCurLine3[j] = ClipBYTE( (iLine[0] + iLine[5] - 5 * (iLine[1] + iLine[4]) + 20 * (iLine[2] + iLine[3]) + 512) >> 10 );

			iLine[0] = iLine[1];
			iLine[1] = iLine[2];
			iLine[2] = iLine[3];
			iLine[3] = iLine[4];
			iLine[4] = iLine[5];
		}
		pCurLine0 += iStride;
		pCurLine1 += iStride;
		pCurLine2 += iStride;
		pCurLine3 += iStride;
	}
}

/**
**************************************************************************
* \brief Adds borders to the plane
*
*  This function fills borders around the plane with outermost pixel
*  values
*
* \param pPlane              [IN/OUT] - pointer to the plane
* \param iWidth              [IN] -  width of the plane
* \param iHeight             [IN] -  height of the plane
* \param iBorder             [IN] -  width of the border
* 
* \return none
*/
void PlaneAddBorders(BYTE* pPlane, int iWidth, int iHeight, int iBorder)
{
	int i;
	BYTE* pLine;
	BYTE* pSrcLine;
	const int iExtWidth = iWidth + (iBorder << 1);

	// left and right
	pLine = pPlane + iBorder * iExtWidth;
	for (i = 0; i < iHeight; i++)
	{
		memset(pLine, pLine[iBorder], iBorder);
		memset(pLine + iBorder + iWidth, pLine[iBorder + iWidth - 1], iBorder);
		pLine += iExtWidth;
	}

	// up
	pLine = pPlane;
	pSrcLine = pPlane + iBorder * iExtWidth;
	for (i = 0; i < iBorder; i++)
	{
		memcpy(pLine, pSrcLine, iExtWidth);
		pLine += iExtWidth;
	}

	// down
	pLine = pPlane + (iBorder + iHeight) * iExtWidth;
	pSrcLine = pLine - iExtWidth;
	for (i = 0; i < iBorder; i++)
	{
		memcpy(pLine, pSrcLine, iExtWidth);
		pLine += iExtWidth;
	}
}

/**
**************************************************************************
* \brief Performs quarter-pixel shifts and writes data into given memory
*
*  This function calculates linear quarter-pixel shifts if needed and
*  writes data to the given block. Source frame is non-extended previous frame.
*
* \param pBlock              [OUT] - pointer to the block to write data to
* \param iBlockWidth         [IN] -  block width
* \param iBlockHeight        [IN] -  block height
* \param iBlockStride        [IN] -  block stride
* \param pFrame              [IN] -  pointer to the frame with image
* \param iWidth              [IN] -  frame width
* \param iMX                 [IN] -  motion vector horizontal component
* \param iPrecMX             [IN] -  motion vector horizontal precision
* \param iMY                 [IN] -  motion vector vertical component
* \param iPrecMY             [IN] -  motion vector vertical precision
* 
* \return none
*/
void GetBlock(BYTE* pBlock, int iBlockWidth, int iBlockHeight, int iBlockStride, const BYTE* pFrame, int iWidth, int iMX, int iPrecMX, int iMY, int iPrecMY)
{
	int ibMX = iMX >> iPrecMX;
	int ibMY = iMY >> iPrecMY;
	const BYTE *pBlockInFrame = pFrame + ibMY * iWidth + ibMX;

	int iCoefX = (1 << iPrecMX) - (iMX & ((1 << iPrecMX) - 1));
	int iCoefY = (1 << iPrecMY) - (iMY & ((1 << iPrecMY) - 1));

	int iShift = iPrecMX + iPrecMY;

	short sCoef0;
	short sCoef1;
	short sCoef2;
	short sCoef3;

	int iCoef0 = sCoef0 = iCoefX * iCoefY;
	int iCoef1 = sCoef1 = ((1 << iPrecMX) - iCoefX) * iCoefY;
	int iCoef2 = sCoef2 = iCoefX * ((1 << iPrecMY) - iCoefY);
	int iCoef3 = sCoef3 = ((1 << iPrecMX) - iCoefX) * ((1 << iPrecMY) - iCoefY);

	int i, j;

	int mmxWidth = iBlockWidth >> 3;

	__m64 mZero = _mm_setzero_si64();

	__m64 mCoef0 = _mm_set1_pi16(sCoef0);
	__m64 mCoef1 = _mm_set1_pi16(sCoef1);
	__m64 mCoef2 = _mm_set1_pi16(sCoef2);
	__m64 mCoef3 = _mm_set1_pi16(sCoef3);

	__m64 mShift = _mm_cvtsi32_si64(iShift);

	__m64 mSum[2];

	__m64 *mSrcLine = (__m64 *)pBlockInFrame;
	__m64 *mNextLine = (__m64 *)(pBlockInFrame + iWidth);
	__m64 *mDstLine = (__m64 *)pBlock;

	for (i = 0; i < iBlockHeight; i++) {
		for (j = 0; j < mmxWidth; j++)
		{
			mSum[0] = _mm_mullo_pi16(_mm_unpacklo_pi8(mSrcLine[j], mZero), mCoef0);
			mSum[1] = _mm_mullo_pi16(_mm_unpackhi_pi8(mSrcLine[j], mZero), mCoef0);
			mSum[0] = _mm_add_pi16(mSum[0], _mm_mullo_pi16(_mm_unpacklo_pi8(*((__m64 *)((BYTE *)&mSrcLine[j] + 1)), mZero), mCoef1));
			mSum[1] = _mm_add_pi16(mSum[1], _mm_mullo_pi16(_mm_unpackhi_pi8(*((__m64 *)((BYTE *)&mSrcLine[j] + 1)), mZero), mCoef1));
			mSum[0] = _mm_add_pi16(mSum[0], _mm_mullo_pi16(_mm_unpacklo_pi8(mNextLine[j], mZero), mCoef2));
			mSum[1] = _mm_add_pi16(mSum[1], _mm_mullo_pi16(_mm_unpackhi_pi8(mNextLine[j], mZero), mCoef2));
			mSum[0] = _mm_add_pi16(mSum[0], _mm_mullo_pi16(_mm_unpacklo_pi8(*((__m64 *)((BYTE *)&mNextLine[j] + 1)), mZero), mCoef3));
			mSum[1] = _mm_add_pi16(mSum[1], _mm_mullo_pi16(_mm_unpackhi_pi8(*((__m64 *)((BYTE *)&mNextLine[j] + 1)), mZero), mCoef3));
			mDstLine[j] = _mm_packs_pu16(_mm_srl_pi16(mSum[0], mShift), _mm_srl_pi16(mSum[1], mShift));
		}

		for (j = mmxWidth << 3; j < iBlockWidth; j++)
		{
			pBlock[j] = (iCoef0 * pBlockInFrame[j] + iCoef1 * pBlockInFrame[j + 1] +
				iCoef2 * pBlockInFrame[iWidth + j] + iCoef3 * pBlockInFrame[iWidth + j + 1]) >> iShift;
		}

		pBlock += iBlockStride;
		pBlockInFrame += iWidth;

		mSrcLine = (__m64 *)((BYTE *)mSrcLine + iWidth);
		mNextLine = (__m64 *)((BYTE *)mNextLine + iWidth);
		mDstLine = (__m64 *)((BYTE *)mDstLine + iBlockStride);
	}

	_mm_empty();
}

/**
**************************************************************************
* \brief Performs quarter-pixel shifts and writes data into given memory
*
*  This function calculates quarter-pixel shifts based on H.264 standard
*  if needed and writes data to the given block
*
* \param pBlock              [OUT] - pointer to the block to write data to
* \param iBlockWidth         [IN] -  block width
* \param iBlockHeight        [IN] -  block height
* \param iBlockStride        [IN] -  block stride
* \param pSrcShifts          [IN] -  array of pointers to the planes with half-pel shifts
* \param iWidth              [IN] -  planes' width
* \param iFirstRowOffset     [IN] -  offset of the first row in plane
* \param iX                  [IN] -  X coordinate of the current block
* \param iY                  [IN] -  Y coordinate of the current block
* \param iMX                 [IN] -  motion vector horizontal component
* \param iMY                 [IN] -  motion vector vertical component
* 
* \return none
*/
void GetBlockH264(BYTE* pBlock, int iBlockWidth, int iBlockHeight, int iBlockStride,
									BYTE** pSrcShifts, int iWidth, int iFirstRowOffset, int iX, int iY, int iMX, int iMY)
{
	BYTE* pSrc1;
	int iCorrection = H264_CORRECTION(iMX, iMY);

	pSrc1 = pSrcShifts[H264_SRC1_INDEX(iMX, iMY, iCorrection)] +
		H264_SRC1_OFFSET(iX, iY, iWidth, iMX, iMY, iCorrection) + iFirstRowOffset;

	if (H264_BLEND(iMX, iMY))
	{
		BYTE* pSrc2 = pSrcShifts[H264_SRC2_INDEX(iMX, iMY, iCorrection)] +
			H264_SRC2_OFFSET(iX, iY, iWidth, iMX, iMY, iCorrection) + iFirstRowOffset;
		
		int i, j;
		for (i = 0; i < iBlockHeight; i++)
		{
			for (j = 0; j < iBlockWidth; j++)
			{
				pBlock[j] = (pSrc1[j] + pSrc2[j] + 1) >> 1;
			}
			pBlock += iBlockStride;
			pSrc1 += iWidth;
			pSrc2 += iWidth;
		}
	}
	else
	{
		int i;
		for (i = 0; i < iBlockHeight; i++)
		{
			memcpy(pBlock, pSrc1, iBlockWidth);
			pBlock += iBlockStride;
			pSrc1 += iWidth;
		}
	}
}

/**
**************************************************************************
* \brief Saves given RGB picture in BMP format. For debug purposes.
*
*/
void SaveBMP(char* filename,BYTE* r,BYTE* g,BYTE* b,int iWidth,int iHeight)
{
	//char st[100] = {0};
	//char st1[10] = {0};
	FILE* out_filef = fopen(filename, "wb");
	long l;
	int i;
	fputc('B',out_filef);
	fputc('M',out_filef);
	l = iWidth * iHeight * 3 + 54;
	fwrite(&l, 4, 1, out_filef);		
	i = 0;
	fwrite(&i, 2, 1, out_filef);		
	fwrite(&i, 2, 1, out_filef);			
	l = 54;
	fwrite(&l, 4, 1, out_filef);		
	l = 40;
	fwrite(&l, 4, 1, out_filef);		
	fwrite(&iWidth, 4, 1, out_filef);
	fwrite(&iHeight, 4, 1, out_filef);	
	i = 1;
	fwrite(&i, 2, 1, out_filef);			
	i = 24;
	fwrite(&i, 2, 1, out_filef);		
	l = 0;
	fwrite(&l, 4, 1, out_filef);			
	l = iWidth * iHeight  * 3;
	fwrite(&l, 4, 1, out_filef);			
	l = 0;
	fwrite(&l, 4, 1, out_filef);			
	fwrite(&l, 4, 1,out_filef);		
	fwrite(&l, 4, 1, out_filef);			
	fwrite(&l, 4, 1, out_filef);
	
	for(i=0;i<iWidth*iHeight;i++)
		fprintf(out_filef,"%c%c%c",b[i],g[i],r[i]);
	fclose(out_filef);
}

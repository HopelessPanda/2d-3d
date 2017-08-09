/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cMEStaff.h
*  \brief File with support functions for motion estimation
*
*  This file contains difinitions of the functions needed to
*  perform motion estimation, such as half- and quarter-pixel
*  shifts, error calculations e.t.c.
*
*  $Id: cMEStaff.h,v 1.16.10.6 2008/07/24 18:04:27 10i_tsv Exp $
*  $Log: cMEStaff.h,v $
*  Revision 1.16.10.6  2008/07/24 18:04:27  10i_tsv
*  UVsoft headers are added in some files
*  @
*  text
*  @d10 56
*  @
*
*
*  1.16.*
*  Revision 1.16.10.4  2008/07/17 10:01:12  10i_tsv
*  UVsoft headers are added in some files
*  @
*  text
*  @d10 56
*  @
*
*
*  1.16.6.2
*  log
*  @YUVsoft header is added
*  @
*  text
*  @a9 9
*  /*
*  ****************
*  Revision 1.16.10.3  2008/07/14 06:13:11  10i_tsv
*  UVsoft headers are added in some files
*  @
*  text
*  @d10 56
*  @
*
*
*  1.16.6.2
*  log
*  @YUVsoft header is added*
*  Revision 1.16.10.2  2008/07/12 18:09:05  10i_tsv
*  UVsoft headers are added in some files
*  @
*  text
*  @d10 56
*  @
*
*
*  1.16.6.2
*  log
*  @Y*
*  Revision 1.16  2007/06/28 08:22:46  05s_gri
*  Headers are changed on YUVsoft headers
*
*  Revision 1.15  2007/06/04 14:40:40  05s_gri
*  Doxygen completed. Doxygen warnings are not fixed.
*
*  Revision 1.14  2007/04/22 20:15:47  05s_put
*  Add/fix doxygen comments
*
*  Revision 1.13  2007/04/10 17:22:23  09k_sim
*  Cache size added
*
*  Revision 1.12  2007/03/05 14:19:07  05s_put
*  - interface refactoring
*  - memory fix
*
*  Revision 1.4  2006/11/13 10:33:25  05s_put
*  fixing path to CommonTypes.h
*
*  Revision 1.3  2006/11/13 10:24:53  05s_put
*  New version of compensation
*
*  Revision 1.8  2006/10/25 21:43:42  09k_sim
*  Refactoring & bug fixing after MPEG-2 changes.
*
*  Revision 1.7  2006/09/22 10:48:21  09k_sim
*  Better multiframe support was added.
*
*  Revision 1.6  2006/09/09 14:52:08  09k_sim
*  Paths bug fixed.
*
*  Revision 1.5  2006/09/09 13:24:57  09k_sim
*  Began to add codec support to the library.
*  MPEG2 ME is supported.
*
*  Revision 1.4  2006/08/01 12:53:01  09k_sim
*  SaveBMP function added for debug purposes.
*
*  Revision 1.3  2006/07/19 23:59:59  09k_sim
*  Square macro added
*
*  Revision 1.2  2006/07/13 07:51:30  05s_put
*  Fixed error
*
*  Revision 1.3  2006/07/11 16:16:49  05s_put
*  test commit of cvs comments
*
*/

#ifndef __CMESTAFF_H__
#define __CMESTAFF_H__

#include <memory.h>
#include <stdio.h>
#include "../../VideoGMLib/Common/CommonTypes.h"

/**
**************************************************************************
* \brief Square macro
*
*/
#define sqr(x) ((x)*(x))

/**
**************************************************************************
* \brief The macro increases block ID and clears block ID's cache, if necessary
*
*/
#define NEW_BLOCK_ID																				                                        \
	pMEStruct->m_curBlockID++;																		                                        \
	if(!pMEStruct->m_curBlockID) /*clear blockID cache*/											                                        \
	{																								                                        \
        memset(pMEStruct->m_blockIDs, 0, sizeof(DWORD) * (2 * pMEStruct->m_iCacheSizeHor + 1) * (2 * pMEStruct->m_iCacheSizeVert + 1));		\
		pMEStruct->m_curBlockID = 1;																	                                        \
	}	


/**
**************************************************************************
* \brief Makes half-pixel shifted planes
*
*/
void MakePlanes(BYTE** pSrc, int iFirstRowOffset, int iWidth, int iHeight, int iStride);

/**
**************************************************************************
* \brief Adds borders to the plane
*
*/
void PlaneAddBorders(BYTE* pPlane, int iWidth, int iHeight, int iBorder);

/*! \def H264_BLEND(mvX, mvY)
*   \brief Quarter-pixel interpolation condition
*
* \param mvX      [IN] -  X coordinate of corresponding MV
* \param mvY      [IN] -  Y coordinate of corresponding MV
* 
*/
#define H264_BLEND(mvX, mvY) (((mvX) | (mvY)) & 1)

/*! \def H264_CORRECTION(mvX, mvY)
*   \brief This value is 1 when we should interpolate between right up and left down pixels
*
* \param mvX      [IN] -  X coordinate of corresponding MV
* \param mvY      [IN] -  Y coordinate of corresponding MV
* 
*/
#define H264_CORRECTION(mvX, mvY) (((mvX) & 1) & ((mvY) & 1) & ( (~((mvX) ^ (mvY)) & 2) >> 1 ))

/*! \def H264_SRC1_INDEX(mvX, mvY, correction)
*   \brief Index of the first half-pixel shifted plane
*
* \param mvX        [IN] -  X coordinate of corresponding MV
* \param mvY        [IN] -  Y coordinate of corresponding MV
* \param correction [IN] -  correction (see H264_CORRECTION)
* 
*/
#define H264_SRC1_INDEX(mvX, mvY, correction) ((((mvY) + (correction)) & 2) + (((mvX) & 2) >> 1))

/*! \def H264_SRC2_INDEX(mvX, mvY, correction)
*   \brief Index of the second half-pixel shifted plane
*
* \param mvX        [IN] -  X coordinate of corresponding MV
* \param mvY        [IN] -  Y coordinate of corresponding MV
* \param correction [IN] -  correction (see H264_CORRECTION)
* 
*/
#define H264_SRC2_INDEX(mvX, mvY, correction) ((((mvY) + 1 - (correction)) & 2) + ((((mvX) + 1) & 2) >> 1))

/*! \def H264_SRC1_OFFSET(x, y, stride, mvX, mvY, correction)
*   \brief Offset of the block if the first half-pixel shifted plane
*
* \param x          [IN] -  x coordinate of block
* \param y          [IN] -  y coordinate of block
* \param stride     [IN] -  plane stride
* \param mvX        [IN] -  X coordinate of corresponding MV
* \param mvY        [IN] -  Y coordinate of corresponding MV
* \param correction [IN] -  correction (see H264_CORRECTION)
* 
*/
#define H264_SRC1_OFFSET(x, y, stride, mvX, mvY, correction) ((y) + (((mvY) + (correction)) >> 2)) * (stride) + (x) + ((mvX) >> 2)

/*! \def H264_SRC2_OFFSET(x, y, stride, mvX, mvY, correction)
*   \brief Offset of the block if the second half-pixel shifted plane
*
* \param x          [IN] -  x coordinate of block
* \param y          [IN] -  y coordinate of block
* \param stride     [IN] -  plane stride
* \param mvX        [IN] -  X coordinate of corresponding MV
* \param mvY        [IN] -  Y coordinate of corresponding MV
* \param correction [IN] -  correction (see H264_CORRECTION)
* 
*/
#define H264_SRC2_OFFSET(x, y, stride, mvX, mvY, correction) ((y) + (((mvY) + 1 - (correction)) >> 2)) * (stride) + (x) + (((mvX) + 1) >> 2)

/**
**************************************************************************
* \brief Performs quarter-pixel shifts and writes data into given memory
*
*/
void GetBlock(BYTE* pBlock, int iBlockWidth, int iBlockHeight, int iBlockStride, const BYTE* pFrame, int iWidth, int iMX, int iPrecMX, int iMY, int iPrecMY);

/**
**************************************************************************
* \brief Performs quarter-pixel shifts and writes data into given memory
*
*/
void GetBlockH264(BYTE* pBlock, int iBlockWidth, int iBlockHeight, int iBlockStride,
                  BYTE** pSrcShiftsY, int iWidth, int iFirstRowOffset, int iX, int iY, int iMX, int iMY);

/**
**************************************************************************
* \brief Saves given RGB picture in BMP format. For debug purposes.
*
*/
void SaveBMP(char* filename,BYTE* r,BYTE* g,BYTE* b,int iWidth,int iHeight);

#endif // __CMESTAFF_H__

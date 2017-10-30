/*
*********************************************************************
  (c) YUVsoft Corp., 2010  www.yuvsoft.com
  This source code is a property of YUVsoft Corp.

  This code can be distributed and used ONLY under a proper licensing
  agreement with YUVsoft
*********************************************************************
*/

/**
*  \file VideoImage.h
*  \brief VideoFrames processing
*
*  This file contains definitions of auxiliary functions needed for
*  video frames processing
*
*/
#ifndef __GMC_VIDEOIMAGE_H__
#define __GMC_VIDEOIMAGE_H__

#include "../../VideoGMLib/Common/CommonTypes.h"
#include <memory.h>
#include <malloc.h>

/**
**************************************************************************
* \brief Returns BYTE count of the color space
*
*/
int gmcVideoImage_ByteCount(gmcColorSpace type);

/**
**************************************************************************
* \brief Expands image to make its sizes multiple of 16
*
*/
void gmcVideoImage_Expand(BYTE* src, BYTE* dst, int oW, int oH, int W, int H, gmcColorSpace Type);

/**
**************************************************************************
* \brief Restores image to the original size
*
*/
void gmcVideoImage_WriteOriginal(BYTE* src, BYTE* dst, int oW, int oH, int W, int H, gmcColorSpace Type);

/**
**************************************************************************
* \brief Converting image from YV12 to YUV
*
*/
void gmcVideoImage_YV12ToYUVConvert(BYTE* dst, const BYTE* src, int W, int H);

/**
**************************************************************************
* \brief Converting image from YUV to YV12
*
*/
void gmcVideoImage_YUVToYV12Convert(BYTE* dst, const BYTE* src, int W, int H);

/**
**************************************************************************
* \brief Converts YUV to RGB24
*
*/
void gmcVideoImage_YUVtoRGBConvert(const BYTE *Y, const BYTE *U, const BYTE *V, BYTE *dst, int width, int height, int src_pitch, int dst_pitch);

/**
**************************************************************************
* \brief Converts RGB24 to YCbYCr
*
*/
void gmcVideoImage_RGB24toYCbCrConvert(const BYTE *src, BYTE *Y, BYTE *Cb, BYTE *Cr, int width, int height, int src_pitch, int dst_pitch);

/**
**************************************************************************
* \brief Converts RGB24 to Yrg
*
*/
void gmcVideoImage_RGB24toYrgConvert(const BYTE *src, BYTE *Y, BYTE *r, BYTE *g, int width, int height, int src_pitch, int dst_pitch);

/**
**************************************************************************
* \brief Converts RGB24 to Yrg
*
*  The function converts frame from RGB24 format into YUV format, 
*
*/
void gmcVideoImage_RGB24toYUVConvert(const BYTE *src, BYTE *Y, BYTE *U, BYTE *V, int width, int height, int src_pitch, int dst_pitch);

/**
**************************************************************************
* \brief Image Reverting
*
*/
void gmcImageRevert(BYTE* img, int h, int pitch, int planes);

/**
**************************************************************************
* \brief Add borders to frame of Y type
*
*/
void cAddBlackBordersY(BYTE* dst, BYTE* src, int srcW, int srcH, int borderL, int borderT, int borderR, int borderB);

/**
**************************************************************************
* \brief Removes borders from YUV image
*
*/
void cDelBordersY( BYTE * dst, const BYTE * src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB );

/**
**************************************************************************
* \brief Removes borders from YUV image
*
*/
void cDelBordersYUV( BYTE * dst, const BYTE* src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB );

/**
**************************************************************************
* \brief Add borders to frame
*
*/
void cAddBlackBorders( BYTE* dst, const BYTE* src, int srcW, int srcH, int borderL, int borderT, int borderR, int borderB, int planes );

/**
**************************************************************************
* \brief Add repeated borders for Y image
*/
int cAddRepeatedBordersY( BYTE * dst, BYTE * src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB );

/**
**************************************************************************
* \brief Add repeating borders for YUV-frame
*/
int cAddRepeatedBordersYUV( BYTE * dst, BYTE * src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB );

/**
**************************************************************************
* \brief Fill mirrored borders for Y image
*/
int cFillMirroredBordersY( BYTE * image, int wExt, int hExt, int borderL, int borderT, int borderR, int borderB );

/**
**************************************************************************
* \brief Add mirrored borders for Y-image
*/
int cAddMirroredBordersY( BYTE * dst, BYTE * src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB );

/**
**************************************************************************
* \brief Copy Y image per line.
*/
int gmcVideoImage_CopyImageY( BYTE * dst, BYTE * src, int w, int h, int pitchDst, int pitchSrc );
/**
**************************************************************************
* \brief Free memory for image represented as BYTE array
*/
BYTE * gmcVideoImage_Allocate( const int w, const int h, const int numPlanes );

/**
**************************************************************************
* \brief Free memory for image represented as BYTE array
*/
void gmcVideoImage_Free( BYTE * image );


/**
**************************************************************************
* \brief Fill plane of image with specified value
*  
*/
int gmcVideoImage_FillPlane( BYTE * image, const int w, const int h, const int indexPlane, BYTE value );
/**
**************************************************************************
* \brief Fill image of image with specified value
*
*/
int gmcVideoImage_Fill( BYTE * image, const int w, const int h, const int numPlanes, BYTE value );


/**
**************************************************************************
* \brief Fill repeated borders for Y image
*
*/
int cFillRepeatedBordersY( BYTE * image, int wExt, int hExt, int borderL, int borderT, int borderR, int borderB );



#endif //__GMC_VIDEOIMAGE_H__

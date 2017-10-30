/*
********************************************************************
(c) MSU Video Group 2004-2005, http://compression.ru/video/
File is from "Graphics&Media Lab Common Source Library".

Redistributions of source code must retain the above copyright
notice and license file (see license.txt).

This source code is provided under terms of GMLib license.
********************************************************************
*/

/**
*  \file Convert.h
*  \brief Functions for color spaces conversions
*
*  This file contains declaration of functions that provide
*  conversion between different color spaces.
*
*  $Id: Convert.h,v 1.36 2008/10/28 12:42:35 05s_put Exp $
*  $Log: Convert.h,v $
*  Revision 1.36  2008/10/28 12:42:35  05s_put
*  - added YUV -> YV16 conversion
*  - added GetImageType funtion to gmVideoWriter
*  - made watermarking console to operate with YUV files of I420 (YV12) and YV16 formats
*
*  Revision 1.35  2008/10/27 17:31:36  05s_put
*  - made multi-bit 4:2:2 yuv, 4:4:4 yuv and 4:4:4 rgb support
*
*  Revision 1.34  2008/10/23 11:59:37  05s_put
*  - made 4:2:0 raw planar 10, 14 and 16 bit yuv support in VQMT
*
*  Revision 1.33  2008/10/22 14:22:33  05s_put
*  - made 10 bit 4:2:0 work
*
*  Revision 1.32  2008/07/17 18:47:09  05s_gri
*  avi (RGB24) to YUV422 (YV16) conversion implemented
*
*  Revision 1.31  2007/09/10 09:09:53  05s_gri
*  Solution is niw compilable. Processing speed time calculation implemented.
*
*  Revision 1.30  2007/05/03 18:18:34  07o_pet
*  Added Y416 support.
*
*  Revision 1.29  2007/05/02 18:41:08  07o_pet
*  Added Y410 processing.
*
*  Revision 1.28  2006/11/03 19:51:07  05s_put
*  Moved Common/CommonTypes.h to the directory Common
*
*  Revision 1.27  2006/10/30 21:59:07  09k_sim
*  SSE was turned on
*
*  Revision 1.26  2006/10/13 14:31:28  05s_put
*  Another fix
*
*  Revision 1.25  2006/09/12 08:23:38  05m_mah
*  RGB24 -> YCbCr Conversion added, Y calculation taked from RGB24 -> YUV
*
*  Revision 1.24  2006/05/31 12:01:46  07o_pet
*  created project configurations to control SSE usage
*
*  Revision 1.23  2006/05/31 08:46:01  07o_pet
*  DetectSSE is moved to public
*
*  Revision 1.22  2006/05/15 16:13:15  05m_mah
*  doxygen comments added
*
*  Revision 1.21  2006/05/15 13:00:00  05m_mah
*  commit for Quartics
*
*  Revision 1.20  2006/05/11 14:11:18  05s_put
*  RGB, YV16 and lsYUVpl added
*
*  Revision 1.7  2006/05/11 05:32:48  05s_put
*  Made almost everything for codec
*
*  Revision 1.6  2006/04/21 16:50:47  05s_put
*  Finished codec support!
*
*  Revision 1.5  2006/04/20 06:16:22  05s_put
*  Still fighting with YV16.
*
*  Revision 1.4  2006/04/19 20:31:00  05s_put
*  Last version of gmVideoImage was taken
*
*  Revision 1.18  2006/04/18 20:35:17  07o_pet
*  Added more color space conversion routines, mostly from different YUV types to YUV
*
*  Revision 1.17  2006/03/24 14:06:48  07o_pet
*  added YV12 to YUV and YUV2 to YUV conversion
*
*  Revision 1.16  2005/07/04 08:18:32  05s_put
*  Licenses fixed
*
*  Revision 1.15  2005/07/03 14:53:40  05m_mah
*  doxygen warnings fixed
*
*  Revision 1.14  2005/07/03 13:16:33  05m_mah
*  Permissions header added
*
*  Revision 1.13  2005/07/03 12:57:56  05m_mah
*  Doxygen comments added
*
*  Revision 1.12  2005/04/29 13:46:48  05s_put
*  Now we can use Intel Compiler. So MMX/SSE are enabled.
*
*  Revision 1.11  2005/04/27 16:38:59  05s_put
*  VS 6.0 compilation bug fixed
*
*  Revision 1.10  2005/04/27 15:52:48  stranger
*  Fixed bug with compiling MMX in VS 6.0 (MMX/SSE are disabled by define)
*
*  Revision 1.9  2005/04/15 17:14:33  05m_mah
*  const declaration added
*
*  Revision 1.8  2005/04/12 09:46:54  stranger
*  ebx is not used for now. No warning appears either in VS 6.0.
*
*  Revision 1.7  2005/04/08 10:25:44  stranger
*  ResetSSE added
*
*  Revision 1.6  2005/04/08 10:17:30  stranger
*  SSE detection is hidden from user. Conversion functions are static members inside the class gmColorSpace
*
*  Revision 1.5  2005/04/06 20:40:44  stranger
*  Implemented all functions in SSE alone with C implementations. Added doxygen comments for all functions.
*
*
*/

#pragma once

#ifndef _CONVERT_H_
#define _CONVERT_H_


#include "../Common/CommonTypes.h"


//#include "filter.h"

// To enable SSE extensions, choose appropriate project configuration
// SSE extensoions are controlled by defining the flag gmSSE_COMPILER_SUPPORT
#ifdef gmSSE_COMPILER_SUPPORT
#undef gmSSE_COMPILER_SUPPORT
#endif

typedef unsigned char byte;

#define gmCNVT_MMX_FEATURE_BIT    0x0001
#define gmCNVT_SSE_FEATURE_BIT    0x0002
#define gmCNVT_SSE2_FEATURE_BIT   0x0004

#define gmCNVT_ALIGNMENT 16
#define gmCNVT_ALIGN __declspec(align(gmCNVT_ALIGNMENT))

#ifndef gmSSE_COMPILER_SUPPORT

#define RGBAtoYUV  RGBAtoYUV_c
#define YUVtoRGBA  YUVtoRGBA_c
#define RGBtoYUV   RGBtoYUV_c
#define RGBtoYCbCr RGBtoYCbCr_c
#define RGBtoYrg   RGBtoYrg_c
#define YUVtoRGB   YUVtoRGB_c
#define RGBtoYUY2  RGBtoYUY2_c
#define YUY2toRGB  YUY2toRGB_c
#define RGBtoYV12  RGBtoYV12_c
#define YV12toRGB  YV12toRGB_c
#define RGBAtoY    RGBAtoY_c
#define YtoRGBA    YtoRGBA_c
#define RGBAtoRGB  RGBAtoRGB_c
#define RGBtoRGBA  RGBtoRGBA_c
#define YV16toYUY2 YV16toYUY2_c
#define YUY2toYV16 YUY2toYV16_c
#define RGBtoRGB24 RGBtoRGB24_c
#define RGB24toRGB RGB24toRGB_c
#define lsYUVpltoRGB24 lsYUVpltoRGB24_c
#define RGB24tolsYUVpl RGB24tolsYUVpl_c

#endif

/*!\brief Class with functions for color spaces conversions
*
*  This class contains functions for color spaces conversions
*/
class gmColorSpace
{
public:

    static bool DisableSSE();                   //!< Disables SSE functions
    static void ResetSSE();                     //!< Reset knowledge about SSE support

#ifdef gmSSE_COMPILER_SUPPORT

    //RGB32->YUV
    static void RGBAtoYUV(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch);  //!< Converts RGB32 to YUV
    //YUV->RGB32
    static void YUVtoRGBA(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch);  //!< Converts YUV to RGB32
    //RGB24->YUV
    static void RGBtoYUV(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch);   //!< Converts RGB24 to YUV
    //RGB24->YCbCr
    static void RGBtoYCbCr(const byte *src, byte *Y, byte *Cb, byte *Cr, int width, int height, int src_pitch, int dst_pitch);   //!< Converts RGB24 to YCbCr
    //RGB24->Yrg
    static void RGBtoYrg(const byte *src, byte *Y, byte *r, byte *g, int width, int height, int src_pitch, int dst_pitch);   //!< Converts RGB24 to Yrg
    //YUV->RGB24
    static void YUVtoRGB(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch);   //!< Converts YUV to RGB24
    //RGB24->YUY2
    static void RGBtoYUY2(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                  //!< Converts RGB24 to YUY2
    //RGB24->YUY2
    static void RGBtoYV16(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch);                  //!< Converts RGB24 to YV16
    //YUY2->RGB24
    static void YUY2toRGB(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                  //!< Converts YUY2 to RGB24
    //RGB24->YV12
    static void RGBtoYV12(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch_Y, int dst_pitch_U);   //!< Converts RGB24 to YV12
    //YV12->RGB24
    static void YV12toRGB(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch_Y, int src_pitch_U, int dst_pitch);   //!< Converts YV12 to RGB24
    //RGB32->Y
    static void RGBAtoY(const byte *src, byte *Y, int width, int height, int src_pitch, int dst_pitch);                      //!< Converts RGB32 to Y
    //Y->RGB32
    static void YtoRGBA(const byte *Y, byte *dst, int width, int height, int src_pitch, int dst_pitch);                      //!< Converts Y to RGB32
    //RGB32->RGB24
    static void RGBAtoRGB(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                  //!< Converts RGB32 to RGB24
    //RGB24->RGB32
    static void RGBtoRGBA(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                  //!< Converts RGB24 to RGB32
    //YV16->YUY2
    static void YV16toYUY2(const byte *sy, const byte *su, const byte *sv, byte *dst, int width, int height, int src_pitch, int dst_pitch);     //!< Converts YV16 to YUY2
	//YV16->YUV
	static void YV16toYUV_c(const byte *Y, const byte *U, const byte *V, byte *dY, byte *dU, byte *dV, int width, int height, int src_pitch_Y, int src_pitch_U, int dst_pitch_Y);     //!< Converts YV16 to YUV
    //YUV->YV16
    static void YUVtoYV16_c(const byte *sy, const byte *su, const byte *sv, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch);
    //YUY2->YV16
    static void YUY2toYV16(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch);                 //!< Converts YUY2 to YV16
    //RGB->RGB24
    static void RGBtoRGB24(const byte *sr, const byte *sg, const byte *sb, byte *dst, int width, int height, int src_pitch, int dst_pitch);     //!< Converts RGB to RGB24
		//RGB24->RGB
    static void RGB24toRGB(const byte *src, byte *dr, byte *dg, byte *db, int width, int height, int src_pitch, int dst_pitch);                 //!< Converts RGB24 to RGB
    //RGB->RGB24
    static void lsYUVpltoRGB24(const byte *sy, const byte *su, const byte *sv, byte *dst, int width, int height, int src_pitch, int dst_pitch); //!< Converts RGB to RGB24
		//RGB24->RGB
    static void RGB24tolsYUVpl(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch);             //!< Converts RGB24 to RGB

#endif

    //RGB32->YUV
    static void RGBAtoYUV_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch);    //!< Converts RGB32 to YUV

    //YUV->RGB32
    static void YUVtoRGBA_c(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch);    //!< Converts YUV to RGB32

    //RGB24->YUV
    static void RGBtoYUV_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch);     //!< Converts RGB24 to YUV

    //RGB24->YCrCb
    static void RGBtoYCbCr_c(const byte *src, byte *Y, byte *Cb, byte *Cr, int width, int height, int src_pitch, int dst_pitch); //!< Converts RGB24 to YCrCb

    //RGB24->Yrg
    static void RGBtoYrg_c(const byte *src, byte *Y, byte *r, byte *g, int width, int height, int src_pitch, int dst_pitch); //!< Converts RGB24 to Yrg

    //YUV->RGB24
    static void YUVtoRGB_c(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch);     //!< Converts YUV to RGB24

    //RGB24->YUY2
    static void RGBtoYUY2_c(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                    //!< Converts RGB24 to YUY2

	//RGB24->YUY2
	static void RGBtoYV16_c(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch);                    //!< Converts RGB24 to YUY2

	//YUY2->RGB24
    static void YUY2toRGB_c(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                    //!< Converts YUY2 to RGB24

	//YV12->YUV
	static void YV12toYUV_c(const byte *Y, const byte *U, const byte *V, byte *dY, byte *dU, byte *dV, int width, int height, int src_pitch_Y, int src_pitch_U, int dst_pitch_Y); //!< Converts YV12 to YUV

	//Y410->YUV
	static void Y410toYUV_c(const byte *src, byte *y, byte *u, byte *v, int width, int height, int dstPitchY);

	//Y416->YUV
	static void Y416toYUV_c(const byte *src, byte *y, byte *u, byte *v, int width, int height, int dstPitchY);

    //P01X->YVU where X could be '0', '4' or '6'
    static void P01XtoYUV_c(const byte *src, byte *y, byte *u, byte *v, int width, int height, int dstPitchY, int bitCount);

    //P01X->YVU where X could be '0', '4' or '6'
    static void P21XtoYUV_c(const byte *src, byte *y, byte *u, byte *v, int width, int height, int dstPitchY, int bitCount);

    //P41X->YVU where X could be '0', '4' or '6'
    static void P41XtoYUVRGB_c(const byte *src, byte *y, byte *u, byte *v, int width, int height, int dstPitchY, int bitCount);

    //YUY2->YUV
	static void YUY2toYUV_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch); //!< Converts YUY2 to YUV 

	//UYVY->YUV
	static void UYVYtoYUV_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch); //!< Converts UYVY to YUV

	//Y->YUV
	static void YtoYUV_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch);    //!< Converts Y to YUV

    //RGB24->YV12
    static void RGBtoYV12_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch_Y, int dst_pitch_U);     //!< Converts RGB24 to YV12

    //YV12->RGB24
    static void YV12toRGB_c(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch_Y, int src_pitch_U, int dst_pitch);     //!< Converts YV12 to RGB24

    //RGB32->Y
    static void RGBAtoY_c(const byte *src, byte *Y, int width, int height, int src_pitch, int dst_pitch);                        //!< Converts RGB32 to Y


    //Y->RGB32
    static void YtoRGBA_c(const byte *Y, byte *dst, int width, int height, int src_pitch, int dst_pitch);                        //!< Converts Y to RGB32

    //RGB32->RGB24
    static void RGBAtoRGB_c(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                    //!< Converts RGB32 to RGB24

    //RGB24->RGB32
    static void RGBtoRGBA_c(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                    //!< Converts RGB24 to RGB32

	//YV16->YUY2
	static void YV16toYUY2_c(const byte *sy, const byte *su, const byte *sv, byte *dst, int width, int height, int src_pitch, int dst_pitch);   //!< Converts YV16 to YUY2

	//YUY2->YV16
	static void YUY2toYV16_c(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch);               //!< Converts YUY2 to YV16

	//RGB24->RGB
	static void RGB24toRGB_c(const byte *src, byte *dr, byte *dg, byte *db, int width, int height, int src_pitch, int dst_pitch);               //!< Converts RGB24 to RGB

	//RGB->RGB24
	static void RGBtoRGB24_c(const byte *sr, const byte *sg, const byte *sb, byte *dst, int width, int height, int src_pitch, int dst_pitch);   //!< Converts RGB to RGB24

	//RGB24->RGB
	static void RGB24tolsYUVpl_c(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch);               //!< Converts RGB24 to lsYUVpl

	//RGB->RGB24
	static void lsYUVpltoRGB24_c(const byte *sr, const byte *sg, const byte *sb, byte *dst, int width, int height, int src_pitch, int dst_pitch);   //!< Converts lsYUVpl to RGB24

#ifdef gmSSE_COMPILER_SUPPORT

public:
	static bool DetectSSE();                       //!< Detects if system supports SSE instructions
protected:

	static int m_bSSEEnabled;                //!< Tells if it is safe to use SSE functions ("1"-Safe, "0"-Not safe, "-1"-Test needed)

    //RGB32->YUV
    static void RGBAtoYUV_SSE(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch);  //!< Converts RGB32 to YUV

    //YUV->RGB32
    static void YUVtoRGBA_SSE(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch);  //!< Converts YUV to RGB32

    //RGB24->YUV
    static void RGBtoYUV_SSE(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch);   //!< Converts RGB24 to YUV

    //YUV->RGB24
    static void YUVtoRGB_SSE(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch);   //!< Converts YUV to RGB24

    //RGB24->YUY2
    static void RGBtoYUY2_SSE(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                  //!< Converts RGB24 to YUY2

    //YUY2->RGB24
    static void YUY2toRGB_SSE(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                  //!< Converts YUY2 to RGB24

    //RGB24->YV12
    static void RGBtoYV12_SSE(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch_Y, int dst_pitch_U);   //!< Converts RGB24 to YV12

    //YV12->RGB24
    static void YV12toRGB_SSE(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch_Y, int src_pitch_U, int dst_pitch);   //!< Converts YV12 to RGB24

    //RGB32->Y
    static void RGBAtoY_SSE(const byte *src, byte *Y, int width, int height, int src_pitch, int dst_pitch);                      //!< Converts RGB32 to Y

    //Y->RGB32
    static void YtoRGBA_SSE(const byte *Y, byte *dst, int width, int height, int src_pitch, int dst_pitch);                      //!< Converts Y to RGB32

    //RGB32->RGB24
    static void RGBAtoRGB_SSE(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                  //!< Converts RGB32 to RGB24

    //RGB24->RGB32
    static void RGBtoRGBA_SSE(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch);                  //!< Converts RGB24 to RGB32

#endif

};

#endif

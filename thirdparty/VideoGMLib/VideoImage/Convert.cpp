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
*  \file Convert.cpp
*  \brief Functions for color spaces conversions
*
*  This file contains implementation of functions that provide
*  conversion operations between different color spaces.
*
*  $Id: Convert.cpp,v 1.37 2008/10/28 14:29:11 05s_put Exp $
*  $Log: Convert.cpp,v $
*  Revision 1.37  2008/10/28 14:29:11  05s_put
*  - fixed YUV -> YV16 conversion
*
*  Revision 1.36  2008/10/28 12:42:35  05s_put
*  - added YUV -> YV16 conversion
*  - added GetImageType funtion to gmVideoWriter
*  - made watermarking console to operate with YUV files of I420 (YV12) and YV16 formats
*
*  Revision 1.35  2008/10/27 17:31:36  05s_put
*  - made multi-bit 4:2:2 yuv, 4:4:4 yuv and 4:4:4 rgb support
*
*  Revision 1.34  2008/10/23 11:59:36  05s_put
*  - made 4:2:0 raw planar 10, 14 and 16 bit yuv support in VQMT
*
*  Revision 1.33  2008/10/22 14:22:33  05s_put
*  - made 10 bit 4:2:0 work
*
*  Revision 1.32  2008/10/07 08:49:38  05s_put
*  - added addition avs support
*  - changed functions of getting the file information a bit (so that info would not contain the file name)
*
*  Revision 1.31  2008/07/17 18:47:08  05s_gri
*  avi (RGB24) to YUV422 (YV16) conversion implemented
*
*  Revision 1.30  2007/06/12 09:21:08  05s_put
*  - warning fixes
*
*  Revision 1.29  2007/05/03 18:18:34  07o_pet
*  Added Y416 support.
*
*  Revision 1.28  2007/05/02 18:41:08  07o_pet
*  Added Y410 processing.
*
*  Revision 1.27  2007/04/22 20:15:36  05s_put
*  Add/fix doxygen comments
*
*  Revision 1.26  2006/10/13 14:31:28  05s_put
*  Another fix
*
*  Revision 1.25  2006/09/12 11:46:25  05m_mah
*  YCbCr conversion fixed
*
*  Revision 1.24  2006/09/12 08:23:38  05m_mah
*  RGB24 -> YCbCr Conversion added, Y calculation taked from RGB24 -> YUV
*
*  Revision 1.23  2006/06/13 14:51:53  07o_pet
*  fixed bug in UYVY->YUV conversionb and Y->YUV conversion
*
*  Revision 1.22  2006/05/15 17:31:53  05m_mah
*  doxyden comments added/fixed
*
*  Revision 1.21  2006/05/11 14:11:18  05s_put
*  RGB, YV16 and lsYUVpl added
*
*  Revision 1.9  2006/05/11 10:32:24  05s_put
*  Made lsYUVpl color space
*
*  Revision 1.8  2006/05/11 05:32:48  05s_put
*  Made almost everything for codec
*
*  Revision 1.7  2006/04/21 16:50:47  05s_put
*  Finished codec support!
*
*  Revision 1.6  2006/04/20 06:16:21  05s_put
*  Still fighting with YV16.
*
*  Revision 1.5  2006/04/19 20:31:00  05s_put
*  Last version of gmVideoImage was taken
*
*  Revision 1.20  2006/04/18 20:35:17  07o_pet
*  Added more color space conversion routines, mostly from different YUV types to YUV
*
*  Revision 1.19  2006/03/24 14:06:48  07o_pet
*  added YV12 to YUV and YUV2 to YUV conversion
*
*  Revision 1.18  2005/10/03 13:48:22  05s_put
*  Fixed memory leak in conversion.cpp
*  Fixed VS6.0 project files
*  Fixed parameters transfer to denoising process
*
*  Revision 1.17  2005/07/04 08:18:32  05s_put
*  Licenses fixed
*
*  Revision 1.16  2005/07/03 13:16:33  05m_mah
*  Permissions header added
*
*  Revision 1.15  2005/07/03 12:57:56  05m_mah
*  Doxygen comments added
*
*  Revision 1.14  2005/05/25 11:22:48  stranger
*  bugfix-2
*
*  Revision 1.13  2005/05/25 11:06:29  stranger
*  Bugfix
*
*  Revision 1.12  2005/04/27 16:38:59  05s_put
*  VS 6.0 compilation bug fixed
*
*  Revision 1.11  2005/04/27 15:52:48  stranger
*  Fixed bug with compiling MMX in VS 6.0 (MMX/SSE are disabled by define)
*
*  Revision 1.10  2005/04/15 17:14:33  05m_mah
*  const declaration added
*
*  Revision 1.9  2005/04/14 20:32:03  stranger
*  Fixed [IN] to [OUT] where needed in doxygen descriptions.
*
*  Revision 1.8  2005/04/12 09:46:53  stranger
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

#include "Convert.h"
//#include <malloc.h>
#include <memory.h>

// #define MEM_DEBUG
#ifdef MEM_DEBUG
#pragma comment(lib, "psapi.lib")
#include <psapi.h>
#include "stdio.h"
#define MEMCOUNT_START                                                                                              \
    DWORD cur_process_id = GetCurrentProcessId();                                                                   \
    unsigned int mem1;                                                                                              \
    PROCESS_MEMORY_COUNTERS_EX memory_info;                                                                            \
                                                                                                                    \
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, cur_process_id );            \
                                                                                                                    \
    if ( hProcess != NULL )                                                                                         \
    {                                                                                                               \
        if ( !GetProcessMemoryInfo( hProcess, (PROCESS_MEMORY_COUNTERS*) &memory_info, sizeof(PROCESS_MEMORY_COUNTERS) ) )                     \
        {                                                                                                           \
            CloseHandle( hProcess );                                                                                \
        }                                                                                                           \
        else                                                                                                        \
        {                                                                                                           \
            mem1 = memory_info.PrivateUsage; /*PeakWorkingSetSize;*/ \
        }                                                                                                           \
    }

#define MEMCOUNT_FINISH(name)                                                                                       \
    unsigned int mem2 = 0;                                                                                          \
    if ( hProcess != NULL )                                                                                         \
    {                                                                                                               \
        if ( GetProcessMemoryInfo( hProcess, (PROCESS_MEMORY_COUNTERS*)&memory_info, sizeof(PROCESS_MEMORY_COUNTERS) ) )                      \
        {                                                                                                           \
            mem2 = memory_info.PrivateUsage; /* PeakWorkingSetSize; */ \
            CloseHandle( hProcess );                                                                                \
        }                                                                                                           \
                                                                                                                    \
    }                                                                                                               \
    if (mem2 - mem1 > 0) {printf(name);printf(" memdiff = %d\n", mem2 - mem1);}                                                   
    
#endif

#ifdef gmSSE_COMPILER_SUPPORT

int gmColorSpace::m_bSSEEnabled = -1;

/**
**************************************************************************
* \brief Disables SSE functions
*
*  The function disables using of SSE regardless of processor and operation system support.
*
* \return True if SSE were enabled. False otherwise.
*/
bool gmColorSpace::DisableSSE()
{
    bool old = (m_bSSEEnabled == 1);
    m_bSSEEnabled = 0;
    return old;
}

/**
**************************************************************************
* \brief Resets knowledge about SSE support
*
*  The function resets the variable that is responsible for SSE support.
*  Before the next call to conversion function support will be tested.
*
* \return None.
*/
void gmColorSpace::ResetSSE()
{
    m_bSSEEnabled = -1;
}

/**
**************************************************************************
* \brief Converts RGB32 to RGB24
*
*  The function converts frame from RGB32 format into RGB24 format.
*
* \param src                [IN] -  Pointer to source frame in RGB32 format
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBAtoRGB(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        RGBAtoRGB_SSE(src, dst, width, height, src_pitch, dst_pitch);
    }
    else
    {
        RGBAtoRGB_c(src, dst, width, height, src_pitch, dst_pitch);
    }
}

/**
**************************************************************************
* \brief Converts RGB32 to Y
*
*  The function generates only Y plane from RGB32 format.
*
* \param src                [IN] -  Pointer to source frame in RGB32 format
* \param Y                  [OUT] -  Pointer to destination Y plane buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBAtoY(const byte *src, byte *Y, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        RGBAtoY_SSE(src, Y, width, height, src_pitch, dst_pitch);
    }
    else
    {
        RGBAtoY_c(src, Y, width, height, src_pitch, dst_pitch);
    }
}

/**
**************************************************************************
* \brief Converts Y to RGB32
*
*  The function generates gray-level image in RGB32 format from Y plane.
*
* \param Y                  [IN] -  Pointer to Y plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YtoRGBA(const byte *Y, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        YtoRGBA_SSE(Y, dst, width, height, src_pitch, dst_pitch);
    }
    else
    {
        YtoRGBA_c(Y, dst, width, height, src_pitch, dst_pitch);
    }
}

/**
**************************************************************************
* \brief Converts RGB32 to YUV
*
*  The function converts frame from RGB32 format into planar uniform YUV (equal size).
*
* \param src                [IN] -  Pointer to source frame in RGB32 format
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in either Y,U,V plane buffer
* 
* \return None.
*/
void gmColorSpace::RGBAtoYUV(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        RGBAtoYUV_SSE(src, Y, U, V, width, height, src_pitch, dst_pitch);
    }
    else
    {
        RGBAtoYUV_c(src, Y, U, V, width, height, src_pitch, dst_pitch);
    }
}

/**
**************************************************************************
* \brief Converts RGB24 to YUV
*
*  The function converts frame from RGB24 format into planar uniform YUV (equal size).
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in either Y,U,V plane buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYUV(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        RGBtoYUV_SSE(src, Y, U, V, width, height, src_pitch, dst_pitch);
    }
    else
    {
        RGBtoYUV_c(src, Y, U, V, width, height, src_pitch, dst_pitch);
    }
}

/**
**************************************************************************
* \brief Converts RGB24 to YUY2
*
*  The function converts frame from RGB24 format into interleaved YUY2 format, 
*  where one U value is shared between two adjacent horizontally pixels, the same 
*  thing is done with V component. Store format: YUYVYUYV...
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYUY2(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        RGBtoYUY2_SSE(src, dst, width, height, src_pitch, dst_pitch);
    }
    else
    {
        RGBtoYUY2_c(src, dst, width, height, src_pitch, dst_pitch);
    }
}

/**
**************************************************************************
* \brief Converts RGB24 to YV16
*
*  The function converts frame from RGB24 format into planar YV16 format. 
*  Store format: YYY...UUU...VVV, where vertical size of all components are the same,
*  but horizontal sizes of color components are two times less than that of luma component.
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYV16(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch)
{
	RGBtoYV16_c(src, dy, du, dv, width, height, src_pitch, dst_pitch);
}
/**
**************************************************************************
* \brief Converts RGB24 to YCrCb
*
*  The function converts frame from RGB24 format into YCrCb format
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination frame Y buffer
* \param U                  [OUT] -  Pointer to destination frame U buffer
* \param V                  [OUT] -  Pointer to destination frame V buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYCbCr(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    //if (m_bSSEEnabled == 1) 
    //{
    //}
    //else
    {
        RGBtoYCbCr_c(src, Y, U, V, width, height, src_pitch, dst_pitch);
        //}
    }
}


/**
**************************************************************************
* \brief Converts RGB24 to Yrg
*
*  The function converts frame from RGB24 format into YCrCb format
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination frame Y buffer
* \param r                  [OUT] -  Pointer to destination frame r buffer
* \param g                  [OUT] -  Pointer to destination frame g buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYrg(const byte *src, byte *Y, byte *r, byte *g, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    //if (m_bSSEEnabled == 1) 
    //{
    //}
    //else
    {
        RGBtoYrg_c(src, Y, r, g, width, height, src_pitch, dst_pitch);
        //}
    }
}

/**
**************************************************************************
* \brief Converts YV12 to RGB24
*
*  The function converts frame from planar YV12 format into RGB24 format.
*  U and V planes have size reduced by two respectively to Y (both horizontally 
*  and vertically).
*
* \param Y                  [IN] -  Pointer to Y plane
* \param U                  [IN] -  Pointer to U plane
* \param V                  [IN] -  Pointer to V plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch_Y        [IN] -  Line offset in the Y plane buffer
* \param src_pitch_U        [IN] -  Line offset in the either U or V plane buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YV12toRGB(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch_Y, int src_pitch_U, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        YV12toRGB_SSE(Y, U, V, dst, width, height, src_pitch_Y, src_pitch_U, dst_pitch);
    }
    else
    {
        YV12toRGB_c(Y, U, V, dst, width, height, src_pitch_Y, src_pitch_U, dst_pitch);
    }
}

/**
**************************************************************************
* \brief Converts RGB24 to YV12
*
*  The function converts frame from RGB24 format into planar YV12, 
*  where Y plane has the size of the source frame, while each of U and V have
*  size reduced by 2 both by horizontal and by vertical
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch_Y        [IN] -  Line offset in Y plane buffer
* \param dst_pitch_U        [IN] -  Line offset in either U or V plane buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYV12(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch_Y, int dst_pitch_U)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        RGBtoYV12_SSE(src, Y, U, V, width, height, src_pitch, dst_pitch_Y, dst_pitch_U);
    }
    else
    {
        RGBtoYV12_c(src, Y, U, V, width, height, src_pitch, dst_pitch_Y, dst_pitch_U);
    }
}

/**
**************************************************************************
* \brief Converts YUV to RGB32
*
*  The function converts frame from planar YUV format into RGB32 format.
*  All planes are of equal size.
*
* \param Y                  [IN] -  Pointer to Y plane
* \param U                  [IN] -  Pointer to U plane
* \param V                  [IN] -  Pointer to V plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUVtoRGBA(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        YUVtoRGBA_SSE(Y, U, V, dst, width, height, src_pitch, dst_pitch);
    }
    else
    {
        YUVtoRGBA_c(Y, U, V, dst, width, height, src_pitch, dst_pitch);
    }
}

/**
**************************************************************************
* \brief Converts YUV to RGB24
*
*  The function converts frame from planar YUV format into RGB24 format.
*  All planes are of equal size.
*
* \param Y                  [IN] -  Pointer to Y plane
* \param U                  [IN] -  Pointer to U plane
* \param V                  [IN] -  Pointer to V plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUVtoRGB(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        YUVtoRGB_SSE(Y, U, V, dst, width, height, src_pitch, dst_pitch);
    }
    else
    {
        YUVtoRGB_c(Y, U, V, dst, width, height, src_pitch, dst_pitch);
    }
}

/**
**************************************************************************
* \brief Converts YUY2 to RGB24
*
*  The function converts frame from interleaved YUY2 format into RGB24 format.
*  U and V planes are shared between two adjacent horizontally pixels.
*  Stored in format: YUYVYUYV...
*
* \param src                [IN] -  Pointer to source frame plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUY2toRGB(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        YUY2toRGB_SSE(src, dst, width, height, src_pitch, dst_pitch);
    }
    else
    {
        YUY2toRGB_c(src, dst, width, height, src_pitch, dst_pitch);
    }
}

/**
**************************************************************************
* \brief Converts RGB24 to RGB32
*
*  The function converts frame from RGB24 format into RGB32 format.
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoRGBA(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
    if (m_bSSEEnabled == -1) 
    {
        m_bSSEEnabled = DetectSSE() ? 1 : 0;
    }

    if (m_bSSEEnabled == 1) 
    {
        RGBtoRGBA_SSE(src, dst, width, height, src_pitch, dst_pitch);
    }
    else
    {
        RGBtoRGBA_c(src, dst, width, height, src_pitch, dst_pitch);
    }
}

/**
**************************************************************************
* \brief Converts YV16 to YUY2
*
*  The function converts frame from YV16 format into YUY2 format.
*
* \param sy                 [IN] -  Pointer to source frame Y component buffer
* \param su                 [IN] -  Pointer to source frame U component buffer
* \param sv                 [IN] -  Pointer to source frame V component buffer
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YV16toYUY2(const byte *sy, const byte *su, const byte *sv, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
    YV16toYUY2_c(sy, su, sv, dst, width, height, src_pitch, dst_pitch);
}

/**
**************************************************************************
* \brief Converts YUY2 to YV16
*
*  The function converts frame from YUY2 format into YV16 format.
*
* \param src                [IN] -  Pointer to source frame in YV16 format
* \param dy                 [OUT] -  Pointer to destination frame Y component buffer
* \param du                 [OUT] -  Pointer to destination frame U component buffer
* \param dv                 [OUT] -  Pointer to destination frame V component buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUY2toYV16(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch)
{
    YUY2toYV16_c(src, dy, du, dv, width, height, src_pitch, dst_pitch);
}

/**
**************************************************************************
* \brief Converts RGB to RGB24
*
*  The function converts frame from RGB format into RGB24 format.
*  Store YV16 format: YYY...UUU...VVV, where vertical size of all components are the same,
*  but horizontal sizes of color components are two times less than that of luma component.
*
* \param sr                 [IN] -  Pointer to source frame red component buffer
* \param sg                 [IN] -  Pointer to source frame green component buffer
* \param sb                 [IN] -  Pointer to source frame blue component buffer
* \param dst                [OUT] -  Pointer to destination frame in RGB24 format
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoRGB24(const byte *sr, const byte *sg, const byte *sb, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	RGBtoRGB24_c(sr, sg, sb, dst, width, height, src_pitch, dst_pitch);
}

/**
**************************************************************************
* \brief Converts RGB24 to RGB
*
*  The function converts frame from RGB24 format into RGB format.
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dr                 [OUT] -  Pointer to destination frame red component buffer
* \param dg                 [OUT] -  Pointer to destination frame green component buffer
* \param db                 [OUT] -  Pointer to destination frame blue component buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGB24toRGB(const byte *src, byte *dr, byte *dg, byte *db, int width, int height, int src_pitch, int dst_pitch)
{
	RGB24toRGB_c(src, dr, dg, db, width, height, src_pitch, dst_pitch);
}

/**
**************************************************************************
* \brief Converts RGB to RGB24
*
*  The function converts frame from RGB format into RGB24 format.
*
* \param sy                 [IN] -  Pointer to source frame red component buffer
* \param su                 [IN] -  Pointer to source frame green component buffer
* \param sv                 [IN] -  Pointer to source frame blue component buffer
* \param dst                [OUT] -  Pointer to destination frame in RGB24 format
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::lsYUVpltoRGB24(const byte *sy, const byte *su, const byte *sv, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	lsYUVpltoRGB24_c(sy, su, sv, dst, width, height, src_pitch, dst_pitch);
}

/**
**************************************************************************
* \brief Converts RGB24 to RGB
*
*  The function converts frame from RGB24 format into RGB format.
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dy                 [OUT] -  Pointer to destination frame red component buffer
* \param du                 [OUT] -  Pointer to destination frame green component buffer
* \param dv                 [OUT] -  Pointer to destination frame blue component buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGB24tolsYUVpl(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch)
{
	RGB24tolsYUVpl_c(src, dy, du, dv, width, height, src_pitch, dst_pitch);
}

#pragma warning (disable: 4731)

/**
**************************************************************************
* \brief Detects if system supports SSE instructions
*
*  The function checks out if processor and operation system support
*  Streamig SIMD Extensions (SSE).
*
* \return True if SSE are supported.
*/
bool gmColorSpace::DetectSSE()
{
    gmCNVT_ALIGN DWORD CPUFeatures;
    //using_sse_FP = false;
    __try 
    {
        //Detect CPU capabilities
        _asm 
        {

			push eax
            push ebx
            push ecx
            push edx

            //Get vendor string, check for cpuid capability
            xor eax, eax
            cpuid

            //get cpu features - execute cpuid mode 1
            mov eax, 1
            cpuid
            mov CPUFeatures, edx

            pop edx
            pop ecx
            pop ebx
            pop eax
		}

        if (!(CPUFeatures & gmCNVT_SSE_FEATURE_BIT)) 
        {
            return false;
        }

        //Detecting OS support for SSE
        __asm
        {
            xorps xmm0, xmm0
                emms
        }
        //if no exceptions raised, OS support is OK
        return true;
    }
    __except (1) 
    {
        return false;
    }
}

#pragma warning (default: 4731)

/**
**************************************************************************
* \brief Converts RGB32 to YUV
*
*  The function converts frame from RGB32 format into planar uniform YUV (equal size).
*
* \param src                [IN] -  Pointer to source frame in RGB32 format
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in either Y,U,V plane buffer
* 
* \return None.
*/
void gmColorSpace::RGBAtoYUV_SSE(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch)
{
	//////////////////////////////////////////////////////////////////////////
	//SSE Section
	short short128[] = {128, 128, 128, 128};
	float y1[] = {0.299f, 0.299f, 0.299f, 0.299f};
	float y2[] = {0.587f, 0.587f, 0.587f, 0.587f};
	float y3[] = {0.114f, 0.114f, 0.114f, 0.114f};
	//gmCNVT_ALIGN float u1[] = {-0.147f, -0.147f, -0.147f, -0.147f};
	//gmCNVT_ALIGN float u2[] = {-0.289f, -0.289f, -0.289f, -0.289f};
	//gmCNVT_ALIGN float u3[] = {0.436f, 0.436f, 0.436f, 0.436f};
	//gmCNVT_ALIGN float v1[] = {0.615f, 0.615f, 0.615f, 0.615f};
	//gmCNVT_ALIGN float v2[] = {-0.515f, -0.515f, -0.515f, -0.515f};
	//gmCNVT_ALIGN float v3[] = {-0.100f, -0.100f, -0.100f, -0.100f};

    byte *_uv = new byte[24 * sizeof(float) + gmCNVT_ALIGNMENT  - 1];
    float *uv = (float*)((__int64)(_uv + gmCNVT_ALIGNMENT - 1) & ~(gmCNVT_ALIGNMENT - 1));
    uv[0] = uv[1] = uv[2] = uv[3] = -0.147f;
    uv[0 + 4] = uv[1 + 4] = uv[2 + 4] = uv[3 + 4] = -0.289f;
    uv[0 + 8] = uv[1 + 8] = uv[2 + 8] = uv[3 + 8] = 0.436f;
    uv[0 + 12] = uv[1 + 12] = uv[2 + 12] = uv[3 + 12] = 0.615f;
    uv[0 + 16] = uv[1 + 16] = uv[2 + 16] = uv[3 + 16] = -0.515f;
    uv[0 + 20] = uv[1 + 20] = uv[2 + 20] = uv[3 + 20] = -0.100f;

	//number of simultaneously processed pixels
	int parallelNo = 4;
	//number of bytes from each src buffer corresponding to 1 pixel
	int src_bpp = 4;
	//number of bytes from each dst buffer corresponding to 1 pixel
	int dst_bpp = 1;
	int h = height;
	//number of loop iterations per one row
	int w_par = (width + parallelNo - 1) / parallelNo;
	byte const *p = src;
	byte *pY = Y;
	byte *pU = U;
	byte *pV = V;
	byte *pDeltaUV = (byte*)(pV - pU);
	//number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
	int src_pitch_exceed = (w_par * parallelNo - width) * src_bpp;
	int dst_pitch_exceed = (w_par * parallelNo - width) * dst_bpp;
	//number of bytes left from the end of the line till the beginning of the next
	int src_pitch_left = src_pitch - width * src_bpp;
	int dst_pitch_left = dst_pitch - width * dst_bpp;

	__asm
	{
		push esi
		push edi
		push eax
		//push ebx
		push ecx
		push edx

		//src-pointer
		mov esi, p

		//Y-pointer
		mov edi, Y
		//U-pointer
		mov edx, pU
		//V-pointer
		//mov edx, pV

		pxor mm7, mm7

		movups xmm5, y1
		movups xmm6, y2
		movups xmm7, y3

		mov eax, h

		//for every line:
next_line:
		mov ecx, w_par

next_4_pixels:

		cmp ecx, 1
		jne not_last

		sub esi, src_pitch_exceed
		sub edi, dst_pitch_exceed
		//sub ebx, dst_pitch_exceed
		sub edx, dst_pitch_exceed

not_last:
		movq mm0, [esi]
		movq mm1, mm0
		psrl mm1, 32

		//taking both two pixels into mm0 (00RRGGBB)
		punpcklbw mm0, mm1

		movq mm1, [esi + 8]
		movq mm2, mm1
		psrl mm2, 32
		//taking next two pixels into mm1 (00RRGGBB)
		punpcklbw mm1, mm2

		//extracting 0000RRRR into mm2
		movq mm2, mm0
		punpckhbw mm2, mm1

		//extracting GGGGBBBB into mm0
		punpcklbw mm0, mm1

		//extracting 0R0R0R0R into mm2
		punpcklbw mm2, mm7

		//extracting 0G0G0G0G into mm1
		movq mm1, mm0
		punpckhbw mm1, mm7

		//extracting 0B0B0B0B into mm0
		punpcklbw mm0, mm7

		//===================================
		//add your code here :)

		//transferring RED
		movq mm3, mm2
		punpckhwd mm3, mm7
		punpcklwd mm2, mm7
		xorps xmm2, xmm2
		cvtpi2ps xmm2, mm3
		shufps xmm2, xmm2, 4Eh 
		cvtpi2ps xmm2, mm2
		//now xmm2 contains four RED values

		//transferring GREEN
		movq mm3, mm1
		punpckhwd mm3, mm7
		punpcklwd mm1, mm7
		xorps xmm1, xmm1
		cvtpi2ps xmm1, mm3
		shufps xmm1, xmm1, 4Eh 
		cvtpi2ps xmm1, mm1
		//now xmm1 contains four GREEN values

		//transferring BLUE
		movq mm3, mm0
		punpckhwd mm3, mm7
		punpcklwd mm0, mm7
		xorps xmm0, xmm0
		cvtpi2ps xmm0, mm3
		shufps xmm0, xmm0, 4Eh 
		cvtpi2ps xmm0, mm0
		//now xmm0 contains four BLUE values

		//calculate Y:
		movaps xmm3, xmm2
		mulps xmm3, xmm5
		movaps xmm4, xmm1
		mulps xmm4, xmm6
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, xmm7
		addps xmm3, xmm4

		shufps xmm3, xmm3, 0D8h

		//Y
		cvtps2pi mm2, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm2, mm3

        push edi
        mov edi, uv

		//calculate U:
		movaps xmm3, xmm2
        mulps xmm3, [edi]
		movaps xmm4, xmm1
		mulps xmm4, [edi + 16]
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, [edi + 32]
		addps xmm3, xmm4

		shufps xmm3, xmm3, 0D8h

		//U
		cvtps2pi mm1, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm1, mm3

		//calculate V:
		movaps xmm3, xmm2
		mulps xmm3, [edi + 48]
		movaps xmm4, xmm1
		mulps xmm4, [edi + 64]
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, [edi + 80]
		addps xmm3, xmm4

        pop edi

		shufps xmm3, xmm3, 0D8h

		//V
		cvtps2pi mm0, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm0, mm3

		//converting to byte:
		paddw mm0, short128
		paddw mm1, short128
		packuswb mm2, mm2
		packuswb mm1, mm1
		packuswb mm0, mm0

		movd [edi], mm2
		movd [edx], mm1
		push edx
		add edx, pDeltaUV
		movd [edx], mm0
		pop edx

		////===================================

		//next four pixels
		add esi, 16
		add edi, 4
		//add ebx, 4
		add edx, 4
		dec ecx
		jne next_4_pixels

		//next line:
		add esi, src_pitch_left
		add edi, dst_pitch_left
		//add ebx, dst_pitch_left
		add edx, dst_pitch_left
		dec eax
		jnz next_line

		emms

		pop edx
		pop ecx
		//pop ebx
		pop eax
		pop edi
		pop esi
	}
	//}
	//End SSE Section
	//////////////////////////////////////////////////////////////////////////
	delete [] _uv;
}

/**
**************************************************************************
* \brief Converts RGB24 to YUV
*
*  The function converts frame from RGB24 format into planar uniform YUV (equal size).
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in either Y,U,V plane buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYUV_SSE(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch)
{
	//////////////////////////////////////////////////////////////////////////
	//SSE Section
	short short128[] = {128, 128, 128, 128};
	gmCNVT_ALIGN float y1[] = {0.299f, 0.299f, 0.299f, 0.299f};
	gmCNVT_ALIGN float y2[] = {0.587f, 0.587f, 0.587f, 0.587f};
	gmCNVT_ALIGN float y3[] = {0.114f, 0.114f, 0.114f, 0.114f};
	gmCNVT_ALIGN float u1[] = {-0.147f, -0.147f, -0.147f, -0.147f};
	gmCNVT_ALIGN float u2[] = {-0.289f, -0.289f, -0.289f, -0.289f};
	gmCNVT_ALIGN float u3[] = {0.436f, 0.436f, 0.436f, 0.436f};
	gmCNVT_ALIGN float v1[] = {0.615f, 0.615f, 0.615f, 0.615f};
	gmCNVT_ALIGN float v2[] = {-0.515f, -0.515f, -0.515f, -0.515f};
	gmCNVT_ALIGN float v3[] = {-0.100f, -0.100f, -0.100f, -0.100f};

	//number of simultaneously processed pixels
	int parallelNo = 4;
	//number of bytes from each src buffer corresponding to 1 pixel
	int src_bpp = 3;
	//number of bytes from each dst buffer corresponding to 1 pixel
	int dst_bpp = 1;
	int h = height;
	//number of loop iterations per one row
	int w_par = (width + parallelNo - 1) / parallelNo;
	byte const *p = src;
	byte *pY = Y;
	byte *pU = U;
	byte *pV = V;
	byte *pDeltaUV = (byte*)(pV - pU);
	//number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
	int src_pitch_exceed = (w_par * parallelNo - width) * src_bpp;
	int dst_pitch_exceed = (w_par * parallelNo - width) * dst_bpp;
	//number of bytes left from the end of the line till the beginning of the next
	int src_pitch_left = src_pitch - width * src_bpp;
	int dst_pitch_left = dst_pitch - width * dst_bpp;

	__asm
	{
		push esi
		push edi
		push eax
		//push ebx
		push ecx
		push edx

		//src-pointer
		mov esi, p

		//Y-pointer
		mov edi, pY
		//U-pointer
		mov edx, pU
		////V-pointer
		//mov edx, pV

		pxor mm7, mm7

		movaps xmm5, y1
		movaps xmm6, y2
		movaps xmm7, y3

		mov eax, h

		//for every line:
next_line:
		mov ecx, w_par

next_4_pixels:

		cmp ecx, 1
		jne not_last

		sub esi, src_pitch_exceed
		sub edi, dst_pitch_exceed
		//sub ebx, dst_pitch_exceed
		sub edx, dst_pitch_exceed

not_last:
		movq mm0, [esi]
		movq mm1, mm0
		psrl mm1, 24

		//taking both two pixels into mm0 (00RRGGBB)
		punpcklbw mm0, mm1

		movq mm1, [esi + 6]
		movq mm2, mm1
		psrl mm2, 24
		//taking next two pixels into mm1 (00RRGGBB)
		punpcklbw mm1, mm2

		//extracting 0000RRRR into mm2
		movq mm2, mm0
		punpckhbw mm2, mm1

		//extracting GGGGBBBB into mm0
		punpcklbw mm0, mm1

		//extracting 0R0R0R0R into mm2
		punpcklbw mm2, mm7

		//extracting 0G0G0G0G into mm1
		movq mm1, mm0
		punpckhbw mm1, mm7

		//extracting 0B0B0B0B into mm0
		punpcklbw mm0, mm7

		//===================================
		//add your code here :)

		//transferring RED
		movq mm3, mm2
		punpckhwd mm3, mm7
		punpcklwd mm2, mm7
		xorps xmm2, xmm2
		cvtpi2ps xmm2, mm3
		shufps xmm2, xmm2, 4Eh 
		cvtpi2ps xmm2, mm2
		//now xmm2 contains four RED values

		//transferring GREEN
		movq mm3, mm1
		punpckhwd mm3, mm7
		punpcklwd mm1, mm7
		xorps xmm1, xmm1
		cvtpi2ps xmm1, mm3
		shufps xmm1, xmm1, 4Eh 
		cvtpi2ps xmm1, mm1
		//now xmm1 contains four GREEN values

		//transferring BLUE
		movq mm3, mm0
		punpckhwd mm3, mm7
		punpcklwd mm0, mm7
		xorps xmm0, xmm0
		cvtpi2ps xmm0, mm3
		shufps xmm0, xmm0, 4Eh 
		cvtpi2ps xmm0, mm0
		//now xmm0 contains four BLUE values

		//calculate Y:
		movaps xmm3, xmm2
		mulps xmm3, xmm5
		movaps xmm4, xmm1
		mulps xmm4, xmm6
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, xmm7
		addps xmm3, xmm4

		shufps xmm3, xmm3, 0D8h

		//Y
		cvtps2pi mm2, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm2, mm3

		//calculate U:
		movaps xmm3, xmm2
		mulps xmm3, u1
		movaps xmm4, xmm1
		mulps xmm4, u2
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, u3
		addps xmm3, xmm4

		shufps xmm3, xmm3, 0D8h

		//U
		cvtps2pi mm1, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm1, mm3

		//calculate V:
		movaps xmm3, xmm2
		mulps xmm3, v1
		movaps xmm4, xmm1
		mulps xmm4, v2
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, v3
		addps xmm3, xmm4

		shufps xmm3, xmm3, 0D8h

		//V
		cvtps2pi mm0, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm0, mm3

		//converting to byte:
		paddw mm0, short128
		paddw mm1, short128
		packuswb mm2, mm2
		packuswb mm1, mm1
		packuswb mm0, mm0

		movd [edi], mm2
		push edx
		movd [edx], mm1
		add edx, pDeltaUV
		movd [edx], mm0
		pop edx

		////===================================

		//next four pixels
		add esi, 12
		add edi, 4
		//add ebx, 4
		add edx, 4
		dec ecx
		jne next_4_pixels

		//next line:
		add esi, src_pitch_left
		add edi, dst_pitch_left
		//add ebx, dst_pitch_left
		add edx, dst_pitch_left
		dec eax
		jnz next_line

		emms

		pop edx
		pop ecx
		//pop ebx
		pop eax
		pop edi
		pop esi
	}
	//}
	//End SSE Section
	//////////////////////////////////////////////////////////////////////////
}

/**
**************************************************************************
* \brief Converts RGB24 to YUY2
*
*  The function converts frame from RGB24 format into interleaved YUY2 format, 
*  where one U value is shared between two adjacent horizontally pixels, the same 
*  thing is done with V component. Store format: YUYVYUYV...
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYUY2_SSE(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	//////////////////////////////////////////////////////////////////////////
	//SSE Section
	gmCNVT_ALIGN float y1[] = {0.299f, 0.299f, 0.299f, 0.299f};
	gmCNVT_ALIGN float y2[] = {0.587f, 0.587f, 0.587f, 0.587f};
	gmCNVT_ALIGN float y3[] = {0.114f, 0.114f, 0.114f, 0.114f};
	gmCNVT_ALIGN float u1[] = {-0.147f, -0.147f, -0.147f, -0.147f};
	gmCNVT_ALIGN float u2[] = {-0.289f, -0.289f, -0.289f, -0.289f};
	gmCNVT_ALIGN float u3[] = {0.436f, 0.436f, 0.436f, 0.436f};
	gmCNVT_ALIGN float v1[] = {0.615f, 0.615f, 0.615f, 0.615f};
	gmCNVT_ALIGN float v2[] = {-0.515f, -0.515f, -0.515f, -0.515f};
	gmCNVT_ALIGN float v3[] = {-0.100f, -0.100f, -0.100f, -0.100f};
	gmCNVT_ALIGN float float05[] = {0.5f, 0.5f, 0.5f, 0.5f};
	gmCNVT_ALIGN float float128[] = {128.0f, 128.0f, 128.0f, 128.0f};

	//number of simultaneously processed pixels
	int parallelNo = 4;
	//number of bytes from each src buffer corresponding to 1 pixel
	int src_bpp = 3;
	//number of bytes from each dst buffer corresponding to 1 pixel
	int dst_bpp = 2;
	int h = height;
	//number of loop iterations per one row
	int w_par = (width + parallelNo - 1) / parallelNo;
	byte const *p = src;
	byte *pYUYV = dst;
	//number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
	int src_pitch_exceed = (w_par * parallelNo - width) * src_bpp;
	int dst_pitch_exceed = (w_par * parallelNo - width) * dst_bpp;
	//number of bytes left from the end of the line till the beginning of the next
	int src_pitch_left = src_pitch - width * src_bpp;
	int dst_pitch_left = dst_pitch - width * dst_bpp;
	int swap_UV = ((w_par * parallelNo - width) & 1) ? 1 : 0;

	__asm
	{
		push esi
		push edi
		push eax
		push ecx
		push edx

		//src-pointer
		mov esi, p

		//YUYV-pointer
		mov edi, pYUYV

		pxor mm7, mm7

		mov eax, h

		//for every line:
next_line:
		mov ecx, w_par

		xor edx, edx

next_4_pixels:

		cmp ecx, 1
		jne not_last

		sub esi, src_pitch_exceed
		sub edi, dst_pitch_exceed
		mov edx, swap_UV

not_last:
		movq mm0, [esi]
		movq mm1, mm0
		psrl mm1, 24

		//taking both two pixels into mm0 (00RRGGBB)
		punpcklbw mm0, mm1

		movq mm1, [esi + 6]
		movq mm2, mm1
		psrl mm2, 24
		//taking next two pixels into mm1 (00RRGGBB)
		punpcklbw mm1, mm2

		//extracting 0000RRRR into mm2
		movq mm2, mm0
		punpckhbw mm2, mm1

		//extracting GGGGBBBB into mm0
		punpcklbw mm0, mm1

		//extracting 0R0R0R0R into mm2
		punpcklbw mm2, mm7

		//extracting 0G0G0G0G into mm1
		movq mm1, mm0
		punpckhbw mm1, mm7

		//extracting 0B0B0B0B into mm0
		punpcklbw mm0, mm7

		//===================================
		//add your code here :)

		//transferring RED
		movq mm3, mm2
		punpckhwd mm3, mm7
		punpcklwd mm2, mm7
		xorps xmm2, xmm2
		cvtpi2ps xmm2, mm3
		shufps xmm2, xmm2, 4Eh 
		cvtpi2ps xmm2, mm2
		//now xmm2 contains four RED values

		//transferring GREEN
		movq mm3, mm1
		punpckhwd mm3, mm7
		punpcklwd mm1, mm7
		xorps xmm1, xmm1
		cvtpi2ps xmm1, mm3
		shufps xmm1, xmm1, 4Eh 
		cvtpi2ps xmm1, mm1
		//now xmm1 contains four GREEN values

		//transferring BLUE
		movq mm3, mm0
		punpckhwd mm3, mm7
		punpcklwd mm0, mm7
		xorps xmm0, xmm0
		cvtpi2ps xmm0, mm3
		shufps xmm0, xmm0, 4Eh 
		cvtpi2ps xmm0, mm0
		//now xmm0 contains four BLUE values

		//calculate Y:
		movaps xmm3, xmm2
		mulps xmm3, y1
		movaps xmm4, xmm1
		mulps xmm4, y2
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, y3
		addps xmm3, xmm4

		//shufps xmm3, xmm3, 0D8h
		
		shufps xmm3, xmm3, 10001101b

		//storing Y for next packing
		movaps xmm7, xmm3

		//calculate U:
		movaps xmm3, xmm2
		mulps xmm3, u1
		movaps xmm4, xmm1
		mulps xmm4, u2
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, u3
		addps xmm3, xmm4

		//shufps xmm3, xmm3, 0D8h
		
		shufps xmm3, xmm3, 10001101b

		//storing U for next packing
		movaps xmm6, xmm3

		//calculate V:
		movaps xmm3, xmm2
		mulps xmm3, v1
		movaps xmm4, xmm1
		mulps xmm4, v2
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, v3
		addps xmm3, xmm4

		//shufps xmm3, xmm3, 0D8h
		
		shufps xmm3, xmm3, 10001101b

		cmp edx, 1
		jne no_swap
		movaps xmm4, xmm3
		movaps xmm3, xmm6
		movaps xmm6, xmm4

no_swap:

		//compressing two neighboring U-values to contain average
		movaps xmm4, xmm6
		shufps xmm4, xmm4, 0B1h
		addps xmm6, xmm4
		mulps xmm6, float05
		addps xmm6, float128

		//compressing two neighboring V-values to contain average
		movaps xmm4, xmm3
		shufps xmm4, xmm4, 0B1h
		addps xmm3, xmm4
		mulps xmm3, float05
		addps xmm3, float128

		//packing
		movaps xmm4, xmm3
		//getting u1u2v1v2
		shufps xmm3, xmm6, 11101110b
		//getting y1y2u1v1
		shufps xmm3, xmm7, 11101101b
		shufps xmm3, xmm3, 11011000b
		
		//getting u3u4v3v4
		shufps xmm4, xmm6, 01000100b
		//getting y3y4u3v3
		shufps xmm4, xmm7, 01001101b
		shufps xmm4, xmm4, 11011000b

		//shufps xmm3, xmm3, 0D8h
		//shufps xmm4, xmm4, 0D8h
		shufps xmm3, xmm3, 00111001b
		shufps xmm4, xmm4, 00111001b

		//first 4 bytes
		cvtps2pi mm0, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm0, mm3

		//second 4 bytes
		cvtps2pi mm1, xmm4
		shufps xmm4, xmm4, 4Eh
		cvtps2pi mm3, xmm4
		packssdw mm1, mm3

		//converting to byte:
		packuswb mm1, mm1
		packuswb mm0, mm0

		psll mm1, 32
		psll mm0, 32
		psrl mm0, 32
		por mm0, mm1
		movq [edi], mm0
		//movd [edi + 4], mm1

		////===================================

		//next four pixels
		add esi, 12
		add edi, 8
		dec ecx
		jne next_4_pixels

		//next line:
		add esi, src_pitch_left
		add edi, dst_pitch_left
		dec eax
		jnz next_line

		emms

		pop edx
		pop ecx
		pop eax
		pop edi
		pop esi
	}
	//}
	//End SSE Section
	//////////////////////////////////////////////////////////////////////////
}

/**
**************************************************************************
* \brief Converts YUY2 to RGB24
*
*  The function converts frame from interleaved YUY2 format into RGB24 format.
*  U and V planes are shared between two adjacent horizontally pixels.
*  Stored in format: YUYVYUYV...
*
* \param src                [IN] -  Pointer to source frame plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUY2toRGB_SSE(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	//////////////////////////////////////////////////////////////////////////
	//SSE Section
	short short128[] = {128, 128, 128, 128};
	gmCNVT_ALIGN unsigned char temp[] = {255, 0, 255, 0, 255, 0, 255, 0};
	gmCNVT_ALIGN short temp255[] = {255, 255, 255, 255};					//maximum for R, G, B
	gmCNVT_ALIGN short temp256[] = {256, 256, 256, 256};					//shift for Y, U, V
	gmCNVT_ALIGN float tempd256[] = {256.0f, 256.0f, 256.0f, 256.0f};		//back shift for Y, U, V
	gmCNVT_ALIGN float r3[] = {1.14f, 1.14f, 1.14f, 1.14f};
	gmCNVT_ALIGN float g2[] = {-0.395f, -0.395f, -0.395f, -0.395f};
	gmCNVT_ALIGN float g3[] = {-0.581f, -0.581f, -0.581f, -0.581f};
	gmCNVT_ALIGN float b2[] = {2.032f, 2.032f, 2.032f, 2.032f};

	//number of simultaneously processed pixels
	int parallelNo = 4;
	//number of bytes from each src buffer corresponding to 1 pixel
	int src_bpp = 2;
	//number of bytes from each dst buffer corresponding to 1 pixel
	int dst_bpp = 3;
	int h = height;
	//number of loop iterations per one row
	int w_par = (width + parallelNo - 1) / parallelNo;
	byte *p = dst;
	byte const *pYUYV = src;
	//number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
	int src_pitch_exceed = (w_par * parallelNo - width) * src_bpp;
	int dst_pitch_exceed = (w_par * parallelNo - width) * dst_bpp;
	//number of bytes left from the end of the line till the beginning of the next
	int src_pitch_left = src_pitch - width * src_bpp;
	int dst_pitch_left = dst_pitch - width * dst_bpp;

	__asm
	{
		push esi
		push edi
		push eax
		push ecx

		//src-pointer
		mov esi, pYUYV

		//dst-pointer
		mov edi, p

		pxor mm7, mm7
		movq mm6, temp
		movq mm4, temp255

		movaps xmm4, b2
		movaps xmm5, r3
		movaps xmm6, g2
		movaps xmm7, g3

		mov eax, h

		//for every line:
next_line:
		mov ecx, w_par

next_4_pixels:

		cmp ecx, 1
		jne not_last

		sub esi, src_pitch_exceed
		sub edi, dst_pitch_exceed

not_last:

		movq mm2, [esi]
		movq mm1, mm2
		pand mm2, mm6

		psrlw mm1, 8
		movq mm0, mm1

		pslld mm1, 16
		movq mm3, mm1
		psrld mm3, 16
		por mm1, mm3

		psrld mm0, 16
		movq mm3, mm0
		pslld mm3, 16
		por mm0, mm3

		//unpack to words:
		//punpckhbw mm2, mm7
		//punpcklbw mm1, mm7
		//punpcklbw mm0, mm7

		psubsw mm0, short128
		psubsw mm1, short128

		//===================================
		//add your code here :)

		//transferring Y
		paddsw mm2, temp256
		movq mm3, mm2
		punpckhwd mm3, mm7
		punpcklwd mm2, mm7
		xorps xmm2, xmm2
		cvtpi2ps xmm2, mm3
        shufps xmm2, xmm2, 4Eh 
        cvtpi2ps xmm2, mm2
		subps xmm2, tempd256
		//now xmm2 contains four Y values

		//transferring U
		paddsw mm1, temp256
		movq mm3, mm1
		punpckhwd mm3, mm7
		punpcklwd mm1, mm7
		xorps xmm1, xmm1
		cvtpi2ps xmm1, mm1
		shufps xmm1, xmm1, 4Eh 
		cvtpi2ps xmm1, mm3
		subps xmm1, tempd256
        shufps xmm1, xmm1, 01001110b 
		//now xmm1 contains four U values

		//transferring V
		paddsw mm0, temp256
		movq mm3, mm0
		punpckhwd mm3, mm7
		punpcklwd mm0, mm7
		xorps xmm0, xmm0
		cvtpi2ps xmm0, mm0
		shufps xmm0, xmm0, 4Eh 
		cvtpi2ps xmm0, mm3
		subps xmm0, tempd256
        shufps xmm0, xmm0, 01001110b 
		//now xmm0 contains four V values

		//calculate R:
		movaps xmm3, xmm0
		mulps xmm3, xmm5
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//R
		cvtps2pi mm2, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm2, mm3

		//calculate G:
		movaps xmm3, xmm1
		mulps xmm3, xmm6
		mulps xmm0, xmm7
		addps xmm3, xmm0
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//G
		cvtps2pi mm1, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm1, mm3

		//calculate B:
		movaps xmm3, xmm1
		mulps xmm3, xmm4
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//B
		cvtps2pi mm0, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm0, mm3

		////===================================

		pminsw mm0, mm4
		pminsw mm1, mm4
		pminsw mm2, mm4
		pmaxsw mm0, mm7
		pmaxsw mm1, mm7
		pmaxsw mm2, mm7

		//packing GGGGBBBB into mm0
		packuswb mm0, mm1

		//packing 0000RRRR into mm2
		packuswb mm2, mm7

		//forming first two pixels' joint register 00RRGGBB
		movq mm1, mm0
		pand mm1, mm6
		movq mm3, mm2
		pand mm3, mm6
		packuswb mm1, mm3

		//forming second two pixels' joint register 00RRGGBB
		movq mm5, mm6
		psllw mm5, 8
		pand mm0, mm5
		pand mm2, mm5
		psrlw mm0, 8
		psrlw mm2, 8
		packuswb mm0, mm2

		//storing first two pixels
		movq mm2, mm1
		pand mm2, mm6
		packuswb mm2, mm7
		pand mm1, mm5
		psrlw mm1, 8
		packuswb mm1, mm7

		//packing
		psll mm1, 40
		psll mm2, 40
		psrl mm1, 16
		psrl mm2, 40
		por mm1, mm2

		//storing last two pixels
		movq mm2, mm0
		pand mm2, mm6
		packuswb mm2, mm7
		pand mm0, mm5
		psrlw mm0, 8
		packuswb mm0, mm7

		//packing
		movq mm3, mm2
		psll mm3, 48
		por mm1, mm3
		psll mm2, 24
		psll mm0, 8
		psrl mm2, 40
		por mm2, mm0

		movq [edi], mm1
		movd [edi + 8], mm2

		////===================================

		//next four pixels
		add esi, 8
		add edi, 12
		dec ecx
		jne next_4_pixels

		//next line:
		add esi, src_pitch_left
		add edi, dst_pitch_left
		dec eax
		jnz next_line

		emms

		pop ecx
		pop eax
		pop edi
		pop esi
	}
	//}
	//End SSE Section
	//////////////////////////////////////////////////////////////////////////
}

/**
**************************************************************************
* \brief Converts YUV to RGB32
*
*  The function converts frame from planar YUV format into RGB32 format.
*  All planes are of equal size.
*
* \param Y                  [IN] -  Pointer to Y plane
* \param U                  [IN] -  Pointer to U plane
* \param V                  [IN] -  Pointer to V plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUVtoRGBA_SSE(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	//////////////////////////////////////////////////////////////////////////
	//SSE Section
	gmCNVT_ALIGN unsigned char temp[] = {255, 0, 255, 0, 255, 0, 255, 0};
	gmCNVT_ALIGN short temp255[] = {255, 255, 255, 255};					//maximum for R, G, B
	gmCNVT_ALIGN short temp256[] = {256, 256, 256, 256};					//shift for Y, U, V
	gmCNVT_ALIGN float tempd256[] = {256.0f, 256.0f, 256.0f, 256.0f};		//back shift for Y, U, V
	gmCNVT_ALIGN short short128[] = {128, 128, 128, 128};					//shift for Y, U, V
	gmCNVT_ALIGN float r3[] = {1.14f, 1.14f, 1.14f, 1.14f};
	gmCNVT_ALIGN float g2[] = {-0.395f, -0.395f, -0.395f, -0.395f};
	gmCNVT_ALIGN float g3[] = {-0.581f, -0.581f, -0.581f, -0.581f};
	gmCNVT_ALIGN float b2[] = {2.032f, 2.032f, 2.032f, 2.032f};

	//number of simultaneously processed pixels
	int parallelNo = 4;
	//number of bytes from each src buffer corresponding to 1 pixel
	int src_bpp = 1;
	//number of bytes from each dst buffer corresponding to 1 pixel
	int dst_bpp = 4;
	int h = height;
	//number of loop iterations per one row
	int w_par = (width + parallelNo - 1) / parallelNo;
	byte *p = dst;
	byte const *pY = Y;
	byte const *pU = U;
	byte const *pV = V;
	byte *pDeltaUV = (byte*)(pV - pU);
	//number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
	int src_pitch_exceed = (w_par * parallelNo - width) * src_bpp;
	int dst_pitch_exceed = (w_par * parallelNo - width) * dst_bpp;
	//number of bytes left from the end of the line till the beginning of the next
	int src_pitch_left = src_pitch - width * src_bpp;
	int dst_pitch_left = dst_pitch - width * dst_bpp;


	__asm
	{
		push esi
		push edi
		push eax
		//push ebx
		push ecx
		push edx

		//dst-pointer
		mov esi, p

		//Y-pointer
		mov edi, pY
		//U-pointer
		mov edx, pU
		////V-pointer
		//mov edx, pV

		pxor mm7, mm7
		movq mm6, temp
		movq mm4, temp255

		movaps xmm4, b2
		movaps xmm5, r3
		movaps xmm6, g2
		movaps xmm7, g3

		mov eax, h

		//for every line:
next_line:
		mov ecx, w_par

next_4_pixels:

		//for the case if width is not divisible by 4
		//branch prediction should work perfectly
		cmp ecx, 1
		jne not_last

		sub esi, dst_pitch_exceed
		sub edi, src_pitch_exceed
		//sub ebx, src_pitch_exceed
		sub edx, src_pitch_exceed
not_last:
		//taking 4 Y values:
		movd mm2, [edi]
		//taking 4 U values:
		push edx
		movd mm1, [edx]
		//taking 4 V values:
		add edx, pDeltaUV
		movd mm0, [edx]
		pop edx

		//unpack to words:
		punpcklbw mm2, mm7
		punpcklbw mm1, mm7
		punpcklbw mm0, mm7

		psubsw mm0, short128
		psubsw mm1, short128

		//===================================
		//add your code here :)

		//transferring Y
		paddsw mm2, temp256
		movq mm3, mm2
		punpckhwd mm3, mm7
		punpcklwd mm2, mm7
		xorps xmm2, xmm2
		cvtpi2ps xmm2, mm3
		shufps xmm2, xmm2, 4Eh 
		cvtpi2ps xmm2, mm2
		subps xmm2, tempd256
		//now xmm2 contains four Y values

		//transferring U
		paddsw mm1, temp256
		movq mm3, mm1
		punpckhwd mm3, mm7
		punpcklwd mm1, mm7
		xorps xmm1, xmm1
		cvtpi2ps xmm1, mm3
		shufps xmm1, xmm1, 4Eh 
		cvtpi2ps xmm1, mm1
		subps xmm1, tempd256
		//now xmm1 contains four U values

		//transferring V
		paddsw mm0, temp256
		movq mm3, mm0
		punpckhwd mm3, mm7
		punpcklwd mm0, mm7
		xorps xmm0, xmm0
		cvtpi2ps xmm0, mm3
		shufps xmm0, xmm0, 4Eh 
		cvtpi2ps xmm0, mm0
		subps xmm0, tempd256
		//now xmm0 contains four V values

		//calculate R:
		movaps xmm3, xmm0
		mulps xmm3, xmm5
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//R
		cvtps2pi mm2, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm2, mm3

		//calculate G:
		movaps xmm3, xmm1
		mulps xmm3, xmm6
		mulps xmm0, xmm7
		addps xmm3, xmm0
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//G
		cvtps2pi mm1, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm1, mm3

		//calculate B:
		movaps xmm3, xmm1
		mulps xmm3, xmm4
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//B
		cvtps2pi mm0, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm0, mm3

		////===================================

		pminsw mm0, mm4
		pminsw mm1, mm4
		pminsw mm2, mm4
		pmaxsw mm0, mm7
		pmaxsw mm1, mm7
		pmaxsw mm2, mm7

		//packing GGGGBBBB into mm0
		packuswb mm0, mm1

		//packing 0000RRRR into mm2
		packuswb mm2, mm7

		//forming first two pixels' joint register 00RRGGBB
		movq mm1, mm0
		pand mm1, mm6
		movq mm3, mm2
		pand mm3, mm6
		packuswb mm1, mm3

		//forming second two pixels' joint register 00RRGGBB
		movq mm5, mm6
		psllw mm5, 8
		pand mm0, mm5
		pand mm2, mm5
		psrlw mm0, 8
		psrlw mm2, 8
		packuswb mm0, mm2

		//storing first two pixels
		movq mm2, mm0
		pand mm2, mm6
		packuswb mm2, mm7
		pand mm0, mm5
		psrlw mm0, 8
		packuswb mm0, mm7

		//packing
		psll mm0, 32
		psll mm2, 32
		psrl mm2, 32
		por mm0, mm2

		movq [esi + 8], mm0

		//storing last two pixels
		movq mm2, mm1
		pand mm2, mm6
		packuswb mm2, mm7
		pand mm1, mm5
		psrlw mm1, 8
		packuswb mm1, mm7

		//packing
		psll mm1, 32
		psll mm2, 32
		psrl mm2, 32
		por mm1, mm2

		movq [esi], mm1

		//next four pixels
		add esi, 16
		add edi, 4
		//add ebx, 4
		add edx, 4
		dec ecx
		jnz next_4_pixels

		//next line:
		add esi, dst_pitch_left
		add edi, src_pitch_left
		//add ebx, src_pitch_left
		add edx, src_pitch_left
		dec eax
		jnz next_line

		emms

		pop edx
		pop ecx
		//pop ebx
		pop eax
		pop edi
		pop esi
	}
	//End SSE Section
	//////////////////////////////////////////////////////////////////////////
}

/**
**************************************************************************
* \brief Converts YUV to RGB24
*
*  The function converts frame from planar YUV format into RGB24 format.
*  All planes are of equal size.
*
* \param Y                  [IN] -  Pointer to Y plane
* \param U                  [IN] -  Pointer to U plane
* \param V                  [IN] -  Pointer to V plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUVtoRGB_SSE(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	//////////////////////////////////////////////////////////////////////////
	//SSE Section
	gmCNVT_ALIGN unsigned char temp[] = {255, 0, 255, 0, 255, 0, 255, 0};
	gmCNVT_ALIGN short temp255[] = {255, 255, 255, 255};					//maximum for R, G, B
	gmCNVT_ALIGN short temp256[] = {256, 256, 256, 256};					//shift for Y, U, V
	gmCNVT_ALIGN float tempd256[] = {256.0f, 256.0f, 256.0f, 256.0f};		//back shift for Y, U, V
	gmCNVT_ALIGN short short128[] = {128, 128, 128, 128};					//shift for Y, U, V
	gmCNVT_ALIGN float r3[] = {1.14f, 1.14f, 1.14f, 1.14f};
	gmCNVT_ALIGN float g2[] = {-0.395f, -0.395f, -0.395f, -0.395f};
	gmCNVT_ALIGN float g3[] = {-0.581f, -0.581f, -0.581f, -0.581f};
	gmCNVT_ALIGN float b2[] = {2.032f, 2.032f, 2.032f, 2.032f};

	//number of simultaneously processed pixels
	int parallelNo = 4;
	//number of bytes from each src buffer corresponding to 1 pixel
	int src_bpp = 1;
	//number of bytes from each dst buffer corresponding to 1 pixel
	int dst_bpp = 3;
	int h = height;
	//number of loop iterations per one row
	int w_par = (width + parallelNo - 1) / parallelNo;
	byte *p = dst;
	byte const *pY = Y;
	byte const *pU = U;
	byte const *pV = V;
	byte *pDeltaUV = (byte*)(pV - pU);
	//number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
	int src_pitch_exceed = (w_par * parallelNo - width) * src_bpp;
	int dst_pitch_exceed = (w_par * parallelNo - width) * dst_bpp;
	//number of bytes left from the end of the line till the beginning of the next
	int src_pitch_left = src_pitch - width * src_bpp;
	int dst_pitch_left = dst_pitch - width * dst_bpp;


	__asm
	{
		push esi
		push edi
		push eax
		//push ebx
		push ecx
		push edx

		//dst-pointer
		mov esi, p

		//Y-pointer
		mov edi, pY
		//U-pointer
		mov edx, pU
		////V-pointer
		//mov edx, pV

		pxor mm7, mm7
		movq mm6, temp
		movq mm4, temp255

		movaps xmm4, b2
		movaps xmm5, r3
		movaps xmm6, g2
		movaps xmm7, g3

		mov eax, h

		//for every line:
next_line:
		mov ecx, w_par

next_4_pixels:

		//for the case if width is not divisible by 4
		//branch prediction should work perfectly
		cmp ecx, 1
		jne not_last

		sub esi, dst_pitch_exceed
		sub edi, src_pitch_exceed
		//sub ebx, src_pitch_exceed
		sub edx, src_pitch_exceed
not_last:
		//taking 4 Y values:
		movd mm2, [edi]
		//taking 4 U values:
		push edx
		movd mm1, [edx]
		//taking 4 V values:
		add edx, pDeltaUV
		movd mm0, [edx]
		pop edx

		//unpack to words:
		punpcklbw mm2, mm7
		punpcklbw mm1, mm7
		punpcklbw mm0, mm7

		psubsw mm0, short128
		psubsw mm1, short128

		//===================================
		//add your code here :)

		//transferring Y
		paddsw mm2, temp256
		movq mm3, mm2
		punpckhwd mm3, mm7
		punpcklwd mm2, mm7
		xorps xmm2, xmm2
		cvtpi2ps xmm2, mm3
		shufps xmm2, xmm2, 4Eh 
		cvtpi2ps xmm2, mm2
		subps xmm2, tempd256
		//now xmm2 contains four Y values

		//transferring U
		paddsw mm1, temp256
		movq mm3, mm1
		punpckhwd mm3, mm7
		punpcklwd mm1, mm7
		xorps xmm1, xmm1
		cvtpi2ps xmm1, mm3
		shufps xmm1, xmm1, 4Eh 
		cvtpi2ps xmm1, mm1
		subps xmm1, tempd256
		//now xmm1 contains four U values

		//transferring V
		paddsw mm0, temp256
		movq mm3, mm0
		punpckhwd mm3, mm7
		punpcklwd mm0, mm7
		xorps xmm0, xmm0
		cvtpi2ps xmm0, mm3
		shufps xmm0, xmm0, 4Eh 
		cvtpi2ps xmm0, mm0
		subps xmm0, tempd256
		//now xmm0 contains four V values

		//calculate R:
		movaps xmm3, xmm0
		mulps xmm3, xmm5
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//R
		cvtps2pi mm2, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm2, mm3

		//calculate G:
		movaps xmm3, xmm1
		mulps xmm3, xmm6
		mulps xmm0, xmm7
		addps xmm3, xmm0
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//G
		cvtps2pi mm1, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm1, mm3

		//calculate B:
		movaps xmm3, xmm1
		mulps xmm3, xmm4
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//B
		cvtps2pi mm0, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm0, mm3

		////===================================

		pminsw mm0, mm4
		pminsw mm1, mm4
		pminsw mm2, mm4
		pmaxsw mm0, mm7
		pmaxsw mm1, mm7
		pmaxsw mm2, mm7

		//packing GGGGBBBB into mm0
		packuswb mm0, mm1

		//packing 0000RRRR into mm2
		packuswb mm2, mm7

		//forming first two pixels' joint register 00RRGGBB
		movq mm1, mm0
		pand mm1, mm6
		movq mm3, mm2
		pand mm3, mm6
		packuswb mm1, mm3

		//forming second two pixels' joint register 00RRGGBB
		movq mm5, mm6
		psllw mm5, 8
		pand mm0, mm5
		pand mm2, mm5
		psrlw mm0, 8
		psrlw mm2, 8
		packuswb mm0, mm2

		//storing first two pixels
		movq mm2, mm1
		pand mm2, mm6
		packuswb mm2, mm7
		pand mm1, mm5
		psrlw mm1, 8
		packuswb mm1, mm7

		//packing
		psll mm1, 40
		psll mm2, 40
		psrl mm1, 16
		psrl mm2, 40
		por mm1, mm2

		//storing last two pixels
		movq mm2, mm0
		pand mm2, mm6
		packuswb mm2, mm7
		pand mm0, mm5
		psrlw mm0, 8
		packuswb mm0, mm7

		//packing
		movq mm3, mm2
		psll mm3, 48
		por mm1, mm3
		psll mm2, 24
		psll mm0, 8
		psrl mm2, 40
		por mm2, mm0

		movq [esi], mm1
		movd [esi + 8], mm2

		//next four pixels
		add esi, 12
		add edi, 4
		//add ebx, 4
		add edx, 4
		dec ecx
		jnz next_4_pixels

		//next line:
		add esi, dst_pitch_left
		add edi, src_pitch_left
		//add ebx, src_pitch_left
		add edx, src_pitch_left
		dec eax
		jnz next_line

		emms

		pop edx
		pop ecx
		//pop ebx
		pop eax
		pop edi
		pop esi
	}
	//End SSE Section
	//////////////////////////////////////////////////////////////////////////
}

/**
**************************************************************************
* \brief Converts YV12 to RGB24
*
*  The function converts frame from planar YV12 format into RGB24 format.
*  U and V planes have size reduced by two respectively to Y (both horizontally 
*  and vertically).
*
* \param Y                  [IN] -  Pointer to Y plane
* \param U                  [IN] -  Pointer to U plane
* \param V                  [IN] -  Pointer to V plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch_Y        [IN] -  Line offset in the Y plane buffer
* \param src_pitch_U        [IN] -  Line offset in the either U or V plane buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YV12toRGB_SSE(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch_Y, int src_pitch_U, int dst_pitch)
{
	//////////////////////////////////////////////////////////////////////////
	//SSE Section
	gmCNVT_ALIGN unsigned char temp[] = {255, 0, 255, 0, 255, 0, 255, 0};
	gmCNVT_ALIGN short temp255[] = {255, 255, 255, 255};					//maximum for R, G, B
	gmCNVT_ALIGN short temp256[] = {256, 256, 256, 256};					//shift for Y, U, V
	gmCNVT_ALIGN float tempd256[] = {256.0f, 256.0f, 256.0f, 256.0f};		//back shift for Y, U, V
	gmCNVT_ALIGN short short128[] = {128, 128, 128, 128};					//shift for Y, U, V
	gmCNVT_ALIGN float r3[] = {1.14f, 1.14f, 1.14f, 1.14f};
	gmCNVT_ALIGN float g2[] = {-0.395f, -0.395f, -0.395f, -0.395f};
	gmCNVT_ALIGN float g3[] = {-0.581f, -0.581f, -0.581f, -0.581f};
	gmCNVT_ALIGN float b2[] = {2.032f, 2.032f, 2.032f, 2.032f};

	//number of simultaneously processed pixels
	int parallelHor = 2;
	int parallelVer = 2;
	//number of bytes from each src buffer corresponding to 1 pixel
	int src_bpp = 1;
	//number of bytes from each dst buffer corresponding to 1 pixel
	int dst_bpp = 3;
	//number of loop iterations per one row
	int w_par = (width + parallelHor - 1) / parallelHor;
	int h_par = (height + parallelVer - 1) / parallelVer;
	byte *p = dst;
	byte const *pY = Y;
	byte const *pU = U;
	byte const *pV = V;
	byte *pDeltaUY = (byte*)(pV - pU);
	//number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
	int src_pitch_exceed_Y = (w_par * parallelHor - width) * src_bpp;
	int dst_pitch_exceed = (w_par * parallelHor - width) * dst_bpp;
	int src_pitch_exceed_Y_vert = (h_par * parallelVer - height) * src_pitch_Y;
	int dst_pitch_exceed_vert = (h_par * parallelVer - height) * dst_pitch;
	//number of bytes left from the end of the line till the beginning of the next
	int src_pitch_left_Y = src_pitch_Y - width * src_bpp;
	int src_pitch_left_U = src_pitch_U - w_par * src_bpp;
	int dst_pitch_left = dst_pitch - width * dst_bpp;
	int src_p_Y = src_pitch_Y;
	int dst_p = dst_pitch;


	__asm
	{
		push esi
		push edi
		push eax
		//push ebx
		push ecx
		push edx

		//dst-pointer
		mov esi, p

		//Y-pointer
		mov edi, pY
		//U-pointer
		mov edx, pU
		////V-pointer
		//mov edx, pV

		pxor mm7, mm7
		movq mm6, temp
		movq mm4, temp255

		movaps xmm4, b2
		movaps xmm5, r3
		movaps xmm6, g2
		movaps xmm7, g3

		mov eax, h_par

		//for every line:
next_line:
		//for the case if height is not divisible by 2
		//branch prediction should work perfectly
		cmp eax, 1
		jne not_last_row

		sub esi, dst_pitch_exceed_vert
		sub edi, src_pitch_exceed_Y_vert

not_last_row:
		mov ecx, w_par

next_4_pixels:

		//for the case if width is not divisible by 2
		//branch prediction should work perfectly
		cmp ecx, 1
		jne not_last

		sub esi, dst_pitch_exceed
		sub edi, src_pitch_exceed_Y
not_last:
		//taking 4 Y values:
		movd mm2, [edi]
		pslld mm2, 16
		add edi, src_p_Y
		movd mm1, [edi]
		pslld mm1, 16
		psrld mm1, 16
		por mm2, mm1
		sub edi, src_p_Y
		//taking 1 U value:
		movd mm1, [edx]
		//taking 1 V value:
		push edx
		add edx, pDeltaUY
		movd mm0, [edx]
		pop edx

		//unpack to words:
		punpcklbw mm2, mm7
		punpcklbw mm1, mm7
		punpcklbw mm0, mm7

		psubsw mm0, short128
		psubsw mm1, short128

		//===================================
		//add your code here :)

		//transferring Y
		paddsw mm2, temp256
		movq mm3, mm2
		punpckhwd mm3, mm7
		punpcklwd mm2, mm7
		xorps xmm2, xmm2
		cvtpi2ps xmm2, mm3
		shufps xmm2, xmm2, 4Eh 
		cvtpi2ps xmm2, mm2
		subps xmm2, tempd256
		//now xmm2 contains four Y values

		//transferring U - copying 1 U value to 4
		paddsw mm1, temp256
		punpcklwd mm1, mm7
		xorps xmm1, xmm1
		cvtpi2ps xmm1, mm1
		shufps xmm1, xmm1, 00000000b 
		subps xmm1, tempd256
		//now xmm1 contains four U values

		//transferring V - copying 1 V value to 4
		paddsw mm0, temp256
		punpcklwd mm0, mm7
		xorps xmm0, xmm0
		cvtpi2ps xmm0, mm0
		shufps xmm0, xmm0, 00000000b 
		subps xmm0, tempd256
		//now xmm0 contains four V values

		//calculate R:
		movaps xmm3, xmm0
		mulps xmm3, xmm5
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//R
		cvtps2pi mm2, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm2, mm3

		//calculate G:
		movaps xmm3, xmm1
		mulps xmm3, xmm6
		mulps xmm0, xmm7
		addps xmm3, xmm0
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//G
		cvtps2pi mm1, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm1, mm3

		//calculate B:
		movaps xmm3, xmm1
		mulps xmm3, xmm4
		addps xmm3, xmm2

		shufps xmm3, xmm3, 0D8h

		//B
		cvtps2pi mm0, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm0, mm3

		////===================================

		pminsw mm0, mm4
		pminsw mm1, mm4
		pminsw mm2, mm4
		pmaxsw mm0, mm7
		pmaxsw mm1, mm7
		pmaxsw mm2, mm7

		//packing GGGGBBBB into mm0
		packuswb mm0, mm1

		//packing 0000RRRR into mm2
		packuswb mm2, mm7

		//forming first two pixels' joint register 00RRGGBB
		movq mm1, mm0
		pand mm1, mm6
		movq mm3, mm2
		pand mm3, mm6
		packuswb mm1, mm3

		//forming second two pixels' joint register 00RRGGBB
		movq mm5, mm6
		psllw mm5, 8
		pand mm0, mm5
		pand mm2, mm5
		psrlw mm0, 8
		psrlw mm2, 8
		packuswb mm0, mm2

		//storing first two pixels
		movq mm2, mm1
		pand mm2, mm6
		packuswb mm2, mm7
		pand mm1, mm5
		psrlw mm1, 8
		packuswb mm1, mm7

		//packing
		psll mm1, 40
		psll mm2, 40
		psrl mm1, 16
		psrl mm2, 40
		por mm1, mm2
		
        push edx

		add esi, dst_p
        movd [esi], mm1
        psrl mm1, 32
        movd edx, mm1
        mov [esi + 4], dx
		sub esi, dst_p

		//storing last two pixels
		movq mm2, mm0
		pand mm2, mm6
		packuswb mm2, mm7
		pand mm0, mm5
		psrlw mm0, 8
		packuswb mm0, mm7

		//packing
		psll mm0, 40
		psll mm2, 40
		psrl mm0, 16
		psrl mm2, 40
		por mm0, mm2
		
        movd [esi], mm0
        psrl mm0, 32
        movd edx, mm0
        mov [esi + 4], dx

        pop edx

		//next four pixels
		add esi, 6
		add edi, 2
		//add ebx, 1
		add edx, 1
		dec ecx
		jnz next_4_pixels

		//next line:
		add esi, dst_pitch_left
		add esi, dst_p
		add edi, src_pitch_left_Y
		add edi, src_p_Y
		//add ebx, src_pitch_left_U
		add edx, src_pitch_left_U
		dec eax
		jnz next_line

		emms

		pop edx
		pop ecx
		//pop ebx
		pop eax
		pop edi
		pop esi
	}
	//End SSE Section
	//////////////////////////////////////////////////////////////////////////
}

/**
**************************************************************************
* \brief Converts RGB24 to YV12
*
*  The function converts frame from RGB24 format into planar YV12, 
*  where Y plane has the size of the source frame, while each of U and V have
*  size reduced by 2 both by horizontal and by vertical
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch_Y        [IN] -  Line offset in Y plane buffer
* \param dst_pitch_U        [IN] -  Line offset in either U or V plane buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYV12_SSE(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch_Y, int dst_pitch_U)
{
	//////////////////////////////////////////////////////////////////////////
	//SSE Section
	gmCNVT_ALIGN short short128[] = {128, 128, 128, 128};					//shift for Y, U, V
	gmCNVT_ALIGN float y1[] = {0.299f, 0.299f, 0.299f, 0.299f};
	gmCNVT_ALIGN float y2[] = {0.587f, 0.587f, 0.587f, 0.587f};
	gmCNVT_ALIGN float y3[] = {0.114f, 0.114f, 0.114f, 0.114f};
	gmCNVT_ALIGN float u1[] = {-0.147f, -0.147f, -0.147f, -0.147f};
	gmCNVT_ALIGN float u2[] = {-0.289f, -0.289f, -0.289f, -0.289f};
	gmCNVT_ALIGN float u3[] = {0.436f, 0.436f, 0.436f, 0.436f};
	gmCNVT_ALIGN float v1[] = {0.615f, 0.615f, 0.615f, 0.615f};
	gmCNVT_ALIGN float v2[] = {-0.515f, -0.515f, -0.515f, -0.515f};
	gmCNVT_ALIGN float v3[] = {-0.100f, -0.100f, -0.100f, -0.100f};

	//number of simultaneously processed pixels
	int parallelHor = 2;
	int parallelVer = 2;
	//number of bytes from each src buffer corresponding to 1 pixel
	int src_bpp = 3;
	//number of bytes from each dst buffer corresponding to 1 pixel
	int dst_bpp = 1;
	//number of loop iterations per one row
	int w_par = (width + parallelHor - 1) / parallelHor;
	int h_par = (height + parallelVer - 1) / parallelVer;
	byte const *p = src;
	byte *pY = Y;
	byte *pU = U;
	byte *pV = V;
	byte *pDeltaUV = (byte*)(pV - pU);
	//number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
	int dst_pitch_exceed_Y = (w_par * parallelHor - width) * dst_bpp;
	int src_pitch_exceed = (w_par * parallelHor - width) * src_bpp;
	int dst_pitch_exceed_Y_vert = (h_par * parallelVer - height) * dst_pitch_Y;
	int src_pitch_exceed_vert = (h_par * parallelVer - height) * src_pitch;
	//number of bytes left from the end of the line till the beginning of the next
	int dst_pitch_left_Y = dst_pitch_Y - width * dst_bpp;
	int dst_pitch_left_U = dst_pitch_U - w_par * dst_bpp;
	int src_pitch_left = src_pitch - width * src_bpp;
	int dst_p_Y = dst_pitch_Y;
	int src_p = src_pitch;


	__asm
	{
		push esi
		push edi
		push eax
		//push ebx
		push ecx
		push edx

		//dst-pointer
		mov esi, p

		//Y-pointer
		mov edi, pY
		//U-pointer
		mov edx, pU
		////V-pointer
		//mov edx, pV

		pxor mm7, mm7

		movaps xmm5, y1
		movaps xmm6, y2
		movaps xmm7, y3

		mov eax, h_par

		//for every line:
next_line:
		//for the case if height is not divisible by 2
		//branch prediction should work perfectly
		cmp eax, 1
		jne not_last_row

		sub esi, src_pitch_exceed_vert
		sub edi, dst_pitch_exceed_Y_vert

not_last_row:
		mov ecx, w_par

next_4_pixels:

		//for the case if width is not divisible by 2
		//branch prediction should work perfectly
		cmp ecx, 1
		jne not_last

		sub esi, src_pitch_exceed
		sub edi, dst_pitch_exceed_Y
not_last:

		movq mm0, [esi]
		movq mm1, mm0
		psrl mm1, 24

		//taking both two pixels into mm0 (00RRGGBB)
		punpcklbw mm0, mm1

		add esi, src_p
		movq mm1, [esi]
		movq mm2, mm1
		psrl mm2, 24
		//taking next two pixels into mm1 (00RRGGBB)
		punpcklbw mm1, mm2
		sub esi, src_p

		//extracting 0000RRRR into mm2
		movq mm2, mm0
		punpckhbw mm2, mm1

		//extracting GGGGBBBB into mm0
		punpcklbw mm0, mm1

		//extracting 0R0R0R0R into mm2
		punpcklbw mm2, mm7

		//extracting 0G0G0G0G into mm1
		movq mm1, mm0
		punpckhbw mm1, mm7

		//extracting 0B0B0B0B into mm0
		punpcklbw mm0, mm7

		//===================================
		//add your code here :)

		//transferring RED
		movq mm3, mm2
		punpckhwd mm3, mm7
		punpcklwd mm2, mm7
		xorps xmm2, xmm2
		cvtpi2ps xmm2, mm3
		shufps xmm2, xmm2, 4Eh 
		cvtpi2ps xmm2, mm2
		//now xmm2 contains four RED values

		//transferring GREEN
		movq mm3, mm1
		punpckhwd mm3, mm7
		punpcklwd mm1, mm7
		xorps xmm1, xmm1
		cvtpi2ps xmm1, mm3
		shufps xmm1, xmm1, 4Eh 
		cvtpi2ps xmm1, mm1
		//now xmm1 contains four GREEN values

		//transferring BLUE
		movq mm3, mm0
		punpckhwd mm3, mm7
		punpcklwd mm0, mm7
		xorps xmm0, xmm0
		cvtpi2ps xmm0, mm3
		shufps xmm0, xmm0, 4Eh 
		cvtpi2ps xmm0, mm0
		//now xmm0 contains four BLUE values

		//calculate Y:
		movaps xmm3, xmm2
		mulps xmm3, xmm5
		movaps xmm4, xmm1
		mulps xmm4, xmm6
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, xmm7
		addps xmm3, xmm4

		shufps xmm3, xmm3, 0D8h

		//Y
		cvtps2pi mm2, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm2, mm3

		//calculate U:
		movaps xmm3, xmm2
		mulps xmm3, u1
		movaps xmm4, xmm1
		mulps xmm4, u2
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, u3
		addps xmm3, xmm4

		shufps xmm3, xmm3, 0D8h

		//U
		cvtps2pi mm1, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm1, mm3

		//calculate V:
		movaps xmm3, xmm2
		mulps xmm3, v1
		movaps xmm4, xmm1
		mulps xmm4, v2
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, v3
		addps xmm3, xmm4

		shufps xmm3, xmm3, 0D8h

		//V
		cvtps2pi mm0, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm0, mm3

		//converting to byte:
		paddw mm0, short128
		paddw mm1, short128
		packuswb mm2, mm2
		packuswb mm1, mm1
		packuswb mm0, mm0

		push eax

		//storing Y-1
		movq mm4, mm2
		movd eax, mm2
		mov [edi], ax
		//movd mm3, [edi]
		//psrl mm3, 16
		//psll mm2, 48
		//psll mm3, 16
		//psrl mm2, 48
		//por mm3, mm2
		//movd [edi], mm3

		//storing Y-2
		add edi, dst_p_Y
		psrl mm4, 16
		movd eax, mm4
		mov [edi], ax
		//movd [edi], mm4
		sub edi, dst_p_Y

		//storing U
		movd eax, mm1
		mov [edx], al
		//movd [edx], mm1

		//storing V
		//push edx
		add edx, pDeltaUV
		movd eax, mm0
		mov [edx], al
		//movd [edx], mm0
		sub edx, pDeltaUV
		//pop edx

		pop eax

		//////////////////////////////////////////////////////////////////////////

		//next four pixels
		add esi, 6
		add edi, 2
		//add ebx, 1
		add edx, 1
		dec ecx
		jnz next_4_pixels

		//next line:
		add esi, src_pitch_left
		add esi, src_p
		add edi, dst_pitch_left_Y
		add edi, dst_p_Y
		//add ebx, dst_pitch_left_U
		add edx, dst_pitch_left_U
		dec eax
		jnz next_line

		emms

		pop edx
		pop ecx
		//pop ebx
		pop eax
		pop edi
		pop esi
	}
	//End SSE Section
	//////////////////////////////////////////////////////////////////////////
}

/**
**************************************************************************
* \brief Converts RGB32 to Y
*
*  The function generates only Y plane from RGB32 format.
*
* \param src                [IN] -  Pointer to source frame in RGB32 format
* \param Y                  [OUT] -  Pointer to destination Y plane buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBAtoY_SSE(const byte *src, byte *Y, int width, int height, int src_pitch, int dst_pitch)
{
	//////////////////////////////////////////////////////////////////////////
	//SSE Section
	short short128[] = {128, 128, 128, 128};
	gmCNVT_ALIGN float y1[] = {0.299f, 0.299f, 0.299f, 0.299f};
	gmCNVT_ALIGN float y2[] = {0.587f, 0.587f, 0.587f, 0.587f};
	gmCNVT_ALIGN float y3[] = {0.114f, 0.114f, 0.114f, 0.114f};

	//number of simultaneously processed pixels
	int parallelNo = 4;
	//number of bytes from each src buffer corresponding to 1 pixel
	int src_bpp = 4;
	//number of bytes from each dst buffer corresponding to 1 pixel
	int dst_bpp = 1;
	int h = height;
	//number of loop iterations per one row
	int w_par = (width + parallelNo - 1) / parallelNo;
	byte const *p = src;
	byte *pY = Y;
	//number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
	int src_pitch_exceed = (w_par * parallelNo - width) * src_bpp;
	int dst_pitch_exceed = (w_par * parallelNo - width) * dst_bpp;
	//number of bytes left from the end of the line till the beginning of the next
	int src_pitch_left = src_pitch - width * src_bpp;
	int dst_pitch_left = dst_pitch - width * dst_bpp;

	__asm
	{
		push esi
		push edi
		push eax
		push ecx

		//src-pointer
		mov esi, p

		//Y-pointer
		mov edi, Y

		pxor mm7, mm7

		movaps xmm5, y1
		movaps xmm6, y2
		movaps xmm7, y3

		mov eax, h

		//for every line:
next_line:
		mov ecx, w_par

next_4_pixels:

		cmp ecx, 1
		jne not_last

		sub esi, src_pitch_exceed
		sub edi, dst_pitch_exceed

not_last:
		movq mm0, [esi]
		movq mm1, mm0
		psrl mm1, 32

		//taking both two pixels into mm0 (00RRGGBB)
		punpcklbw mm0, mm1

		movq mm1, [esi + 8]
		movq mm2, mm1
		psrl mm2, 32

		//taking next two pixels into mm1 (00RRGGBB)
		punpcklbw mm1, mm2

		//extracting 0000RRRR into mm2
		movq mm2, mm0
		punpckhbw mm2, mm1

		//extracting GGGGBBBB into mm0
		punpcklbw mm0, mm1

		//extracting 0R0R0R0R into mm2
		punpcklbw mm2, mm7

		//extracting 0G0G0G0G into mm1
		movq mm1, mm0
		punpckhbw mm1, mm7

		//extracting 0B0B0B0B into mm0
		punpcklbw mm0, mm7

		//===================================
		//add your code here :)

		//transferring RED
		movq mm3, mm2
		punpckhwd mm3, mm7
		punpcklwd mm2, mm7
		xorps xmm2, xmm2
		cvtpi2ps xmm2, mm3
		shufps xmm2, xmm2, 4Eh 
		cvtpi2ps xmm2, mm2
		//now xmm2 contains four RED values

		//transferring GREEN
		movq mm3, mm1
		punpckhwd mm3, mm7
		punpcklwd mm1, mm7
		xorps xmm1, xmm1
		cvtpi2ps xmm1, mm3
		shufps xmm1, xmm1, 4Eh 
		cvtpi2ps xmm1, mm1
		//now xmm1 contains four GREEN values

		//transferring BLUE
		movq mm3, mm0
		punpckhwd mm3, mm7
		punpcklwd mm0, mm7
		xorps xmm0, xmm0
		cvtpi2ps xmm0, mm3
		shufps xmm0, xmm0, 4Eh 
		cvtpi2ps xmm0, mm0
		//now xmm0 contains four BLUE values

		//calculate Y:
		movaps xmm3, xmm2
		mulps xmm3, xmm5
		movaps xmm4, xmm1
		mulps xmm4, xmm6
		addps xmm3, xmm4
		movaps xmm4, xmm0
		mulps xmm4, xmm7
		addps xmm3, xmm4

		shufps xmm3, xmm3, 0D8h

		//Y
		cvtps2pi mm2, xmm3
		shufps xmm3, xmm3, 4Eh
		cvtps2pi mm3, xmm3
		packssdw mm2, mm3

		//converting to byte:
		packuswb mm2, mm2

		movd [edi], mm2

		////===================================

		//next four pixels
		add esi, 16
		add edi, 4
		dec ecx
		jne next_4_pixels

		//next line:
		add esi, src_pitch_left
		add edi, dst_pitch_left
		dec eax
		jnz next_line

		emms

		pop ecx
		pop eax
		pop edi
		pop esi
	}
	//}
	//End SSE Section
	//////////////////////////////////////////////////////////////////////////
}

/**
**************************************************************************
* \brief Converts Y to RGB32
*
*  The function generates gray-level image in RGB32 format from Y plane.
*
* \param Y                  [IN] -  Pointer to Y plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YtoRGBA_SSE(const byte *Y, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	//////////////////////////////////////////////////////////////////////////
	//SSE Section
	gmCNVT_ALIGN unsigned char temp[] = {255, 0, 255, 0, 255, 0, 255, 0};

	//number of simultaneously processed pixels
	int parallelNo = 4;
	//number of bytes from each src buffer corresponding to 1 pixel
	int src_bpp = 1;
	//number of bytes from each dst buffer corresponding to 1 pixel
	int dst_bpp = 4;
	int h = height;
	//number of loop iterations per one row
	int w_par = (width + parallelNo - 1) / parallelNo;
	byte const *p = dst;
	byte const *pY = Y;
	//number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
	int src_pitch_exceed = (w_par * parallelNo - width) * src_bpp;
	int dst_pitch_exceed = (w_par * parallelNo - width) * dst_bpp;
	//number of bytes left from the end of the line till the beginning of the next
	int src_pitch_left = src_pitch - width * src_bpp;
	int dst_pitch_left = dst_pitch - width * dst_bpp;


	__asm
	{
		push esi
		push edi
		push eax
		push ecx

		//dst-pointer
		mov esi, p

		//Y-pointer
		mov edi, pY

		pxor mm7, mm7
		movq mm6, temp

		mov eax, h

		//for every line:
next_line:
		mov ecx, w_par

next_4_pixels:

		//for the case if width is not divisible by 4
		//branch prediction should work perfectly
		cmp ecx, 1
		jne not_last

		sub esi, dst_pitch_exceed
		sub edi, src_pitch_exceed
not_last:
		//taking 4 Y values:
		movd mm2, [edi]

		//unpack to words:
		punpcklbw mm2, mm7

		movq mm0, mm2
		movq mm1, mm2

		//packing GGGGBBBB into mm0
		packuswb mm0, mm1

		//packing 0000RRRR into mm2
		packuswb mm2, mm7

		//forming first two pixels' joint register 00RRGGBB
		movq mm1, mm0
		pand mm1, mm6
		movq mm3, mm2
		pand mm3, mm6
		packuswb mm1, mm3

		//forming second two pixels' joint register 00RRGGBB
		movq mm5, mm6
		psllw mm5, 8
		pand mm0, mm5
		pand mm2, mm5
		psrlw mm0, 8
		psrlw mm2, 8
		packuswb mm0, mm2

		//storing first two pixels
		movq mm2, mm0
		pand mm2, mm6
		packuswb mm2, mm7
		pand mm0, mm5
		psrlw mm0, 8
		packuswb mm0, mm7

		//storing last two pixels
		movq mm3, mm1
		pand mm3, mm6
		packuswb mm3, mm7
		pand mm1, mm5
		psrlw mm1, 8
		packuswb mm1, mm7

		//swap needed
		movq mm4, mm1
		movq mm1, mm2
		movq mm2, mm4

		//packing
		psll mm1, 32
		psll mm3, 32
		psrl mm3, 32
		por mm1, mm3

		movq [esi], mm1

		//packing
		psll mm0, 32
		psll mm2, 32
		psrl mm2, 32
		por mm0, mm2

		movq [esi + 8], mm0

		//next four pixels
		add esi, 16
		add edi, 4
		dec ecx
		jnz next_4_pixels

		//next line:
		add esi, dst_pitch_left
		add edi, src_pitch_left
		dec eax
		jnz next_line

		emms

		pop ecx
		pop eax
		pop edi
		pop esi
	}
	//End SSE Section
	//////////////////////////////////////////////////////////////////////////
}

/**
**************************************************************************
* \brief Converts RGB32 to RGB24
*
*  The function converts frame from RGB32 format into RGB24 format.
*
* \param src                [IN] -  Pointer to source frame in RGB32 format
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBAtoRGB_SSE(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
    //////////////////////////////////////////////////////////////////////////
    //SSE Section

    byte mask1b[] = {255, 255, 255, 0, 0, 0, 0, 0};
    byte mask2b[] = {0, 0, 0, 0, 255, 255, 255, 0};

    //number of simultaneously processed pixels
    int parallelNo = 4;
    //number of bytes from each src buffer corresponding to 1 pixel
    int src_bpp = 4;
    //number of bytes from each dst buffer corresponding to 1 pixel
    int dst_bpp = 3;
    int h = height;
    //number of loop iterations per one row
    int w_par = (width + parallelNo - 1) / parallelNo;
    byte const *p = src;
    byte *pd = dst;
    //number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
    int src_pitch_exceed = (w_par * parallelNo - width) * src_bpp;
    int dst_pitch_exceed = (w_par * parallelNo - width) * dst_bpp;
    //number of bytes left from the end of the line till the beginning of the next
    int src_pitch_left = src_pitch - width * src_bpp;
    int dst_pitch_left = dst_pitch - width * dst_bpp;

    __asm
    {
        push esi
        push edi
        push eax
        push ecx

        //src-pointer
        mov esi, p

        //dst-pointer
        mov edi, pd

        movq mm7, mask1b
        movq mm6, mask2b

        mov eax, h

        //for every line:
next_line:
        mov ecx, w_par

next_4_pixels:

        cmp ecx, 1
        jne not_last

        sub esi, src_pitch_exceed
        sub edi, dst_pitch_exceed

not_last:
        movq mm0, [esi]
        movq mm1, [esi + 8]

        movq mm2, mm0
        pand mm2, mm6
        pand mm0, mm7
        psrl mm2, 8
        por mm0, mm2

        movq mm2, mm1
        pand mm2, mm6
        pand mm1, mm7
        movq mm3, mm1
        psll mm3, 48
        por mm0, mm3

        movq [edi], mm0

        psrl mm1, 16
        psrl mm2, 24
        por mm1, mm2

        movd [edi + 8], mm1

        ////===================================

        //next four pixels
        add esi, 16
        add edi, 12
        dec ecx
        jne next_4_pixels

        //next line:
        add esi, src_pitch_left
        add edi, dst_pitch_left
        dec eax
        jnz next_line

        emms

        pop ecx
        pop eax
        pop edi
        pop esi
    }
    //}
    //End SSE Section
    //////////////////////////////////////////////////////////////////////////
}

/**
**************************************************************************
* \brief Converts RGB24 to RGB32
*
*  The function converts frame from RGB24 format into RGB32 format.
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoRGBA_SSE(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
    //////////////////////////////////////////////////////////////////////////
    //SSE Section

    byte mask1b[] = {255, 255, 255, 0, 0, 0, 0, 0};
    byte mask2b[] = {0, 0, 0, 255, 255, 255, 0, 0};
    byte mask3b[] = {0, 0, 0, 0, 0, 0, 255, 255};

    //number of simultaneously processed pixels
    int parallelNo = 4;
    //number of bytes from each src buffer corresponding to 1 pixel
    int src_bpp = 3;
    //number of bytes from each dst buffer corresponding to 1 pixel
    int dst_bpp = 4;
    int h = height;
    //number of loop iterations per one row
    int w_par = (width + parallelNo - 1) / parallelNo;
    byte const *p = src;
    byte *pd = dst;
    //number of bytes that exceed the basic width when processing w_par times by parallelNo pixels
    int src_pitch_exceed = (w_par * parallelNo - width) * src_bpp;
    int dst_pitch_exceed = (w_par * parallelNo - width) * dst_bpp;
    //number of bytes left from the end of the line till the beginning of the next
    int src_pitch_left = src_pitch - width * src_bpp;
    int dst_pitch_left = dst_pitch - width * dst_bpp;

    __asm
    {
        push esi
        push edi
        push eax
        push ecx

        //src-pointer
        mov esi, p

        //dst-pointer
        mov edi, pd

        movq mm7, mask1b
        movq mm6, mask2b
        movq mm5, mask3b

        mov eax, h

        //for every line:
next_line:
        mov ecx, w_par

next_4_pixels:

        cmp ecx, 1
        jne not_last

        sub esi, src_pitch_exceed
        sub edi, dst_pitch_exceed

not_last:
        movq mm0, [esi]
        movd mm1, [esi + 8]

        movq mm2, mm0
        movq mm3, mm0
        pand mm2, mm6
        pand mm3, mm7
        psll mm2, 8
        por mm3, mm2
        movq [edi], mm3

        psrl mm0, 48
        psll mm1, 16
        por mm0, mm1
        movq mm3, mm0
        pand mm0, mm6
        pand mm3, mm7
        psll mm0, 8
        por mm3, mm0
        movq [edi + 8], mm3

        ////===================================

        //next four pixels
        add esi, 12
        add edi, 16
        dec ecx
        jne next_4_pixels

        //next line:
        add esi, src_pitch_left
        add edi, dst_pitch_left
        dec eax
        jnz next_line

        emms

        pop ecx
        pop eax
        pop edi
        pop esi
    }
    //}
    //End SSE Section
    //////////////////////////////////////////////////////////////////////////
}

//#endif
#else

/**
**************************************************************************
* \brief Disables SSE functions
*
*  This is just a stub function to provide compatibility with x64 compiling environment
*
* \return True if SSE were enabled. False otherwise.
*/
bool gmColorSpace::DisableSSE()
{
    return false;
}

/**
**************************************************************************
* \brief Resets knowledge about SSE support
*
*  This is just a stub function to provide compatibility with x64 compiling environment
*
*  Original function resets the variable that is responsible for SSE support.
*  Before the next call to conversion function support will be tested.
*
* \return None.
*/
void gmColorSpace::ResetSSE()
{
    
}

#endif



/**
**************************************************************************
* \brief Converts RGB32 to Y
*
*  The function generates only Y plane from RGB32 format.
*
* \param src                [IN] -  Pointer to source frame in RGB32 format
* \param Y                  [OUT] -  Pointer to destination Y plane buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBAtoY_c(const byte *src, byte *Y, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j, y, r, g, b;
	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
			r = src[2];
			g = src[1];
			b = src[0];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			if (y > 255) y = 255; else if(y < 0) y = 0;
			Y[j] = y;
			src += 4;
		}
		src += src_pitch - 4 * width;
		Y += dst_pitch;
	}
}

/**
**************************************************************************
* \brief Converts RGB32 to RGB24
*
*  The function converts frame from RGB32 format into RGB24 format.
*
* \param src                [IN] -  Pointer to source frame in RGB32 format
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBAtoRGB_c(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j;
	byte *pd;
	byte const *ps;
	pd = dst;
	ps = src;
	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
			memcpy(pd, ps, 3);
			ps += 4;
			pd += 3;
		}
		ps += src_pitch - 4 * width;
		pd += dst_pitch - 3 * width;
	}
}

/**
**************************************************************************
* \brief Converts RGB32 to YUV
*
*  The function converts frame from RGB32 format into planar uniform YUV (equal size).
*
* \param src                [IN] -  Pointer to source frame in RGB32 format
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in either Y,U,V plane buffer
* 
* \return None.
*/
void gmColorSpace::RGBAtoYUV_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j, y, u, v, r, g, b;
	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
			r = src[2];
			g = src[1];
			b = src[0];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			u = (int)(r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			v = (int)(r * 0.615f + g * (-0.515f) + b * (-0.100f) + 128.5f);
			if (y > 255) y = 255; else if(y < 0) y = 0;
			if (u > 255) u = 255; else if(u < 0) u = 0;
			if (v > 255) v = 255; else if(v < 0) v = 0;
			Y[j] = y;
			U[j] = u;
			V[j] = v;
			src += 4;
		}
		src += src_pitch - 4 * width;
		Y += dst_pitch;
		U += dst_pitch;
		V += dst_pitch;
	}
}

/**
**************************************************************************
* \brief Converts RGB24 to RGB32
*
*  The function converts frame from RGB24 format into RGB32 format.
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoRGBA_c(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j;
	byte *pd;
	byte const *ps;
	pd = dst;
	ps = src;
	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
			memcpy(pd, ps, 3);
			pd[3] = 0;
			ps += 3;
			pd += 4;
		}
		ps += src_pitch - 3 * width;
		pd += dst_pitch - 4 * width;
	}
}

/**
**************************************************************************
* \brief Converts YV16 to YUY2
*
*  The function converts frame from YV16 format into YUY2 format.
*
* \param sy                 [IN] -  Pointer to source frame Y component buffer
* \param su                 [IN] -  Pointer to source frame U component buffer
* \param sv                 [IN] -  Pointer to source frame V component buffer
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YV16toYUY2_c(const byte *sy, const byte *su, const byte *sv, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j;
	byte *pd;
	byte const *psy, *psu, *psv;
	pd = dst;
	psy = sy;
	psv = sv;
	psu = su;
	for (i = 0; i < height; i++)
	{
		for(j = 0; j < width; j+=2)
		{
			*(pd++) = *(psy++);
			*(pd++) = *(psu++);
			*(pd++) = *(psy++);
			*(pd++) = *(psv++);
		}
		psy += src_pitch - width;
		psu += src_pitch / 2 - width / 2;
		psv += src_pitch / 2 - width / 2;
		pd  += dst_pitch - width*2;
	}
}

#ifdef gmSSE_COMPILER_SUPPORT
/**
**************************************************************************
* \brief Converts YV16 to YUY2
*
*  The function converts frame from YV16 format into YUV format.
*
* \param sy                 [IN] -  Pointer to source frame Y component buffer
* \param su                 [IN] -  Pointer to source frame U component buffer
* \param sv                 [IN] -  Pointer to source frame V component buffer
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YV16toYUV_c(const byte *Y, const byte *U, const byte *V, byte *dY, byte *dU, byte *dV, int width, int height, int src_pitch_Y, int src_pitch_U, int dst_pitch_Y)
{
	int i, j;

	byte const *pY = Y;
	byte const *pU = U;
	byte const *pV = V;
	byte *pdY = dY;
	byte *pdU = dU;
	byte *pdV = dV;

	// copy Y plane
	for(i = 0; i < height; i++)
	{
		memcpy(pdY, pY, src_pitch_Y);
		pY += src_pitch_Y;
		pdY += dst_pitch_Y;
	}
	// copy Y and V planes, multiplying by 2
	for(i = 0; i < height; i++)
		for(j = 0; j < width; j++)
		{
			pdU[i*dst_pitch_Y + j] = pU[i*src_pitch_U + j/2];
			pdV[i*dst_pitch_Y + j] = pV[i*src_pitch_U + j/2];
		}
}
#endif // gmSSE_COMPILER_SUPPOR

#ifdef gmSSE_COMPILER_SUPPORT
/**
**************************************************************************
* \brief Converts YUV to YV16
*
*  The function converts frame from YUV format into YV16 format.
*  Store YV16 format: YYY...UUU...VVV, where vertical size of all components are the same,
*  but horizontal sizes of color components are two times less than that of luma component.
*
* \param sy                 [IN] -  Pointer to source frame Y component buffer
* \param su                 [IN] -  Pointer to source frame U component buffer
* \param sv                 [IN] -  Pointer to source frame V component buffer
* \param dy                 [OUT] -  Pointer to destination frame Y component buffer
* \param du                 [OUT] -  Pointer to destination frame U component buffer
* \param dv                 [OUT] -  Pointer to destination frame V component buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUVtoYV16_c(const byte *sy, const byte *su, const byte *sv, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch)
{
    int i, j;
    const byte *psy, *psu, *psv;
    psy = sy;
    psv = sv;
    psu = su;
    byte *pdy, *pdu, *pdv;
    pdy = dy;
    pdv = dv;
    pdu = du;

    for (i = 0; i < height; i++)
    {
        for(j = 0; j < width; j += 2)
        {
            pdv[j/2] = psv[j];
            pdu[j/2] = psu[j];
        }

        memcpy(pdy, psy, width);
        pdy += dst_pitch;
        psy += src_pitch;

        pdv += dst_pitch / 2;
        pdu += dst_pitch / 2;

        psv += src_pitch;
        psu += src_pitch;
    }
}
#endif // gmSSE_COMPILER_SUPPORT

/**
**************************************************************************
* \brief Converts YUY2 to YV16
*
*  The function converts frame from YUY2 format into YV16 format.
*  Store YV16 format: YYY...UUU...VVV, where vertical size of all components are the same,
*  but horizontal sizes of color components are two times less than that of luma component.
*
* \param src                [IN] -  Pointer to source frame in YV16 format
* \param dy                 [OUT] -  Pointer to destination frame Y component buffer
* \param du                 [OUT] -  Pointer to destination frame U component buffer
* \param dv                 [OUT] -  Pointer to destination frame V component buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUY2toYV16_c(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j;
	byte *pdy, *pdu, *pdv;
	byte const *ps;
	ps = src;
	pdy = dy;
	pdv = dv;
	pdu = du;

	for (i = 0; i < height; i++)
	{
		for(j = 0; j < width; j+=2)
		{
			*(pdy++) = *(ps++);
			*(pdu++) = *(ps++);
			*(pdy++) = *(ps++);
			*(pdv++) = *(ps++);
		}
		ps  += src_pitch - width*2;
		pdy += dst_pitch - width;
		pdu += dst_pitch / 2 - width / 2;
		pdv += dst_pitch / 2 - width / 2;
	}
}

/**
**************************************************************************
* \brief Converts RGB24 to RGB
*
*  The function converts frame from RGB24 format into RGB format.
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dr                 [OUT] -  Pointer to destination frame red component buffer
* \param dg                 [OUT] -  Pointer to destination frame green component buffer
* \param db                 [OUT] -  Pointer to destination frame blue component buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGB24toRGB_c(const byte *src, byte *dr, byte *dg, byte *db, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j;
	byte *pdr, *pdg, *pdb;
	byte const *ps;
	ps = src;
	pdr = dr;
	pdg = dg;
	pdb = db;

	for (i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			*(pdb++) = *(ps++);
			*(pdg++) = *(ps++);
			*(pdr++) = *(ps++);
		}
		ps  += src_pitch - width*3;
		pdr += dst_pitch - width;
		pdg += dst_pitch - width;
		pdb += dst_pitch - width;
	}
}

/**
**************************************************************************
* \brief Converts RGB to RGB24
*
*  The function converts frame from RGB format into RGB24 format.
*
* \param sr                 [IN] -  Pointer to source frame red component buffer
* \param sg                 [IN] -  Pointer to source frame green component buffer
* \param sb                 [IN] -  Pointer to source frame blue component buffer
* \param dst                [OUT] -  Pointer to destination frame in RGB24 format
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoRGB24_c(const byte *sr, const byte *sg, const byte *sb, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j;
	byte *pd;
	byte const *psr, *psg, *psb;
	pd = dst;
	psr = sr;
	psg = sg;
	psb = sb;

	for (i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			*(pd++) = *(psb++);
			*(pd++) = *(psg++);
			*(pd++) = *(psr++);
		}
		pd  += dst_pitch - width*3;
		psr += src_pitch - width;
		psg += src_pitch - width;
		psb += src_pitch - width;
	}
}

/**
**************************************************************************
* \brief Converts RGB24 to RGB
*
*  The function converts frame from RGB24 format into lsYUVpl format.
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dy                 [OUT] -  Pointer to destination frame Y component buffer
* \param du                 [OUT] -  Pointer to destination frame U component buffer
* \param dv                 [OUT] -  Pointer to destination frame V component buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGB24tolsYUVpl_c(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch)
{
#define COMPR ps[2]
#define COMPG ps[1]
#define COMPB ps[0]
	int i, j;
	byte *pdy;
	char *pdu, *pdv;
	byte const *ps;
	ps = src;
	pdy = dy;
	pdu = (char*)du;
	pdv = (char*)dv;

	for (i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			*(pdy++) = COMPG;
			*(pdv++) = COMPB - COMPG;
			*(pdu++) = COMPR - COMPG;
			ps+=3;
//			*(pdu++) = *(ps++);
//			*(pdv++) = *(ps++);
//			*(pdy++) = *(ps++);
		}
		ps  += src_pitch - width*3;
		pdy += dst_pitch - width;
		pdu += dst_pitch - width;
		pdv += dst_pitch - width;
	}
#undef COMPR
#undef COMPG
#undef COMPB
}

/**
**************************************************************************
* \brief Converts RGB to RGB24
*
*  The function converts frame from lsYUVpl format into RGB24 format.
*
* \param sy                 [IN] -  Pointer to source frame Y component buffer
* \param su                 [IN] -  Pointer to source frame U component buffer
* \param sv                 [IN] -  Pointer to source frame V component buffer
* \param dst                [OUT] -  Pointer to destination frame in RGB24 format
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::lsYUVpltoRGB24_c(const byte *sy, const byte *su, const byte *sv, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
#define COMPR pd[2]
#define COMPG pd[1]
#define COMPB pd[0]
	int i, j;
	byte *pd;
	byte const *psy;
	char const *psu, *psv;
	pd = dst;
	psy = sy;
	psu = (const char*)su;
	psv = (const char*)sv;

	for (i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			COMPG = *psy;
			COMPR = *psu + *psy;
			COMPB = *psv + *psy;
			pd+=3; psy++; psu++; psv++;
//			*(pd++) = *(psu++);
//			*(pd++) = *(psv++);
//			*(pd++) = *(psy++);
		}
		pd  += dst_pitch - width*3;
		psy += src_pitch - width;
		psu += src_pitch - width;
		psv += src_pitch - width;
	}
#undef COMPR
#undef COMPG
#undef COMPB
}

/**
**************************************************************************
* \brief Converts YUV to RGB24
*
*  The function converts frame from planar YUV format into RGB24 format.
*  All planes are of equal size.
*
* \param Y                  [IN] -  Pointer to Y plane
* \param U                  [IN] -  Pointer to U plane
* \param V                  [IN] -  Pointer to V plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUVtoRGB_c(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j, y, u, v, r, g, b;
	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
			u = U[j] - 128;
			v = V[j] - 128;
			y = Y[j];
			r = (int)(y + 1.14 * v + 0.5);
			g = (int)(y - 0.395 * u - 0.581 * v + 0.5);
			b = (int)(y + 2.032 * u + 0.5);
			if (r > 255) r = 255; else if(r < 0) r = 0;
			if (g > 255) g = 255; else if(g < 0) g = 0;
			if (b > 255) b = 255; else if(b < 0) b = 0;
			dst[2] = r;
			dst[1] = g;
			dst[0] = b;
			dst += 3;
		}
		dst += dst_pitch - 3 * width;
		Y += src_pitch;
		U += src_pitch;
		V += src_pitch;
	}
}

/**
**************************************************************************
* \brief Converts RGB24 to YUY2
*
*  The function converts frame from RGB24 format into interleaved YUY2 format, 
*  where one U value is shared between two adjacent horizontally pixels, the same 
*  thing is done with V component. Store format: YUYVYUYV...
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYUY2_c(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j, ju;
	int r, g, b, y, ua, va;
	float u[2], v[2];
	int w2 = width & ~1, wleft = width & 1;
	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < w2; j += 2) 
		{
			ju = j << 1;
			r = src[2];
			g = src[1];
			b = src[0];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			u[0] = (r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			v[0] = (r * 0.615f + g * (-0.515f) + b * (-0.100f) + 128.5f);
			if (y > 255) y = 255; else if(y < 0) y = 0;
			dst[ju] = y;
			src += 3;

			r = src[2];
			g = src[1];
			b = src[0];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			u[1] = (r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			v[1] = (r * 0.615f + g * (-0.515f) + b * (-0.100f) + 128.5f);
			if (y > 255) y = 255; else if(y < 0) y = 0;
			dst[ju + 2] = y;
			ua = (int)((u[0] + u[1]) / 2.0f);
			va = (int)((v[0] + v[1]) / 2.0f);
			if (ua > 255) ua = 255; else if(ua < 0) ua = 0;
			if (va > 255) va = 255; else if(va < 0) va = 0;
			dst[ju + 1] = ua;
			dst[ju + 3] = va;
			src += 3;
		}
		if (wleft) 
		{
			j = width - 1;
			ju = j << 1;
			r = src[2];
			g = src[1];
			b = src[0];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			ua = (int)(r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			if (y > 255) y = 255; else if(y < 0) y = 0;
			if (ua > 255) ua = 255; else if(ua < 0) ua = 0;
			dst[ju] = y;
			dst[ju + 1] = ua;
		}
		src += src_pitch - 3 * w2;
		dst += dst_pitch;
	}
}

/**
**************************************************************************
* \brief Converts RGB24 to YV16
*
*  The function converts frame from RGB24 format into planar YV16 format. 
*  Store format: YYY...UUU...VVV, where vertical size of all components are the same,
*  but horizontal sizes of color components are two times less than that of luma component.
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param dy                 [OUT] -  Pointer to destination frame Y component buffer
* \param du                 [OUT] -  Pointer to destination frame U component buffer
* \param dv                 [OUT] -  Pointer to destination frame V component buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch        [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYV16_c(const byte *src, byte *dy, byte *du, byte *dv, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j;
	int r, g, b, y, ua, va;
	float u[2], v[2];
	int w2 = width & ~1, wleft = width & 1;

	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < w2; j += 2) 
		{
			//ju = j << 1;
			r = src[2];
			g = src[1];
			b = src[0];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			u[0] = (r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			v[0] = (r * 0.615f + g * (-0.515f) + b * (-0.100f) + 128.5f);
			
			if (y > 255) y = 255; else if(y < 0) y = 0;
			*(dy++) = y;

			src += 3;

			r = src[2];
			g = src[1];
			b = src[0];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			u[1] = (r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			v[1] = (r * 0.615f + g * (-0.515f) + b * (-0.100f) + 128.5f);
			
			if (y > 255) y = 255; else if(y < 0) y = 0;
			*(dy++) = y;

			ua = (int)((u[0] + u[1]) / 2.0f);
			va = (int)((v[0] + v[1]) / 2.0f);
			if (ua > 255) ua = 255; else if(ua < 0) ua = 0;
			if (va > 255) va = 255; else if(va < 0) va = 0;
			*(du++) = ua;
			*(dv++) = va;

			src += 3;
		}

		if (wleft) 
		{
			//j = width - 1;
			//ju = j << 1;
			r = src[2];
			g = src[1];
			b = src[0];

			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			ua = (int)(r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			if (y > 255) y = 255; else if(y < 0) y = 0;
			if (ua > 255) ua = 255; else if(ua < 0) ua = 0;

			*(dy++) = y;
			*(du++) = ua;
		}

		src += src_pitch - 3 * w2;
		dy += dst_pitch - (w2 + wleft);
		du += dst_pitch / 2 - (w2 / 2 + wleft);
		dv += dst_pitch / 2 - w2 / 2;
	}
}
/**
**************************************************************************
* \brief Converts RGB24 to YCbYCr
*
*  The function converts frame from RGB24 format into YCbYCr format, 
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination frame Y buffer
* \param Cb                 [OUT] -  Pointer to destination frame Cb buffer
* \param Cr                 [OUT] -  Pointer to destination frame Cr buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYCbCr_c(const byte *src, byte *Y, byte *Cb, byte *Cr, int width, int height, int src_pitch, int dst_pitch)
{
    int i, j, y, cr, cb, r, g, b;

    for (i = 0; i < height; i++) 
    {
        for (j = 0; j < width; j++) 
        {
            r = src[2];
            g = src[1];
            b = src[0];

            y  = (int)(r *     306   + g *     601   + b *     117           ) >> 10;
            cb = (int)(r * (-0.148f) + g * (-0.291f) + b *   0.439f  + 128.5f);
            cr = (int)(r *   0.439f  + g * (-0.368f) + b * (-0.071f) + 128.5f);
            y  = CLIP(y , 0, 255);
            cb = CLIP(cb, 0, 255);
            cr = CLIP(cr, 0, 255);

            Y[j]  = y;
            Cb[j] = cb;
            Cr[j] = cr;
            src += 3;
        }
        src += src_pitch - 3 * width;
        Y   += dst_pitch;
        Cb  += dst_pitch;
        Cr  += dst_pitch;
    }
}


/**
**************************************************************************
* \brief Converts RGB24 to Yrg
*
*  The function converts frame from RGB24 format into Yrg format, 
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination frame Y buffer
* \param r                  [OUT] -  Pointer to destination frame r buffer
* \param g                  [OUT] -  Pointer to destination frame g buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYrg_c(const byte *src, byte *Y, byte *r, byte *g, int width, int height, int src_pitch, int dst_pitch)
{
    int i, j, y_, r_, g_, R, G, B;

    for (i = 0; i < height; i++) 
    {
        for (j = 0; j < width; j++) 
        {
            R = src[2];
            G = src[1];
            B = src[0];

            y_ = (R * 306  + G * 601  + B * 117) >> 10;

            r_ = (R * 255 / (R + G + B + 1));
            g_ = (G * 255 / (R + G + B + 1));

            Y[j] = y_;
            r[j] = r_;
            g[j] = g_;
            src += 3;
        }
        src += src_pitch - 3 * width;
        Y   += dst_pitch;
        r   += dst_pitch;
        g   += dst_pitch;
    }
}


/**
**************************************************************************
* \brief Converts YUV to RGB32
*
*  The function converts frame from planar YUV format into RGB32 format.
*  All planes are of equal size.
*
* \param Y                  [IN] -  Pointer to Y plane
* \param U                  [IN] -  Pointer to U plane
* \param V                  [IN] -  Pointer to V plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUVtoRGBA_c(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j, y, u, v, r, g, b;
	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
			u = U[j] - 128;
			v = V[j] - 128;
			y = Y[j];
			r = (int)(y + 1.14 * v + 0.5);
			g = (int)(y - 0.395 * u - 0.581 * v + 0.5);
			b = (int)(y + 2.032 * u + 0.5);
			if (r > 255) r = 255; else if(r < 0) r = 0;
			if (g > 255) g = 255; else if(g < 0) g = 0;
			if (b > 255) b = 255; else if(b < 0) b = 0;
			dst[2] = r;
			dst[1] = g;
			dst[0] = b;
			dst += 4;
		}
		dst += dst_pitch - 4 * width;
		Y += src_pitch;
		U += src_pitch;
		V += src_pitch;
	}
}

/**
**************************************************************************
* \brief Converts RGB24 to YUV
*
*  The function converts frame from RGB24 format into planar uniform YUV (equal size).
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in either Y,U,V plane buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYUV_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j, y, u, v, r, g, b;
	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
			r = src[2];
			g = src[1];
			b = src[0];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			u = (int)(r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			v = (int)(r * 0.615f + g * (-0.515f) + b * (-0.100f) + 128.5f);
			if (y > 255) y = 255; else if(y < 0) y = 0;
			if (u > 255) u = 255; else if(u < 0) u = 0;
			if (v > 255) v = 255; else if(v < 0) v = 0;
			Y[j] = y;
			U[j] = u;
			V[j] = v;
			src += 3;
		}
		src += src_pitch - 3 * width;
		Y += dst_pitch;
		U += dst_pitch;
		V += dst_pitch;
	}
}

/**
**************************************************************************
* \brief Converts Y to RGB32
*
*  The function generates gray-level image in RGB32 format from Y plane.
*
* \param Y                  [IN] -  Pointer to Y plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YtoRGBA_c(const byte *Y, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j, y;
	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
			y = Y[j];
			dst[2] = y;
			dst[1] = y;
			dst[0] = y;
			dst += 4;
		}
		dst += dst_pitch - 4 * width;
		Y += src_pitch;
	}
}

/**
**************************************************************************
* \brief Converts YV12 to RGB24
*
*  The function converts frame from planar YV12 format into RGB24 format.
*  U and V planes have size reduced by two respectively to Y (both horizontally 
*  and vertically).
*
* \param Y                  [IN] -  Pointer to Y plane
* \param U                  [IN] -  Pointer to U plane
* \param V                  [IN] -  Pointer to V plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch_Y        [IN] -  Line offset in the Y plane buffer
* \param src_pitch_U        [IN] -  Line offset in the either U or V plane buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YV12toRGB_c(const byte *Y, const byte *U, const byte *V, byte *dst, int width, int height, int src_pitch_Y, int src_pitch_U, int dst_pitch)
{
	int i, j, j3;
	int y, u, v, r, g, b;
	byte const *pY = Y;
	byte const *pU = U;
	byte const *pV = V;
	byte *pd = dst;
	int h2 = (height + 1) / 2;
	int w2 = (width + 1) / 2;
	int exceed_w = w2 * 2 - width;
	int exceed_h = h2 * 2 - height;
	for (i = 0; i < h2; i++) 
	{
		if (i == h2 - 1) 
		{
			pd -= exceed_h * dst_pitch;
			pY -= exceed_h * src_pitch_Y;
		}
		for (j = 0; j < w2; j++) 
		{
			if (j == w2 - 1) 
			{
				pd -= exceed_w * 3;
				pY -= exceed_w;
			}
			j3 = j * 6;
			u = pU[j] - 128;
			v = pV[j] - 128;
			y = pY[j << 1];
			r = (int)(y + 1.14 * v + 0.5);
			g = (int)(y - 0.395 * u - 0.581 * v + 0.5);
			b = (int)(y + 2.032 * u + 0.5);
			if(r > 255) r = 255; else if(r < 0) r = 0;
			if(g > 255) g = 255; else if(g < 0) g = 0;
			if(b > 255) b = 255; else if(b < 0) b = 0;
			pd[j3] = (byte)b;
			pd[j3 + 1] = (byte)g;
			pd[j3 + 2] = (byte)r;
			y = pY[(j << 1) + 1];
			r = (int)(y + 1.14 * v + 0.5);
			g = (int)(y - 0.395 * u - 0.581 * v + 0.5);
			b = (int)(y + 2.032 * u + 0.5);
			if(r > 255) r = 255; else if(r < 0) r = 0;
			if(g > 255) g = 255; else if(g < 0) g = 0;
			if(b > 255) b = 255; else if(b < 0) b = 0;
			pd[j3 + 3] = (byte)b;
			pd[j3 + 4] = (byte)g;
			pd[j3 + 5] = (byte)r;
			pd += dst_pitch;
			pY += src_pitch_Y;
			y = pY[j << 1];
			r = (int)(y + 1.14 * v + 0.5);
			g = (int)(y - 0.395 * u - 0.581 * v + 0.5);
			b = (int)(y + 2.032 * u + 0.5);
			if(r > 255) r = 255; else if(r < 0) r = 0;
			if(g > 255) g = 255; else if(g < 0) g = 0;
			if(b > 255) b = 255; else if(b < 0) b = 0;
			pd[j3] = (byte)b;
			pd[j3 + 1] = (byte)g;
			pd[j3 + 2] = (byte)r;
			y = pY[(j << 1) + 1];
			r = (int)(y + 1.14 * v + 0.5);
			g = (int)(y - 0.395 * u - 0.581 * v + 0.5);
			b = (int)(y + 2.032 * u + 0.5);
			if(r > 255) r = 255; else if(r < 0) r = 0;
			if(g > 255) g = 255; else if(g < 0) g = 0;
			if(b > 255) b = 255; else if(b < 0) b = 0;
			pd[j3 + 3] = (byte)b;
			pd[j3 + 4] = (byte)g;
			pd[j3 + 5] = (byte)r;
			pd -= dst_pitch;
			pY -= src_pitch_Y;
		}
		pd += dst_pitch * 2 + exceed_w * 3;
		pY += src_pitch_Y * 2 + exceed_w;
		pU += src_pitch_U;
		pV += src_pitch_U;
	}
}

/**
**************************************************************************
* \brief Converts YV12 to YUV
*
*  The function converts frame from planar YV12 format into planar YUV format.
*  U and V planes have size reduced by two respectively to Y (both horizontally 
*  and vertically).
*
* \param Y                  [IN] -  Pointer to Y plane
* \param U                  [IN] -  Pointer to U plane
* \param V                  [IN] -  Pointer to V plane
* \param dY                 [IN] -  Pointer to Y plane
* \param dU                 [IN] -  Pointer to U plane
* \param dV                 [IN] -  Pointer to V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch_Y        [IN] -  Line offset in the Y plane buffer
* \param src_pitch_U        [IN] -  Line offset in the either U or V plane buffer
* \param dst_pitch_Y          [IN] -  Line offset in each destination plane
* 
* \return None.
*/
void gmColorSpace::YV12toYUV_c(const byte *Y, const byte *U, const byte *V, byte *dY, byte *dU, byte *dV, int width, int height, int src_pitch_Y, int src_pitch_U, int dst_pitch_Y)
{
	int i, j;

	byte const *pY = Y;
	byte const *pU = U;
	byte const *pV = V;
	byte *pdY = dY;
	byte *pdU = dU;
	byte *pdV = dV;

	int h2 = (height + 1) / 2;
	int w2 = (width + 1) / 2;
	
	// copy Y plane
	for(i = 0; i < height; i++)
	{
		memcpy(pdY, pY, src_pitch_Y);
		pY += src_pitch_Y;
		pdY += dst_pitch_Y;
	}
	// copy Y and V planes, multiplying by 2
	for(i = 0; i < height; i++)
		for(j = 0; j < width; j++)
		{
			pdU[i*dst_pitch_Y + j] = pU[(i/2)*src_pitch_U + (j/2)];
			pdV[i*dst_pitch_Y + j] = pV[(i/2)*src_pitch_U + (j/2)];
		}
}



/**
**************************************************************************
* \brief Converts Y410 to YUV
*
*  The function converts frame from packed Y410 format into planar YUV format.
*  WARNING: this function is probably had not been tested.
*
* \param src                [IN] -  Pointer to source frame
* \param y                  [IN] -  Pointer to Y plane
* \param u                  [IN] -  Pointer to U plane
* \param v                  [IN] -  Pointer to V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param dstPitchY          [IN] -  Line offset in destination frame
* 
* \return None.
*/
void gmColorSpace::Y410toYUV_c(const byte *src, byte *y, byte *u, byte *v, int width, int height, int dstPitchY)
{
	byte yuvData[12];

	for(int i = 0; i < height; i++)
    {
		for(int j = 0; j < width; j+=6)
        {
			int n;
			for (n = 0; n < 4; n++)
            {
				yuvData[3*n] = ((*src << 2) | (*(src + 1) >> 6)) >> 2;
				yuvData[3*n + 1] = (((*(src + 1) & 0x3F) << 4) | (*(src + 2) >> 4)) >> 2;
				yuvData[3*n + 2] = (((*(src + 2) & 0x0F) << 6) | (*(src + 3) >> 2)) >> 2;
				src += 4;
			}
			for (n = 0; n < 3; n++)
            {
				y[j + 2*n] = yuvData[4*n + 1];
				u[j + 2*n] = yuvData[4*n];
				v[j + 2*n] = yuvData[4*n + 2];

				y[j + 2*n + 1] = yuvData[4*n + 3];
				u[j + 2*n + 1] = yuvData[4*n];
				v[j + 2*n + 1] = yuvData[4*n + 2];
			}
		}

		y += dstPitchY;
		u += dstPitchY;
		v += dstPitchY;
	}
}

/**
**************************************************************************
* \brief Converts Y416 to YUV
*
*  The function converts frame from packed Y410 format into planar YUV format.
*  WARNING: this function is probably had not been tested.
*
* \param src                [IN] -  Pointer to source frame
* \param y                  [IN] -  Pointer to Y plane
* \param u                  [IN] -  Pointer to U plane
* \param v                  [IN] -  Pointer to V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param dstPitchY          [IN] -  Line offset in destination frame
* 
* \return None.
*/
void gmColorSpace::Y416toYUV_c(const byte *src, byte *y, byte *u, byte *v, int width, int height, int dstPitchY)
{
	for(int i = 0; i < height; i++)
    {
		for(int j = 0; j < width; j+=2)
        {
			u[j] = ((*src << 8) + *(src + 1)) >> 8;
			u[j+1] = u[j];
			y[j] = ((*(src + 2) << 8) + *(src + 3)) >> 8;
			v[j] = ((*(src + 4) << 8) + *(src + 5)) >> 8;
			v[j+1] = v[j];
			y[j+1] = ((*(src + 6) << 8) + *(src + 7)) >> 8;

			src += 8;
		}

		y += dstPitchY;
		u += dstPitchY;
		v += dstPitchY;
	}
}

/**
**************************************************************************
* \brief Converts P01X to YUV
*
*  The function converts frame from planar P01X format family into planar YUV
*  format. The format is taken from h264 developers site. It has full Y plane,
*  followed by u and v planes decreased in both dimensions twice. Each component
*  is stored using 2 bytes.
*
* \param src                [IN] -  Pointer to source frame
* \param y                  [IN] -  Pointer to Y plane
* \param u                  [IN] -  Pointer to U plane
* \param v                  [IN] -  Pointer to V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param dstPitchY          [IN] -  Line offset in destination frame
* \param bitCount           [IN] -  Count of bits per component, supported values are 10, 14 and 16
* 
* \return None.
*/
void gmColorSpace::P01XtoYUV_c(const byte *src, byte *y, byte *u, byte *v, int width, int height, int dstPitchY, int bitCount)
{
    int componentShift;
    switch(bitCount)
    {
    case 10:
        componentShift = 2;
    	break;
    case 14:
        componentShift = 6;
    	break;
    case 16:
        componentShift = 8;
        break;
    default:
        return;
        break;
    }
    const WORD *srcY = (const WORD *)src;
    const WORD *srcY2 = srcY + width;
    const WORD *srcColor = srcY + width * height;
    const WORD *srcColor2 = srcY + width * height * 5 / 4;
    for(int i = 0; i < height; i += 2)
    {
        for(int j = 0; j < width; j += 2)
        {
            WORD wordData;
            wordData = srcY[0];
            y[j] = (wordData >> componentShift) & 0xFF;
            wordData = srcY[1];
            y[j+1] = (wordData >> componentShift) & 0xFF;
            wordData = srcY2[0];
            y[j + width] = (wordData >> componentShift) & 0xFF;
            wordData = srcY2[1];
            y[j + width + 1] = (wordData >> componentShift) & 0xFF;

            wordData = srcColor[0];
            u[j] = (wordData >> componentShift) & 0xFF;
            u[width + j + 1] = u[width + j] = u[j + 1] = u[j];

            wordData = srcColor2[0];
            v[j] = (wordData >> componentShift) & 0xFF;
            v[width + j + 1] = v[width + j] = v[j + 1] = v[j];

            srcY += 2;
            srcY2 += 2;
            srcColor += 1;
            srcColor2 += 1;
        }
        srcY  += width;
        srcY2 += width;

        y += dstPitchY * 2;
        u += dstPitchY * 2;
        v += dstPitchY * 2;
    }
}

/**
**************************************************************************
* \brief Converts P21X to YUV
*
*  The function converts frame from planar P01X format family into planar YUV
*  format. The format is taken from h264 developers site. It has full Y plane,
*  followed by u and v planes decreased in width twice. Each component is stored
*  using 2 bytes.
*
* \param src                [IN] -  Pointer to source frame
* \param y                  [IN] -  Pointer to Y plane
* \param u                  [IN] -  Pointer to U plane
* \param v                  [IN] -  Pointer to V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param dstPitchY          [IN] -  Line offset in destination frame
* \param bitCount           [IN] -  Count of bits per component, supported values are 10, 14 and 16
* 
* \return None.
*/
void gmColorSpace::P21XtoYUV_c(const byte *src, byte *y, byte *u, byte *v, int width, int height, int dstPitchY, int bitCount)
{
    int componentShift;
    switch(bitCount)
    {
    case 10:
        componentShift = 2;
        break;
    case 14:
        componentShift = 6;
        break;
    case 16:
        componentShift = 8;
        break;
    default:
        return;
        break;
    }

    const WORD *srcY = (const WORD *)src;
    const WORD *srcColor = srcY + width * height;
    const WORD *srcColor2 = srcY + width * height * 3 / 2;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j += 2)
        {
            WORD wordData;
            wordData = srcY[0];
            y[j] = (wordData >> componentShift) & 0xFF;
            wordData = srcY[1];
            y[j+1] = (wordData >> componentShift) & 0xFF;

            wordData = srcColor[0];
            u[j + 1] = u[j] = (wordData >> componentShift) & 0xFF;

            wordData = srcColor2[0];
            v[j + 1] = v[j] = (wordData >> componentShift) & 0xFF;

            srcY += 2;
            srcColor += 1;
            srcColor2 += 1;
        }

        y += dstPitchY;
        u += dstPitchY;
        v += dstPitchY;
    }
}

/**
**************************************************************************
* \brief Converts P41X to YUV
*
*  The function converts frame from planar P41X format family into planar YUV
*  format. The format is taken from h264 developers site. It has full Y plane,
*  followed by full u and v planes. Each component is stored using 2 bytes.
*
* \param src                [IN] -  Pointer to source frame
* \param y                  [IN] -  Pointer to Y plane
* \param u                  [IN] -  Pointer to U plane
* \param v                  [IN] -  Pointer to V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param dstPitchY          [IN] -  Line offset in destination frame
* \param bitCount           [IN] -  Count of bits per component, supported values are 10, 14 and 16
* 
* \return None.
*/
void gmColorSpace::P41XtoYUVRGB_c(const byte *src, byte *y, byte *u, byte *v, int width, int height, int dstPitchY, int bitCount)
{
    int componentShift;
    switch(bitCount)
    {
    case 10:
        componentShift = 2;
        break;
    case 14:
        componentShift = 6;
        break;
    case 16:
        componentShift = 8;
        break;
    default:
        return;
        break;
    }
    const WORD *srcY = (const WORD *)src;
    const WORD *srcColor = srcY + width * height;
    const WORD *srcColor2 = srcY + width * height * 2;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            WORD wordData;

            wordData = srcY[0];
            y[j] = (wordData >> componentShift) & 0xFF;

            wordData = srcColor[0];
            u[j] = (wordData >> componentShift) & 0xFF; // u

            wordData = srcColor2[0];
            v[j] = (wordData >> componentShift) & 0xFF; // v

            // u[j] += y[j];

            // u[j] = v[j] = 128;

            srcY++;
            srcColor++;
            srcColor2++;
        }

        y += dstPitchY;
        u += dstPitchY;
        v += dstPitchY;
    }
}

/**
**************************************************************************
* \brief Converts RGB24 to YV12
*
*  The function converts frame from RGB24 format into planar YV12, 
*  where Y plane has the size of the source frame, while each of U and V have
*  size reduced by 2 both by horizontal and by vertical
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch_Y        [IN] -  Line offset in Y plane buffer
* \param dst_pitch_U        [IN] -  Line offset in either U or V plane buffer
* 
* \return None.
*/
void gmColorSpace::RGBtoYV12_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch_Y, int dst_pitch_U)
{
	int i, j, j3;
	int y, r, g, b, ua, va;
	float u[4], v[4];
	byte *pY = Y;
	byte *pU = U;
	byte *pV = V;
	byte const *ps = src;
	int h2 = (height + 1) / 2;
	int w2 = (width + 1) / 2;
	int exceed_w = w2 * 2 - width;
	int exceed_h = h2 * 2 - height;
	for (i = 0; i < h2; i++) 
	{
		if (i == h2 - 1) 
		{
			ps -= exceed_h * src_pitch;
			pY -= exceed_h * dst_pitch_Y;
		}
		for (j = 0; j < w2; j++) 
		{
			if (j == w2 - 1) 
			{
				ps -= exceed_w * 3;
				pY -= exceed_w;
			}
			j3 = j * 6;
			r = ps[j3 + 2];
			g = ps[j3 + 1];
			b = ps[j3];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			u[0] = (r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			v[0] = (r * 0.615f + g * (-0.515f) + b * (-0.100f) + 128.5f);
			if(y > 255) y = 255; else if(y < 0) y = 0;
			pY[j << 1] = (byte)y;
			r = ps[j3 + 5];
			g = ps[j3 + 4];
			b = ps[j3 + 3];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			u[1] = (r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			v[1] = (r * 0.615f + g * (-0.515f) + b * (-0.100f) + 128.5f);
			if(y > 255) y = 255; else if(y < 0) y = 0;
			pY[(j << 1) + 1] = (byte)y;
			ps += src_pitch;
			pY += dst_pitch_Y;
			r = ps[j3 + 2];
			g = ps[j3 + 1];
			b = ps[j3];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			u[2] = (r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			v[2] = (r * 0.615f + g * (-0.515f) + b * (-0.100f) + 128.5f);
			if(y > 255) y = 255; else if(y < 0) y = 0;
			pY[j << 1] = (byte)y;
			r = ps[j3 + 5];
			g = ps[j3 + 4];
			b = ps[j3 + 3];
			y = (int)(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
			u[3] = (r * (-0.147f) + g * (-0.289f) + b * 0.436f + 128.5f);
			v[3] = (r * 0.615f + g * (-0.515f) + b * (-0.100f) + 128.5f);
			if(y > 255) y = 255; else if(y < 0) y = 0;
			pY[(j << 1) + 1] = (byte)y;
			ua = (int)((u[0] + u[1] + u[2] + u[3]) / 4);
			va = (int)((v[0] + v[1] + v[2] + v[3]) / 4);
			if(ua > 255) ua = 255; else if(ua < 0) ua = 0;
			if(va > 255) va = 255; else if(va < 0) va = 0;
			pU[j] = (byte)ua;
			pV[j] = (byte)va;
			ps -= src_pitch;
			pY -= dst_pitch_Y;
		}
		ps += src_pitch * 2 + exceed_w * 3;
		pY += dst_pitch_Y * 2 + exceed_w;
		pU += dst_pitch_U;
		pV += dst_pitch_U;
	}
}

/**
**************************************************************************
* \brief Converts YUY2 to RGB24
*
*  The function converts frame from interleaved YUY2 format into RGB24 format.
*  U and V planes are shared between two adjacent horizontally pixels.
*  Stored in format: YUYVYUYV...
*
* \param src                [IN] -  Pointer to source frame plane
* \param dst                [OUT] -  Pointer to destination frame buffer
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUY2toRGB_c(const byte *src, byte *dst, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j, js, ju, jd;
	int r, g, b, y, u, v;
	byte *pd;
	byte const *ps;
	pd = dst;
	ps = src;
	for (i = 0; i < height; i++) 
	{
		for (j = 0, js = 0, jd = 0; j < width; j++, js += 2, jd += 3) 
		{
			ju = (js >> 2) * 4;
			y = ps[js];
			u = ps[ju + 1] - 128;
			v = ps[ju + 3] - 128;
			r = (int)(y + 1.14 * v + 0.5);
			g = (int)(y - 0.395 * u - 0.581 * v + 0.5);
			b = (int)(y + 2.032 * u + 0.5);
			if(r > 255) r = 255; else if(r < 0) r = 0;
			if(g > 255) g = 255; else if(g < 0) g = 0;
			if(b > 255) b = 255; else if(b < 0) b = 0;
			pd[jd] = (byte)b;
			pd[jd + 1] = (byte)g;
			pd[jd + 2] = (byte)r;
		}
		ps += src_pitch;
		pd += dst_pitch;
	}
}

/**
**************************************************************************
* \brief Converts YUY2 to YUV
*
*  The function converts frame from interleaved YUY2 format into YUV format.
*  U and V planes are shared between two adjacent horizontally pixels.
*  Stored in format: YUYVYUYV...
*
* \param src                [IN] -  Pointer to source frame plane
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YUY2toYUV_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j, js, ju, jd;

	const byte *ps = src;


#ifdef MEM_DEBUG
    MEMCOUNT_START;
#endif
	for (i = 0; i < height; i++) 
    {
		for (j = 0, js = 0, jd = 0; j < width; j++, js += 2, jd += 3) 
		{
			ju = (js >> 2) * 4;
			Y[j] = ps[js];
			U[j] = ps[ju + 1];
			V[j] = ps[ju + 3];
		}
		ps += src_pitch;

		Y += dst_pitch;
		U += dst_pitch;
		V += dst_pitch;
	}
#ifdef MEM_DEBUG
    MEMCOUNT_FINISH("YUY2toYUV");
#endif
}

/**
**************************************************************************
* \brief Converts UYVY to YUV
*
* \param src                [IN] -  Pointer to source frame plane
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::UYVYtoYUV_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch)
{
	int i, j, curr_macropixel;

	const byte *ps = src;

	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j+= 2) 
		{
			curr_macropixel = (j << 1);
			Y[j] = ps[curr_macropixel + 1];
			Y[j + 1] = ps[curr_macropixel + 3];;

			U[j] = U[j+1] = ps[curr_macropixel];
			V[j] = V[j+1] = ps[curr_macropixel + 2];
		}

		ps += src_pitch;

		Y += dst_pitch;
		U += dst_pitch;
		V += dst_pitch;
	}
}

/**
**************************************************************************
* \brief Converts Y to YUV
*
* \param src                [IN] -  Pointer to source frame plane
* \param Y                  [OUT] -  Pointer to destination Y plane
* \param U                  [OUT] -  Pointer to destination U plane
* \param V                  [OUT] -  Pointer to destination V plane
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return None.
*/
void gmColorSpace::YtoYUV_c(const byte *src, byte *Y, byte *U, byte *V, int width, int height, int src_pitch, int dst_pitch)
{
	int i;

	const byte *ps = src;

	const int half_width = width / 2;

	for (i = 0; i < height; i++) 
	{
		memcpy(Y, ps, sizeof(byte) * width);
		memset(U, 128, sizeof(byte)*width);
		memset(V, 128, sizeof(byte)*width);

		ps += src_pitch;

		Y += dst_pitch;
		U += dst_pitch;
		V += dst_pitch;
	}
}

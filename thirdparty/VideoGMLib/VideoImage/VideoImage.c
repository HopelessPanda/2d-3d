/*
*********************************************************************
  (c) YUVsoft Corp., 2010  www.yuvsoft.com
  This source code is a property of YUVsoft Corp.

  This code can be distributed and used ONLY under a proper licensing
  agreement with YUVsoft
*********************************************************************
*/

/**
*  \file VideoImage.c
*  \brief VideoFrames processing
*
*  This file contains implementations of auxiliary functions needed for
*  video frames processing
*
*/
#include "VideoImage.h"

/**
**************************************************************************
* \brief Returns BYTE count of the color space
*
* This function returns BYTE count of given color space
*
* \param type             [IN]  -  image color space
*
* \return Byte count
*/
int gmcVideoImage_ByteCount(gmcColorSpace type)
{
	switch (type)
	{
		case Y:
			return 1;
		case UYVY:
		case YV12:
		case IYUV:
			return 2;
		case YUV:
		case RGB24:
		case YCbCr:
			return 3;
	}
	return 0;
}

/**
**************************************************************************
* \brief Restores image to the original size (IYUV color space)
*
* This function restores image in YV12 or IYUV color space to the original
* size from the expanded one
*
* \param src             [IN]  -  source image (with expanded size)
* \param dst             [OUT] -  destination image (original size)
* \param oW              [IN]  -  width of original image
* \param oH              [IN]  -  height of original image
* \param W               [IN]  -  width of expanded image
* \param H               [IN]  -  height of expanded image
*
* \return NONE
*/
void WriteOriginalYV12(BYTE* src, BYTE* dst, int oW, int oH, int W, int H)
{
	int j;
	BYTE *s, *d;

	//upper part
	s = src;
	d = dst;

	for (j = 0; j < oH; j++)
	{
		memcpy(d, s, oW);
		d += oW;
		s += W;
	}

	s = src +  W *  H;
	d = dst + oW * oH;

	for (j = 0; j < oH; j++)
	{
		memcpy(d, s, oW / 2);
		s +=  W / 2;
		d += oW / 2;
	}

	s = src +  W *  H * 5 / 4;
	d = dst + oW * oH * 5 / 4;

	for (j = 0; j < oH / 2; j++)
	{
		memcpy(d, s, oW/2);
		d += oW/2;
		s += W/2;
	}
}

/**
**************************************************************************
* \brief Expands image to make its sizes multiple of 16 (IYUV color space)
*
* This function expands image in YV12 or IYUV color space to make it sizes
* multiple of 16
*
* \param src             [IN]  -  source image (original size)
* \param dst             [OUT] -  destination image (expanded size)
* \param oW              [IN]  -  width of original image
* \param oH              [IN]  -  height of original image
* \param W               [IN]  -  width of expanded image
* \param H               [IN]  -  height of expanded image
*
* \return NONE
*/
void ExpandYV12(BYTE* src, BYTE* dst, int oW, int oH, int W, int H)
{
	int j;
	BYTE *s, *d;
	int ResW = W-oW;
	int ResH = H-oH;

//upper part
	s = src;
	d = dst;
	for (j = 0; j < oH; j++)
	{
		memcpy(d, s, oW);
		d += oW;
		memset(d, d[-1], ResW);
		d += ResW;
		s += oW;
	}

//left border
	s = d - W;
	for (j = 0; j < ResH; j++)
	{
		memcpy(d, s, W);
		d += W;
	}

	ResW = W/2 - oW/2;
	ResH = H/2 - oH/2;

//upper part
	s = src + oW*oH;
	d = dst + W*H;
	for (j = 0; j < oH/2; j++)
	{
		memcpy(d, s, oW/2);
		d += oW/2;
		memset(d, d[-1], ResW);
		d += ResW;
		s += oW/2;
	}

//left border
	s = d - W;
	for (j = 0; j < ResH; j++)
	{
		memcpy(d, s, W/2);
		d += W/2;
	}

//upper part
	s = src + oW*oH + oW*oH/4;
	d = dst + W*H + W*H/4;
	for (j = 0; j < oH/2; j++)
	{
		memcpy(d, s, oW/2);
		d += oW/2;
		memset(d, d[-1], ResW);
		d += ResW;
		s += oW/2;
	}

//left border
	s = d - W;
	for (j = 0; j < ResH; j++)
	{
		memcpy(d, s, W/2);
		d += W/2;
	}
}

/**
**************************************************************************
* \brief Restores image to the original size
*
* This function restores image to the original size from the expanded one
*
* \param src             [IN]  -  source image (with expanded size)
* \param dst             [OUT] -  destination image (original size)
* \param oW              [IN]  -  width of original image
* \param oH              [IN]  -  height of original image
* \param W               [IN]  -  width of expanded image
* \param H               [IN]  -  height of expanded image
* \param Type            [IN]  -  color space
*
* \return NONE
*/
void gmcVideoImage_WriteOriginal(BYTE* src, BYTE* dst, int oW, int oH, int W, int H, gmcColorSpace Type)
{
	switch (Type)
	{
	case IYUV:
		WriteOriginalYV12(src, dst, oW, oH, W, H);
		break;
	}
}

/**
**************************************************************************
* \brief Expands image to make its sizes multiple of 16 (IYUV color space)
*
* This function expands image to make it sizes multiple of 16
*
* \param src             [IN]  -  source image (original size)
* \param dst             [OUT] -  destination image (expanded size)
* \param oW              [IN]  -  width of original image
* \param oH              [IN]  -  height of original image
* \param W               [IN]  -  width of expanded image
* \param H               [IN]  -  height of expanded image
* \param Type            [IN]  -  color space
*
* \return NONE
*/
void gmcVideoImage_Expand(BYTE* src, BYTE* dst, int oW, int oH, int W, int H, gmcColorSpace Type)
{
	switch (Type)
	{
	case IYUV:
		ExpandYV12(src, dst, oW, oH, W, H);
		break;
	}
}

/**
**************************************************************************
* \brief Converting image from YV12 to YUV
*
* \param src             [IN]  -  source image
* \param dst             [OUT] -  destination image
* \param W               [IN]  -  width of image
* \param H               [IN]  -  height of image
*
* \return NONE
*/
void gmcVideoImage_YV12ToYUVConvert(BYTE* dst, const BYTE* src, int W, int H)
{
	int x, y, k;
	const BYTE* s;
	BYTE* d;

	memcpy(dst, src, W * H * sizeof(BYTE));

	if (W % 2 != 0 || H % 2 != 0)
		return;

	for (k = 0; k < 2; k++)
	{
		s = src + W*H + k*W*H/4;
		d = dst + (1 + k)*W*H;
		for (y = 0; y < H/2; y++)
		{
			for (x = 0; x < W/2; x++)
			{
				d[0] = d[1] = d[W] = d[W+1] = s[0];

				s ++;
				d +=2;
			}
			d += W;
		}
	}
}

/**
**************************************************************************
* \brief Converting image from YUV to YV12
*
* \param src             [IN]  -  source image
* \param dst             [OUT] -  destination image
* \param W               [IN]  -  width of image
* \param H               [IN]  -  height of image
*
* \return NONE
*/
void gmcVideoImage_YUVToYV12Convert(BYTE* dst, const BYTE* src, int W, int H)
{
	int x, y, k;
	const BYTE* s;
	BYTE* d;

	memcpy(dst, src, W * H * sizeof(BYTE));

	if (W % 2 != 0 || H % 2 != 0)
		return;

	for (k = 0; k < 2; k++)
	{
		s = src + (1 + k)*W*H;
		d = dst + W*H + k*W*H/4;
		for (y = 0; y < H/2; y++)
		{
			for (x = 0; x < W/2; x++)
			{
				d[0] = (s[0] + s[1] + s[W] + s[W+1]) / 4;

				s +=2;
				d ++;
			}
			s += W;
		}
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
* \return NONE.
*/
void gmcVideoImage_YUVtoRGBConvert(const BYTE *Y, const BYTE *U, const BYTE *V, BYTE *dst, int width, int height, int src_pitch, int dst_pitch)
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
* \brief Converts RGB24 to YCbYCr
*
*  The function converts frame from RGB24 format into YCbYCr format, 
*
* \param src                [IN]  -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination frame buffer of Y component
* \param Cb                 [OUT] -  Pointer to destination frame buffer of Cb component
* \param Cr                 [OUT] -  Pointer to destination frame buffer of Cr component
* \param width              [IN]  -  Width of the source frame
* \param height             [IN]  -  Height of the source frame
* \param src_pitch          [IN]  -  Line offset in the source frame buffer
* \param dst_pitch          [IN]  -  Line offset in destination frame buffer
* 
* \return NONE.
*/
void gmcVideoImage_RGB24toYCbCrConvert(const BYTE *src, BYTE *Y, BYTE *Cb, BYTE *Cr, int width, int height, int src_pitch, int dst_pitch)
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
* \param Y                  [OUT] -  Pointer to destination frame buffer of Y component
* \param r                  [OUT] -  Pointer to destination frame buffer of r component
* \param g                  [OUT] -  Pointer to destination frame buffer of g component
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return NONE.
*/
void gmcVideoImage_RGB24toYrgConvert(const BYTE *src, BYTE *Y, BYTE *r, BYTE *g, int width, int height, int src_pitch, int dst_pitch)
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
* \brief Converts RGB24 to Yrg
*
*  The function converts frame from RGB24 format into YUV format, 
*
* \param src                [IN] -  Pointer to source frame in RGB24 format
* \param Y                  [OUT] -  Pointer to destination frame buffer of Y component
* \param U                  [OUT] -  Pointer to destination frame buffer of U component
* \param V                  [OUT] -  Pointer to destination frame buffer of V component
* \param width              [IN] -  Width of the source frame
* \param height             [IN] -  Height of the source frame
* \param src_pitch          [IN] -  Line offset in the source frame buffer
* \param dst_pitch          [IN] -  Line offset in destination frame buffer
* 
* \return NONE.
*/
void gmcVideoImage_RGB24toYUVConvert(const BYTE *src, BYTE *Y, BYTE *U, BYTE *V, int width, int height, int src_pitch, int dst_pitch)
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
* \brief Image reverting
*
*   Used to abolish orientation difference between console and Virtual Dub
*   algorithm's variants.
*
* \param img        [IN/OUT] - image to revert
* \param h          [IN]     - height of image
* \param pitch      [IN]     - image line offset
* \param planes     [IN]     - number of image planes (1 for Y reverting, 3 for YUV or RBG reverting etc.)
* 
* \return NONE
*/
void gmcImageRevert(BYTE* img, int h, int pitch, int planes)
{
    BYTE* t;
    int n;
    int uoff = pitch * h;
    int h2   = h / 2;
    INIT_MEMORY_C(t, 0, pitch, BYTE);

    for (n = 0; n < planes; n++)
    {
        int i;
        BYTE* s1 = img;
        BYTE* s2 = img  + uoff - pitch;

        for (i = 0; i < h2; i++)
        {
            memcpy(t , s1, pitch);
            memcpy(s1, s2, pitch);
            memcpy(s2, t , pitch);

            s1 += pitch;
            s2 -= pitch;
        }

        img += uoff;
    }

    DEINIT_MEMORY_C(t);
}

/**
**************************************************************************
* \brief Add borders to frame of Y type
*
*   Dimensions of destination frame calculates using source dimensions and 
*   borders values. Borders values should be >= 0
*
* \param dst        [IN] - destination frame
* \param src        [IN] - source frame
* \param srcW       [IN] - width of source frame
* \param srcH       [IN] - height of source frame
* \param borderL    [IN] - size of left border
* \param borderT    [IN] - size of top border 
* \param borderR    [IN] - size of right border
* \param borderB    [IN] - size of bottom border
* 
* \return NONE
*/
void cAddBlackBordersY(BYTE* dst, BYTE* src, int srcW, int srcH, int borderL, int borderT, int borderR, int borderB)
{
	int y;
	int dstW = srcW + borderL + borderR;
	int dstH = srcH + borderT + borderB;

    memset(dst, 0, dstW * dstH);

    dst += borderT * dstW + borderL;
    for( y = 0; y < srcH; y++)
	{
		memcpy( dst, src, srcW );

		src += srcW;
		dst += dstW;
	}
}

/**
**************************************************************************
* \brief Add borders to frame
*
*   Dimensions of destination frame calculates using source dimensions and 
*   borders values. Borders values should be >= 0
*
* \param dst        [IN] - destination frame
* \param src        [IN] - source frame
* \param srcW       [IN] - width of source frame
* \param srcH       [IN] - height of source frame
* \param borderL    [IN] - size of left border
* \param borderT    [IN] - size of top border 
* \param borderR    [IN] - size of right border
* \param borderB    [IN] - size of bottom border
* \param planes     [IN] - number of planes
* 
* \return NONE
*/
void cAddBlackBorders( BYTE* dst, const BYTE* src, int srcW, int srcH, int borderL, int borderT, int borderR, int borderB, int planes )
{
	int y, n;
	int dstW = srcW + borderL + borderR;
	int dstH = srcH + borderT + borderB;

    int srcWH = srcW * srcH;
    int dstWH = dstW * dstH;

    memset(dst, 0, dstW * dstH * planes);

    dst += borderT * dstW + borderL;

    for( n=0; n<planes; n++ )
    {
        BYTE*       d = dst + n*dstWH;
        const BYTE* s = src + n*srcWH;

        for( y = 0; y < srcH; y++)
	    {
		    memcpy( d, s, srcW );

		    s += srcW;
		    d += dstW;
	    }
    }
}

/**
**************************************************************************
* \brief Removes borders from frame of Y type
*
*   Dimensions of destination frame calculates using source dimensions and 
*   borders values. Borders values should be >= 0
*
* \param dst        [OUT]   - destination frame
* \param src        [IN]    - source frame
* \param wSrc       [IN]    - width of source frame
* \param hSrc       [IN]    - height of source frame
* \param borderL    [IN]    - size of left border
* \param borderT    [IN]    - size of top border 
* \param borderR    [IN]    - size of right border
* \param borderB    [IN]    - size of bottom border
* 
* \return NONE
*/
void cDelBordersY( BYTE * dst, const BYTE * src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB )
{
	int y;
	int dstW = wSrc - borderL - borderR;
	int dstH = hSrc - borderT - borderB;

	// get without borders
	src += borderT * wSrc + borderL;

	for( y = 0; y < dstH; y++)
	{
		memcpy( dst, src, dstW );
		dst += dstW;
		src += wSrc;
	}
}

/**
**************************************************************************
* \brief Removes borders from YUV frame
*
*   Dimensions of destination frame calculates using source dimensions and 
*   borders values. Borders values should be >= 0
*
* \param dst        [OUT]   - destination frame
* \param src        [IN]    - source frame
* \param wSrc       [IN]    - width of source frame
* \param hSrc       [IN]    - height of source frame
* \param borderL    [IN]    - size of left border
* \param borderT    [IN]    - size of top border 
* \param borderR    [IN]    - size of right border
* \param borderB    [IN]    - size of bottom border
* 
* \return NONE
*/
void cDelBordersYUV( BYTE * dst, const BYTE * src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB )
{
    int dstW = wSrc - borderL - borderR;
    int dstH = hSrc - borderT - borderB;

    cDelBordersY( dst                  , src                  , wSrc, hSrc, borderL, borderT, borderR, borderB );
    cDelBordersY( dst + dstW * dstH    , src + wSrc * hSrc    , wSrc, hSrc, borderL, borderT, borderR, borderB );
    cDelBordersY( dst + dstW * dstH * 2, src + wSrc * hSrc * 2, wSrc, hSrc, borderL, borderT, borderR, borderB );

}


/**
**************************************************************************
* \brief Fill repeated borders for Y image
* 
* Parameter image should point to the beginning of the extended image
*
* \param image			[IN/OUT]    - Y-image with borders 
* \param wExt	   		[IN]        - frame width with borders
* \param hExt	   		[IN]        - frame height with borders
* \param borderL        [IN]        - size of left border
* \param borderT        [IN]        - size of top border 
* \param borderR        [IN]        - size of right border
* \param borderB        [IN]        - size of bottom border
* 
* \return Error code
*/
int cFillRepeatedBordersY( BYTE * image, int wExt, int hExt, int borderL, int borderT, int borderR, int borderB )
{
    int y;
    int w = wExt - borderL - borderR;  // width without borders
    int h = hExt - borderT - borderB;  // height without borders
    int pitch = wExt;
    BYTE * src;
    BYTE * dst;
    
    //left border
    src = &image[ borderT * wExt + borderL ]; // set src on the beginning of original image
    dst = &image[ borderT * wExt           ]; // set dst on the beginning of the left border of the first line
    for ( y = 0; y < h; y++ )
    {
        memset( dst, src[0], borderL );
        src += pitch;
        dst += pitch;
    }

    //right border
    src = &image[ borderT * wExt + borderL + w - 1 ]; // set src on the end of first line of the original image
    dst = &image[ borderT * wExt + borderL + w     ]; // set dst after the end of first line 
    for ( y = 0; y < h; y++ )
    {
        memset( dst, src[0], borderR );
        src += pitch;
        dst += pitch;
    }

    // top border
    src = &image[ borderT * wExt ]; // set src on the first extended line of the original image 
    dst = &image[ 0              ]; // set dst on the first line of the extended image
    for ( y = 0; y < borderT; y++ )
    {
        memcpy( dst, src, wExt );
        dst += pitch; // move dst on the next line, src remains on the same place
    }

    // bottom border
    src = &image[ ( borderT + h - 1 ) * wExt ]; // set src on the last extended line of the original image 
    dst = &image[ ( borderT + h     ) * wExt ]; // set dst after the last line of the extended image
    for ( y = 0; y < borderB; y++ )
    {
        memcpy( dst, src, wExt );
        dst += pitch; // move dst on the next line, src remains on the same place
    }

    return 0;
}

/**
**************************************************************************
* \brief Fill repeated borders for YUV image
* 
* Parameter image should point to the beginning of the extended image
*
* \param image			[IN/OUT]    - YUV-image with borders 
* \param wExt	   		[IN]        - frame width with borders
* \param hExt	   		[IN]        - frame height with borders
* \param borderL        [IN]        - size of left border
* \param borderT        [IN]        - size of top border 
* \param borderR        [IN]        - size of right border
* \param borderB        [IN]        - size of bottom border
* 
* \return Error code
*/
int cFillRepeatedBordersYUV( BYTE * image, int wExt, int hExt, int borderL, int borderT, int borderR, int borderB )
{
    const int numOfPlane = 3;

    int plane; 
    int sizeOfPlane = wExt * hExt;

    for ( plane = 0; plane < numOfPlane; plane ++ )
    {
        cFillRepeatedBordersY( &image[ sizeOfPlane * plane ], wExt, hExt, borderL, borderT, borderR, borderB );
    }

    return 0;
}



/**
**************************************************************************
* \brief Add repeated borders for Y image
* 
* \param dst  			[OUT]       - output Y-image with borders 
* \param src  			[IN]        - input Y-image without borders 
* \param wSrc	   		[IN]        - frame width with borders
* \param hSrc	   		[IN]        - frame height with borders
* \param borderL        [IN]        - size of left border
* \param borderT        [IN]        - size of top border 
* \param borderR        [IN]        - size of right border
* \param borderB        [IN]        - size of bottom border
* 
* \return Error code
*/
int cAddRepeatedBordersY( BYTE * dst, BYTE * src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB )
{
    int wDst = wSrc + borderL + borderR;  // width with borders
    int hDst = hSrc + borderT + borderB;  // height with borders
    int pitchDst = wDst;
    int pitchSrc = wSrc;

    gmcVideoImage_CopyImageY( &dst[ borderT * pitchDst + borderL ], src, wSrc, hSrc, pitchDst, pitchSrc );

    cFillRepeatedBordersY( dst, wDst, hDst, borderL, borderT, borderR, borderB );

    return 0;
}

/**
**************************************************************************
* \brief Add repeated borders for YUV image
* 
* \param dst  			[OUT]       - output YUV-image with borders 
* \param src  			[IN]        - input YUV-image without borders 
* \param wSrc	   		[IN]        - frame width with borders
* \param hSrc	   		[IN]        - frame height with borders
* \param borderL        [IN]        - size of left border
* \param borderT        [IN]        - size of top border 
* \param borderR        [IN]        - size of right border
* \param borderB        [IN]        - size of bottom border
* 
* \return Error code
*/
int cAddRepeatedBordersYUV( BYTE * dst, BYTE * src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB )
{
    int plane;
    int sizeOfSrcPlane = wSrc * hSrc;
    int sizeOfDstPlane = ( wSrc + borderL + borderR ) * ( hSrc + borderT + borderB );
    for ( plane = 0; plane < 3; plane ++ )
    {
        cAddRepeatedBordersY( dst + sizeOfDstPlane * plane, src + sizeOfSrcPlane * plane, 
            wSrc, hSrc, borderL, borderT, borderR, borderB );
    }
    
    return 0;
}


/**
**************************************************************************
* \brief Add mirrored borders for Y-image
*
* \param src				[IN] - Pointer to source frame
* \param dst				[IN/OUT] - Pointer to bordered frame
* \param border				[IN] - Border size
* \param width 				[IN] - Frame width
* \param height				[IN] - Frame height
*
* \return Error code
*/
int cAddMirroredBordersY( BYTE * dst, BYTE * src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB )
{
    int wDst = wSrc + borderL + borderR;  // width with borders
    int hDst = hSrc + borderT + borderB;  // height with borders
    int pitchDst = wDst;
    int pitchSrc = wSrc;
    
    gmcVideoImage_CopyImageY( &dst[ borderT * pitchDst + borderL ], src, wSrc, hSrc, pitchDst, pitchSrc );

    cFillMirroredBordersY( dst, wDst, hDst, borderL, borderT, borderR, borderB );

    return 0;
}


/**
**************************************************************************
* \brief Fill mirrored borders for Y image
* 
* Parameter image should point to the beginning of the extended image
*
* \param image			[IN/OUT]    - Y-image with borders 
* \param wExt	   		[IN]        - frame width with borders
* \param hExt	   		[IN]        - frame height with borders
* \param borderL        [IN]        - size of left border
* \param borderT        [IN]        - size of top border 
* \param borderR        [IN]        - size of right border
* \param borderB        [IN]        - size of bottom border
* 
* \return Error code
*/
int cFillMirroredBordersY( BYTE * image, int wExt, int hExt, int borderL, int borderT, int borderR, int borderB )
{
    int y;
    int w = wExt - borderL - borderR;  // width without borders
    int h = hExt - borderT - borderB;  // height without borders
    int pitch = wExt;
    BYTE * lineSrc;
    BYTE * lineDst;
    
    //left border
    lineDst = &image[ borderT * wExt + borderL ]; // set lineDst on the beginning of original image
    for ( y = 0; y < h; y ++ )
    {
        int x;
        for ( x = 0; x < borderL; x ++ )
        {
            lineDst[ - x - 1 ] = lineDst[ x ];
        }
        lineDst += pitch;
    }

    //right border
    lineDst = &image[ borderT * wExt + borderL + w ]; // set lineDst after the end of first line of the original image
    for ( y = 0; y < h; y ++ )
    {
        int x;
        for ( x = 0; x < borderR; x ++ )
        {
            lineDst[ x ] = lineDst[ - x - 1 ];
        }
        lineDst += pitch;
    }

    
    // top border
    lineSrc = &image[ borderT * wExt ];         // set lineSrc on extended line #0 of the original image 
    lineDst = &image[ ( borderT - 1 ) * wExt ]; // set lineDst on extended line #-1 of the original image 
    for ( y = 0; y < borderT; y++ )
    {
        memcpy( lineDst, lineSrc, wExt );
        lineDst -= pitch; // move lineDst on the previous line
        lineSrc += pitch; // move lineDst on the next line
    }

    // bottom border
    lineSrc = &image[ ( borderT + h - 1 ) * wExt ]; // set lineSrc on the last extended line of the original image 
    lineDst = &image[ ( borderT + h     ) * wExt ]; // set lineDst after the last line of the extended image
    for ( y = 0; y < borderB; y++ )
    {
        memcpy( lineDst, lineSrc, wExt );
        lineDst += pitch; // move lineDst on the next line
        lineSrc -= pitch; // move lineDst on the previous line
    }

    return 0;
}


/**
**************************************************************************
* \brief Copy Y image per line.
*  
* Images must have same width and height, but can have different pitches
*
* \param dst			[OUT]       - destination Y-image 
* \param src			[IN]        - source Y-image 
* \param w   	   		[IN]        - image width
* \param h   	   		[IN]        - image height
* \param pitchDst 		[IN]        - pitch of destination image
* \param pitchSrc 		[IN]        - pitch of source image
* 
* \return Error code
*/
int gmcVideoImage_CopyImageY( BYTE * dst, BYTE * src, int w, int h, int pitchDst, int pitchSrc )
{
    int y;
    BYTE * lineSrc;
    BYTE * lineDst;

    lineSrc = src;
    lineDst = dst;

    for ( y = 0; y < h; y++ )
    {
        memcpy( lineDst, lineSrc, w );
        lineSrc += pitchSrc;
        lineDst += pitchDst;
    }

    return 0;
}


/**
**************************************************************************
* \brief Allocate memory for image represented as BYTE array
*  
* 
*
* \param w   	   		[IN]        - image width
* \param h   	   		[IN]        - image height
* \param numPlanes 		[IN]        - number of planes in image

* 
* \return pointer to allocated memory or NULL in case of error
*/
BYTE * gmcVideoImage_Allocate( const int w, const int h, const int numPlanes )
{
    BYTE * result;
    INIT_MEMORY_C( result, 0, w * h * numPlanes, BYTE );
    return result;
}

/**
**************************************************************************
* \brief Free memory for image represented as BYTE array
*  
* 
*
* \param image   	   		[IN]        - pointer to the image
* 
* \return None
*/
void gmcVideoImage_Free( BYTE * image )
{
    DEINIT_MEMORY_C( image );
}

/**
**************************************************************************
* \brief Fill plane of image with specified value
*  
* 
* \param image	   		[IN/OUT]    - destination image 
* \param w   	   		[IN]        - image width
* \param h   	   		[IN]        - image height
* \param indexPlane     [IN]        - index of plane
* \param value          [IN]        - value for filling
* 
* \return error code
*/
int gmcVideoImage_FillPlane( BYTE * image, const int w, const int h, const int indexPlane, BYTE value )
{
    
    memset( image + indexPlane * w * h, value, w * h );
    return 0;
}

/**
**************************************************************************
* \brief Fill image of image with specified value
*  
* 
* \param image	   		[IN/OUT]    - destination image 
* \param w   	   		[IN]        - image width
* \param h   	   		[IN]        - image height
* \param numPlanes      [IN]        - number of planes in the image
* \param value          [IN]        - value for filling
* 
* \return error code
*/
int gmcVideoImage_Fill( BYTE * image, const int w, const int h, const int numPlanes, BYTE value )
{
    int i;
    for ( i = 0; i < numPlanes; i ++ )
    {
        memset( image + i * w * h, value, w * h );
    }
    return 0;
}
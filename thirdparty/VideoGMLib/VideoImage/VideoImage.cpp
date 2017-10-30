#include "VideoImage.h"

int gmcVideoImage_ByteCount(gmcColorSpace type)
{
	switch (type)
	{
	case UYVY:
	case YV12:
	case IYUV:
		return 2;
	}
	return 0;
}

void WriteOriginalYV12(BYTE* src, BYTE* dst, int oW, int oH, int W, int H)
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
	s = d;
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
	s = d;
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
	s = d;
	for (j = 0; j < ResH; j++)
	{
		memcpy(d, s, W/2);
		d += W/2;
	}
}

void gmcVideoImage_WriteOriginal(BYTE* src, BYTE* dst, int oW, int oH, int W, int H, gmcColorSpace Type)
{
	switch (Type)
	{
	case IYUV:
		WriteOriginalYV12(src, dst, oW, oH, W, H);
		break;
	}
}

void gmcVideoImage_Expand(BYTE* src, BYTE* dst, int oW, int oH, int W, int H, gmcColorSpace Type)
{
	switch (Type)
	{
	case IYUV:
		ExpandYV12(src, dst, oW, oH, W, H);
		break;
	}
}
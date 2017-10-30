
#include "MExt_FrameTransform.h"
#include <VideoGMLib/VideoImage/VideoImage.h>

// frame contains YUV (or RGB) planes
void ME_Frame_Init( ME_Frame *frame, int w, int h, int me_border, int additional_buf_number )
{
	Size_Init(&frame->orig_dim, w, h);
	Size_Init(&frame->ali_dim, Align16(w), Align16(h) );
	
	frame->align_w_diff = Align16(w) - w;
	frame->align_h_diff = Align16(h) - h;
	frame->me_border = me_border;
	
	Size_Init(&frame->ext_dim, Align16(w) + 2*me_border, Align16(h) + 2*me_border);

	INIT_MEMORY_C(frame->original, 0, frame->orig_dim.m_iS*3, BYTE);
	INIT_MEMORY_C(frame->extended, 0, frame->ext_dim.m_iS*3,  BYTE);
	INIT_MEMORY_C(frame->aligned,  0, frame->ali_dim.m_iS*3,  BYTE);

	frame->additional_number = additional_buf_number;
	if( frame->additional_number > 0 )
	{
		int ind;
		INIT_MEMORY_C(frame->additional_extended, 0, frame->additional_number, BYTE*);
		for( ind = 0; ind < frame->additional_number; ++ ind )
		{
			INIT_MEMORY_C(frame->additional_extended[ind], 0, frame->ext_dim.m_iS, BYTE);
		}
	}
}

void ME_Frame_DeInit(ME_Frame *frame)
{
	DEINIT_MEMORY_C(frame->original);
	DEINIT_MEMORY_C(frame->extended);
	DEINIT_MEMORY_C(frame->aligned);
	
	if(frame->additional_number > 0)
	{
		int ind;
		for ( ind = 0; ind < frame->additional_number; ++ ind )
		{
			DEINIT_MEMORY_C(frame->additional_extended[ind]);
		}
		DEINIT_MEMORY_C(frame->additional_extended);
	}
}

void ME_Frame_Fill_Additional_Buffer ( ME_Frame * frame, BYTE * in_plane, int plane_num )
{
	if(plane_num >= frame->additional_number)
		return;

	cAddRepeatedBordersY(frame->additional_extended[plane_num], in_plane, frame->ali_dim.m_iW, 
		frame->ali_dim.m_iH, frame->me_border, frame->me_border, frame->me_border, frame->me_border );
}

BYTE ** ME_Frame_Get_AddBufferPointer( ME_Frame * frame )
{
	return frame->additional_extended;
}

void ME_Frame_Fill_From_Original(ME_Frame *frame, const BYTE *src)
{
	if(frame->original != src)
		memcpy(frame->original, src, frame->orig_dim.m_iS * 3);

	cAddRepeatedBordersYUV(frame->aligned, frame->original, frame->orig_dim.m_iW, 
		frame->orig_dim.m_iH, 0,0,frame->align_w_diff, frame->align_h_diff);

	cAddRepeatedBordersYUV(frame->extended, frame->aligned, frame->ali_dim.m_iW, 
		frame->ali_dim.m_iH, frame->me_border, frame->me_border, frame->me_border, frame->me_border );
}

void ME_Frame_Fill_From_Planes ( ME_Frame * frame, BYTE * plane1, BYTE * plane2, BYTE * plane3 )
{
	if(plane1)
		memcpy(frame->original,								plane1, frame->orig_dim.m_iS );
	if(plane2)
		memcpy(frame->original + frame->orig_dim.m_iS,		plane2, frame->orig_dim.m_iS );
	if(plane3)
		memcpy(frame->original + frame->orig_dim.m_iS*2,	plane3, frame->orig_dim.m_iS );

	cAddRepeatedBordersYUV(frame->aligned, frame->original, frame->orig_dim.m_iW, 
		frame->orig_dim.m_iH, 0,0,frame->align_w_diff, frame->align_h_diff);

	cAddRepeatedBordersYUV(frame->extended, frame->aligned, frame->ali_dim.m_iW, 
		frame->ali_dim.m_iH, frame->me_border, frame->me_border, frame->me_border, frame->me_border );
}

void ME_Frame_Fill_From_Extended(ME_Frame *frame, BYTE *src)
{
	if(frame->extended != src)
		memcpy(frame->extended, src, frame->ext_dim.m_iS * 3);
	
	cDelBordersYUV(frame->aligned, frame->extended, frame->ext_dim.m_iW, frame->ext_dim.m_iH,
		frame->me_border, frame->me_border, frame->me_border, frame->me_border);
	cDelBordersYUV(frame->original, frame->aligned, frame->ali_dim.m_iW, frame->ali_dim.m_iH,
		0,0,frame->align_w_diff, frame->align_h_diff);
}

BYTE* ME_Frame_Get_Frame_Buf_Pointer(ME_Frame *frame, frame_type type)
{
	switch(type)
	{
	case orig:		return frame->original;
	case align:		return frame->aligned;
	case extend:	return frame->extended;

	default:		return frame->original;

	}
}

Size ME_Frame_Get_Dims(ME_Frame *frame, frame_type type)
{
	switch(type)
	{
	case orig:		return frame->orig_dim;
	case align:		return frame->ali_dim;
	case extend:	return frame->ext_dim;

	default:		return frame->orig_dim;
	}
}

int ME_Frame_Get_Width(ME_Frame *frame, frame_type type)
{
	switch(type)
	{
	case orig:		return frame->orig_dim.m_iW;
	case align:		return frame->ali_dim.m_iW;
	case extend:	return frame->ext_dim.m_iW;

	default:		return frame->orig_dim.m_iW;
	}
}

int ME_Frame_Get_Height(ME_Frame *frame, frame_type type)
{
	switch(type)
	{
	case orig:		return frame->orig_dim.m_iH;
	case align:		return frame->ali_dim.m_iH;
	case extend:	return frame->ext_dim.m_iH;

	default:		return frame->orig_dim.m_iH;
	}
}

int ME_Frame_Get_Area(ME_Frame *frame, frame_type type)
{
	switch(type)
	{
	case orig:		return frame->orig_dim.m_iS;
	case align:		return frame->ali_dim.m_iS;
	case extend:	return frame->ext_dim.m_iS;

	default:		return frame->orig_dim.m_iS;
	}
}


void DownScale_frame_by_two(BYTE *src_frame, BYTE *dst_frame, int w, int h)
{
	int x, y;
	int w_sm = w/2; 
	int h_sm = h/2;

	for (y = 0; y < h_sm; ++y)
	{
		for (x = 0; x < w_sm; ++x)
		{
			int sum = 0;
			sum += src_frame[(y * 2    ) * w + x * 2    ];
			sum += src_frame[(y * 2 + 1) * w + x * 2    ];
			sum += src_frame[(y * 2    ) * w + x * 2 + 1];
			sum += src_frame[(y * 2 + 1) * w + x * 2 + 1];
			sum = sum >> 2;
			dst_frame[y * w_sm + x] = sum;
		}
	}

	return;
}

void ME_Frame_DownScale_by_two(ME_Frame *src, ME_Frame *dst, frame_type type)
{
	int w = ME_Frame_Get_Width(src, type);
	int h = ME_Frame_Get_Height(src, type);

	int x, y;
	int w_sm = w/2; 
	int h_sm = h/2;

	BYTE *src_frame = ME_Frame_Get_Frame_Buf_Pointer(src, type);
	BYTE *dst_frame = ME_Frame_Get_Frame_Buf_Pointer(dst, type);

	// for 3 planes - height is multiplied by 3.

	for (y = 0; y < h_sm * 3; ++y)
	{
		for (x = 0; x < w_sm; ++x)
		{
			int sum = 0;
			sum += src_frame[(y * 2    ) * w + x * 2    ];
			sum += src_frame[(y * 2 + 1) * w + x * 2    ];
			sum += src_frame[(y * 2    ) * w + x * 2 + 1];
			sum += src_frame[(y * 2 + 1) * w + x * 2 + 1];
			sum = sum >> 2;
			dst_frame[y * w_sm + x] = sum;
		}
	}

	return;
}

// w and h are for source frame.
void UpScale_frame_by_factor(BYTE *src_frame, BYTE *dst_frame, int w, int h, int factor)
{
	int x, y;
	int w_big = w*factor; 
	int h_big = h*factor;

	for (y = 0; y < h_big; ++y)
	{
		for (x = 0; x < w_big; ++x)
		{
			dst_frame[y*w_big + x] = src_frame[(y/factor) * w + x/factor];
		}
	}

	return;
}

// w and h are for source frame.
void UpScale_frame_by_factor_MV(cMV *src_frame, cMV *dst_frame, int w, int h, int factor)
{
	int x, y;
	int w_big = w*factor; 
	int h_big = h*factor;

	for (y = 0; y < h_big; ++y)
	{
		for (x = 0; x < w_big; ++x)
		{
			dst_frame[y*w_big + x] = src_frame[(y/factor) * w + x/factor];
		}
	}

	return;
}

void cDelBorders_cMV( cMV * dst, const cMV * src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB )
{
	int y;
	int dstW = wSrc - borderL - borderR;
	int dstH = hSrc - borderT - borderB;

	// get without borders
	src += borderT * wSrc + borderL;

	for( y = 0; y < dstH; y++)
	{
		memcpy( dst, src, dstW*sizeof(cMV) );
		dst += dstW;
		src += wSrc;
	}
}

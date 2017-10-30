#pragma once

#include <VideoVGCLib/MEstimation/cMV.h>

#include <VideoVGCLib/VGLibCommon/Sizes.h>
#include <VideoGMLib/Common/CommonTypes.h>

/*! \enum frame_type
*   \brief size of frame, stored in ME_Frame
*	"orig" stands for original resolution, "align" - for aligned to div16 resolution,
*	"extend" - for resolution of frame, used by ME instance
*/

typedef enum {orig, align, extend} frame_type;


/*! \struct ME_Frame
*   \brief Handles frame processing for Motion Estimation
*	Provides encapsulation of creation and work with different resolutions
*	during Motion Estimation launch
*/
typedef struct ME_Frame_Info
{
	BYTE *original;		// frame buffer of original resolution
	BYTE *aligned;		// frame buffer of aligned to 16 resolution
	BYTE *extended;		// frame buffer with added borders (which are required for ME)

	BYTE **additional_extended;
	int	additional_number;

	Size orig_dim;		// dimensions of original frame
	Size ali_dim;		// dimensions of aligned to 16 frame
	Size ext_dim;		// dimensions of extended frame with borders.

	int me_border;		// width of border for Motion Estimation
	int align_w_diff;	// difference between original and aligned width
	int align_h_diff;	// difference between original and aligned height
} ME_Frame;

/**
**************************************************************************
* \brief Initialization of ME frame planes
*/
void ME_Frame_Init(ME_Frame *frame, int w, int h, int me_border, int additional_buf_number);

/**
**************************************************************************
* \brief deallocating ME frame memory
*/
void ME_Frame_DeInit(ME_Frame *frame);

/**
**************************************************************************
* \brief Fill ME frame buffer, using external buffer of original dimensions
*/
void ME_Frame_Fill_From_Original(ME_Frame *frame, const BYTE *src);

void ME_Frame_Fill_From_Planes ( ME_Frame * frame, BYTE * plane1, BYTE * plane2, BYTE * plane3 );
/**
**************************************************************************
* \brief Fill ME frame buffer, using external buffer of extended dimensions
*/
void ME_Frame_Fill_From_Extended(ME_Frame *frame, BYTE *src);

void ME_Frame_Fill_Additional_Buffer ( ME_Frame * frame, BYTE * in_plane, int plane_num );

/**
**************************************************************************
* \brief Returns pointer to the plane of specified type
*/
BYTE*	ME_Frame_Get_Frame_Buf_Pointer(ME_Frame *frame, frame_type type);

/**
**************************************************************************
* \brief Returns dimensions of frame plane of specified type
*/
Size	ME_Frame_Get_Dims	(ME_Frame *frame, frame_type type);

/**
**************************************************************************
* \brief Returns width of frame plane of specified type
*/
int		ME_Frame_Get_Width	(ME_Frame *frame, frame_type type);

/**
**************************************************************************
* \brief Returns height of frame plane of specified type
*/
int		ME_Frame_Get_Height	(ME_Frame *frame, frame_type type);

BYTE ** ME_Frame_Get_AddBufferPointer( ME_Frame * frame );

/**
**************************************************************************
* \brief Returns plane size of specified type
*/
int		ME_Frame_Get_Area	(ME_Frame *frame, frame_type type);

void ME_Frame_DownScale_by_two(ME_Frame *src, ME_Frame *dst, frame_type type);

void UpScale_frame_by_factor(BYTE *src_frame, BYTE *dst_frame, int w, int h, int factor);

void DownScale_frame_by_two(BYTE *src_frame, BYTE *dst_frame, int w, int h);

void UpScale_frame_by_factor_MV(cMV *src_frame, cMV *dst_frame, int w, int h, int factor);

void cDelBorders_cMV( cMV * dst, const cMV * src, int wSrc, int hSrc, int borderL, int borderT, int borderR, int borderB );

#pragma once

#include <VideoVGCLib/MEstimation/cMV.h>

/*! \enum BYTE_buffer_type
*   \brief  defines all additional buffers of BYTE type
*	
*	This enum contains all types of  additional buffers, which are (or can be) 
*	calculated by hierarchic ME. This enum is  important - constants from here 
*	are used to  gain a  pointer or copy of a calculated buffer.
*	To add new buffer expand enum and change constant below
*/	
typedef enum 
{
	BYTE_disparity				= 0,		// Disparity map for stereo input (unfiltered)
	BYTE_disparity_filtered		= 1,		// Filtered disparity
	BYTE_confidence				= 2,		// confidence of ME vectors. Obsolete, usefulness unknown
	BYTE_filter_mask			= 3,		// Mask of vector refinement
	BYTE_MVdispersion			= 4,		// Dispersion of motion vectors (filtered, using color information). Nearly useless.
	BYTE_compensated_frame		= 5,		// Buffer, used for frame compensation
	BYTE_motion_error			= 6,		// Error of compensation
	BYTE_LR_compensation		= 7,		// Buffer to store Left-Right Compensation result
	BYTE_SAD_contrast			= 8,		// SAD error between source stereo frames
	BYTE_zinger_fill			= 9,		// Occlusions, filled by zinger algorithm
	BYTE_compensated_fill		= 10,		// Occlusions, filled by compensating from another disparity
	BYTE_difference_fill		= 11,		// Difference between 2 types of occlusion filling
	BYTE_contrast				= 12,		// Contrast (texturization) map of frame
	BYTE_occlusion_mask			= 13,		// Occlusion mask for source stereo (based on motion vectors)
	BYTE_lrc_mask				= 14,		// Left-Right Consistency map
	BYTE_filter_mask_prepared	= 15,		// Mask of vector refinement - new method.
	BYTE_filter_mask_texture	= 16,		// Mask for filtering motion errors in high-textured areas
	BYTE_filter_mask_uniform	= 17,		// Mask for filtering motion errors in uniform areas
	BYTE_frame_dispersion		= 18,		// dispersion of color info in frame (block-based)
	
	// for depth-based filter
	BYTE_lrc_texture_filter		= 19,
	BYTE_lrc_uniform_filter		= 20,
	BYTE_depth_map				= 21,
	BYTE_depth_map_dilation		= 22,
	BYTE_depth_map_erosion		= 23,
	BYTE_depth_gradient_map		= 24,
	BYTE_distransform			= 25,
	BYTE_distransform_oneway	= 26,

} BYTE_buffer_type;

/*! \define BYTE_buffer_type_capacity
*	Defines total number of declared buffers. It is for internal use.
*/
#define BYTE_buffer_type_capacity 27

//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
/*! \define BYTE3_buffer_type_shift
*	Defines shift for 3-plane BYTE buffer storage
*/
#define BYTE3_buffer_type_shift 50

/*! \enum BYTE3_buffer_type
*   \brief  defines additional buffers, which have 3 planes inside of them
*	
*/	
typedef enum
{
	BYTE3_compensated_frame		= BYTE3_buffer_type_shift + 0,
	BYTE3_compensated_frame_dil	= BYTE3_buffer_type_shift + 1,
	BYTE3_compensated_frame_ero = BYTE3_buffer_type_shift + 2,
	BYTE3_frame_difference_dil	= BYTE3_buffer_type_shift + 3,
	BYTE3_frame_difference_ero	= BYTE3_buffer_type_shift + 4,
} BYTE3_buffer_type;

/*! \define BYTE3_buffer_type_capacity
*	Defines total number of declared buffers. It is for internal use.
*/
#define BYTE3_buffer_type_capacity 5

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*! \define cMV_buffer_type_capacity
*	Defines shift for MV buffers constant numbers. Do not change if you don't understand.
*/
#define cMV_buffer_type_shift 100

/*! \enum cMV_buffer_type
*   \brief  defines all additional buffers of cMV type
*	
*	This enum contains constants for all buffers, which are vector-based
*	To add new buffer expand enum and change constant below
*/	
typedef enum
{
	cMV_field				= cMV_buffer_type_shift + 0,
	cMV_field_filtered		= cMV_buffer_type_shift + 1,
	cMV_texture_filtered	= cMV_buffer_type_shift + 2,
	cMV_uniform_filtered	= cMV_buffer_type_shift + 3,
	cMV_dilated_field		= cMV_buffer_type_shift + 4,
	cMV_eroded_field		= cMV_buffer_type_shift + 5,
} cMV_buffer_type;

/*! \define cMV_buffer_type_capacity
*	Defines total number of declared buffers. It is for internal use.
*/
#define cMV_buffer_type_capacity 6


/*! \struct Buffer_Storage
*   \brief Encapsulates work with multiple buffers, using constant enum
*	
*/
typedef struct 
{
	BYTE *BYTE_buffers[BYTE_buffer_type_capacity];	// buffers of BYTE type
	cMV  *cMV_buffers[cMV_buffer_type_capacity];	// buffers of cMV type

	BYTE *BYTE_3plane_buffers[BYTE3_buffer_type_capacity];
	
	int width;			// width  of frame buffer
	int height;			// height of frame buffer
	long frame_size;	// area   of frame buffer

} Buffer_Storage;

/**
**************************************************************************
* \brief Initializes and allocates memory for Buffer_Storage
*/
void Buffer_Storage_Allocate(Buffer_Storage * storage, int width, int height);

/**
**************************************************************************
* \brief Deallocates memory for Buffer_Storage
*/
void Buffer_Storage_Deallocate(Buffer_Storage * storage);

/**
**************************************************************************
* \brief Returns pointer to memory buffer of given id, BYTE type
*/
BYTE *Buffer_Storage_GetBuffer_BYTE(Buffer_Storage * storage, BYTE_buffer_type type);

/**
**************************************************************************
* \brief Returns pointer to memory buffer of given id, cMV type
*/
cMV  *Buffer_Storage_GetBuffer_cMV(Buffer_Storage * storage, cMV_buffer_type type);

/**
**************************************************************************
* \brief Copy buffer from internal memory to external memory
*/
void Buffer_Storage_ExportBYTE(Buffer_Storage * storage, BYTE_buffer_type src_type, BYTE *dst);

/**
**************************************************************************
* \brief Performs internal copy between to buffer types inside of Buffer_Storage
*/
void Buffer_Storage_InternalCopy(Buffer_Storage * storage, BYTE_buffer_type src_type, BYTE_buffer_type dst_type);


#define GETSTORAGEBUF_LEFT_BYTE(scale, buf_type)  ( Buffer_Storage_GetBuffer_BYTE( & MEstruct->left_frame_buffers[scale], buf_type) )
#define GETSTORAGEBUF_RIGHT_BYTE(scale, buf_type) ( Buffer_Storage_GetBuffer_BYTE( & MEstruct->right_frame_buffers[scale], buf_type) )

#define GETSTORAGEBUF_LEFT_cMV(scale, buf_type)  ( Buffer_Storage_GetBuffer_cMV( & MEstruct->left_frame_buffers[scale], buf_type) )
#define GETSTORAGEBUF_RIGHT_cMV(scale, buf_type) ( Buffer_Storage_GetBuffer_cMV( & MEstruct->right_frame_buffers[scale], buf_type) )

#define BUF_BYTE(buf_type) Buffer_Storage_GetBuffer_BYTE ( frame_buffers, buf_type )
#define BUF_cMV(buf_type) Buffer_Storage_GetBuffer_cMV ( frame_buffers, buf_type )


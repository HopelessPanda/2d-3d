#pragma once

#include "Calculation_Buffers.h"
#include "MExt_FrameTransform.h"
#include "HierME_Params.h"

#include "OcclEstimation_2D.h"
#include "OcclEstimation_Stereo.h"

#include <VideoGMLib/Common/CommonTypes.h>
#include <VideoVGCLib/MEstimation/cMEstimation.h>

//#include "../FRCCore/types.h"

/*! \enum frame_id
*   \brief Defines left and right frame IDs, which are used during processing for unification 
*
*/	
typedef enum {left = 0, right = 1} frame_id;

/*! \enum MEBlockSize
*   \brief  Block sizes, which are used for contrast calculation
*
*/	
typedef enum 
{
	B_4x4 = 0,
	B_8x8 = 1, 
	B_16x16 = 2,
	B_32x32 = 3
} MEBlockSize;

/*! \typedef pGetBlockError
*   \brief Pointer to the SAD function between 2 blocks
*
*/	
typedef long (*pGetBlockError) (const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);

/*! \struct MVCBlock
*   \brief Defines block, used during Motion vectors filtering.
*
*/	
typedef struct MVContrastBlock
{
	int x, y;			// coordinates of block center
	cMV vector;			// vector, corresponding to the block
	int block_contrast;	// contrast value, corresponding to the block
	int block_size;		// size of block
} MVCBlock;

/*! \struct Hierarchic_ME
*   \brief Contains all data, required for hierarchic bidirectional 
*   motion vectors calculation and refinement
*   
*   This structure is used to launch several instances of ME (on different 
*   resolutions - each ME runs on different scale. Structure contains 
*   special information buffers for both of provided frames. Bidirectional 
*   processing is used in any case - it provides much more information for 
*   additional refinement.
*   Pointers are used everywhere because there are N independent processing 
*   scales, calculating same information for different resolutions.
*   Originally, processing was developed for stereo (so main goal is to 
*   calculate a HQ disparity). But most can be used for arbitrary ME also. 
*
*/
typedef struct Hierarchic_ME
{
	ME_Frame *left_source_frames;					// left  source frames
	ME_Frame *right_source_frames;					// right source frames

	cMEParams *ME_Parameters;						// Motion estimation parameters
	gmcMotionEstimator* left_frame_ME_instances;	// ME instances for left frame
	gmcMotionEstimator* right_frame_ME_instances;	// ME instances for right frame

	Buffer_Storage *left_frame_buffers;				// storage of additional info for left  frame
	Buffer_Storage *right_frame_buffers;			// storage of additional info for right frame

	// 1 - number of fields = scale
	// 2 - number of block sizes
	// 3 - frame buffer
	int				***left_frame_contrast;			// internal buffers for left frame contrast calculation
	int				***right_frame_contrast;		// internal buffers for right frame contrast calculation

	pGetBlockError		GetContrastErrorFuncs[4];	// functions, which are used for contrast calculation (SAD functions)
	contrast_conversion ConvertContrastFunc;		// function, which is used for converting contrast to 0..255 diapason

	Hier_ME_Params hierarchy_parameters;			// parameters of Hierarhic ME launch (number of scales, etc.)

	long left_frame_average_contrast[4];			// OBSOLETE: average contrast error of different scales 
	long right_frame_average_contrast[4];			// OBSOLETE: average contrast error of different scales

	MVCBlock *candidates_list;						// internal buffer of candidates for motion vectors filtering
	int dispersion_threshold[2];					// OBSOLETE: threshold, which was used in old variant of uniform mask creation

//	FRCData *confidence_data;						// internal data for calculation of vectors confidence
	DisparityOcclData *occlusion_data;				// internal data, which is used for occlusion refinement 
	
}Hierarchic_ME;

/**
**************************************************************************
* \brief Returns pointer to internal ME instance of given direction and scale
*/
gmcMotionEstimator * Hierarchy_ME_GetMEInstance(Hierarchic_ME *ME_struct, frame_id direction, int scale);

/**
**************************************************************************
* \brief Allocates memory for all buffers of all scales and initializes ME instances
*/
void Hierarchy_ME_Init(Hierarchic_ME* MEstruct, Hier_ME_Params* Params, int w, int h);

/**
**************************************************************************
* \brief Deallocates all used memory
*/
void Hierarchy_ME_DeInit(Hierarchic_ME* MEstruct);

/**
**************************************************************************
* \brief Launches main processing routine for 2 frames - ME calculation.
*/
void Hierarchy_ME_MainProcess(Hierarchic_ME* MEstruct, const BYTE *cur_frame, const BYTE *ref_frame);

/**
**************************************************************************
* \brief Provides recalculation of most metrics for new vector field
*/
void Hierarchy_ME_RecalculateMetrics( Hierarchic_ME* MEstruct, int scale );

/**
**************************************************************************
* \brief Performs filtering of disparities or motion fields.
*/
void Hierarchy_ME_Filter_MVF(Hierarchic_ME *MEstruct, void *buffer_left, void *buffer_right, frame_type type_of_output, int scale);

/**
**************************************************************************
* \brief Performs confidence-based filtering (probably useless)
*/
void Hierarchy_ME_Confidence_Based_Filter(Hierarchic_ME * me_struct, BYTE *dst_disparity, frame_id direction, int scale, int filter_radius);

/**
**************************************************************************
* \brief Provides pointer (or copy) of one of precalculated buffers with given type, ID and scale
*/
void * Hierarchy_ME_GetPrecalcBuffer(Hierarchic_ME *me_struct, void *dst, int type_of_output, frame_id direction, int scale);

/**
**************************************************************************
* \brief Provides refinement of occlusion areas in disparity and vector fields
*/
void Hierarchy_ME_OcclusionRefinement(Hierarchic_ME * MEstruct, int scale, BOOL uniform_filter);

/**
**************************************************************************
* \brief Provides disparity with quarter-pixel precision
*/
void Hierarchy_ME_GetDisparity(Hierarchic_ME * MEstruct, short * dst, int scale, frame_id direction);

/**
**************************************************************************
* \brief Draws MV buffer, using color wheel
*/
void Hierarchy_ME_Draw_MV_Visualization( Hierarchic_ME * MEstruct, BYTE * rgb_output, cMV_buffer_type field, frame_id direction, int scale);

/**
**************************************************************************
* \brief Sets external depth mapsfor better filtering of 2D
*/
void Hierarchy_ME_SetExternalDepth( Hierarchic_ME * MEstruct, BYTE * left_depth, BYTE * right_depth );

int Hierarchy_ME_Params_size();

int Hierarchy_ME_Instance_size();

void Hierarchy_ME_get_vector(Hierarchic_ME * MEstruct,int x,int y,int *vx,int *vy);

#pragma once

#include <VideoGMLib/Common/CommonTypes.h>
#include <VideoVGCLib/MEstimation/MEParams/cMEParams.h>

/*! \enum MVFilter_type
*   \brief Defines filtering type of motion vector field
*
*/
typedef enum { MVFilter_Unknown, MVFilter_2D, MVFilter_Stereo } MVFilter_type;

/*! \typedef contrast_conversion
*   \brief Pointer to the function, which performs contrast conversion from int to 0..255
*
*/
typedef BYTE (*contrast_conversion)(int contrast_val);

/*! \struct Hierarchic_ME_Params
*   \brief Defines Hierarchy ME parameters
*
*/
typedef struct Hierarchic_ME_Params
{
	cMEParams major_params;						// Motion Estimation general parameters.
	int scale_num;								// Number of scales for Hierarchy ME calculation
	int candidates_num;							// Number of candidates for motion vectors filtering
	double MV_mult_koef;						// Multiplication coefficient of depth filling. 4 for quarter-pixel precision

	contrast_conversion ConvertContrastFunc;	// function, that is used for converting contrast to 0..255
	BOOL use_uniform_filter;
	int uniform_filter_strength;				// strength of motion vectors filtering

	int additional_buffers_number;

	MVFilter_type mvfilter_type;				// used type of filtering

	BOOL use_occlusion_processing;

}Hier_ME_Params;

/**
**************************************************************************
* \brief Copies parameter struct from one to another
*/
void cCopyHierMEParams(Hier_ME_Params* dst, Hier_ME_Params* src);

/**
**************************************************************************
* \brief Initializes parameters structure
*/
void cInitHierMEParams ( Hier_ME_Params* Params, MVFilter_type type, int frame_width, int frame_height );

/**
**************************************************************************
* \brief Changes scale of processing
*/
_inline void cSetHierMEScale(Hier_ME_Params *params, int scale)
{
	params->scale_num = scale;
}

/**
**************************************************************************
* \brief Sets conversion function for contrast calculation
*/
_inline void cSetHierMEContrastConversionFunc(Hier_ME_Params *params, contrast_conversion func)
{
	params->ConvertContrastFunc = func;
}

/**
**************************************************************************
* \brief Sets multiplication precision coefficient
*/
_inline void cSetHierMEMultKoef(Hier_ME_Params *params, double mult_koef)
{
	params->MV_mult_koef = mult_koef;
}

/**
**************************************************************************
* \brief Sets number of candidates for motion filtering
*/
_inline void cSetHierMEFilterCandidatesNumber(Hier_ME_Params *params, int cands)
{
	params->candidates_num = cands;
}

/**
**************************************************************************
* \brief Sets MV filtering mode
*/
_inline void cSetHierMEFilterType(Hier_ME_Params *params, MVFilter_type type)
{
	params->mvfilter_type = type;
}

/*_inline void cSetHierMEAdditionalBuffersNum(Hier_ME_Params * params, int number)
{
	params->additional_buffers_number = number;
	//cSetAdditionalBuffersNumberME( &params->major_params, number );
}*/
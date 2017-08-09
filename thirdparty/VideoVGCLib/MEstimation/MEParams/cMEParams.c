/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/
/**
*  \file cMEParams.c
*  \brief Motion estimation parameters.
*
*  This file contains implementations of operation functions
*  over motion estimation pareameters structure.
*
*/

#include "cMEParams.h"

/**
**************************************************************************
* \brief Default initialization of cMEParams
*
*  This function initializes cMEParams structure with default ME settings
*
* \param cMEParamsStruct     [IN/OUT] - ME parameters structure
*
* \return none
*/
void cInitMEParams(cMEParams* cMEParamsStruct)
{
	cMEParamsStruct->m_iMaxMotHor = 32;
	cMEParamsStruct->m_iMaxMotVert = 32;
	cMEParamsStruct->m_MEAlgorithm = MSU;
	cMEParamsStruct->m_MEMetricAlgorithm = METRIC_SAD;
	cMEParamsStruct->m_PrecMX = QuarterPixel;
	cMEParamsStruct->m_PrecMY = QuarterPixel;
	cMEParamsStruct->m_bIsInterlaced = FALSE;
	cMEParamsStruct->m_MaxBlSize = cbt_16x16;
	cMEParamsStruct->m_MinBlSize = cbt_4x4;
	cMEParamsStruct->m_interAlg = SixTap;
	cMEParamsStruct->m_bBordersAllowed = TRUE;
	cMEParamsStruct->m_pRDOinfo=NULL;
    cMEParamsStruct->m_bSkipPreviousFrameCandidates = FALSE;
    cInitDevMEParams(&(cMEParamsStruct->m_devMEParams));
    cInitAdvancedMEParams(&(cMEParamsStruct->m_advMEParams));
}

/**
**************************************************************************
* \brief Default creation of cMEParams
*
*  This function creates and initializes cMEParams structure with default
*  ME settings
*
* \param ppMEParams          [OUT] - ME parameters structure
*
* \return none
*/
void cCreateMEParams ( cMEParams** ppMEParams )
{
    *ppMEParams = (cMEParams *)malloc( sizeof( cMEParams ) );
    cInitMEParams(*ppMEParams);
}

/**
**************************************************************************
* \brief Copying of cMEParams.
*
*  This function copies one cMEParams structure to another.
*
* \param dst                 [IN/OUT] - pointer to destination cMEParams structure
* \param src                 [IN]     - pointer to source cMEParams structure
*
* \return none
*/
void cCopyMEParams( cMEParams* dst, const cMEParams* src )
{
    cDeInitMEParams(dst);
    if (src == NULL) return;
    *dst = *src;

    cInitAdvancedMEParams(&(dst->m_advMEParams));
    cCopyAdvMEParams( &(dst->m_advMEParams), &(src->m_advMEParams) );

    cInitDevMEParams(&(dst->m_devMEParams));
    cCopyDevMEParams(&(dst->m_devMEParams), &(src->m_devMEParams));
}

/**
**************************************************************************
* \brief Sets ME search area
*
*  This function sets ME search area in cMEParams structure.
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param iMaxMotHor          [IN]     - maximal horizontal motion vector length in pixels
* \param iMaxMotVert         [IN]     - maximal vertical motion vector length in pixels
*
* \return none
*/
void cSetMotionAreaME( cMEParams* params, int iMaxMotHor, int iMaxMotVert )
{
    params->m_iMaxMotHor = iMaxMotHor;
    params->m_iMaxMotVert = iMaxMotVert;
}

/**
**************************************************************************
* \brief Choses ME algorithm
*
*  This function sets to cMEParams what ME algorithm should be used.
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param meAlg               [IN]     - ME algorithm to use
*
* \return none
*/
void cSetMEAlgME( cMEParams* params, MEAlgorithm meAlg )
{
    params->m_MEAlgorithm = meAlg;
}

/**
**************************************************************************
* \brief Sets ME precision.
*
*  This function sets to cMEParams what precision should ME use: pixel,
*  half-pixel or quarter-pixel.
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param precMX              [IN]     - ME precision in horizontal direction
* \param precMY              [IN]     - ME precision in vertical direction
*
* \return none
*/
void cSetPrecisionME( cMEParams* params, EstimationPrecision precMX, EstimationPrecision precMY )
{
    params->m_PrecMX = precMX;
    params->m_PrecMY = precMY;
}

/**
**************************************************************************
* \brief Sets information if about source video (interlaced/progressive).
*
*  This function sets to cMEParams format of the video to operate upon:
*  progressive or interlaced.
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param bIntrlcd            [IN]     - TRUE if video is interlaced
*
* \return none
*/
void cSetInterlacedME( cMEParams* params, BOOL bIntrlcd )
{
    params->m_bIsInterlaced = bIntrlcd;
}

/**
**************************************************************************
* \brief Sets ME block sizes.
*
*  This function sets to cMEParams what block sizes should ME use: 4x4,
*  8x8, 16x16 or some combinations of them. COmbination of block sizes
*  means that ME algorithm can chose on its own whether one of the
*  available block sizes will be used in current macroblock.
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param MaxBlSz             [IN]     - maximal block size for ME algorithm usage
* \param MinBlSz             [IN]     - minimal block size for ME algorithm usage
*
* \return none
*/
void cSetBlockSizeME( cMEParams* params, cMacroBlockPartitions MaxBlSz, cMacroBlockPartitions MinBlSz )
{
    params->m_MaxBlSize = MaxBlSz;
    params->m_MinBlSize = MinBlSz;
}

/**
**************************************************************************
* \brief Sets if motion vector can point to the area outside the frame.
*
*  This function sets to cMEParams if ME algorithm can make vectors,
*  pointing outside the frame area.
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param bAllow              [IN]     - allow on TRUE
*
* \return none
*/
void cSetAllowBordersME( cMEParams* params, BOOL bAllow )
{
    params->m_bBordersAllowed = bAllow;
}

/**
**************************************************************************
* \brief Sets ME interpolation algorithm for sub-pixel shifts.
*
*  This function sets to cMEParams what interpolation algorithm ME should
*  use for sub-pixel shifts: as in H.264 standart or simple linear.
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param interpAlg           [IN]     - interpolation algorithm to use
*
* \return none
*/
void cSetInterpolationME( cMEParams* params, InterpolationAlgorithm interpAlg )
{
    params->m_interAlg = interpAlg;
}

/**
**************************************************************************
* \brief Sets additional info used by RDO error functions.
*
*  This function sets to cMEParams a pointer to additional info used by
*  RDO error functions
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param pRDOInfo            [IN]     - pointer to RDO information
*
* \return none
*/
void cSetRDOInfoME( cMEParams* params, void* pRDOInfo )
{
    params->m_pRDOinfo = pRDOInfo;
}

/**
**************************************************************************
* \brief Sets ME block difference functions for luma component.
*
*  This function sets to cMEParams what function for block difference
*  calculation using luma component for usage during ME.
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param pGetError_16x16     [IN]     - pointer to the 16x16 block difference calculation function
* \param pGetError_16x8      [IN]     - pointer to the 16x8 block difference calculation function
* \param pGetError_8x8       [IN]     - pointer to the 8x8 block difference calculation function
* \param pGetError_4x4       [IN]     - pointer to the 4x4 block difference calculation function
*
* \return none
*/
void cSetSADFunctionsME( cMEParams* params, BlockDiffFunction pGetError_16x16, BlockDiffFunction pGetError_16x8, BlockDiffFunction pGetError_8x8, BlockDiffFunction pGetError_4x4 )
{
    cSetSADFunctions( &(params->m_advMEParams), pGetError_16x16, pGetError_16x8, pGetError_8x8, pGetError_4x4);
}

/**
**************************************************************************
* \brief Sets ME block difference functions for chroma component.
*
*  This function sets to cMEParams what function for block difference
*  calculation using chroma component for usage during ME.
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param pGetErrorChr_16x16  [IN]     - pointer to the 16x16 block difference calculation function
* \param pGetErrorChr_8x8    [IN]     - pointer to the 8x8 block difference calculation function
* \param pGetErrorChr_4x4    [IN]     - pointer to the 4x4 block difference calculation function
*
* \return none
*/
void cSetSADFunctionsChromaME( cMEParams* params, BlockDiffFunction pGetErrorChr_16x16, BlockDiffFunction pGetErrorChr_8x8, BlockDiffFunction pGetErrorChr_4x4 )
{
    cSetSADFunctionsChroma( &(params->m_advMEParams), pGetErrorChr_16x16, pGetErrorChr_8x8, pGetErrorChr_4x4 );
}

/**
**************************************************************************
* \brief Sets ME chroma settings.
*
*  This function sets to cMEParams ME algorithm should use chroma for
*  block difference calculation and sizes of chroma in comparison to
*  luma (if it is sampled down or not).
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param ConsiderChroma      [IN]     - TRUE if ME algorithm should take chroma into count
* \param ChrXMod             [IN]     - sampling down of chroma in horizontal direction
* \param ChrYMod             [IN]     - sampling down of chroma in vertical direction
*
* \return none
*/
void cSetChromaSettingsME( cMEParams* params, BOOL ConsiderChroma, SuitableChromaModifiers ChrXMod, SuitableChromaModifiers ChrYMod )
{
    cSetChromaSettings( &(params->m_advMEParams), ConsiderChroma, ChrXMod, ChrYMod );
}

/**
**************************************************************************
* \brief Sets ME hierarchic reduction settings.
*
*  This function sets to cMEParams the factor of hierarchic reduction (if
*  the one is used during ME).
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param HierReduction       [IN]     - hierarchic reduction factor
*
* \return none
*/
void cSetHierReductionME( cMEParams* params, SuitableHierReduction HierReduction )
{
    cSetHierReduction( &(params->m_advMEParams), HierReduction );
}

/**
**************************************************************************
* \brief Sets if ME should save its statistic.
*
*  This function sets to cMEParams if ME algorithm should save statistic
*  about itself and the name of the file to save statistic to. Useful for
*  debugging.
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param bSaveStat           [IN]     - TRUE if ME algorithm should save statistics
* \param sName               [IN]     - name of file to save statistic to
*
* \return none
*/
void cSetStatisticFileNameME( cMEParams* params, BOOL bSaveStat, char* sName )
{
    cSetStatisticFileName( &(params->m_devMEParams), bSaveStat, sName );
}

/**
**************************************************************************
* \brief Sets coefficients for ME mean error calculation.
*
*  This function sets to cMEParams coefficients and initial values of ME
*  mean error per pixel. Calculated mean error is used in block division
*  decisions.
*
* \param params              [IN/OUT] - cMEParams structure to operate upon
* \param fMeanErrorPP        [IN]     - initial mean error per pixel
* \param fMeanErrRenew       [IN]     - mean error per pixel renew coefficient
* \param fDispersionPP       [IN]     - initial error dispersion parameter
* \param fDispersionRenew    [IN]     - dispersion renew coefficient
*
* \return none
*/
void cSetCoefficientsME( cMEParams* params, float fMeanErrorPP, float fMeanErrRenew, float fDispersionPP, float fDispersionRenew )
{
    cSetCoefficients( &(params->m_devMEParams), fMeanErrorPP, fMeanErrRenew, fDispersionPP, fDispersionRenew );
}


/**
**************************************************************************
* \brief Default deinitialization of cMEParams
*
*  This function deinitializes cMEParams structure
*
* \param cMEParamsStruct     [IN/OUT] - pointer to ME parameters structure
*
* \return none
*/
void cDeInitMEParams( cMEParams* cMEParamsStruct )
{
	cDeInitDevMEParams(&(cMEParamsStruct->m_devMEParams));
    cDeInitAdvancedMEParams(&(cMEParamsStruct->m_advMEParams));
}

/**
**************************************************************************
* \brief Destruction of cMEParams.
*
*  This function deinitializes and destroys cMEParams structure.
*
* \param ppMEParams          [IN/OUT] - ME parameters structure
*
* \return none
*/
void cDestroyMEParams( cMEParams** ppMEParams )
{
    cDeInitMEParams(*ppMEParams);
    free(*ppMEParams);
    *ppMEParams = NULL;
}
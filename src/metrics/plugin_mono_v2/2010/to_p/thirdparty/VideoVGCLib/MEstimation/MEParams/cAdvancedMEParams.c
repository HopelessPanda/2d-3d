/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/
/**
*  \file cAdvancedMEParams.c
*  \brief Motion estimation advanced parameters.
*
*  This file contains implimentations of operation functions over
*  motion estimation algorithm advanced parameters structure. One
*  whould not touch this parameters normally.
*
*/

#include "cAdvancedMEParams.h"

/**
**************************************************************************
* \brief Default initialization of cAdvancedMEParams
*
*  This function initializes cAdvancedMEParams structure with default
*  advanced ME settings
*
* \param advMEParams         [IN/OUT] - ME parameters structure
*
* \return none
*/
void cInitAdvancedMEParams( cAdvancedMEParams* advMEParams )
{
	advMEParams->pGetError_16x16       = GetErrorSAD_16x16_MMX;
	advMEParams->pGetError_16x8        = GetErrorSAD_16x8_MMX;
	advMEParams->pGetError_8x8         = GetErrorSAD_8x8_MMX;
	advMEParams->pGetError_4x4         = GetErrorSAD_4x4_C;
	advMEParams->m_bConsiderChroma     = FALSE;
	advMEParams->m_ChrXMod            = crTwoTimes;
    advMEParams->m_ChrYMod            = crTwoTimes;
	advMEParams->pGetErrorChr_16x16    = GetErrorSAD_8x8_MMX;
	advMEParams->pGetErrorChr_8x8      = GetErrorSAD_4x4_C;
	advMEParams->pGetErrorChr_4x4      = GetErrorSAD_2x2_C;
    advMEParams->m_HierarchicReduction = (SuitableHierReduction)crTwoTimes;
	advMEParams->m_CandidatesMemoryAlloc = cmaOnFly;
	advMEParams->m_CandKeepPrealloc    = 28;

    //TODO: what value is better?
    //Added some value because of uninitialized memory usage
    advMEParams->m_bConsiderVectorLength = TRUE;
}

/**
**************************************************************************
* \brief Default creation of cAdvancedMEParams
*
*  This function creates and initializes cMEParams structure with default
*  advanced ME settings
*
* \param ppAdvParams         [OUT]    - ME parameters structure
*
* \return none
*/
void cCreateAdvancedMEParams( cAdvancedMEParams** ppAdvParams )
{
    *ppAdvParams = ( cAdvancedMEParams* )malloc( sizeof(cAdvancedMEParams) );
    cInitAdvancedMEParams(*ppAdvParams);
}

/**
**************************************************************************
* \brief Sets ME block difference functions for luma component.
*
*  This function sets to cAdvancedMEParams what function for block difference
*  calculation using luma component for usage during ME.
*
* \param advMEParams         [IN/OUT] - cMEParams structure to operate upon
* \param pGetError_16x16     [IN]     - pointer to the 16x16 block difference calculation function
* \param pGetError_16x8      [IN]     - pointer to the 16x8 block difference calculation function
* \param pGetError_8x8       [IN]     - pointer to the 8x8 block difference calculation function
* \param pGetError_4x4       [IN]     - pointer to the 4x4 block difference calculation function
*
* \return none
*/
void cSetSADFunctions( cAdvancedMEParams* advMEParams,
                       BlockDiffFunction pGetError_16x16,
                       BlockDiffFunction pGetError_16x8,
                       BlockDiffFunction pGetError_8x8,
                       BlockDiffFunction pGetError_4x4
)
{
    advMEParams->pGetError_16x16 = pGetError_16x16;
    advMEParams->pGetError_16x8  = pGetError_16x8;
    advMEParams->pGetError_8x8   = pGetError_8x8;
    advMEParams->pGetError_4x4   = pGetError_4x4;
}

/**
**************************************************************************
* \brief Sets ME block difference functions for chroma component.
*
*  This function sets to cMEParams what function for block difference
*  calculation using chroma component for usage during ME.
*
* \param advMEParams         [IN/OUT] - cMEParams structure to operate upon
* \param pGetErrorChr_16x16  [IN]     - pointer to the 16x16 block difference calculation function
* \param pGetErrorChr_8x8    [IN]     - pointer to the 8x8 block difference calculation function
* \param pGetErrorChr_4x4    [IN]     - pointer to the 4x4 block difference calculation function
*
* \return none
*/
void cSetSADFunctionsChroma( cAdvancedMEParams* advMEParams,
                             BlockDiffFunction pGetErrorChr_16x16,
                             BlockDiffFunction pGetErrorChr_8x8,
                             BlockDiffFunction pGetErrorChr_4x4
)
{
    advMEParams->pGetErrorChr_16x16 = pGetErrorChr_16x16;
    advMEParams->pGetErrorChr_8x8   = pGetErrorChr_8x8;
    advMEParams->pGetErrorChr_4x4   = pGetErrorChr_4x4;
}

/**
**************************************************************************
* \brief Sets ME chroma settings.
*
*  This function sets to cMEParams ME algorithm should use chroma for
*  block difference calculation and sizes of chroma in comparison to
*  luma (if it is sampled down or not).
*
* \param advMEParams         [IN/OUT] - cMEParams structure to operate upon
* \param ConsiderChroma      [IN]     - TRUE if ME algorithm should take chroma into count
* \param ChrXMod             [IN]     - sampling down of chroma in horizontal direction
* \param ChrYMod             [IN]     - sampling down of chroma in vertical direction
*
* \return none
*/
void cSetChromaSettings(cAdvancedMEParams* advMEParams, BOOL ConsiderChroma, SuitableChromaModifiers ChrXMod, SuitableChromaModifiers ChrYMod)
{
    advMEParams->m_bConsiderChroma = ConsiderChroma;
    advMEParams->m_ChrXMod        = ChrXMod;
    advMEParams->m_ChrYMod        = ChrYMod;
}

/**
**************************************************************************
* \brief Sets ME error calculation settings.
*
*  This function sets to cMEParams ME algorithm should add vector length
*  to error to produce shorter vectors
*
* \param advMEParams          [IN/OUT] - cMEParams structure to operate upon
* \param ConsiderVectorLength [IN]     - TRUE if ME algorithm should add vector lenght to error
*
* \return none
*/
void cSetConsiderVector(cAdvancedMEParams* advMEParams, BOOL ConsiderVectorLength)
{
	advMEParams->m_bConsiderVectorLength = ConsiderVectorLength;
}

/**
**************************************************************************
* \brief Sets ME hierarchic reduction settings.
*
*  This function sets to cMEParams the factor of hierarchic reduction (if
*  the one is used during ME).
*
* \param advMEParams         [IN/OUT] - cMEParams structure to operate upon
* \param HierReduction       [IN]     - hierarchic reduction factor
*
* \return none
*/
void cSetHierReduction( cAdvancedMEParams* advMEParams, SuitableHierReduction HierReduction )
{
    advMEParams->m_HierarchicReduction = HierReduction;
}

/**
**************************************************************************
* \brief Copying of cAdvancedMEParams.
*
*  This function copies one cAdvancedMEParams structure to another.
*
* \param dst                 [IN/OUT] - pointer to destination cMEParams structure
* \param src                 [IN]     - pointer to source cMEParams structure
*
* \return none
*/
void cCopyAdvMEParams( cAdvancedMEParams* dst, const cAdvancedMEParams* src )
{
    cDeInitAdvancedMEParams( dst );
    *dst = *src;
}

/**
**************************************************************************
* \brief Deinitialization of cMEParams
*
*  This function deinitializes cAdvancedMEParams structure with default
*  ME settings
*
* \param advMEParams         [IN/OUT] - advanced ME parameters structure
*
* \return none
*/
void cDeInitAdvancedMEParams( cAdvancedMEParams* advMEParams )
{
}

/**
**************************************************************************
* \brief Destruction of cAdvancedMEParams.
*
*  This function deinitializes and destroys cAdvancedMEParams structure.
*
* \param ppAdvParams         [IN/OUT] - advanced ME parameters structure
*
* \return none
*/
void cDestroyAdvancedMEParams( cAdvancedMEParams** ppAdvParams )
{
    cDeInitAdvancedMEParams(*ppAdvParams);
    free(*ppAdvParams);
    *ppAdvParams = NULL;
}
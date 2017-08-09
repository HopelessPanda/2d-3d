/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/
/**
*  \file cAdvancedMEParams.h
*  \brief Motion estimation advanced parameters.
*
*  This file contains definition of motion estimation algorithm
*  advanced parameters structure and declarations of operation
*  functions over this structure. One whould not touch this
*  parameters normally.
*
*/

#ifndef __CADVANCEDMEPARAMS_H__
#define __CADVANCEDMEPARAMS_H__

#include <stdlib.h>
#include "../../../VideoGMLib/Common/CommonTypes.h"
#include "../cGetError.h"

/*! \enum SuitableHierReduction
*   \brief Hierarchic reduction
*
*  Describes suitable values for hierarchic reduction
*/
typedef enum
{
    hrTwoTimes = 2,   // two times reduction
    hrFourTimes = 4   // four times reduction
} SuitableHierReduction;

/*! \enum SuitableChromaModifiers
*   \brief Chroma reduction
*
*  Describes suitable values for chroma reduction
*/
typedef enum
{
    crNoReduction = 1,   // no reduction
    crTwoTimes = 2       // two times reduction
} SuitableChromaModifiers;

/*! \enum CandMemoryAllocation
*   \brief MV candidates memory allocation
*
*  Describes how to allocate memory for MV candidates
*/
typedef enum
{
	cmaOnStart = 0, // default, requires much memory on start
	cmaOnFly = 1   // requires lesser memory
} CandMemoryAllocation;

/*! \struct cAdvancedMEParams
*   \brief Structure of advanced ME parameters
*
*  This structure contains advanced parameters of motion
*  estimation algorithm
*/	
typedef struct
{
	BlockDiffFunction pGetError_16x16;                        //!< Pointer to the currently used difference function for 16x16 blocks
	BlockDiffFunction pGetError_16x8;                         //!< Pointer to the currently used difference function for 16x8 blocks
	BlockDiffFunction pGetError_8x8;                          //!< Pointer to the currently used difference function for 8x8 blocks
	BlockDiffFunction pGetError_4x4;                          //!< Pointer to the currently used difference function for 4x4 blocks
	BlockDiffFunction pGetErrorChr_16x16;                     //!< Pointer to the currently used difference function for 16x16 blocks
	BlockDiffFunction pGetErrorChr_8x8;                       //!< Pointer to the currently used difference function for 8x8 blocks
	BlockDiffFunction pGetErrorChr_4x4;                       //!< Pointer to the currently used difference function for 4x4 blocks
	BOOL m_bConsiderChroma;                                   //!< TRUE if we should consider chroma
	BOOL m_bConsiderVectorLength;                             //!< TRUE if we should add vector length to blocks' difference
	SuitableChromaModifiers m_ChrXMod, m_ChrYMod;             //!< colorspace resampling down of chroma components horizontally and vertically
	SuitableHierReduction m_HierarchicReduction;              //!< reduction of block sizes during hierarchic search
	CandMemoryAllocation m_CandidatesMemoryAlloc;             //!< memory allocation for MV candidates
	int m_CandKeepPrealloc;                                   //!< memory for how many MV candidates is preallocated
} cAdvancedMEParams;

/**
**************************************************************************
* \brief Default initialization of cAdvancedMEParams
*
*/
void cInitAdvancedMEParams( cAdvancedMEParams* advMEParams );
/**
**************************************************************************
* \brief Default creation of cAdvancedMEParams
*
*/
void cCreateAdvancedMEParams( cAdvancedMEParams** ppAdvParams );

/**
**************************************************************************
* \brief Sets ME block difference functions for luma component.
*
*/
void cSetSADFunctions(cAdvancedMEParams* advMEParams, BlockDiffFunction pGetError_16x16, BlockDiffFunction pGetError_16x8, BlockDiffFunction pGetError_8x8, BlockDiffFunction pGetError_4x4);
/**
**************************************************************************
* \brief Sets ME block difference functions for chroma component.
*
*/
void cSetSADFunctionsChroma(cAdvancedMEParams* advMEParams, BlockDiffFunction pGetErrorChr_16x16, BlockDiffFunction pGetErrorChr_8x8, BlockDiffFunction pGetErrorChr_4x4);
/**
**************************************************************************
* \brief Sets ME chroma settings.
*
*/
void cSetChromaSettings(cAdvancedMEParams* advMEParams, BOOL ConsiderChroma, SuitableChromaModifiers ChrXMod, SuitableChromaModifiers ChrYMod);
/**
**************************************************************************
* \brief Sets ME error calculation settings.
*
*/
void cSetConsiderVector(cAdvancedMEParams* advMEParams, BOOL ConsiderVectorLength);

/**
**************************************************************************
* \brief Sets ME hierarchic reduction settings.
*
*/
void cSetHierReduction(cAdvancedMEParams* advMEParams, SuitableHierReduction HierReduction);

/**
**************************************************************************
* \brief Copying of cAdvancedMEParams.
*
*/
void cCopyAdvMEParams(cAdvancedMEParams* dst, const cAdvancedMEParams* src);
/**
**************************************************************************
* \brief Deinitialization of cMEParams
*
*/
void cDeInitAdvancedMEParams(cAdvancedMEParams* advMEParams);
/**
**************************************************************************
* \brief Destruction of cAdvancedMEParams.
*
*/
void cDestroyAdvancedMEParams( cAdvancedMEParams** ppAdvParams );

#endif

/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/
/**
*  \file cMEParams.h
*  \brief Motion estimation parameters.
*
*  This file contains definition of motion estimation algorithm
*  parameters structure and declarations of operation functions
*  over this structure.
*
*/

#ifndef __CMEPARAMS_H__
#define __CMEPARAMS_H__

#include "cAdvancedMEParams.h"
#include "cDevMEParams.h"

/*! \enum MEAlgorithm
*   \brief ME algorithm
*
*  Describes available motion estimation algorithms
*/
typedef enum
{
    _3DRS,
    E3DRS,
    FSA,
    MEDS,
    HIER,
    MSU
} MEAlgorithm;

/*! \enum MEMetricAlgorithm
*   \brief ME metric algorithm
*
*  Describes available metrics for ME
*/
typedef enum
{
	METRIC_SAD = 0,               // use SAD metric (default)
	METRIC_COLORINDEPENDENT = 1   // use color independent metric
} MEMetricAlgorithm;

/*! \enum InterpolationAlgorithm
*   \brief Halfpel interpolation algorithm
*
*  Describes the algorithm used to interpolate half-pixel values
*/
typedef enum
{
    SixTap,		//6-tap filter
    Bilinear	//Bilinear interpolation
} InterpolationAlgorithm;

/*! \struct cMEParams
*   \brief Structure of ME parameters
*
*  This structure contains parameters of motion estimation
*  algorithm
*/
typedef struct
{
	int m_iMaxMotVert, m_iMaxMotHor;            //!< maximum vertical and horizontal dimensions of search area (in pixels) for the entire video
	MEAlgorithm m_MEAlgorithm;                  //!< ME algorithm
	MEMetricAlgorithm m_MEMetricAlgorithm;      //!< ME Metric algorithm
	EstimationPrecision m_PrecMX, m_PrecMY;     //!< precision of motion vectors in X and Y directions
	BOOL m_bIsInterlaced;                       //!< estimating interlaced video or not
	cMacroBlockPartitions m_MaxBlSize;          //!< maximum block size in macroblock
	cMacroBlockPartitions m_MinBlSize;          //!< minimum block size in macroblock
	BOOL m_bBordersAllowed;                     //!< True, if MC from frame borders is allowed
	InterpolationAlgorithm m_interAlg;          //!< Halfpel interpolation algorithm
	void* m_pRDOinfo;                           //!< Pointer to additional info used by RDO error functions
	cDevMEParams m_devMEParams;                 //!< Pointer to the inner params structure
	cAdvancedMEParams m_advMEParams;            //!< Advanced motion estimation parameters
    BOOL m_bSkipPreviousFrameCandidates;        //!< Skip usage of candidates from the previous frame
} cMEParams;

/**
**************************************************************************
* \brief Default initialization of cMEParams
*
*/
void cInitMEParams( cMEParams* cMEParamsStruct );
/**
**************************************************************************
* \brief Default creation of cMEParams
*
*/
void cCreateMEParams( cMEParams** ppMEParams );

/**
**************************************************************************
* \brief Copying of cMEParams.
*
*/
void cCopyMEParams( cMEParams* dst, const cMEParams* src );

/**
**************************************************************************
* \brief Sets ME search area
*
*/
void cSetMotionAreaME( cMEParams* params, int iMaxMotHor, int iMaxMotVert );
/**
**************************************************************************
* \brief Choses ME algorithm
*
*/
void cSetMEAlgME( cMEParams* params, MEAlgorithm meAlg );
/**
**************************************************************************
* \brief Sets ME precision.
*
*/
void cSetPrecisionME( cMEParams* params, EstimationPrecision precMX, EstimationPrecision precMY );
/**
**************************************************************************
* \brief Sets information if about source video (interlaced/progressive).
*
*/
void cSetInterlacedME( cMEParams* params, BOOL bIntrlcd );
/**
**************************************************************************
* \brief Sets ME block sizes.
*
*/
void cSetBlockSizeME( cMEParams* params, cMacroBlockPartitions MaxBlSz, cMacroBlockPartitions MinBlSz );
/**
**************************************************************************
* \brief Sets if motion vector can point to the area outside the frame.
*
*/
void cSetAllowBordersME( cMEParams* params, BOOL bAllow );
/**
**************************************************************************
* \brief Sets ME interpolation algorithm for sub-pixel shifts.
*
*/
void cSetInterpolationME( cMEParams* params, InterpolationAlgorithm interpAlg );
/**
**************************************************************************
* \brief Sets additional info used by RDO error functions.
*
*/
void cSetRDOInfoME( cMEParams* params, void* pRDOInfo );

/**
**************************************************************************
* \brief Sets ME block difference functions for luma component.
*
*/
void cSetSADFunctionsME( cMEParams* params,
                         BlockDiffFunction pGetError_16x16, BlockDiffFunction pGetError_16x8,
                         BlockDiffFunction pGetError_8x8, BlockDiffFunction pGetError_4x4 );
/**
**************************************************************************
* \brief Sets ME block difference functions for chroma component.
*
*/
void cSetSADFunctionsChromaME( cMEParams* params,
                               BlockDiffFunction pGetErrorChr_16x16,
                               BlockDiffFunction pGetErrorChr_8x8,
                               BlockDiffFunction pGetErrorChr_4x4 );
/**
**************************************************************************
* \brief Sets ME chroma settings.
*
*/
void cSetChromaSettingsME( cMEParams* params, BOOL ConsiderChroma, SuitableChromaModifiers ChrXMod, SuitableChromaModifiers ChrYMod );
/**
**************************************************************************
* \brief Sets ME hierarchic reduction settings.
*
*/
void cSetHierReductionME( cMEParams* params, SuitableHierReduction HierReduction );

/**
**************************************************************************
* \brief Sets if ME should save its statistic.
*
*/
void cSetStatisticFileNameME( cMEParams* params, BOOL bSaveStat, char* sName );
/**
**************************************************************************
* \brief Sets coefficients for ME mean error calculation.
*
*/
void cSetCoefficientsME( cMEParams* params, float fMeanErrorPP, float fMeanErrRenew, float fDispersionPP, float fDispersionRenew );

/**
**************************************************************************
* \brief Default deinitialization of cMEParams
*
*/
void cDeInitMEParams( cMEParams* cMEParamsStruct );
/**
**************************************************************************
* \brief Destruction of cMEParams.
*
*/
void cDestroyMEParams( cMEParams** ppMEParams );

#endif

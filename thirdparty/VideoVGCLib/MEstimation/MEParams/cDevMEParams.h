/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/
/**
*  \file cDevMEParams.h
*  \brief Motion estimation development parameters.
*
*  This file contains definition of motion estimation algorithm
*  parameters structure, useful for developers, and declarations
*  of operation functions over this structure.
*
*/

#ifndef __CDEVMEPARAMS_H__
#define __CDEVMEPARAMS_H__

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "../../../VideoGMLib/Common/CommonTypes.h"

/*! \struct cDevMEParams
*   \brief Structure of ME parameters, useful for developers.
*
*  This structure contains parameters of motion estimation useful for
*  developers usage.
*/	
typedef struct
{
	int m_iFileNameMem;             //!< amount of memory, allocated for file name storage
	char* m_sFileName;              //!< name of file to store statistic info in
	BOOL m_bSaveStatistic;          //!< true if we want to store statistic info
	float m_fMeanErrorPP;           //!< used to initialize mean per pixel error
	float m_fMeanErrRenew;          //!< coefficient used to renew mean error value
	float m_fDispersionPP;          //!< used to initialize "dispersion" of SAD values per pixel in frame
	float m_fDispersionRenew;       //!< used to initialize coefficient used to renew "dispersion" of SAD values pixel error
} cDevMEParams;

/**
**************************************************************************
* \brief Default initialization of cDevMEParams.
*
*/
void cInitDevMEParams( cDevMEParams* innerMEParams );
/**
**************************************************************************
* \brief Default creation of cDevMEParams.
*
*/
void cCreateDevMEParams( cDevMEParams** ppDevParams );

/**
**************************************************************************
* \brief Copying of cDevMEParams.
*
*/
void cCopyDevMEParams( cDevMEParams* dst, const cDevMEParams* src );

/**
**************************************************************************
* \brief Sets if ME should save its statistic.
*
*/
void cSetStatisticFileName( cDevMEParams* devParams, BOOL bSaveStat, char* sName );
/**
**************************************************************************
* \brief Sets coefficients for ME mean error calculation.
*
*/
void cSetCoefficients( cDevMEParams* devParams, float fMeanErrorPP, float fMeanErrRenew, float fDispersionPP, float fDispersionRenew );

/**
**************************************************************************
* \brief Default deinitialization of cDevMEParams
*
*/
void cDeInitDevMEParams( cDevMEParams* innerMEParams );
/**
**************************************************************************
* \brief Destruction of cDevMEParams.
*
*/
void cDestroyDevMEParams( cDevMEParams **ppDevParams );

#endif //__CDEVMEPARAMS_H__
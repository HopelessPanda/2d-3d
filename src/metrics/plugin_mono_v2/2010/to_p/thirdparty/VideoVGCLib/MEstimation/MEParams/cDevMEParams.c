/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/
/**
*  \file cDevMEParams.c
*  \brief Motion estimation development parameters.
*
*  This file contains implimentations of operation functions over
*  motion estimation algorithm parameters structure, useful for
*  developers.
*
*/
#include "cDevMEParams.h"

#pragma warning(disable:4996)

/**
**************************************************************************
* \brief Default initialization of cDevMEParams.
*
*  This function initializes cMEParams structure with default ME settings.
*
* \param devMEParams         [IN/OUT] - development ME parameters structure
*
* \return none
*/
void cInitDevMEParams( cDevMEParams* devMEParams )
{
    devMEParams->m_bSaveStatistic = FALSE;
	devMEParams->m_iFileNameMem = 100;
	devMEParams->m_sFileName = (char*)malloc(100);
	strcpy( devMEParams->m_sFileName, "C:\\_statistic.txt" );
	devMEParams->m_fMeanErrorPP = 1500.0f;
	devMEParams->m_fMeanErrRenew = 0.3f;
	devMEParams->m_fDispersionPP = 500.0f;
	devMEParams->m_fDispersionRenew = 0.3f;
}

/**
**************************************************************************
* \brief Default creation of cDevMEParams.
*
*  This function creates and initializes cDevMEParams structure
*  with default ME settings.
*
* \param ppDevParams         [OUT]    - development ME parameters structure
*
* \return none
*/
void cCreateDevMEParams( cDevMEParams** ppDevParams )
{
    *ppDevParams = (cDevMEParams*)malloc(sizeof(cDevMEParams));
    cInitDevMEParams(*ppDevParams);
}

/**
**************************************************************************
* \brief Copying of cDevMEParams.
*
*  This function copies one cDevMEParams structure to another.
*
* \param dst                 [IN/OUT] - pointer to destination cDevMEParams structure
* \param src                 [IN]     - pointer to source cDevMEParams structure
*
* \return none
*/
void cCopyDevMEParams( cDevMEParams* dst, const cDevMEParams* src )
{
    cDeInitDevMEParams(dst);
    *dst = *src;
    dst->m_sFileName = (char*)malloc( dst->m_iFileNameMem );
    strcpy( dst->m_sFileName, src->m_sFileName );
}

/**
**************************************************************************
* \brief Sets if ME should save its statistic.
*
*  This function sets to cDevMEParams if ME algorithm should save
*  statistic about itself and the name of the file to save statistic to.
*  Useful for debugging.
*
* \param devParams           [IN/OUT] - cDevMEParams structure to operate upon
* \param bSaveStat           [IN]     - TRUE if ME algorithm should save statistics
* \param sName               [IN]     - name of file to save statistic to
*
* \return none
*/
void cSetStatisticFileName( cDevMEParams* devParams, BOOL bSaveStat, char* sName )
{
    devParams->m_bSaveStatistic = bSaveStat;
    if (sName)
    {
        if (strlen(sName) + 1 > (size_t)devParams->m_iFileNameMem)
        {
            free(devParams->m_sFileName);
            devParams->m_iFileNameMem = (int)strlen(sName) + 1;
            devParams->m_sFileName = (char*)malloc(devParams->m_iFileNameMem);
        }
        strcpy(devParams->m_sFileName, sName);
    }
}

/**
**************************************************************************
* \brief Sets coefficients for ME mean error calculation.
*
*  This function sets to cDevMEParams coefficients and initial values of
*  ME mean error per pixel. Calculated mean error is used in block
*  division decisions.
*
* \param devParams           [IN/OUT] - cDevMEParams structure to operate upon
* \param fMeanErrorPP        [IN]     - initial mean error per pixel
* \param fMeanErrRenew       [IN]     - mean error per pixel renew coefficient
* \param fDispersionPP       [IN]     - initial error dispersion parameter
* \param fDispersionRenew    [IN]     - dispersion renew coefficient
*
* \return none
*/
void cSetCoefficients( cDevMEParams* devParams, float fMeanErrorPP, float fMeanErrRenew, float fDispersionPP, float fDispersionRenew )
{
    devParams->m_fMeanErrorPP     = fMeanErrorPP;
    devParams->m_fMeanErrRenew    = fMeanErrRenew;
    devParams->m_fDispersionPP    = fDispersionPP;
    devParams->m_fDispersionRenew = fDispersionRenew;
}

/**
**************************************************************************
* \brief Default deinitialization of cDevMEParams
*
*  This function deinitializes cDevMEParams structure
*
* \param devParams           [IN/OUT] - development ME parameters structure
*
* \return none
*/
void cDeInitDevMEParams( cDevMEParams* devParams )
{
	if (devParams->m_sFileName)
	{
		free(devParams->m_sFileName);
		devParams->m_sFileName = NULL;
		devParams->m_iFileNameMem = 0;
	}
}

/**
**************************************************************************
* \brief Destruction of cDevMEParams.
*
*  This function deinitializes and destroys cDevMEParams structure.
*
* \param ppDevParams         [IN/OUT] - development ME parameters structure
*
* \return none
*/
void cDestroyDevMEParams(cDevMEParams **ppDevParams)
{
    cDeInitDevMEParams(*ppDevParams);
    free(*ppDevParams);
    *ppDevParams = NULL;
}
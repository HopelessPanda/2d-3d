/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cMEstimation.c
*  \brief Performs motion estimation
*
*  This file contains implementation of the class that performs
*  motion estimation in video.
*/

#include "cMEstimation.h"

#pragma warning(disable:4996)

/**
**************************************************************************
* \brief Calculates error and error root-mean-square deviation
*
*  This function calculates error and error root-mean-square deviation for
*  current frame and updates statistic for entire sequence
*
* \param pMEStruct           [IN/OUT] -  pointer to gmcMotionEstimator structure
* 
* \return none
*/
void CalculateError(gmcMotionEstimator* pMEStruct)
{
	int i, j;
	float fDispersion;
	int iMBCounter = 0;

	// SAD calls quantity
	pMEStruct->m_MInfo.m_fFrameAverageSADsPerPixel /= (pMEStruct->m_iWidth*pMEStruct->m_iHeight);
	pMEStruct->m_fSequnceAverageSADsPerPixel += pMEStruct->m_MInfo.m_fFrameAverageSADsPerPixel;

	pMEStruct->m_fFrameAverageMEErrorPerBlock = 0;

	for (i = 0; i<pMEStruct->m_iMBlocksInFrame; i++)
	{
		if (pMEStruct->m_CurMBlocks[i].m_BlockPartition == cbt_16x16)
		{
			if (pMEStruct->m_CurMBlocks[i].m_DataMVs[0][0].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_FORWARD))
			{
				pMEStruct->m_fFrameAverageMEErrorPerBlock += pMEStruct->m_CurMBlocks[i].m_DataMVs[0][0].error;
				iMBCounter++;
			}
			
			if (pMEStruct->m_CurMBlocks[i].m_DataMVs[1][0].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_BACKWARD))
			{
				pMEStruct->m_fFrameAverageMEErrorPerBlock += pMEStruct->m_CurMBlocks[i].m_DataMVs[1][0].error;
				iMBCounter++;
			}
		}
		else if (pMEStruct->m_CurMBlocks[i].m_BlockPartition == cbt_16x8)
		{
			if (pMEStruct->m_CurMBlocks[i].m_DataMVs[0][0].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_FORWARD))
			{
				pMEStruct->m_fFrameAverageMEErrorPerBlock += pMEStruct->m_CurMBlocks[i].m_DataMVs[0][0].error;
				iMBCounter++;
			}
			
			if (pMEStruct->m_CurMBlocks[i].m_DataMVs[0][8].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_FORWARD))
			{
				pMEStruct->m_fFrameAverageMEErrorPerBlock += pMEStruct->m_CurMBlocks[i].m_DataMVs[0][8].error << 1;
				iMBCounter++;
			}
			
			if (pMEStruct->m_CurMBlocks[i].m_DataMVs[1][0].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_BACKWARD))
			{
				pMEStruct->m_fFrameAverageMEErrorPerBlock += pMEStruct->m_CurMBlocks[i].m_DataMVs[1][0].error;
				iMBCounter++;
			}

			if (pMEStruct->m_CurMBlocks[i].m_DataMVs[1][8].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_BACKWARD))
			{
				pMEStruct->m_fFrameAverageMEErrorPerBlock += pMEStruct->m_CurMBlocks[i].m_DataMVs[1][8].error << 1;
				iMBCounter++;
			}
		}
	    else
		{
			for (j = 0; j < 4; j++)
			{
				if (pMEStruct->m_CurMBlocks[i].m_SubBlockPartition[j] == cbt_8x8)
				{
					if (pMEStruct->m_CurMBlocks[i].m_DataMV[j << 2].error > 0)
					{
						pMEStruct->m_fFrameAverageMEErrorPerBlock += pMEStruct->m_CurMBlocks[i].m_DataMV[j << 2].error << 2;
						iMBCounter++;
					}
				}
				else
				{
					if (pMEStruct->m_CurMBlocks[i].m_DataMV[ j << 2     ].error > 0)
					{
						pMEStruct->m_fFrameAverageMEErrorPerBlock += pMEStruct->m_CurMBlocks[i].m_DataMV[ j << 2     ].error << 4;
						iMBCounter++;
					}
					if (pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 1].error > 0)
					{
						pMEStruct->m_fFrameAverageMEErrorPerBlock += pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 1].error << 4;
						iMBCounter++;
					}
					if (pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 2].error > 0)
					{
						pMEStruct->m_fFrameAverageMEErrorPerBlock += pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 2].error << 4;
						iMBCounter++;
					}
					if (pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 3].error > 0)
					{
						pMEStruct->m_fFrameAverageMEErrorPerBlock += pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 3].error << 4;
						iMBCounter++;
					}
				}
			}
		}
	}

	if ( iMBCounter <= 0 )
		 iMBCounter = 1; // To avoid division by zero

	pMEStruct->m_fFrameAverageMEErrorPerBlock /= iMBCounter;
	pMEStruct->m_fSequnceAverageMEErrorPerPixel += pMEStruct->m_fFrameAverageMEErrorPerBlock / 256;

	if (pMEStruct->m_iFrameCount == 1)
		pMEStruct->m_fPrvFrameAverageMEErrorPerBlock = pMEStruct->m_fFrameAverageMEErrorPerBlock;

	pMEStruct->m_fMeanErrorPP =
		pMEStruct->m_fMeanErrRenew * MIN(pMEStruct->m_fFrameAverageMEErrorPerBlock, pMEStruct->m_fPrvFrameAverageMEErrorPerBlock) + 
		(1 - pMEStruct->m_fMeanErrRenew) * pMEStruct->m_fMeanErrorPP;

	pMEStruct->m_fPrvFrameAverageMEErrorPerBlock = pMEStruct->m_fFrameAverageMEErrorPerBlock;

	fDispersion = 0;

	for (i=0; i<pMEStruct->m_iMBlocksInFrame; i++)
	{
		int iError;
		if (pMEStruct->m_CurMBlocks[i].m_BlockPartition == cbt_16x16)
		{
			if (pMEStruct->m_CurMBlocks[i].m_DataMVs[0][0].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_FORWARD))
			{
				iError = pMEStruct->m_CurMBlocks[i].m_DataMVs[0][0].error;
				fDispersion += sqr(iError - pMEStruct->m_fFrameAverageMEErrorPerBlock);
			}
            if (pMEStruct->m_CurMBlocks[i].m_DataMVs[1][0].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_BACKWARD))
			{
				iError = pMEStruct->m_CurMBlocks[i].m_DataMVs[1][0].error;
				fDispersion += sqr(iError - pMEStruct->m_fFrameAverageMEErrorPerBlock);
			}
		}
		else if (pMEStruct->m_CurMBlocks[i].m_BlockPartition == cbt_16x8)
		{
			if (pMEStruct->m_CurMBlocks[i].m_DataMVs[0][0].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_FORWARD))
			{
				iError = pMEStruct->m_CurMBlocks[i].m_DataMVs[0][0].error;
				fDispersion += sqr(iError - pMEStruct->m_fFrameAverageMEErrorPerBlock);
			}

			if (pMEStruct->m_CurMBlocks[i].m_DataMVs[0][8].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_FORWARD))
			{
				iError = pMEStruct->m_CurMBlocks[i].m_DataMVs[0][8].error << 1;
				fDispersion += sqr(iError - pMEStruct->m_fFrameAverageMEErrorPerBlock);
			}

            if (pMEStruct->m_CurMBlocks[i].m_DataMVs[1][0].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_BACKWARD))
			{
				iError = pMEStruct->m_CurMBlocks[i].m_DataMVs[1][0].error;
				fDispersion += sqr(iError - pMEStruct->m_fFrameAverageMEErrorPerBlock);
			}

			if (pMEStruct->m_CurMBlocks[i].m_DataMVs[1][8].error > 0 && (pMEStruct->m_CurMBlocks[i].m_type & MBT_BACKWARD))
			{
				iError = pMEStruct->m_CurMBlocks[i].m_DataMVs[1][8].error << 1;
				fDispersion += sqr(iError - pMEStruct->m_fFrameAverageMEErrorPerBlock);
			}
		}
		else
		{
			for (j = 0; j < 4; j++)
			{
				if (pMEStruct->m_CurMBlocks[i].m_SubBlockPartition[j] == cbt_8x8)
				{
					if (pMEStruct->m_CurMBlocks[i].m_DataMV[j << 2].error > 0)
					{
						iError = pMEStruct->m_CurMBlocks[i].m_DataMV[j << 2].error << 2;
						fDispersion += (iError - pMEStruct->m_fFrameAverageMEErrorPerBlock)*(iError - pMEStruct->m_fFrameAverageMEErrorPerBlock);
					}
				}
				else
				{
					if (pMEStruct->m_CurMBlocks[i].m_DataMV[ j << 2     ].error > 0)
					{
						iError = pMEStruct->m_CurMBlocks[i].m_DataMV[ j << 2     ].error << 4;
						fDispersion += (iError - pMEStruct->m_fFrameAverageMEErrorPerBlock)*(iError - pMEStruct->m_fFrameAverageMEErrorPerBlock);
					}
					if (pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 1].error > 0)
					{
						iError = pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 1].error << 4;
						fDispersion += (iError - pMEStruct->m_fFrameAverageMEErrorPerBlock)*(iError - pMEStruct->m_fFrameAverageMEErrorPerBlock);
					}
					if (pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 2].error > 0)
					{
						iError = pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 2].error << 4;
						fDispersion += (iError - pMEStruct->m_fFrameAverageMEErrorPerBlock)*(iError - pMEStruct->m_fFrameAverageMEErrorPerBlock);
					}
					if (pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 3].error > 0)
					{
						iError = pMEStruct->m_CurMBlocks[i].m_DataMV[(j << 2) + 3].error << 4;
						fDispersion += (iError - pMEStruct->m_fFrameAverageMEErrorPerBlock)*(iError - pMEStruct->m_fFrameAverageMEErrorPerBlock);
					}
				}
			}
		}
	}

	fDispersion /= iMBCounter;
	fDispersion = (float)sqrt(fDispersion);

	if (pMEStruct->m_iFrameCount == 1)
		pMEStruct->m_fPrvFrameMEErrorDeviationPerBlock = fDispersion;

	pMEStruct->m_fDispersionPP = pMEStruct->m_fDispersionRenew * MIN(fDispersion, pMEStruct->m_fPrvFrameMEErrorDeviationPerBlock) +
		(1 - pMEStruct->m_fDispersionRenew) * pMEStruct->m_fDispersionPP;

	pMEStruct->m_fPrvFrameMEErrorDeviationPerBlock = fDispersion;

}

/**
**************************************************************************
* \brief Sets pointers to the externally created shifts
*
*  This function makes possible usage of externally created half-pixel
*  shifts. It takes array of pointer to the shifted Y-planes of previous
*  frame. pShifts[1] should point to the half-pixel left shifted plane,
*  pShifts[2] should point to the half-pixel up shifted plane,
*  pShifts[3] should point to the half-pixel left and half-pixel up
*  shifted plane
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* \param pShifts             [IN]     - array of pointers on shifted planes
* 
* \return none
*/
void cGiveShifts(gmcMotionEstimator* pMEStruct, BYTE** pShifts)
{
	pMEStruct->m_bUsePreparedShifts = TRUE;
	pMEStruct->m_pRefShiftsYMem = pMEStruct->m_pRefShiftsY[1];
	pMEStruct->m_pRefShiftsY[1] = pShifts[1];
	pMEStruct->m_pRefShiftsY[2] = pShifts[2];
	pMEStruct->m_pRefShiftsY[3] = pShifts[3];
}

/**
**************************************************************************
* \brief Sets pointers to the externally created chroma shifts
*
*  This function makes possible usage of externally created half-pixel
*  chroma shifts. It takes two arrays of pointer to the shifted U and V
*  planes of previous frame. Data layout is the same as in cGiveShifts
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* \param pShiftsChr1         [IN]     - array of pointers on shifted U-planes
* \param pShiftsChr2         [IN]     - array of pointers on shifted V-planes
* 
* \return none
*/
void cGiveChromaShifts(gmcMotionEstimator* pMEStruct, BYTE** pShiftsChr1, BYTE** pShiftsChr2)
{
	if (pMEStruct->m_bConsiderChroma)
	{
		pMEStruct->m_bUsePreparedChromaShifts = TRUE;

		pMEStruct->m_pRefShiftsChr1Mem = pMEStruct->m_pRefShiftsChr1[1];
		pMEStruct->m_pRefShiftsChr1[1] = pShiftsChr1[1];
		pMEStruct->m_pRefShiftsChr1[2] = pShiftsChr1[2];
		pMEStruct->m_pRefShiftsChr1[3] = pShiftsChr1[3];

		pMEStruct->m_pRefShiftsChr2Mem = pMEStruct->m_pRefShiftsChr2[1];
		pMEStruct->m_pRefShiftsChr2[1] = pShiftsChr2[1];
		pMEStruct->m_pRefShiftsChr2[2] = pShiftsChr2[2];
		pMEStruct->m_pRefShiftsChr2[3] = pShiftsChr2[3];
	}
}
	
/**
**************************************************************************
* \brief Restores pointers after usage of externally created shifts
*
*  This function restores pointers to the memory for shifted planes storage
*  after usage of externally created shifts
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* 
* \return none
*/
static void RestoreShiftsMemory(gmcMotionEstimator* pMEStruct)
{
	if (pMEStruct->m_bUsePreparedShifts)
	{
		pMEStruct->m_pRefShiftsY[1] = pMEStruct->m_pRefShiftsYMem;
		pMEStruct->m_pRefShiftsY[2] = pMEStruct->m_pRefShiftsY[1] + pMEStruct->m_iExtHeight * pMEStruct->m_iExtWidth;
		pMEStruct->m_pRefShiftsY[3] = pMEStruct->m_pRefShiftsY[2] + pMEStruct->m_iExtHeight * pMEStruct->m_iExtWidth;
		pMEStruct->m_bUsePreparedShifts = FALSE;
	}
}

/**
**************************************************************************
* \brief Restores pointers after usage of externally created chroma shifts
*
*  This function restores pointers to the memory for shifted U and V
*  planes storage after usage of externally created chroma shifts
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* 
* \return none
*/
static void RestoreChromaShiftsMemory(gmcMotionEstimator* pMEStruct)
{
	if (pMEStruct->m_bUsePreparedChromaShifts && pMEStruct->m_bConsiderChroma)
	{
		int extSizeChr = (pMEStruct->m_iExtHeight / pMEStruct->m_iChrYMod) * (pMEStruct->m_iExtWidth / pMEStruct->m_iChrXMod);

		pMEStruct->m_pRefShiftsChr1[1] = pMEStruct->m_pRefShiftsChr1Mem;
		pMEStruct->m_pRefShiftsChr1[2] = pMEStruct->m_pRefShiftsChr1[1] + extSizeChr;
		pMEStruct->m_pRefShiftsChr1[3] = pMEStruct->m_pRefShiftsChr1[2] + extSizeChr;
		pMEStruct->m_pRefShiftsChr2[1] = pMEStruct->m_pRefShiftsChr2Mem;
		pMEStruct->m_pRefShiftsChr2[2] = pMEStruct->m_pRefShiftsChr2[1] + extSizeChr;
		pMEStruct->m_pRefShiftsChr2[3] = pMEStruct->m_pRefShiftsChr2[2] + extSizeChr;
		pMEStruct->m_bUsePreparedChromaShifts = FALSE;
	}
}

/**
**************************************************************************
* \brief Applies motion estimation using chosen algorithm
*
*  This function estimates motion using chosen algorithm
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* \param CurFrame            [IN] -  pointer to the current frame
* \param PrevFrame           [IN] -  pointer to the reference frame
* \param bBotBase            [IN] -  TRUE if bottom lines contains first field
* 
* \return none
*/
void cEstimate(gmcMotionEstimator* pMEStruct, BYTE* CurFrame, BYTE* PrevFrame, BOOL bBotBase)
{
	int i;
	float fDispersion = 0.0;
	pMEStruct->m_iFrameCount++;
	pMEStruct->m_fFrameAverageMEErrorPerBlock = 0;
	pMEStruct->m_MInfo.m_fFrameAverageSADsPerPixel = 0;

	pMEStruct->m_MInfo.m_bBotBase = bBotBase;

	for(i=0;i<pMEStruct->m_iMBlocksInFrame;i++)
		pMEStruct->m_RefMBlocks[i]=pMEStruct->m_CurMBlocks[i];

	pMEStruct->CurFrame = CurFrame;
	pMEStruct->RefFrames[0][0] = PrevFrame;
	pMEStruct->RefFrame=pMEStruct->RefFrames[0][0];
		
	if(pMEStruct->m_interAlg == SixTap && !pMEStruct->m_bUsePreparedShifts)
	{
		pMEStruct->m_pRefShiftsY[0] = PrevFrame;	
		MakePlanes(pMEStruct->m_pRefShiftsY, pMEStruct->m_iFirstRowOffset, pMEStruct->m_iWidth, pMEStruct->m_iHeight, pMEStruct->m_iExtWidth);
	
		PlaneAddBorders(pMEStruct->m_pRefShiftsY[1], pMEStruct->m_iWidth, pMEStruct->m_iHeight, pMEStruct->m_iBorderSize);
		PlaneAddBorders(pMEStruct->m_pRefShiftsY[2], pMEStruct->m_iWidth, pMEStruct->m_iHeight, pMEStruct->m_iBorderSize);
		PlaneAddBorders(pMEStruct->m_pRefShiftsY[3], pMEStruct->m_iWidth, pMEStruct->m_iHeight, pMEStruct->m_iBorderSize);
	}
	else if (pMEStruct->m_interAlg == SixTap && pMEStruct->m_bUsePreparedShifts)
	{
		pMEStruct->m_pRefShiftsY[0] = PrevFrame;
	}

	if (pMEStruct->m_bConsiderChroma && !pMEStruct->m_bUsePreparedChromaShifts) {
		pMEStruct->m_pRefShiftsChr1[0] = PrevFrame + pMEStruct->m_iSecCompOff - pMEStruct->m_iFirstRowOffsetChr;	
		MakePlanes(pMEStruct->m_pRefShiftsChr1, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iWidth / pMEStruct->m_iChrXMod, pMEStruct->m_iHeight / pMEStruct->m_iChrYMod, pMEStruct->m_iExtWidth / pMEStruct->m_iChrXMod);
	
		// 
		PlaneAddBorders(pMEStruct->m_pRefShiftsChr1[1], pMEStruct->m_iWidth / pMEStruct->m_iChrXMod, pMEStruct->m_iHeight / pMEStruct->m_iChrYMod, pMEStruct->m_iBorderSize / pMEStruct->m_iChrXMod);
		PlaneAddBorders(pMEStruct->m_pRefShiftsChr1[2], pMEStruct->m_iWidth / pMEStruct->m_iChrXMod, pMEStruct->m_iHeight / pMEStruct->m_iChrYMod, pMEStruct->m_iBorderSize / pMEStruct->m_iChrXMod);
		PlaneAddBorders(pMEStruct->m_pRefShiftsChr1[3], pMEStruct->m_iWidth / pMEStruct->m_iChrXMod, pMEStruct->m_iHeight / pMEStruct->m_iChrYMod, pMEStruct->m_iBorderSize / pMEStruct->m_iChrXMod);

		pMEStruct->m_pRefShiftsChr2[0] = PrevFrame + pMEStruct->m_iThdCompOff - pMEStruct->m_iFirstRowOffsetChr;
		MakePlanes(pMEStruct->m_pRefShiftsChr2, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iWidth / pMEStruct->m_iChrXMod, pMEStruct->m_iHeight / pMEStruct->m_iChrYMod, pMEStruct->m_iExtWidth / pMEStruct->m_iChrXMod);

		// 
		PlaneAddBorders(pMEStruct->m_pRefShiftsChr2[1], pMEStruct->m_iWidth / pMEStruct->m_iChrXMod, pMEStruct->m_iHeight / pMEStruct->m_iChrYMod, pMEStruct->m_iBorderSize / pMEStruct->m_iChrXMod);
		PlaneAddBorders(pMEStruct->m_pRefShiftsChr2[2], pMEStruct->m_iWidth / pMEStruct->m_iChrXMod, pMEStruct->m_iHeight / pMEStruct->m_iChrYMod, pMEStruct->m_iBorderSize / pMEStruct->m_iChrXMod);
		PlaneAddBorders(pMEStruct->m_pRefShiftsChr2[3], pMEStruct->m_iWidth / pMEStruct->m_iChrXMod, pMEStruct->m_iHeight / pMEStruct->m_iChrYMod, pMEStruct->m_iBorderSize / pMEStruct->m_iChrXMod);
	}
	else if (pMEStruct->m_bUsePreparedChromaShifts)
	{
		pMEStruct->m_pRefShiftsChr1[0] = PrevFrame + pMEStruct->m_iSecCompOff - pMEStruct->m_iFirstRowOffsetChr;
		pMEStruct->m_pRefShiftsChr2[0] = PrevFrame + pMEStruct->m_iThdCompOff - pMEStruct->m_iFirstRowOffsetChr;
	}
	
	// call to the ME function
	(pMEStruct->m_pEstimate)(pMEStruct);
	
	RestoreShiftsMemory(pMEStruct);
	RestoreChromaShiftsMemory(pMEStruct);
	
	// frame statistic counting
	// warning! only works in case of one object of pMEStruct class in application
	CalculateError(pMEStruct);
}


/**
**************************************************************************
* \brief Initialization for gmcMotionEstimator structure
*
*  This function is a default initialization for gmcMotionEstimator
*  structure
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
*
* \return none
*/
void cInitME(gmcMotionEstimator* pMEStruct)
{ 
	int i;
	
	pMEStruct->MVPrev = NULL;
	pMEStruct->MVectors = NULL;
	pMEStruct->m_MeanMVs = NULL;
	pMEStruct->m_bExistPrevMV = FALSE;
	pMEStruct->m_MVCandidates = NULL;
	pMEStruct->m_pEstimate = NULL;

	pMEStruct->m_pRefShiftsY[1] = NULL;
	pMEStruct->m_pRefShiftsChr1[1] = NULL;
	pMEStruct->m_pRefShiftsChr2[1] = NULL;

	pMEStruct->m_AvailableMEFunctions[_3DRS] = Estimate3DRS;
	pMEStruct->m_AvailableMEFunctions[E3DRS] = EstimateE3DRS;
	pMEStruct->m_AvailableMEFunctions[  FSA] = EstimateFSA;
	pMEStruct->m_AvailableMEFunctions[ MEDS] = EstimateDS;
	pMEStruct->m_AvailableMEFunctions[ HIER] = EstimateHierarchic;
	pMEStruct->m_AvailableMEFunctions[ MSU] = EstimateNewMSU_MB;

	// temporarily
	pMEStruct->m_iMaxError = 5000;
	pMEStruct->m_iDropNearVectorThresh = 3;
	pMEStruct->m_iSteps = 3;

	pMEStruct->m_pCurrentHierarchicBlock = NULL;
	pMEStruct->m_pHierarchicPrevFrameArea = NULL;

	pMEStruct->m_iBorderSize = 0;

	pMEStruct->m_CurBlock = NULL;
	pMEStruct->m_RefBlock = NULL;

	pMEStruct->m_MVPrvCand = NULL;
	pMEStruct->m_MVNxtCand = NULL;

	pMEStruct->m_pHierLaunchMap = NULL;

	pMEStruct->m_CurMBlocks = NULL;
	
	pMEStruct->m_sConfigFileName = NULL;

	pMEStruct->m_fSequnceAverageSADsPerPixel = 0;
	pMEStruct->m_fSequnceAverageMEErrorPerPixel = 0;

	pMEStruct->m_bSaveStatistic = 0;

	pMEStruct->m_blockIDs = NULL;
    pMEStruct->m_iCacheSizeHor = 0;
    pMEStruct->m_iCacheSizeVert = 0;

	pMEStruct->m_fNoiseVar = 0;

	for(i=0;i<NUM_REF_FRAMES;i++)
	{
		pMEStruct->m_MVPrvCand__[i][FT_TOP] = NULL;
		pMEStruct->m_MVPrvCand__[i][FT_BOTTOM] = NULL;
		
		pMEStruct->m_MVNxtCand__[i][FT_TOP] = NULL;
		pMEStruct->m_MVNxtCand__[i][FT_BOTTOM] = NULL;
        
		pMEStruct->m_RefMBlocks__[i][FT_TOP] = NULL;
		pMEStruct->m_RefMBlocks__[i][FT_BOTTOM] = NULL;

		pMEStruct->RefFrames[i][FT_TOP] = NULL;
		pMEStruct->RefFrames[i][FT_BOTTOM] = NULL;
	}
};

// ============================================================================

typedef int (*MetricProto)(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo);

/*! \var BlockSize_index
*   \brief array of shifts used in METRIC_INDEX macro
*
* array of shifts used in METRIC_INDEX macros
* cbt_16x8 -> 1,
* cbt_16x16 -> 0,
* cbt_8x8 -> 2,
* cbt_4x4 -> 3
* ** (-1) in case of incorrect input data will cause (-1) incorrect result
*/
static int BlockSize_index[] = {1,(-1),(-1),(-1), 3,(-1),(-1),(-1), 2,(-1),(-1),(-1), (-1),(-1),(-1),(-1), 0};

/*! \def METRIC_INDEX
*   \brief Calculates index in Metrics array
*
* Calculates index in Metrics array according to Chroma Modifiers, Metric Algorithm of Motion Estimator
* and Macro Block Type of current block
*
* NOTE: Pay attention here, if SuitableChromaModifiers, MEMetricAlgorithm or cMacroBlockPartitions types changed!
*
* \param pMEStruct   [IN] - Motion Estimator structure
* \param BlockSize   [IN] - type of current macro block
* \return Index of Metrics array for corresponding metric function
*/
#define METRIC_INDEX(pMEStruct, BlockSize) ( \
	/* 0-1 bits */ (BlockSize_index[BlockSize]) | \
	/* 2 bit */ ((pMEStruct->m_iChrXMod & 2) << 1) | \
	/* 3 bit */ ((pMEStruct->m_iChrYMod & 2) << 2) | \
	/* 4 bit */ ((pMEStruct->m_iMEMetricAlg) << 4) \
)

/*
	NOTICE:
	in function name MetricAA_BB_CC
	values of AA means name of metric,
	values of CC means blocksize,
	values of BB means:
		NN --> m_ChrXMod == crNoReduction && m_ChrYMod == crNoReduction
		XN --> m_ChrXMod == crTwoTimes && m_ChrYMod == crNoReduction
		NY --> m_ChrXMod == crNoReduction && m_ChrYMod == crTwoTimes
		XY --> m_ChrXMod == crTwoTimes && m_ChrYMod == crTwoTimes
*/

// === METRIC_SAD =============================================================

/**
**************************************************************************
* \brief Calculate SAD metric for 16x16 macro block size with no chroma reduce
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int MetricSAD_NN_16x16(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
	int iError;
	iError = GetErrorSAD_16x16_H264_MMX(pMEStruct->m_CurBlock, MBwidth[cbt_16x16], pMEStruct->m_pRefShiftsY, pMEStruct->m_iFirstRowOffset, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
	if (pMEStruct->m_bConsiderChroma) {
		iError += GetErrorSAD_16x16_H264_MMX(pMEStruct->m_CurBlChr1, MBwidth[cbt_16x16], pMEStruct->m_pRefShiftsChr1, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
		iError += GetErrorSAD_16x16_H264_MMX(pMEStruct->m_CurBlChr2, MBwidth[cbt_16x16], pMEStruct->m_pRefShiftsChr2, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
		// that also works:
		//iError += GetErrorSAD_16x16_H264_MMX(pMEStruct->m_CurBlChr1, MBwidth[cbt_16x16], pMEStruct->m_pRefShiftsChr1, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth, 0, 0, (iX << 2) + iMX, (iY << 2) + iMY);
		//iError += GetErrorSAD_16x16_H264_MMX(pMEStruct->m_CurBlChr2, MBwidth[cbt_16x16], pMEStruct->m_pRefShiftsChr2, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth, 0, 0, (iX << 2) + iMX, (iY << 2) + iMY);
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 256 + 64 + 64;
	} else {
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 256;
	}
	return iError;
}

/**
**************************************************************************
* \brief Calculate SAD metric for 16x8 macro block size with no chroma reduce
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int MetricSAD_NN_16x8(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
	int iError;
	if (pMEStruct->m_bConsiderChroma) {
		assert(FALSE); // ConsiderChroma is not supported for that metric
	} else {
		iError = GetErrorSAD_16x8_H264_MMX(pMEStruct->m_CurBlock, MBwidth[cbt_16x8], pMEStruct->m_pRefShiftsY, pMEStruct->m_iFirstRowOffset, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 128;
	}
	return iError;
}

/**
**************************************************************************
* \brief Calculate SAD metric for 8x8 macro block size with no chroma reduce
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int MetricSAD_NN_8x8(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
	int iError;
	iError = GetErrorSAD_8x8_H264_MMX(pMEStruct->m_CurBlock, MBwidth[cbt_8x8], pMEStruct->m_pRefShiftsY, 
		pMEStruct->m_iFirstRowOffset, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
	if (pMEStruct->m_bConsiderChroma) {
		iError += GetErrorSAD_8x8_H264_MMX(pMEStruct->m_CurBlChr1, MBwidth[cbt_8x8], pMEStruct->m_pRefShiftsChr1, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
		iError += GetErrorSAD_8x8_H264_MMX(pMEStruct->m_CurBlChr2, MBwidth[cbt_8x8], pMEStruct->m_pRefShiftsChr2, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 64 + 16 + 16;
	} else {
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 64;
	}
	return iError;
}

/**
**************************************************************************
* \brief Calculate SAD metric for 4x4 macro block size with no chroma reduce
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int MetricSAD_NN_4x4(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
	int iError;
	iError = GetErrorSAD_4x4_H264_MMX(pMEStruct->m_CurBlock, MBwidth[cbt_4x4], pMEStruct->m_pRefShiftsY, pMEStruct->m_iFirstRowOffset, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
	if (pMEStruct->m_bConsiderChroma) {
		iError += GetErrorSAD_4x4_H264_MMX(pMEStruct->m_CurBlChr1, MBwidth[cbt_4x4], pMEStruct->m_pRefShiftsChr1, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
		iError += GetErrorSAD_4x4_H264_MMX(pMEStruct->m_CurBlChr2, MBwidth[cbt_4x4], pMEStruct->m_pRefShiftsChr2, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 16 + 4 + 4;
	} else {
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 16;
	}
	return iError;
}

/**
**************************************************************************
* \brief Calculate SAD metric for 16x16 macro block size with two times chroma reduce
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int MetricSAD_XY_16x16(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
	int iError;
	iError = GetErrorSAD_16x16_H264_MMX(pMEStruct->m_CurBlock, MBwidth[cbt_16x16], pMEStruct->m_pRefShiftsY, pMEStruct->m_iFirstRowOffset, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
	if (pMEStruct->m_bConsiderChroma)
	{
		iError += GetErrorSAD_8x8_H264_MMX(pMEStruct->m_CurBlChr1, MBwidth[cbt_16x16] >> 1, pMEStruct->m_pRefShiftsChr1, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth >> 1, 0, 0, ((iX << 2) + iMX) >> 1, ((iY << 2) + iMY) >> 1);
		iError += GetErrorSAD_8x8_H264_MMX(pMEStruct->m_CurBlChr2, MBwidth[cbt_16x16] >> 1, pMEStruct->m_pRefShiftsChr2, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth >> 1, 0, 0, ((iX << 2) + iMX) >> 1, ((iY << 2) + iMY) >> 1);
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 256 + 64 + 64;
	}
	else
	{
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 256;
	}
	return iError;
}

/**
**************************************************************************
* \brief Calculate SAD metric for 16x8 macro block size with two times chroma reduce
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int MetricSAD_XY_16x8(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
	int iError;
	if (pMEStruct->m_bConsiderChroma) {
		return -1;		// should be replaced by assert(false); left for compatibility
	} else {
		iError = GetErrorSAD_16x8_H264_MMX(pMEStruct->m_CurBlock, MBwidth[cbt_16x8], pMEStruct->m_pRefShiftsY, pMEStruct->m_iFirstRowOffset, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 128;
	}
	return iError;
}

/**
**************************************************************************
* \brief Calculate SAD metric for 8x8 macro block size with two times chroma reduce
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int MetricSAD_XY_8x8(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
	int iError;
	iError = GetErrorSAD_8x8_H264_MMX(pMEStruct->m_CurBlock, MBwidth[cbt_8x8], pMEStruct->m_pRefShiftsY, pMEStruct->m_iFirstRowOffset, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
	if (pMEStruct->m_bConsiderChroma) {
		iError += GetErrorSAD_4x4_H264_MMX(pMEStruct->m_CurBlChr1, MBwidth[cbt_8x8] >> 1, pMEStruct->m_pRefShiftsChr1, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth >> 1, 0, 0, ((iX << 2) + iMX) >> 1, ((iY << 2) + iMY) >> 1);
		iError += GetErrorSAD_4x4_H264_MMX(pMEStruct->m_CurBlChr2, MBwidth[cbt_8x8] >> 1, pMEStruct->m_pRefShiftsChr1, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth >> 1, 0, 0, ((iX << 2) + iMX) >> 1, ((iY << 2) + iMY) >> 1);
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 64 + 16 + 16;
	} else {
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 64;
	}
	return iError;
}

/**
**************************************************************************
* \brief Calculate SAD metric for 4x4 macro block size with two times chroma reduce
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int MetricSAD_XY_4x4(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
	int iError;
	iError = GetErrorSAD_4x4_H264_MMX(pMEStruct->m_CurBlock, MBwidth[cbt_4x4], pMEStruct->m_pRefShiftsY, pMEStruct->m_iFirstRowOffset, pMEStruct->m_iExtWidth, iX, iY, iMX, iMY);
	if (pMEStruct->m_bConsiderChroma) {
		iError += GetErrorSAD_2x2_H264_C(pMEStruct->m_CurBlChr1, MBwidth[cbt_4x4] >> 1, pMEStruct->m_pRefShiftsChr1, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth >> 1, 0, 0, ((iX << 2) + iMX) >> 1, ((iY << 2) + iMY) >> 1);
		iError += GetErrorSAD_2x2_H264_C(pMEStruct->m_CurBlChr2, MBwidth[cbt_4x4] >> 1, pMEStruct->m_pRefShiftsChr2, pMEStruct->m_iFirstRowOffsetChr, pMEStruct->m_iExtWidth >> 1, 0, 0, ((iX << 2) + iMX) >> 1, ((iY << 2) + iMY) >> 1);
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 16 + 4 + 4;
	} else {
		if (pMInfo)
			pMInfo->m_fFrameAverageSADsPerPixel += 16;
	}
	return iError;
}


// === METRIC_COLORINDEPENDENT ================================================

/*! \def ME_CIND_METRIC_NUMBER
*   \brief Number of Sum of Normalized differences experimental formula
*/
#define ME_CIND_METRIC_NUMBER 1 // 1..3
/*! \def ME_CIND_USE_SAD
*   \brief Condition of using SAD in SND metric
*/
#define ME_CIND_USE_SAD 0

/**
**************************************************************************
* \brief Calculate sum of pixel differences between blocks with blending
* 
* \param pCur        [IN] - reference block
* \param curStride   [IN] - reference block stride
* \param p1          [IN] - 1 of 2 shift surfaces of input block for blending
* \param p2          [IN] - 2 of 2 shift surfaces of input block for blending
* \param stride      [IN] - input block stride
* \return sum of pixel differences between blocks
*/
__inline int MetricCIND_calcSum_blend(BYTE *pCur, int curStride, BYTE *p1, BYTE *p2, int stride) {
	int accum = 0;
	int i, j;
	for(i=0; i<curStride; i++) {
		BYTE *p1loc = p1, *p2loc = p2;
		for(j=0; j<curStride; j++, pCur++, p1loc++, p2loc++) {
			accum += (int)*pCur - (((int)*p1loc + *p2loc + 1) >> 1);
		}
		p1 += stride;
		p2 += stride;
	}
	return accum;
}

/**
**************************************************************************
* \brief Calculate sum of pixel differences between blocks without blending
* 
* \param pCur        [IN] - reference block
* \param curStride   [IN] - reference block stride
* \param p1          [IN] - surface of input block
* \param stride      [IN] - input block stride
* \return sum of pixel differences between blocks
*/
__inline int MetricCIND_calcSum(BYTE *pCur, int curStride, BYTE *p1, int stride) {
	int accum = 0;
	int i, j;
	for(i=0; i<curStride; i++) {
		BYTE *p1loc = p1;
		for(j=0; j<curStride; j++, pCur++, p1loc++) {
			accum += (int)*pCur - *p1loc;
		}
		p1 += stride;
	}
	return accum;
}

/**
**************************************************************************
* \brief Calculate numerator used in SAD (Sum of Normalized Differences) formula with blending
* 
* \param pCur        [IN] - reference block
* \param curStride   [IN] - reference block stride
* \param p1          [IN] - 1 of 2 shift surfaces of input block for blending
* \param p2          [IN] - 2 of 2 shift surfaces of input block for blending
* \param stride      [IN] - input block stride
* \return numerator used in SAD formula
*/
__inline int MetricCIND_calcDiffSum_blend(BYTE *pCur, int curStride, BYTE *p1, BYTE *p2, int stride, int sum) {
	int accum = 0;
	int i, j;
	for(i=0; i<curStride; i++) {
		BYTE *p1loc = p1, *p2loc = p2;
		for(j=0; j<curStride; j++, pCur++, p1loc++, p2loc++) {
			accum += abs( (((int)*p1loc + *p2loc + 1) >> 1) - *pCur + sum );
		}
		p1 += stride;
		p2 += stride;
	}
	return accum;
}

/**
**************************************************************************
* \brief Calculate numerator used in SAD (Sum of Normalized Differences) formula without blending
* 
* \param pCur        [IN] - reference block
* \param curStride   [IN] - reference block stride
* \param p1          [IN] - surface of input block
* \param stride      [IN] - input block stride
* \return numerator used in SAD formula
*/
__inline int MetricCIND_calcDiffSum(BYTE *pCur, int curStride, BYTE *p1, int stride, int sum) {
	int accum = 0;
	int i, j;
	for(i=0; i<curStride; i++) {
		BYTE *p1loc = p1;
		for(j=0; j<curStride; j++, pCur++, p1loc++) {
			accum += abs( (int)*p1loc - *pCur + sum );
		}
		p1 += stride;
	}
	return accum;
}

/**
**************************************************************************
* \brief Calculate Color Independent metric for 16x16 macro block size with no chroma reduce
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int MetricCIND_NN_16x16(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
#define BLOCK_TYPE cbt_16x16
	int iError;
	int iCorrection = H264_CORRECTION(iMX, iMY);
	BYTE 
		// stride here = pMEStruct->m_iExtWidth  (where to search match part)
		*pR1 = pMEStruct->m_pRefShiftsY[H264_SRC1_INDEX(iMX, iMY, iCorrection)] + H264_SRC1_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffset,
		*pG1 = pMEStruct->m_pRefShiftsChr1[H264_SRC1_INDEX(iMX, iMY, iCorrection)] + H264_SRC1_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr,
		*pB1 = pMEStruct->m_pRefShiftsChr2[H264_SRC1_INDEX(iMX, iMY, iCorrection)] + H264_SRC1_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr,
		// stride here = MBwidth[BLOCK_TYPE]  (reference block)
		*pCurR = pMEStruct->m_CurBlock,
		*pCurG = pMEStruct->m_CurBlChr1,
		*pCurB = pMEStruct->m_CurBlChr2;
	int diffsum, sum, sum_accum;
	if (H264_BLEND(iMX, iMY)) {
		BYTE		
			*pR2 = pMEStruct->m_pRefShiftsY[H264_SRC2_INDEX(iMX, iMY, iCorrection)] + H264_SRC2_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffset,
			*pG2 = pMEStruct->m_pRefShiftsChr1[H264_SRC2_INDEX(iMX, iMY, iCorrection)] + H264_SRC2_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr,
			*pB2 = pMEStruct->m_pRefShiftsChr2[H264_SRC2_INDEX(iMX, iMY, iCorrection)] + H264_SRC2_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr;
		sum      = abs(sum_accum = MetricCIND_calcSum_blend(pCurR, MBwidth[BLOCK_TYPE], pR1, pR2, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum  =             MetricCIND_calcDiffSum_blend(pCurR, MBwidth[BLOCK_TYPE], pR1, pR2, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum_blend(pCurG, MBwidth[BLOCK_TYPE], pG1, pG2, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum_blend(pCurG, MBwidth[BLOCK_TYPE], pG1, pG2, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum_blend(pCurB, MBwidth[BLOCK_TYPE], pB1, pB2, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum_blend(pCurB, MBwidth[BLOCK_TYPE], pB1, pB2, pMEStruct->m_iExtWidth, sum_accum);
	} else {
		sum      = abs(sum_accum = MetricCIND_calcSum(pCurR, MBwidth[BLOCK_TYPE], pR1, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum  =             MetricCIND_calcDiffSum(pCurR, MBwidth[BLOCK_TYPE], pR1, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum(pCurG, MBwidth[BLOCK_TYPE], pG1, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum(pCurG, MBwidth[BLOCK_TYPE], pG1, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum(pCurB, MBwidth[BLOCK_TYPE], pB1, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum(pCurB, MBwidth[BLOCK_TYPE], pB1, pMEStruct->m_iExtWidth, sum_accum);
	}		
#if ME_CIND_METRIC_NUMBER == 1
	// metric #1 : the best at the moment
#if ME_CIND_USE_SAD
	iError = 256*diffsum + sqr(MetricSAD_NN_16x16(pMEStruct, iX, iY, iMX, iMY, pMInfo)/16) / 4; 
#else
	iError = diffsum + sqr(sum) / 4;
#endif
#elif ME_CIND_METRIC_NUMBER == 2
	// metric #2 : bad, except 'avatar4'
	iError = diffsum + sum;
#else
	// metric #3 : strong, has much difficulties (requires big blocksize)
	iError = diffsum;
#endif
	return iError;
#undef BLOCK_TYPE
}

/**
**************************************************************************
* \brief Calculate Color Independent metric for 8x8 macro block size with no chroma reduce
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int MetricCIND_NN_8x8(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
#define BLOCK_TYPE cbt_8x8
	/* 
		blockRef = pMEStruct->m_CurBlock || pMEStruct->m_CurBlChr1 || pMEStruct->m_CurBlChr2
		strideRef = MBwidth[BLOCK_TYPE]
		pSrcShifts = pMEStruct->m_pRefShiftsY || pMEStruct->m_pRefShiftsChr1 || pMEStruct->m_pRefShiftsChr2
		iFirstRowOffset = pMEStruct->m_iFirstRowOffset || pMEStruct->m_iFirstRowOffsetChr || --//--
		iStride = pMEStruct->m_iExtWidth,
		sum = iError
	*/
	int iError;
	int iCorrection = H264_CORRECTION(iMX, iMY);
	BYTE 
		// stride here = pMEStruct->m_iExtWidth  (where to search match part)
		*pR1 = pMEStruct->m_pRefShiftsY[H264_SRC1_INDEX(iMX, iMY, iCorrection)] + H264_SRC1_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffset,
		*pG1 = pMEStruct->m_pRefShiftsChr1[H264_SRC1_INDEX(iMX, iMY, iCorrection)] + H264_SRC1_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr,
		*pB1 = pMEStruct->m_pRefShiftsChr2[H264_SRC1_INDEX(iMX, iMY, iCorrection)] + H264_SRC1_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr,
		// stride here = MBwidth[BLOCK_TYPE]  (reference block)
		*pCurR = pMEStruct->m_CurBlock,
		*pCurG = pMEStruct->m_CurBlChr1,
		*pCurB = pMEStruct->m_CurBlChr2;
	int diffsum, sum, sum_accum;
	if (H264_BLEND(iMX, iMY)) {
		BYTE		
			*pR2 = pMEStruct->m_pRefShiftsY[H264_SRC2_INDEX(iMX, iMY, iCorrection)] + H264_SRC2_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffset,
			*pG2 = pMEStruct->m_pRefShiftsChr1[H264_SRC2_INDEX(iMX, iMY, iCorrection)] + H264_SRC2_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr,
			*pB2 = pMEStruct->m_pRefShiftsChr2[H264_SRC2_INDEX(iMX, iMY, iCorrection)] + H264_SRC2_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr;
		sum      = abs(sum_accum = MetricCIND_calcSum_blend(pCurR, MBwidth[BLOCK_TYPE], pR1, pR2, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum  =             MetricCIND_calcDiffSum_blend(pCurR, MBwidth[BLOCK_TYPE], pR1, pR2, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum_blend(pCurG, MBwidth[BLOCK_TYPE], pG1, pG2, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum_blend(pCurG, MBwidth[BLOCK_TYPE], pG1, pG2, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum_blend(pCurB, MBwidth[BLOCK_TYPE], pB1, pB2, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum_blend(pCurB, MBwidth[BLOCK_TYPE], pB1, pB2, pMEStruct->m_iExtWidth, sum_accum);
	} else {
		sum      = abs(sum_accum = MetricCIND_calcSum(pCurR, MBwidth[BLOCK_TYPE], pR1, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum  =             MetricCIND_calcDiffSum(pCurR, MBwidth[BLOCK_TYPE], pR1, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum(pCurG, MBwidth[BLOCK_TYPE], pG1, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum(pCurG, MBwidth[BLOCK_TYPE], pG1, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum(pCurB, MBwidth[BLOCK_TYPE], pB1, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum(pCurB, MBwidth[BLOCK_TYPE], pB1, pMEStruct->m_iExtWidth, sum_accum);
	}		
#if ME_CIND_METRIC_NUMBER == 1
	// metric #1 : the best at the moment
#if ME_CIND_USE_SAD
	iError = 256*diffsum + sqr(MetricSAD_NN_16x16(pMEStruct, iX, iY, iMX, iMY, pMInfo)/16); 
#else
	iError = diffsum + sqr(sum);
#endif
#elif ME_CIND_METRIC_NUMBER == 2
	// metric #2 : bad, except 'avatar4'
	iError = diffsum + 2*sum;
#else
	// metric #3 : strong, has much difficulties (requires big blocksize)
	iError = diffsum;
#endif
	return iError;
#undef BLOCK_TYPE
}

/**
**************************************************************************
* \brief Calculate Color Independent metric for 4x4 macro block size with no chroma reduce
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int MetricCIND_NN_4x4(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
#define BLOCK_TYPE cbt_4x4
	int iError;
	int iCorrection = H264_CORRECTION(iMX, iMY);
	BYTE 
		// stride here = pMEStruct->m_iExtWidth  (where to search match part)
		*pR1 = pMEStruct->m_pRefShiftsY[H264_SRC1_INDEX(iMX, iMY, iCorrection)] + H264_SRC1_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffset,
		*pG1 = pMEStruct->m_pRefShiftsChr1[H264_SRC1_INDEX(iMX, iMY, iCorrection)] + H264_SRC1_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr,
		*pB1 = pMEStruct->m_pRefShiftsChr2[H264_SRC1_INDEX(iMX, iMY, iCorrection)] + H264_SRC1_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr,
		// stride here = MBwidth[BLOCK_TYPE]  (reference block)
		*pCurR = pMEStruct->m_CurBlock,
		*pCurG = pMEStruct->m_CurBlChr1,
		*pCurB = pMEStruct->m_CurBlChr2;
	int diffsum, sum, sum_accum;
	if (H264_BLEND(iMX, iMY)) {
		BYTE		
			*pR2 = pMEStruct->m_pRefShiftsY[H264_SRC2_INDEX(iMX, iMY, iCorrection)] + H264_SRC2_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffset,
			*pG2 = pMEStruct->m_pRefShiftsChr1[H264_SRC2_INDEX(iMX, iMY, iCorrection)] + H264_SRC2_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr,
			*pB2 = pMEStruct->m_pRefShiftsChr2[H264_SRC2_INDEX(iMX, iMY, iCorrection)] + H264_SRC2_OFFSET(iX, iY, pMEStruct->m_iExtWidth, iMX, iMY, iCorrection) + pMEStruct->m_iFirstRowOffsetChr;
		sum      = abs(sum_accum = MetricCIND_calcSum_blend(pCurR, MBwidth[BLOCK_TYPE], pR1, pR2, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum  =             MetricCIND_calcDiffSum_blend(pCurR, MBwidth[BLOCK_TYPE], pR1, pR2, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum_blend(pCurG, MBwidth[BLOCK_TYPE], pG1, pG2, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum_blend(pCurG, MBwidth[BLOCK_TYPE], pG1, pG2, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum_blend(pCurB, MBwidth[BLOCK_TYPE], pB1, pB2, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum_blend(pCurB, MBwidth[BLOCK_TYPE], pB1, pB2, pMEStruct->m_iExtWidth, sum_accum);
	} else {
		sum      = abs(sum_accum = MetricCIND_calcSum(pCurR, MBwidth[BLOCK_TYPE], pR1, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum  =             MetricCIND_calcDiffSum(pCurR, MBwidth[BLOCK_TYPE], pR1, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum(pCurG, MBwidth[BLOCK_TYPE], pG1, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum(pCurG, MBwidth[BLOCK_TYPE], pG1, pMEStruct->m_iExtWidth, sum_accum);
		sum     += abs(sum_accum = MetricCIND_calcSum(pCurB, MBwidth[BLOCK_TYPE], pB1, pMEStruct->m_iExtWidth) / sqr(MBwidth[BLOCK_TYPE]));
		diffsum +=             MetricCIND_calcDiffSum(pCurB, MBwidth[BLOCK_TYPE], pB1, pMEStruct->m_iExtWidth, sum_accum);
	}		
#if ME_CIND_METRIC_NUMBER == 1
	// metric #1 : the best at the moment
#if ME_CIND_USE_SAD
	iError = 256*diffsum + sqr(MetricSAD_NN_16x16(pMEStruct, iX, iY, iMX, iMY, pMInfo)/16) * 4;
#else
	iError = diffsum + sqr(sum) * 4;
#endif
#elif ME_CIND_METRIC_NUMBER == 2
	// metric #2 : bad, except 'avatar4'
	iError = diffsum + 4*sum;
#else
	// metric #3 : strong, has much difficulties (requires big blocksize)
	iError = diffsum;
#endif
	return iError;
#undef BLOCK_TYPE
}

#undef ME_CIND_METRIC_NUMBER
#undef ME_CIND_USE_SAD


/**
**************************************************************************
* \brief Function for not supported metrics
* 
* \param pMEStruct     [IN] - Motion Estimator structure
* \param iX            [IN] - X coordinate of reference block
* \param iY            [IN] - Y coordinate of reference block
* \param iMX           [IN] - X coordinate of motion vector for reference block
* \param iMY           [IN] - Y coordinate of motion vector for reference block
* \param pMInfo        [IN/OUT] - structure to store statistic data of motion computation
* \return penalty by metric
*/
int Metric_NOT_SUPPORTED(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo) {
	assert(FALSE); // Metric Not Supported
	return -1; // to avoid compiler warning
}

/*! \var Metrics
*   \brief array of metric functions
*
* Array of metric functions. Use METRIC_INDEX macro to get index for coresponding metric.
*/
static MetricProto Metrics[] = {
	/* 1. METRIC_SAD */
	MetricSAD_NN_16x16, MetricSAD_NN_16x8, MetricSAD_NN_8x8, MetricSAD_NN_4x4,
	Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED,
	Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED,
	MetricSAD_XY_16x16, MetricSAD_XY_16x8, MetricSAD_XY_8x8, MetricSAD_XY_4x4,
	
	/* 2. METRIC_COLORINDEPENDENT */
	MetricCIND_NN_16x16, Metric_NOT_SUPPORTED, MetricCIND_NN_8x8, MetricCIND_NN_4x4,
	Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, 
	Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, 
	Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED, Metric_NOT_SUPPORTED
};

// ============================================================================


/*
**************************************************************************
* \brief Calculates difference of the blocks
*
*  This function calculates difference between blocks in frame, using main
*  difference function, used by ME
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param pRefBlock           [IN] -  pointer to the reference block
* \param iX                  [IN] -  X coordinate of the current block
* \param iY                  [IN] -  Y coordinate of the current block
* \param iMX                 [IN] -  motion vector, x component
* \param iMY                 [IN] -  motion vector, y component
* \param pMInfo              [IN/OUT] - pointer to the structure with motion information about current block
* \param BlockSize           [IN] -  size of current block
* 
* \return sum of absolute differences between two blocks
*/
long MainDiffFunction(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo, cMacroBlockPartitions BlockSize)
{
	int iError;
	DWORD* pID;
	
	if (!(-pMEStruct->m_iMaxMVX <= iMX && iMX <= pMEStruct->m_iMaxMVX &&
		-pMEStruct->m_iMaxMVY <= iMY && iMY <= pMEStruct->m_iMaxMVY))
		return INT_MAX;
	
	if (!pMEStruct->m_bBordersAllowed)
	{
		int x, y, pelMX, pelMY;
	
		if(iMX > 0)
			pelMX = (iMX + pMEStruct->m_PrecMX) >> pMEStruct->m_PrecMX;
		else
			pelMX = iMX >> pMEStruct->m_PrecMX;

		if(iMY > 0)
			pelMY = (iMY + pMEStruct->m_PrecMY) >> pMEStruct->m_PrecMY;
		else	
			pelMY = iMY >> pMEStruct->m_PrecMY;

		x = iX + pelMX;
		if (pMInfo->m_bFieldInFrame)
			y = (iY & ~1) + (pelMY << 1) + pMInfo->m_refField;
		else
			y = iY + pelMY;

	
		if (!(0 <= x && x + MBwidth[BlockSize] <= pMEStruct->m_iWidth &&
			0 <= y && y + (MBheight[BlockSize] << pMInfo->m_bFieldInFrame) <= pMEStruct->m_iHeight))
			return INT_MAX;
	}
	
	if (pMInfo->m_bFieldInFrame)
		pID = &pMEStruct->m_blockIDs[((iMY << 1) + pMInfo->m_refField + (pMEStruct->m_iMaxMVY << 1)) * ((pMEStruct->m_iMaxMVX << 1) + 1) + iMX + pMEStruct->m_iMaxMVX];
	else
        pID = &pMEStruct->m_blockIDs[(iMY + pMEStruct->m_iMaxMVY) * ((pMEStruct->m_iMaxMVX << 1) + 1) + iMX + pMEStruct->m_iMaxMVX];
	
	if (*pID != pMEStruct->m_curBlockID)
	{
		if (pMInfo)
		{
			pMInfo->m_CurMV.x = iMX;
			pMInfo->m_CurMV.y = iMY;
			
			pMInfo->m_iX = iX;
			pMInfo->m_iY = iY;
			
			pMInfo->m_iBlX = iX >> 4;
			pMInfo->m_iBlY = iY >> 4;
		}

		iMX <<= (2 - pMEStruct->m_PrecMX);
		iMY <<= (2 - pMEStruct->m_PrecMY);

		iError = METRIC_INDEX(pMEStruct, BlockSize);
		if (iError != -1)
			iError = Metrics[iError](pMEStruct, iX, iY, iMX, iMY, pMInfo);
		if (iError == -1)
			return -1;

		*pID = pMEStruct->m_curBlockID;

		if (pMEStruct->m_bConsiderVectorLength)
		{
			iError += (abs(pMInfo->m_CurMV.x << 2) >> (pMInfo->m_PrecMX + 2)) +
	       (abs(pMInfo->m_CurMV.y << 2) >> (pMInfo->m_PrecMY + 2));
		}
		
		return iError;
	}
	return INT_MAX;
}

/**
**************************************************************************
* \brief Initializes internal variables used by ME process
*
*  Initializes all internal variables and allocates necessary memory used
*  by ME process
*
* \param pMEStruct           [IN/OUT] - structure with all data needed to perform ME
* \param h                   [IN] -  height of frames to be processed (should be multiple of 16)
* \param w                   [IN] -  width of frames to be processed (should be multiple of 16)
* \param pMEParams           [IN] -  parameters of ME algorithm, can be NULL
*
* \return 0 if no errors, -1 otherwise
*/
int cStartME(gmcMotionEstimator* pMEStruct, int h, int w, cMEParams* pMEParams)
{
	int i,j;
    cMEParams localMEParams;

	pMEStruct->m_pParams = pMEParams;
    cStopME(pMEStruct);
    cInitMEParams(&localMEParams);
    cCopyMEParams(&localMEParams, pMEParams);

    assert(!(w&15) && !(h&15));
	assert(!(localMEParams.m_MEMetricAlgorithm == METRIC_COLORINDEPENDENT && !localMEParams.m_advMEParams.m_bConsiderChroma));

	pMEStruct->m_bBordersAllowed = localMEParams.m_bBordersAllowed;

	if(pMEStruct->m_bBordersAllowed)
		pMEStruct->m_iBorderSize = 16 + (MAX(localMEParams.m_iMaxMotHor, localMEParams.m_iMaxMotVert)+15)&~15;//48
	else
		pMEStruct->m_iBorderSize=0;

	pMEStruct->m_iHeight = h;
	pMEStruct->m_iWidth = w;
	pMEStruct->m_iExtHeight = h + 2*pMEStruct->m_iBorderSize;
	pMEStruct->m_iExtWidth = w + 2*pMEStruct->m_iBorderSize;
	pMEStruct->m_iFirstRowOffset = pMEStruct->m_iExtWidth * pMEStruct->m_iBorderSize + pMEStruct->m_iBorderSize;
	pMEStruct->m_iSecCompOff = pMEStruct->m_iExtWidth * pMEStruct->m_iExtHeight +
	                           pMEStruct->m_iExtWidth / localMEParams.m_advMEParams.m_ChrXMod *
	                           pMEStruct->m_iBorderSize / localMEParams.m_advMEParams.m_ChrYMod +
	                           pMEStruct->m_iBorderSize / localMEParams.m_advMEParams.m_ChrXMod;
	pMEStruct->m_iThdCompOff = pMEStruct->m_iSecCompOff + pMEStruct->m_iExtWidth / localMEParams.m_advMEParams.m_ChrXMod *
	                           pMEStruct->m_iExtHeight / localMEParams.m_advMEParams.m_ChrYMod;

	pMEStruct->m_iBlockSize = (AcceptableBlockSizes) localMEParams.m_MaxBlSize;
	pMEStruct->m_iPelsInBlock = pMEStruct->m_iBlockSize*pMEStruct->m_iBlockSize;
	pMEStruct->m_iBlocksHor = w/pMEStruct->m_iBlockSize;
	pMEStruct->m_iBlocksVert = h/pMEStruct->m_iBlockSize;
	pMEStruct->m_bExistPrevMV = FALSE;
	pMEStruct->m_iMBlocksHor = w/ME_MACROBLOCK_SIZE;
	pMEStruct->m_iMBlocksVert = h/ME_MACROBLOCK_SIZE;

	pMEStruct->m_iBlocksInFrame = pMEStruct->m_iBlocksHor * pMEStruct->m_iBlocksVert;
	pMEStruct->m_iMBlocksInFrame = pMEStruct->m_iMBlocksHor * pMEStruct->m_iMBlocksVert;
	pMEStruct->MVPrev = malloc( pMEStruct->m_iBlocksInFrame * sizeof(cMV) );
	pMEStruct->MVectors = malloc( pMEStruct->m_iBlocksInFrame * sizeof(cMV) );
	pMEStruct->m_MeanMVs = malloc( pMEStruct->m_iBlocksInFrame * sizeof(cMV) );
	
	for(i=0;i<NUM_REF_FRAMES;i++)
	{
		pMEStruct->m_MVPrvCand__[i][FT_TOP] = malloc( pMEStruct->m_iMBlocksInFrame * sizeof(cMVList) );
		pMEStruct->m_MVPrvCand__[i][FT_BOTTOM] = malloc( pMEStruct->m_iMBlocksInFrame * sizeof(cMVList) );
		
		pMEStruct->m_MVNxtCand__[i][FT_TOP] = malloc( pMEStruct->m_iMBlocksInFrame * sizeof(cMVList) );
		pMEStruct->m_MVNxtCand__[i][FT_BOTTOM] = malloc( pMEStruct->m_iMBlocksInFrame * sizeof(cMVList) );
		
		pMEStruct->m_RefMBlocks__[i][FT_TOP] = malloc( pMEStruct->m_iMBlocksInFrame * sizeof(cMacroBlock) );
		pMEStruct->m_RefMBlocks__[i][FT_BOTTOM] = malloc( pMEStruct->m_iMBlocksInFrame * sizeof(cMacroBlock) );

        for (j=0; j<pMEStruct->m_iMBlocksInFrame; j++)
		{
			cInitMacroBlock(pMEStruct->m_RefMBlocks__[i][FT_TOP] + j);
			cInitMacroBlock(pMEStruct->m_RefMBlocks__[i][FT_BOTTOM] + j);
			
			cInitMVListNode(pMEStruct->m_MVPrvCand__[i][FT_TOP] + j);
			cInitMVListNode(pMEStruct->m_MVPrvCand__[i][FT_BOTTOM] + j);
			
			cInitMVListNode(pMEStruct->m_MVNxtCand__[i][FT_TOP] + j);
			cInitMVListNode(pMEStruct->m_MVNxtCand__[i][FT_BOTTOM] + j);
		}
	}
	pMEStruct->m_MVPrvCand = pMEStruct->m_MVPrvCand__[0][FT_TOP];
	pMEStruct->m_MVNxtCand = pMEStruct->m_MVNxtCand__[0][FT_TOP];
	pMEStruct->m_RefMBlocks = pMEStruct->m_RefMBlocks__[0][FT_TOP];
	
	pMEStruct->m_CurMBlocks = malloc( pMEStruct->m_iMBlocksInFrame * sizeof(cMacroBlock) );
    if (!pMEStruct->m_CurMBlocks) return -1;
	
	for (i=0; i<pMEStruct->m_iBlocksInFrame; i++)
	{
		cInitMV(pMEStruct->MVPrev + i);
		cInitMV(pMEStruct->MVectors + i);
		cInitMV(pMEStruct->m_MeanMVs + i);
	}
	for (i=0; i<pMEStruct->m_iMBlocksInFrame; i++)
	{
		cInitMacroBlock(pMEStruct->m_CurMBlocks + i);
	}

	pMEStruct->m_MInfo.m_iBlocksHor = pMEStruct->m_iBlocksHor;
	pMEStruct->m_MInfo.m_iBlocksVert = pMEStruct->m_iBlocksVert;
	pMEStruct->m_MInfo.pCurMVs = pMEStruct->MVectors;
	pMEStruct->m_MInfo.pPrvMVs = pMEStruct->MVPrev;
	pMEStruct->m_MInfo.m_bFieldInFrame = FALSE;
	pMEStruct->m_MInfo.m_MEDir = ME_FWD;
	pMEStruct->m_MInfo.m_iPicNum = 0;
	
	pMEStruct->m_MInfo.m_CodecInfo.m_iPicNumProcOrder = 0;
	pMEStruct->m_MInfo.m_CodecInfo.m_pRDOinfo = localMEParams.m_pRDOinfo;
	pMEStruct->m_MInfo.m_CodecInfo.m_pMVPredictor = NULL;
	
	pMEStruct->m_MInfo.m_PrecMX = localMEParams.m_PrecMX;
	pMEStruct->m_MInfo.m_PrecMY = localMEParams.m_PrecMY;

	pMEStruct->pGetError_16x16 = localMEParams.m_advMEParams.pGetError_16x16;
	pMEStruct->pGetError_16x8 = localMEParams.m_advMEParams.pGetError_16x8;
	pMEStruct->pGetError_8x8   = localMEParams.m_advMEParams.pGetError_8x8;
	pMEStruct->pGetError_4x4   = localMEParams.m_advMEParams.pGetError_4x4;

	pMEStruct->pGetErrorChr_16x16 = localMEParams.m_advMEParams.pGetErrorChr_16x16;
	pMEStruct->pGetErrorChr_8x8   = localMEParams.m_advMEParams.pGetErrorChr_8x8;
	pMEStruct->pGetErrorChr_4x4   = localMEParams.m_advMEParams.pGetErrorChr_4x4;

	pMEStruct->m_iMaxMotHor = localMEParams.m_iMaxMotHor;
	pMEStruct->m_iMaxMotVert = localMEParams.m_iMaxMotVert;
	pMEStruct->m_PrecMX = localMEParams.m_PrecMX;
	pMEStruct->m_PrecMY = localMEParams.m_PrecMY;
	pMEStruct->m_iMaxMVX = pMEStruct->m_iMaxMotHor << pMEStruct->m_PrecMX;
	pMEStruct->m_iMaxMVY = pMEStruct->m_iMaxMotVert << pMEStruct->m_PrecMY;
    pMEStruct->m_iCacheSizeVert = pMEStruct->m_iMaxMVY;
    pMEStruct->m_iCacheSizeHor = pMEStruct->m_iMaxMVX;

	pMEStruct->m_iMEAlg = localMEParams.m_MEAlgorithm;
	pMEStruct->m_pEstimate = pMEStruct->m_AvailableMEFunctions[pMEStruct->m_iMEAlg];
	pMEStruct->m_iMEMetricAlg = localMEParams.m_MEMetricAlgorithm;
	pMEStruct->m_bIsInterlaced = localMEParams.m_bIsInterlaced;
	pMEStruct->m_bBordersAllowed = localMEParams.m_bBordersAllowed;
	pMEStruct->m_interAlg = (InterpolationAlgorithm)localMEParams.m_interAlg;

    // mode of MV candidates memory management
    pMEStruct->m_CandidatesMemoryAlloc = localMEParams.m_advMEParams.m_CandidatesMemoryAlloc;
    // number of preallocated elements for onfly mode
    pMEStruct->m_CandKeepPrealloc = localMEParams.m_advMEParams.m_CandKeepPrealloc;
    if (pMEStruct->m_CandKeepPrealloc < 1) pMEStruct->m_CandKeepPrealloc = 1;
    if (pMEStruct->m_CandidatesMemoryAlloc == cmaOnStart)
    {
        // old version of MV candidates memory management
        for (j=0; j<pMEStruct->m_iMBlocksInFrame; j++)
        {
            int res;
            res = cInitMVList( pMEStruct->m_MVPrvCand + j, ((((pMEStruct->m_iMaxMotHor + 15) >> 4) << 1) + 1) * ((((pMEStruct->m_iMaxMotVert + 15) >> 4) << 1) + 1) * 4 + 10 );
            if (res) return res;
            res = cInitMVList( pMEStruct->m_MVNxtCand + j, ((((pMEStruct->m_iMaxMotHor + 15) >> 4) << 1) + 1) * ((((pMEStruct->m_iMaxMotVert + 15) >> 4) << 1) + 1) * 4 + 10 );
            if (res) return res;
        }
    }
    else
    {
        // new version
        // allocate less memory and when processing free unused memory
        for (j=0; j<pMEStruct->m_iMBlocksInFrame; j++)
        {
            int res;
            res = cInitMVList( pMEStruct->m_MVPrvCand + j, pMEStruct->m_CandKeepPrealloc );
            if (res) return res;
            res = cInitMVList( pMEStruct->m_MVNxtCand + j, pMEStruct->m_CandKeepPrealloc );
            if (res) return res;
        }
    }

    // initializing statistic gathering variables for entire sequence
	pMEStruct->m_fSequnceAverageMEErrorPerPixel = 0;
	pMEStruct->m_fSequnceAverageSADsPerPixel = 0;
	pMEStruct->m_iFrameCount = 0;

	pMEStruct->m_fMeanErrorPP = localMEParams.m_devMEParams.m_fMeanErrorPP;
	pMEStruct->m_fMeanErrRenew = localMEParams.m_devMEParams.m_fMeanErrRenew;
	pMEStruct->m_fDispersionPP = localMEParams.m_devMEParams.m_fDispersionPP;
	pMEStruct->m_fDispersionRenew = localMEParams.m_devMEParams.m_fDispersionRenew;
	pMEStruct->m_bConsiderChroma = localMEParams.m_advMEParams.m_bConsiderChroma;

	pMEStruct->m_MVCandidates = malloc( 5 * sizeof(cMV) );
	for (i = 0; i < 5; i++)
	{
		cInitMV(pMEStruct->m_MVCandidates + i);
	}
	pMEStruct->m_CurBlock = malloc( ME_MACROBLOCK_SIZE * ME_MACROBLOCK_SIZE * 3 );
	pMEStruct->m_CurBlChr1 = pMEStruct->m_CurBlock + ME_MACROBLOCK_SIZE * ME_MACROBLOCK_SIZE;
	pMEStruct->m_CurBlChr2 = pMEStruct->m_CurBlock + ME_MACROBLOCK_SIZE * ME_MACROBLOCK_SIZE * 2;
	pMEStruct->m_RefBlock = malloc( ME_MACROBLOCK_SIZE * ME_MACROBLOCK_SIZE * 3 );
	pMEStruct->m_RefBlChr1 = pMEStruct->m_RefBlock + ME_MACROBLOCK_SIZE * ME_MACROBLOCK_SIZE;
	pMEStruct->m_RefBlChr2 = pMEStruct->m_RefBlock + ME_MACROBLOCK_SIZE * ME_MACROBLOCK_SIZE * 2;

	pMEStruct->m_iChrXMod = localMEParams.m_advMEParams.m_ChrXMod;
	pMEStruct->m_iChrYMod = localMEParams.m_advMEParams.m_ChrYMod;
	pMEStruct->m_iFirstRowOffsetChr = (pMEStruct->m_iExtWidth / pMEStruct->m_iChrXMod) * (pMEStruct->m_iBorderSize / pMEStruct->m_iChrYMod) + pMEStruct->m_iBorderSize / pMEStruct->m_iChrXMod;

	pMEStruct->m_bConsiderVectorLength = localMEParams.m_advMEParams.m_bConsiderVectorLength;

	//hierarchic
	pMEStruct->m_iHierarchicReduction = localMEParams.m_advMEParams.m_HierarchicReduction;
	pMEStruct->m_iHierarchicMaxMotHor = pMEStruct->m_iMaxMotHor/pMEStruct->m_iHierarchicReduction;
	pMEStruct->m_iHierarchicMaxMotVert = pMEStruct->m_iMaxMotVert/pMEStruct->m_iHierarchicReduction;
	
	pMEStruct->m_pCurrentHierarchicBlock = malloc (sqr(pMEStruct->m_iBlockSize/pMEStruct->m_iHierarchicReduction) );
	pMEStruct->m_pHierarchicPrevFrameArea = malloc ((pMEStruct->m_iHierarchicMaxMotHor*2+pMEStruct->m_iBlockSize/pMEStruct->m_iHierarchicReduction) * (pMEStruct->m_iHierarchicMaxMotVert*2+pMEStruct->m_iBlockSize/pMEStruct->m_iHierarchicReduction));

	pMEStruct->m_pHierLaunchMap = malloc ( pMEStruct->m_iMBlocksInFrame * 16);
	memset( pMEStruct->m_pHierLaunchMap, 0, pMEStruct->m_iMBlocksInFrame * 16);

	pMEStruct->m_bAccept_16x16 = localMEParams.m_MinBlSize <= cbt_16x16 && cbt_16x16 <= localMEParams.m_MaxBlSize;
	pMEStruct->m_bAccept_8x8   = localMEParams.m_MinBlSize <= cbt_8x8   && cbt_8x8   <= localMEParams.m_MaxBlSize;
	pMEStruct->m_bAccept_4x4   = localMEParams.m_MinBlSize <= cbt_4x4   && cbt_4x4   <= localMEParams.m_MaxBlSize;

	if(pMEStruct->m_interAlg==SixTap)
	{
		pMEStruct->m_pRefShiftsY[1] = malloc(pMEStruct->m_iExtHeight * pMEStruct->m_iExtWidth * 3);
		pMEStruct->m_pRefShiftsY[2] = pMEStruct->m_pRefShiftsY[1] + pMEStruct->m_iExtHeight * pMEStruct->m_iExtWidth;
		pMEStruct->m_pRefShiftsY[3] = pMEStruct->m_pRefShiftsY[2] + pMEStruct->m_iExtHeight * pMEStruct->m_iExtWidth;
		pMEStruct->m_pRefShiftsYMem = NULL;
		pMEStruct->m_bUsePreparedShifts = FALSE;

        //TODO: looks like this variable should be used only when m_bConsiderChroma is true...
        //But it is used further in cEstimate()...
        pMEStruct->m_bUsePreparedChromaShifts = FALSE;

		if (pMEStruct->m_bConsiderChroma) {
			int extSizeChr = (pMEStruct->m_iExtHeight / pMEStruct->m_iChrYMod) * (pMEStruct->m_iExtWidth / pMEStruct->m_iChrXMod);

			pMEStruct->m_pRefShiftsChr1[1] = malloc(extSizeChr * 3);
			pMEStruct->m_pRefShiftsChr1[2] = pMEStruct->m_pRefShiftsChr1[1] + extSizeChr;
			pMEStruct->m_pRefShiftsChr1[3] = pMEStruct->m_pRefShiftsChr1[2] + extSizeChr;
			pMEStruct->m_pRefShiftsChr1Mem = NULL;

			pMEStruct->m_pRefShiftsChr2[1] = malloc(extSizeChr * 3);
			pMEStruct->m_pRefShiftsChr2[2] = pMEStruct->m_pRefShiftsChr2[1] + extSizeChr;
			pMEStruct->m_pRefShiftsChr2[3] = pMEStruct->m_pRefShiftsChr2[2] + extSizeChr;
			pMEStruct->m_pRefShiftsChr2Mem = NULL;
            //pMEStruct->m_bUsePreparedChromaShifts = FALSE; //see comment above...
		}
	}

    pMEStruct->m_blockIDs=(DWORD*)malloc(sizeof(DWORD) * (2 * pMEStruct->m_iCacheSizeHor + 1) * (2 * pMEStruct->m_iCacheSizeVert + 1));
	memset(pMEStruct->m_blockIDs, 0, sizeof(DWORD) * (2 * pMEStruct->m_iCacheSizeHor + 1) * (2 * pMEStruct->m_iCacheSizeVert + 1));
	pMEStruct->m_curBlockID = 1;

    cDeInitMEParams(&localMEParams);

    return 0;
};

/**
**************************************************************************
* \brief Fills user buffer with motion information
*
*  This function fills given buffer with motion information
*  such as motion vector coordinates. Used in video coding
*
* \param dst                 [OUT] - pointer to users buffer
* \param MVectors            [IN] -  pointer to the cMV array
* \param iBlocksInFrame      [IN] -  quantity of motion vectors
* 
* \return number of used bytes in buffer
*/
int cGetMVs(BYTE* dst, cMV* MVectors, int iBlocksInFrame)
{
	int i, count=0;
	BYTE* p = dst;
	BYTE* q = dst + iBlocksInFrame;

	for(i=0; i<iBlocksInFrame; i++)
	{
		*(p++) = MVectors[i].x;
		*(q++) = MVectors[i].y;
		count++;
	}

	return count;
}

/**
**************************************************************************
* \brief Sets motion information to motion estimation object
*
*  This function fills motion information of motion estimation
*  object from buffer, given by user. This motion information
*  includes motion vector coordinates. Used in video decoding.
*
* \param src                 [IN] -  pointer to users buffer
* \param MVectors            [IN] -  pointer to the cMV array
* \param iBlocksInFrame      [IN] -  quantity of motion vectors
* 
* \return none
*/
void cSetMVs(signed char* src, cMV* MVectors, int iBlocksInFrame)
{
	int i;
	signed char* p = src;
	signed char* q = src + iBlocksInFrame;

	for(i=0; i<iBlocksInFrame; i++)
	{
		MVectors[i].x = *(p++);
		MVectors[i].y = *(q++);
	}
}

/**
**************************************************************************
* \brief Prints configuration of gmcMotionEstimator after processing
*
*  This function prints to file all necessary for debug data from 
*  gmcMotionEstimator structure to file
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* \param sFileName           [IN] -  file name
* 
* \return none
*/
void PrintConfig(gmcMotionEstimator* pMEStruct, char* sFileName)
{
	FILE* f;
	if(!pMEStruct->m_iFrameCount)
		return;
	
	pMEStruct->m_fSequnceAverageSADsPerPixel /= pMEStruct->m_iFrameCount;
	pMEStruct->m_fSequnceAverageMEErrorPerPixel /= pMEStruct->m_iFrameCount;
	f = fopen(sFileName, "a");
	fprintf(f,"ME algorithm info:\n");
	switch(pMEStruct->m_iMEAlg)
	{
	case _3DRS:
		fprintf(f, "Algorithm is 3DRS\n");
		break;
	case E3DRS:
		fprintf(f, "Algorithm is E3DRS\n");
		break;
	case FSA:
		fprintf(f, "Algorithm is FSA\n");
		break;
	case MEDS:
		fprintf(f, "Algorithm is DS\n");
		break;
	case HIER:
		fprintf(f, "Algorithm is HS\n");
		break;
	case MSU:
		fprintf(f, "Algorithm is MSU\n");
		break;
	default:
		fprintf(f, "Unknown algorithm\n");
		break;
	}
	fprintf(f, "Search range: %dx%d\n", pMEStruct->m_iMaxMotHor, pMEStruct->m_iMaxMotVert);
	fprintf(f, "Block sizes: %dx%d\n", pMEStruct->m_iBlockSize, pMEStruct->m_iBlockSize);
	fprintf(f, "Average SAD operations per pixel: %f\n", pMEStruct->m_fSequnceAverageSADsPerPixel);
	fprintf(f, "Average SAD value per pixel: %f\n", pMEStruct->m_fSequnceAverageMEErrorPerPixel);
	fprintf(f, "----------------------------------------------------\n");
		
	fclose(f);
}

/**
**************************************************************************
* \brief Frees resources had been used by ME
*
*  Deallocates all memory had been used for ME
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
*
* \return none
*/
void cStopME(gmcMotionEstimator* pMEStruct)
{
	int i,j;
	
	pMEStruct->m_iFrameCount=0;
	pMEStruct->m_fSequnceAverageSADsPerPixel = 0;
	pMEStruct->m_fSequnceAverageMEErrorPerPixel = 0;
	
	if (pMEStruct->m_sConfigFileName)
	{
		free(pMEStruct->m_sConfigFileName);
		pMEStruct->m_sConfigFileName = NULL;
	}
	if (pMEStruct->MVPrev)
	{
		free(  pMEStruct->MVPrev );
		pMEStruct->MVPrev = NULL;
	}
	if (pMEStruct->MVectors)
	{
		free( pMEStruct->MVectors );
		pMEStruct->MVectors = NULL;
	}
	if (pMEStruct->m_MeanMVs)
	{
		free( pMEStruct->m_MeanMVs );
		pMEStruct->m_MeanMVs = NULL;
	}
	if (pMEStruct->m_MVCandidates)
	{
		free( pMEStruct->m_MVCandidates );
		pMEStruct->m_MVCandidates = NULL;
	}
	if (pMEStruct->m_pCurrentHierarchicBlock)
	{
		free( pMEStruct->m_pCurrentHierarchicBlock );
		pMEStruct->m_pCurrentHierarchicBlock = NULL;
	}
	if (pMEStruct->m_pHierarchicPrevFrameArea)
	{
		free( pMEStruct->m_pHierarchicPrevFrameArea );
		pMEStruct->m_pHierarchicPrevFrameArea = NULL;
	}
	if (pMEStruct->m_CurBlock)
	{
		free( pMEStruct->m_CurBlock );
		pMEStruct->m_CurBlock = NULL;
	}
	if (pMEStruct->m_RefBlock)
	{
		free( pMEStruct->m_RefBlock );
		pMEStruct->m_RefBlock = NULL;
	}
	for(i=0;i<NUM_REF_FRAMES;i++)
	{
		if (pMEStruct->m_MVPrvCand__[i][FT_TOP])
		{
			for (j=0; j<pMEStruct->m_iMBlocksInFrame; j++) cDeInitMVList( pMEStruct->m_MVPrvCand__[i][FT_TOP] + j );
			free( pMEStruct->m_MVPrvCand__[i][FT_TOP] );
			pMEStruct->m_MVPrvCand__[i][FT_TOP] = NULL;
		}
		if (pMEStruct->m_MVPrvCand__[i][FT_BOTTOM])
		{
			for ( j=0; j<pMEStruct->m_iMBlocksInFrame; j++) cDeInitMVList(pMEStruct->m_MVPrvCand__[i][FT_BOTTOM] + j );
			free( pMEStruct->m_MVPrvCand__[i][FT_BOTTOM] );
			pMEStruct->m_MVPrvCand__[i][FT_BOTTOM] = NULL;
		}

		if (pMEStruct->m_MVNxtCand__[i][FT_TOP])
		{
			for (j=0; j<pMEStruct->m_iMBlocksInFrame; j++) cDeInitMVList(pMEStruct->m_MVNxtCand__[i][FT_TOP] + j);
			free( pMEStruct->m_MVNxtCand__[i][FT_TOP] );
			pMEStruct->m_MVNxtCand__[i][FT_TOP] = NULL;
		}

		if (pMEStruct->m_MVNxtCand__[i][FT_BOTTOM])
		{
			for (j=0; j<pMEStruct->m_iMBlocksInFrame; j++) cDeInitMVList(pMEStruct->m_MVNxtCand__[i][FT_BOTTOM] + j);
			free( pMEStruct->m_MVNxtCand__[i][FT_BOTTOM] );
			pMEStruct->m_MVNxtCand__[i][FT_BOTTOM] = NULL;
		}
		
		free(pMEStruct->m_RefMBlocks__[i][FT_TOP]);
		pMEStruct->m_RefMBlocks__[i][FT_TOP]=NULL;

		free(pMEStruct->m_RefMBlocks__[i][FT_BOTTOM]);
		pMEStruct->m_RefMBlocks__[i][FT_BOTTOM]=NULL;
	}
	
	if (pMEStruct->m_pHierLaunchMap)
	{
		free( pMEStruct->m_pHierLaunchMap );
		pMEStruct->m_pHierLaunchMap = NULL;
	}
	if (pMEStruct->m_CurMBlocks)
	{
		free( pMEStruct->m_CurMBlocks );
		pMEStruct->m_CurMBlocks = NULL;
	}
	if (pMEStruct->m_pRefShiftsY[1])
	{
		free(pMEStruct->m_pRefShiftsY[1]);
		pMEStruct->m_pRefShiftsY[1] = NULL;
		pMEStruct->m_pRefShiftsY[2] = NULL;
		pMEStruct->m_pRefShiftsY[3] = NULL;
	}
	if (pMEStruct->m_pRefShiftsChr1[1])
	{
		free(pMEStruct->m_pRefShiftsChr1[1]);
		pMEStruct->m_pRefShiftsChr1[1] = NULL;
		pMEStruct->m_pRefShiftsChr1[2] = NULL;
		pMEStruct->m_pRefShiftsChr1[3] = NULL;
	}
	if (pMEStruct->m_pRefShiftsChr2[1])
	{
		free(pMEStruct->m_pRefShiftsChr2[1]);
		pMEStruct->m_pRefShiftsChr2[1] = NULL;
		pMEStruct->m_pRefShiftsChr2[2] = NULL;
		pMEStruct->m_pRefShiftsChr2[3] = NULL;
	}

	if(pMEStruct->m_blockIDs)
	{
		free(pMEStruct->m_blockIDs);
		pMEStruct->m_blockIDs = NULL;
	}	
}

/**
**************************************************************************
* \brief Change the size of MV search area
*
*  Changes the size of MV search area
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* \param iMaxMotHor          [IN/OUT] - width of new MV search area (in pixels)
* \param iMaxMotVert         [IN/OUT] - height of new MV search area (in pixels)
*
* \return none
*/
void cChangeMotionArea(gmcMotionEstimator* pMEStruct, int iMaxMotHor, int iMaxMotVert)
{
    int iMaxMVX = iMaxMotHor << pMEStruct->m_PrecMX;
    int iMaxMVY = iMaxMotVert << pMEStruct->m_PrecMY;

    if ((iMaxMVX <= pMEStruct->m_iCacheSizeHor) && (iMaxMVY <= pMEStruct->m_iCacheSizeVert))
    {
        pMEStruct->m_iMaxMotHor = iMaxMotHor;
        pMEStruct->m_iMaxMotVert = iMaxMotVert;

        pMEStruct->m_iMaxMVX = iMaxMVX;
        pMEStruct->m_iMaxMVY = iMaxMVY;
    }    
}
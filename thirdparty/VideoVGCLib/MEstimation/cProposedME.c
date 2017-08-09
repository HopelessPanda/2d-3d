/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cProposedME.c
*  \brief File with implimentation of proposed ME algorithms
*
*  This file contains implementation of proposed motion estimation
*  algorithms in video.
*
*  $Id: cProposedME.c,v 1.17.4.4 2008/07/27 10:57:09 10i_tsv Exp $
*  $Log: cProposedME.c,v $
*  Revision 1.17.4.4  2008/07/27 10:57:09  10i_tsv
*  Needless comme*
*  Revision 1.17.4.3  2008/07/24 18:13:21  10i_tsv
*  Needle*
*  Revision 1.17.4.2  2008/07/14 06:13:11  10i_tsv
*  Needless comments removed
*  @
*  text
*  @d17 1
*  a17 1
*  *  $Id: cProposedME.c,v 1.17 2007/06/28 08:22:46 *
*  Revision 1.17.4.1  2008/07/12 18:09:05  10i_tsv
*  Needless comments removed
*  @
*  text
*  @d17 1
*  a17 1
*  *  $Id: cProposedME.c,v 1.1*
*  Revision 1.17  2007/06/28 08:22:46  05s_gri
*  og
*  @Needless comments removed
*  @
*  text
*  @d*
*  Revision 1.16  2007/06/04 14:40:40  05s_gri
*  Doxygen completed. Doxygen warnings are not fixed.
*
*  Revision 1.15  2007/04/22 20:15:47  05s_put
*  Add/fix doxygen comments
*
*  Revision 1.14  2007/04/20 10:19:35  05s_gri
*  Source files structure improved
*
*  Revision 1.13  2007/03/05 14:19:07  05s_put
*  - interface refactoring
*  - memory fix
*
*  Revision 1.8  2006/12/20 17:36:07  05s_put
*  - doxygen comments fix
*  - small code cleanup
*
*  Revision 1.7  2006/12/20 16:56:51  05s_gri
*  Code doxygenning is continued
*
*  Revision 1.6  2006/12/18 13:46:12  05s_put
*  - doxygen comments fixed
*
*  Revision 1.5  2006/11/23 10:59:59  05s_put
*  - fixed cSetPrvMVs for 4x4 blocks
*  - made function, that makes possible give ME externally made half-pixel shifts
*  - fixed negative error values
*  - fixed warning with unreferenced parameter
*
*  Revision 1.4  2006/11/17 21:39:20  05s_gri
*  Command line implementation and first release preparation finished
*
*  Revision 1.2  2006/11/13 10:24:53  05s_put
*  New version of compensation
*
*  Revision 1.15  2006/10/25 12:05:20  09k_sim
*  Unnecessary content was removed.
*
*  Revision 1.14  2006/10/22 09:18:35  09k_sim
*  Field-in-frame ME improved.
*
*  Revision 1.13  2006/10/14 09:56:51  09k_sim
*  Bug fixed.
*
*  Revision 1.12  2006/10/14 09:25:46  09k_sim
*  16x8 prediction in field pictures was improved.
*
*  Revision 1.11  2006/10/13 17:48:20  09k_sim
*  Began to add field processing.
*
*  Revision 1.10  2006/10/08 18:30:12  09k_sim
*  Refactoring.
*
*  Revision 1.9  2006/10/02 01:08:20  09k_sim
*  Temporal candidates changed.
*
*  Revision 1.8  2006/09/24 10:29:59  09k_sim
*  Some refactoring concerning inertia MV candidates was done (needed by gmCodecME).
*
*  Revision 1.7  2006/09/22 10:48:22  09k_sim
*  Better multiframe support was added.
*
*  Revision 1.6  2006/09/09 13:24:57  09k_sim
*  Began to add codec support to the library.
*  MPEG2 ME is supported.
*
*  Revision 1.5  2006/09/04 15:25:19  09k_sim
*  Bugs fixed
*
*  Revision 1.4  2006/08/01 13:20:20  09k_sim
*  Block IDs caching was added.
*  Block splitting criteria were changed. Threshold calculation was changed: function for neighbourhood contrast calculation was added.
*  A some kind of noise estimation was added, GetBlockVarMean function added.
*
*  Revision 1.3  2006/07/20 00:20:26  09k_sim
*  Some unnecessary function parameters and variables deleted.
*  SAD cache added, but not in use
*
*  Revision 1.2  2006/07/13 07:51:30  05s_put
*  Fixed error
*
*/

#include "cMEstimation.h"
#include "cSimpleMEAlgs.h"
#include "cProposedME.h"

#include<stdio.h>

/**
**************************************************************************
* \brief This function filters motion vector candidate list
*
*  This function removes similar motion vectors from the list except one
*
* \param pMVList             [IN/OUT] - pointer to the list to filter
* \param iAbsValue           [IN] -  defining similarity value
* 
* \return none
*/
void FilterMVList(cMVList* pMVList, int iAbsValue)
{
	cMVList* pInitElem = pMVList;
	cMVList* pPrvInitElem = pMVList;
	cMVList* pReviewElem;
	cMVList* pPrvReviewElem;
	cMVList UnUsedList;
	cMVList* pCurUnUsedElem = &UnUsedList;

	while (pInitElem && pInitElem->m_bUsed)
	{
		pReviewElem = pInitElem->m_pNext;
		pPrvReviewElem = pInitElem;
		while (pReviewElem && pReviewElem->m_bUsed)
		{
			if (MVABS(&pReviewElem->m_DataMV, &pInitElem->m_DataMV) < iAbsValue)
			{
				pCurUnUsedElem->m_pNext = pReviewElem;
				pCurUnUsedElem = pReviewElem;
				pReviewElem->m_bUsed = FALSE;
				pPrvReviewElem->m_pNext = pReviewElem->m_pNext;
				pReviewElem = pReviewElem->m_pNext;
			}
			else
			{
				pReviewElem = pReviewElem->m_pNext;
				pPrvReviewElem = pPrvReviewElem->m_pNext;
			}
		}
		pPrvInitElem = pInitElem;
		pInitElem = pInitElem->m_pNext;
	}
	pCurUnUsedElem->m_pNext = NULL;

	while (pPrvInitElem->m_pNext)
	{
		pPrvInitElem = pPrvInitElem->m_pNext;
	}
	pPrvInitElem->m_pNext = UnUsedList.m_pNext;
	UnUsedList.m_pNext = NULL;
}


int LauncheNumber = 0;
int DepthSum = 0;
/**
**************************************************************************
* \brief Deciding the best vector from the given list
*
*  This function decides which vector from the given list has the smallest
*  error in current block.
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* \param pMVList             [IN] -  pointer to the first element of the list
* \param iX                  [IN] -  horizontal coordinate of the block
* \param iY                  [IN] -  vertical coordinate of the block
* \param pMInfo              [IN/OUT] - pointer to the structure with motion information about current block
* \param BlockSize           [IN] -  size of current block
* 
* \return none
*/
cMV CheckMVList(gmcMotionEstimator* pMEStruct, cMVList* pMVList, int iX, int iY, MInfoStruct* pMInfo, cMacroBlockPartitions BlockSize)
{
	cMV ResultMV;
	int MX,MY,iError;
    int Depth = 0;
	//const float fThreshold = (pMEStruct->m_fMeanErrorPP + pMEStruct->m_fDispersionPP) / (ME_MACROBLOCK_SIZE / BlockSize) / (ME_MACROBLOCK_SIZE / BlockSize);

	cInitMV(&ResultMV);

    LauncheNumber++;
	while (pMVList && pMVList->m_bUsed)
	{
		MX=pMVList->m_DataMV.x;
		if(pMEStruct->m_MInfo.m_bFieldInFrame)
			MY=pMVList->m_DataMV.y>>1;
		else
			MY=pMVList->m_DataMV.y;

		iError = MainDiffFunction(pMEStruct, iX, iY, MX, MY, pMInfo, BlockSize);
		if (iError < ResultMV.error)
		{
			ResultMV.x = MX;
			ResultMV.y = MY;
			ResultMV.error = iError;
		}
		pMVList = pMVList->m_pNext;
        Depth++;
	}
    DepthSum += Depth;

	return ResultMV;
}

/**
**************************************************************************
* \brief Calculates maximal deviation of motion vectors
*
*  This function calculates maximal deviation between motion vector
*  components for the vectors of given list. Also mean of the vectors of
*  this list is calculated. Deviation is stored as an error of the mean
*  vector.
*
* \param pMVList             [IN] -  pointer to the first element of motion vector list
* 
* \return mean vector
*/
cMV CalculateMVDisp(cMVList* pMVList)
{
	cMV ResultMV;
	cMVList* pCurMV/*,* pDifMV*/;
	int iMVQuantity = 0;
// 	int iMaxDif=0;
	int iCurDiff;
	cInitMV(&ResultMV);

	pCurMV = pMVList;
	while (pCurMV && pCurMV->m_bUsed)
	{
		ResultMV.x += pCurMV->m_DataMV.x;
		ResultMV.y += pCurMV->m_DataMV.y;
		iMVQuantity++;
// 		pDifMV = pCurMV->m_pNext;
// 		while (pDifMV && pDifMV->m_bUsed)
// 		{
// 			iCurDiff = MAX(abs(pCurMV->m_DataMV.x-pDifMV->m_DataMV.x), abs(pCurMV->m_DataMV.y-pDifMV->m_DataMV.y));//TODO: ѕеределать к €поне маме!
// 			if (iCurDiff > iMaxDif) iMaxDif = iCurDiff;
// 			pDifMV = pDifMV->m_pNext;
// 		}
		pCurMV = pCurMV->m_pNext;
	}
	if (iMVQuantity)
	{
		ResultMV.x /= iMVQuantity;
		ResultMV.y /= iMVQuantity;
		ResultMV.error = 0;

        pCurMV = pMVList;
        while (pCurMV && pCurMV->m_bUsed)
        {
            iCurDiff = MAX( abs( pCurMV->m_DataMV.x - ResultMV.x ), abs( pCurMV->m_DataMV.y - ResultMV.y ) );
            if ( iCurDiff > ResultMV.error ) ResultMV.error = iCurDiff;
            pCurMV = pCurMV->m_pNext;
        }
    }
	else
	{
		ResultMV.x = 0;
		ResultMV.y = 0;
		ResultMV.error = 0;
	}

	return ResultMV;
}

/**
**************************************************************************
* \brief Calculates dispersions of motion vectors
*
*  This function calculates dispersion of motion vectors in the area of
*  the current block. Also mean of the vectors of this list is calculated.
*  Deviation is stored as an error of the mean vector.
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param iBX                 [IN] -  horizontal coordinate of the current block
* \param iBY                 [IN] -  vertical coordinate of the current block
* 
* \return mean vector
*/
cMV CalculateMVDisp2(gmcMotionEstimator* pMEStruct, int iBX, int iBY)
{
	cMV MeanMV;
	int i, j, k, h;
	int iIndex;
	int iCounter;
	int iStartY = MAX(iBY-2, 0);
	int iStartX = MAX(iBX-2, 0);
	int iEndX   = MIN(iBX+2, pMEStruct->m_iMBlocksHor);
	float fDisp;

	cInitMV(&MeanMV);

	iCounter = 0;
	// 2 previous lines of blocks
	iIndex = iStartY * pMEStruct->m_iMBlocksHor;
	for (i = iStartY; i < iBY; i++)
	{
		for (j = iStartX; j < iEndX; j++)
		{
			register int iCurIndex = iIndex + j;
			if (pMEStruct->m_CurMBlocks[iCurIndex].m_BlockPartition == cbt_16x16)
			{
				MeanMV.x += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[0].x;
				MeanMV.y += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[0].y;
				iCounter++;
			}
			else
			{
				for (k = 0; k < 4; k++)
				{
					if (pMEStruct->m_CurMBlocks[iCurIndex].m_BlockPartition == cbt_8x8)
					{
						register int iCurIndexMV = k << 2;
						MeanMV.x += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[iCurIndexMV].x;
						MeanMV.y += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[iCurIndexMV].y;
						iCounter++;
					}
					else
					{
						for (h = 0; h < 4; h++)
						{
							register int iCurIndexMV = (k << 2) + h;
							MeanMV.x += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[iCurIndexMV].x;
							MeanMV.y += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[iCurIndexMV].y;
							iCounter++;
						}
					}
				}
			}
		}
		iIndex += pMEStruct->m_iMBlocksHor;
	}

	// 2 blocks to the left
	iIndex = iBY * pMEStruct->m_iMBlocksHor;
	for (j = iStartX; j < iBX; j++)
	{
		register int iCurIndex = iIndex + j;
		if (pMEStruct->m_CurMBlocks[iCurIndex].m_BlockPartition == cbt_16x16)
		{
			MeanMV.x += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[0].x;
			MeanMV.y += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[0].y;
			iCounter++;
		}
		else
		{
			for (k = 0; k < 4; k++)
			{
				if (pMEStruct->m_CurMBlocks[iCurIndex].m_BlockPartition == cbt_8x8)
				{
					register int iCurIndexMV = k << 2;
					MeanMV.x += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[iCurIndexMV].x;
					MeanMV.y += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[iCurIndexMV].y;
					iCounter++;
				}
				else
				{
					for (h = 0; h < 4; h++)
					{
						register int iCurIndexMV = (k << 2) + h;
						MeanMV.x += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[iCurIndexMV].x;
						MeanMV.y += pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[iCurIndexMV].y;
						iCounter++;
					}
				}
			}
		}
	}

	if (iCounter)
	{
		MeanMV.x /= iCounter;
		MeanMV.y /= iCounter;
	}
	else
	{
		MeanMV.x = 0;
		MeanMV.y = 0;
		MeanMV.error = 0;
		return MeanMV;
	}
	fDisp = 0;

	// 2 previous lines of blocks
	iIndex = iStartY * pMEStruct->m_iMBlocksHor;
	for (i = iStartY; i < iBY; i++)
	{
		for (j = iStartX; j < iEndX; j++)
		{
			register int iMVABS;
			register int iCurIndex = iIndex + j;
			if (pMEStruct->m_CurMBlocks[iCurIndex].m_BlockPartition == cbt_16x16)
			{
				iMVABS = MVABS(&MeanMV, &pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[0]);
				fDisp += iMVABS;
			}
			else
			{
				for (k = 0; k < 4; k++)
				{
					if (pMEStruct->m_CurMBlocks[iCurIndex].m_BlockPartition == cbt_8x8)
					{
						register int iCurIndexMV = k << 2;
						iMVABS = MVABS(&MeanMV, &pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[iCurIndexMV]);
						fDisp += iMVABS;
					}
					else
					{
						for (h = 0; h < 4; h++)
						{
							register int iCurIndexMV = (k << 2) + h;
							iMVABS = MVABS(&MeanMV, &pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[iCurIndexMV]);
							fDisp += iMVABS;
						}
					}
				}
			}
		}
		iIndex += pMEStruct->m_iMBlocksHor;
	}

	// 2 blocks to the left
	iIndex = iBY * pMEStruct->m_iMBlocksHor;
	for (j = iStartX; j < iBX; j++)
	{
		register int iMVABS;
		register int iCurIndex = iIndex + j;
		if (pMEStruct->m_CurMBlocks[iCurIndex].m_BlockPartition == cbt_16x16)
		{
			iMVABS = MVABS(&MeanMV, &pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[0]);
			fDisp += iMVABS;
		}
		else
		{
			for (k = 0; k < 4; k++)
			{
				if (pMEStruct->m_CurMBlocks[iCurIndex].m_BlockPartition == cbt_8x8)
				{
					iMVABS = MVABS(&MeanMV, &pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[k << 2]);
					fDisp += iMVABS;
				}
				else
				{
					for (h = 0; h < 4; h++)
					{
						iMVABS = MVABS(&MeanMV, &pMEStruct->m_CurMBlocks[iCurIndex].m_DataMV[(k << 2) + h]);
						fDisp += iMVABS;
					}
				}
			}
		}
	}

	fDisp /= iCounter;
	fDisp = (float)sqrt(fDisp);
	MeanMV.error = (int)fDisp;

	return MeanMV;
}

/**
**************************************************************************
* \brief This function adds motion vector candidate to the list
*
*  This function adds motion vector candidate to the list of candidates of
*  given block
*
* \param MVCandArr           [IN/OUT] - pointer to the array of motion vector candidate lists
* \param CandMV              [IN] -  candidate motion vector
* \param iBlX                [IN] -  horizontal coordinate of the block
* \param iBlY                [IN] -  vertical coordinate of the block
* \param iBlocksHor          [IN] -  quantity of blocks in a frame line
* 
* \return none
*/
void AddCandidateMV(cMVList* MVCandArr, const cMV* CandMV, int iBlX, int iBlY, int iBlocksHor)
{
	cMVList* pPrvMVList;
	cMVList* pMVList;
	//	assert( -128 <= CandMV->x && CandMV->x <= 128 && -128 <= CandMV->y && CandMV->y <= 128 );
	//if(CandMV->error==-1)
	//	return;

	pMVList = MVCandArr + iBlY*iBlocksHor + iBlX;
	while (pMVList && pMVList->m_bUsed)
	{
		pPrvMVList = pMVList;
		pMVList = pMVList->m_pNext;
	}
	if (!pMVList)
	{
		pMVList = pPrvMVList->m_pNext = malloc(sizeof(cMVList));
		cInitMVListNode(pPrvMVList->m_pNext);
	}
	pMVList->m_bUsed = TRUE;
	pMVList->m_DataMV = *CandMV;
}

/**
**************************************************************************
* \brief Adding motion candidate
*
*  This function adds motion vector to candidate set for every block in
*  next frame.
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* \param x                   [IN] -  horizontal component of motion vector
* \param y                   [IN] -  vertical component of motion vector
* 
* \return none
*/
void cAddMotionCandidate(gmcMotionEstimator* pMEStruct, int x, int y)
{
	cMV Vector;
	int i, j;
	Vector.x = x, Vector.y = y;
	for (i = 0; i<pMEStruct->m_iBlocksVert; i++)
		for (j = 0; j < pMEStruct->m_iBlocksHor; j++)
		{
			AddCandidateMV(pMEStruct->m_MVPrvCand, &Vector, j, i, pMEStruct->m_iBlocksHor);
		}
}

/**
**************************************************************************
* \brief This function clears given motion vectors list
*
*  This function clears given motion vectors list
*
* \param pMVList             [IN/OUT] - pointer to the first element of the list
* 
* \return none
*/
void ClearMVCand(cMVList* pMVList)
{
	while (pMVList)
	{
		pMVList->m_bUsed = FALSE;
		pMVList = pMVList->m_pNext;
	}
}

/**
**************************************************************************
* \brief This function deallocates memory of unused MV candidates
*
*  This function deallocates memory of unused MV candidates for the elements 
*  which number is greater than given value
*
* \param pMVList             [IN/OUT] - pointer to the first element of the list
* \param keepPrealloc        [IN] - Number of elements that is kept in list
* 
* \return none
*/
void RemoveUnusedMVCand(cMVList* pMVList, int keepPrealloc)
{
	cMVList* pPrevMVList = pMVList;
	int listLength;
	if (!pMVList || !pMVList->m_pNext) return;
	
	pMVList = pMVList->m_pNext;
	listLength = 1;
	while (pMVList && (pMVList->m_bUsed || listLength < keepPrealloc))
	{
		pPrevMVList = pMVList;
		pMVList = pMVList->m_pNext;
		++listLength;
	}
	if (!pMVList) return;
	pPrevMVList->m_pNext = NULL;
	while(pMVList)
	{
		pPrevMVList = pMVList;
		pMVList = pMVList->m_pNext;
		free(pPrevMVList);
	}
}

/**
**************************************************************************
* \brief This function adds motion vector candidate to the list
*
*  This function adds motion vector candidate to the list of candidates of
*  given block using upper macroblock as source of candidate motion
*  vectors
*
* \param MVCandArr           [IN/OUT] - pointer to the array of motion vector candidate lists
* \param CandMB              [IN] -  pointer to the upper macroblock
* \param iBlX                [IN] -  horizontal coordinate of the block
* \param iBlY                [IN] -  vertical coordinate of the block
* \param iRefFrame           [IN] -  reference frame index
* \param iMBlocksHor         [IN] -  quantity of macroblocks in a frame line
* 
* \return none
*/
__inline void AddCandidateUp(cMVList* MVCandArr, const cMacroBlock* CandMB, int iBlX, int iBlY, int iRefFrame, int iMBlocksHor)
{
	const cMV* mv=CandMB->m_DataMVs[iRefFrame];
	if (CandMB->m_BlockPartition == cbt_16x16)
	{
		AddCandidateMV(MVCandArr, mv, iBlX, iBlY, iMBlocksHor);
	}
	else if (CandMB->m_BlockPartition == cbt_16x8)
	{
		AddCandidateMV(MVCandArr, mv + 8, iBlX, iBlY, iMBlocksHor);
	}
	else
	{
		if (CandMB->m_SubBlockPartition[2] == cbt_8x8)
		{
			AddCandidateMV(MVCandArr, mv + 8, iBlX, iBlY, iMBlocksHor);
		}
		else
		{
			AddCandidateMV(MVCandArr, mv + 10, iBlX, iBlY, iMBlocksHor);
			AddCandidateMV(MVCandArr, mv + 11, iBlX, iBlY, iMBlocksHor);
		}
		if (CandMB->m_SubBlockPartition[3] == cbt_8x8)
		{
			AddCandidateMV(MVCandArr, mv + 12, iBlX, iBlY, iMBlocksHor);
		}
		else
		{
			AddCandidateMV(MVCandArr, mv + 14, iBlX, iBlY, iMBlocksHor);
			AddCandidateMV(MVCandArr, mv + 15, iBlX, iBlY, iMBlocksHor);
		}
	}
}

/**
**************************************************************************
* \brief This function adds motion vector candidate to the list
*
*  This function adds motion vector candidate to the list of candidates of
*  given block using upper left macroblock as source of candidate motion
*  vectors
*
* \param MVCandArr           [IN/OUT] - pointer to the array of motion vector candidate lists
* \param CandMB              [IN] -  pointer to the upper macroblock
* \param iBlX                [IN] -  horizontal coordinate of the block
* \param iBlY                [IN] -  vertical coordinate of the block
* \param iRefFrame           [IN] -  reference frame index
* \param iMBlocksHor         [IN] -  quantity of macroblocks in a frame line
* 
* \return none
*/
__inline void AddCandidateUpLeft(cMVList* MVCandArr, const cMacroBlock* CandMB, int iBlX, int iBlY, int iRefFrame, int iMBlocksHor)
{
	const cMV* mv=CandMB->m_DataMVs[iRefFrame];
	if (CandMB->m_BlockPartition == cbt_16x16)
	{
		AddCandidateMV(MVCandArr, mv, iBlX, iBlY, iMBlocksHor);
	}
	else if (CandMB->m_BlockPartition == cbt_16x8)
	{
		AddCandidateMV(MVCandArr, mv + 8, iBlX, iBlY, iMBlocksHor);
	}
	else
	{
		if (CandMB->m_SubBlockPartition[3] == cbt_8x8)
		{
			AddCandidateMV(MVCandArr, mv + 12, iBlX, iBlY, iMBlocksHor);
		}
		else
		{
			AddCandidateMV(MVCandArr, mv + 15, iBlX, iBlY, iMBlocksHor);
		}
	}
}

/**
**************************************************************************
* \brief This function adds motion vector candidate to the list
*
*  This function adds motion vector candidate to the list of candidates of
*  given block using upper right macroblock as source of candidate motion
*  vectors
*
* \param MVCandArr           [IN/OUT] - pointer to the array of motion vector candidate lists
* \param CandMB              [IN] -  pointer to the upper macroblock
* \param iBlX                [IN] -  horizontal coordinate of the block
* \param iBlY                [IN] -  vertical coordinate of the block
* \param iRefFrame           [IN] -  reference frame index
* \param iMBlocksHor         [IN] -  quantity of macroblocks in a frame line
* 
* \return none
*/
__inline void AddCandidateUpRight(cMVList* MVCandArr, const cMacroBlock* CandMB, int iBlX, int iBlY, int iRefFrame, int iMBlocksHor)
{
	const cMV* mv=CandMB->m_DataMVs[iRefFrame];
	if (CandMB->m_BlockPartition == cbt_16x16)
	{
		AddCandidateMV(MVCandArr, mv, iBlX, iBlY, iMBlocksHor);
	}
	else if (CandMB->m_BlockPartition == cbt_16x8)
	{
		AddCandidateMV(MVCandArr, mv + 8, iBlX, iBlY, iMBlocksHor);
	}
	else
	{
		if (CandMB->m_SubBlockPartition[2] == cbt_8x8)
		{
			AddCandidateMV(MVCandArr, mv + 8, iBlX, iBlY, iMBlocksHor);
		}
		else
		{
			AddCandidateMV(MVCandArr, mv + 10, iBlX, iBlY, iMBlocksHor);
		}
	}
}

/**
**************************************************************************
* \brief This function adds motion vector candidate to the list
*
*  This function adds motion vector candidate to the list of candidates of
*  given block using left macroblock as source of candidate motion vectors
*
* \param MVCandArr           [IN/OUT] - pointer to the array of motion vector candidate lists
* \param CandMB              [IN] -  pointer to the upper macroblock
* \param iBlX                [IN] -  horizontal coordinate of the block
* \param iBlY                [IN] -  vertical coordinate of the block
* \param iRefFrame           [IN] -  reference frame index
* \param iMBlocksHor         [IN] -  quantity of macroblocks in a frame line
* 
* \return none
*/
__inline void AddCandidateLeft(cMVList* MVCandArr, const cMacroBlock* CandMB, int iBlX, int iBlY, int iRefFrame, int iMBlocksHor)
{
	const cMV* mv=CandMB->m_DataMVs[iRefFrame];
	if (CandMB->m_BlockPartition == cbt_16x16)
	{
		AddCandidateMV(MVCandArr, mv, iBlX, iBlY, iMBlocksHor);
	}
	else if (CandMB->m_BlockPartition == cbt_16x8)
	{
		AddCandidateMV(MVCandArr, mv, iBlX, iBlY, iMBlocksHor);
		AddCandidateMV(MVCandArr, mv + 8, iBlX, iBlY, iMBlocksHor);
	}
	else
	{
		if (CandMB->m_SubBlockPartition[1] == cbt_8x8)
		{
			AddCandidateMV(MVCandArr, mv + 4, iBlX, iBlY, iMBlocksHor);
		}
		else
		{
			AddCandidateMV(MVCandArr, mv + 5, iBlX, iBlY, iMBlocksHor);
			AddCandidateMV(MVCandArr, mv + 7, iBlX, iBlY, iMBlocksHor);
		}
		if (CandMB->m_SubBlockPartition[3] == cbt_8x8)
		{
			AddCandidateMV(MVCandArr, mv + 12, iBlX, iBlY, iMBlocksHor);
		}
		else
		{
			AddCandidateMV(MVCandArr, mv + 13, iBlX, iBlY, iMBlocksHor);
			AddCandidateMV(MVCandArr, mv + 15, iBlX, iBlY, iMBlocksHor);
		}
	}
}

/**
**************************************************************************
* \brief This function adds motion vector candidate to the list
*
*  This function adds motion vector candidate to the list of candidates of
*  given block using macroblock from the previous frame as source of
*  candidate motion vectors
*
* \param MVCandArr           [IN/OUT] - pointer to the array of motion vector candidate lists
* \param CandMB              [IN] -  pointer to the upper macroblock
* \param iBlX                [IN] -  horizontal coordinate of the block
* \param iBlY                [IN] -  vertical coordinate of the block
* \param iRefFrame           [IN] -  reference frame index
* \param iMBlocksHor         [IN] -  quantity of macroblocks in a frame line
* 
* \return none
*/
__inline void AddCandidateTemp(cMVList* MVCandArr, const cMacroBlock* CandMB, int iBlX, int iBlY, int iRefFrame, int iMBlocksHor)
{
	const cMV* mv=CandMB->m_DataMVs[iRefFrame];
	int i, j;
	int iCounter = 0;
	cMV CandMV;
	cInitMV(&CandMV);
	if (CandMB->m_BlockPartition == cbt_16x16)
	{
		AddCandidateMV(MVCandArr, mv, iBlX, iBlY, iMBlocksHor);
	}
	else if (CandMB->m_BlockPartition == cbt_16x8)
	{
		CandMV.x=(mv[0].x+mv[8].x)>>1;
		CandMV.y=(mv[0].y+mv[8].y)>>1;
		CandMV.error=mv[0].error+mv[8].error;
		
		AddCandidateMV(MVCandArr, &CandMV, iBlX, iBlY, iMBlocksHor);
		//AddCandidateMV(MVCandArr, mv, iBlX, iBlY, iMBlocksHor);
		//AddCandidateMV(MVCandArr, mv+8, iBlX, iBlY, iMBlocksHor);
	}
	else
	{
		for (i = 0; i < 4; i++)
		{
			if (CandMB->m_SubBlockPartition[i] == cbt_8x8)
			{
				CandMV.x += mv[i*4].x;
				CandMV.y += mv[i*4].y;
				iCounter++;
			}
			else
			{
				for (j = 0; j < 4; j++)
				{
					CandMV.x += mv[i*4 + j].x;
					CandMV.y += mv[i*4 + j].y;
					iCounter++;
				}
			}
		}
		CandMV.x /= iCounter;
		CandMV.y /= iCounter;
		AddCandidateMV(MVCandArr, &CandMV, iBlX, iBlY, iMBlocksHor);
	}
}

/**
**************************************************************************
* \brief Decides how uniform motion is in a local area of the block
*
*  This function decides how uniform motion is in a local area of the
*  block. It also calculates mean motion vector of the block's area.
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param iBX                 [IN] -  horizontal coordinate of the block
* \param iBY                 [IN] -  vertical coordinate of the block
* \param iRefFrame           [IN] -  reference frame index
* \param field               [IN] -  reference field
* 
* \return Mean motion vector containing uniformity of motion value as vector's error
*/
cMV LocalDecideMotion(gmcMotionEstimator* pMEStruct, int iBY, int iBX, int iRefFrame, int field)
{
	cMV retVal;
	if (iBY > 0)
	{
		AddCandidateUp(pMEStruct->m_MVPrvCand__[iRefFrame][field], &pMEStruct->m_CurMBlocks[(iBY-1)*pMEStruct->m_iMBlocksHor + iBX], iBX, iBY, iRefFrame, pMEStruct->m_iMBlocksHor);
		if (iBX > 0)
			AddCandidateUpLeft(pMEStruct->m_MVPrvCand__[iRefFrame][field], &pMEStruct->m_CurMBlocks[(iBY-1)*pMEStruct->m_iMBlocksHor + iBX-1], iBX, iBY, iRefFrame, pMEStruct->m_iMBlocksHor);
		if (iBX < pMEStruct->m_iMBlocksHor - 1)
			AddCandidateUpRight(pMEStruct->m_MVPrvCand__[iRefFrame][field], &pMEStruct->m_CurMBlocks[(iBY-1)*pMEStruct->m_iMBlocksHor + iBX+1], iBX, iBY, iRefFrame, pMEStruct->m_iMBlocksHor);
	}
	if (iBX > 0)
	{
		AddCandidateLeft(pMEStruct->m_MVPrvCand__[iRefFrame][field], &pMEStruct->m_CurMBlocks[iBY*pMEStruct->m_iMBlocksHor + iBX-1], iBX, iBY, iRefFrame, pMEStruct->m_iMBlocksHor);
	}
	// adding temporal candidates
	if (iBY < pMEStruct->m_iMBlocksVert/*-2*/ && pMEStruct->m_bExistPrevMV)
		AddCandidateTemp(pMEStruct->m_MVPrvCand__[iRefFrame][field], &pMEStruct->m_RefMBlocks__[iRefFrame][field][(iBY/*+2*/)*pMEStruct->m_iMBlocksHor + iBX], iBX, iBY, iRefFrame, pMEStruct->m_iMBlocksHor);
	
	if (iBY < pMEStruct->m_iMBlocksVert-1 && pMEStruct->m_bExistPrevMV)
		AddCandidateTemp(pMEStruct->m_MVPrvCand__[iRefFrame][field], &pMEStruct->m_RefMBlocks__[iRefFrame][field][(iBY+1)*pMEStruct->m_iMBlocksHor + iBX], iBX, iBY, iRefFrame, pMEStruct->m_iMBlocksHor);

	retVal = CalculateMVDisp(pMEStruct->m_MVPrvCand__[iRefFrame][field] + iBY*pMEStruct->m_iMBlocksHor + iBX);

	return retVal;
}

/*! \def CHECKMV(mvy,mvx)
*   \brief Check if the given vector has lower error then the best current one
*
*  This is a macro that checks if the current vector (\a mvy, \a mvx) has
*  lower error then the current best one. If it has then this vector is
*  stored as the best current one
*/
#define CHECKMV(mvy,mvx)																														\
	iError = MainDiffFunction(pMEStruct, iX, iY, iCurMVX+(mvx), iCurMVY+(mvy), Minfo, BlockSize);												\
	if (iError<CurrentVector->error)																											\
	{																																			\
		bMVChanged = TRUE;																														\
		CurrentVector->error = iError;																											\
		CurrentVector->x = iCurMVX+(mvx);																										\
		CurrentVector->y = iCurMVY+(mvy);																										\
	}																																			\

/**
**************************************************************************
* \brief Applies SDP search around given center
*
*  This function applies refining search of motion vector using small
*  diamond pattern. Start point is current best motion vector. Quantity
*  of competent steps is limited.
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param iX                  [IN] -  horizontal coordinate of the current block
* \param iY                  [IN] -  vertical coordinate of the current block
* \param CurrentVector       [IN/OUT] - current best motion vector
* \param iRange              [IN] -  number of competent steps
* \param Minfo               [IN/OUT] - pointer to the structure with motion information about current block
* \param BlockSize           [IN] -  size of the current block
* 
* \return TRUE if the best motion vector was found in the middle of the pattern within competent steps, FALSE otherwise
*/
BOOL LocalSDP(gmcMotionEstimator* pMEStruct, int iX, int iY, cMV* CurrentVector, int iRange, MInfoStruct* Minfo, cMacroBlockPartitions BlockSize)
{
	int iStep;
	BOOL bMVChanged;
	int iError, iCurMVX, iCurMVY;
	int iStepX = 1<<pMEStruct->m_PrecMX, iStepY = 1<<pMEStruct->m_PrecMY;

	for (iStep = 0; iStep<iRange; iStep++)
	{
		iCurMVX = CurrentVector->x;
		iCurMVY = CurrentVector->y;
		bMVChanged= FALSE;

		CHECKMV(iStepY,0);
		CHECKMV(-iStepY,0);
		CHECKMV(0,-iStepX);
		CHECKMV(0,iStepX);
		
		if (!bMVChanged)
			return TRUE;
	}
	return FALSE;
}

/**
**************************************************************************
* \brief Applies LDP-EDP switching search around given center
*
*  This function applies refining search of motion vector switching
*  between large diamond pattern and elastic diamond pattern. Start point
*  is current best motion vector. Quantity of competent steps is limited.
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param iX                  [IN] -  horizontal coordinate of the current block
* \param iY                  [IN] -  vertical coordinate of the current block
* \param CurrentVector       [IN/OUT] - current best motion vector
* \param iRange              [IN] -  number of competent steps
* \param Minfo               [IN/OUT] - pointer to the structure with motion information about current block
* \param BlockSize           [IN] -  size of the current block
* 
* \return TRUE if the best motion vector was found in the middle of the pattern within competent steps, FALSE otherwise
*/
BOOL LocalLDPEDP(gmcMotionEstimator* pMEStruct, int iX, int iY, cMV* CurrentVector, int iRange, MInfoStruct* Minfo, cMacroBlockPartitions BlockSize)
{
	BOOL bMVChanged;
	int iError, iCurMVX, iCurMVY;
	int iStepXEDP = 1<<pMEStruct->m_PrecMX, iStepYEDP = 1<<pMEStruct->m_PrecMY;
	int iStepXLDP = 1<<(pMEStruct->m_PrecMX+1), iStepYLDP = 1<<(pMEStruct->m_PrecMY+1);
	int iStep;

	for (iStep = 0; iStep<iRange; iStep++)
	{
		iCurMVX = CurrentVector->x;
		iCurMVY = CurrentVector->y;

		CHECKMV(iStepYEDP,iStepXEDP);
		CHECKMV(-iStepYEDP,iStepXEDP);
		CHECKMV(iStepYEDP,-iStepXEDP);
		CHECKMV(-iStepYEDP,-iStepXEDP);

		iCurMVX = CurrentVector->x;
		iCurMVY = CurrentVector->y;
		bMVChanged= FALSE;

		CHECKMV(iStepYLDP,0);
		CHECKMV(-iStepYLDP,0);
		CHECKMV(0,-iStepXLDP);
		CHECKMV(0,iStepXLDP);

		if (!bMVChanged) return TRUE;
	}
	return FALSE;
}

/**
**************************************************************************
* \brief Applies sub-pixel vector refinement
*
*  This function applies sub-pixel vector refinement using 4-point diamond
*  pattern. Start point is current best motion vector. Quantity of
*  competent steps is limited.
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param iX                  [IN] -  horizontal coordinate of the current block
* \param iY                  [IN] -  vertical coordinate of the current block
* \param CurrentVector       [IN/OUT] - current best motion vector
* \param Minfo               [IN/OUT] - pointer to the structure with motion information about current block
* \param BlockSize           [IN] -  size of the current block
* 
* \return none
*/
void LocalSubPelRefine(gmcMotionEstimator* pMEStruct, int iX, int iY, cMV* CurrentVector, MInfoStruct* Minfo, cMacroBlockPartitions BlockSize)
{
	BOOL bMVChanged = TRUE;
	int iError, iCurMVX, iCurMVY;
	int iStepX = pMEStruct->m_PrecMX, iStepY = pMEStruct->m_PrecMY;

	while (iStepX || iStepY)
	{
		iCurMVX = CurrentVector->x;
		iCurMVY = CurrentVector->y;
		CHECKMV(iStepY,0);
		CHECKMV(-iStepY,0);
		CHECKMV(0,-iStepX);
		CHECKMV(0,iStepX);
		iStepX >>= 1;iStepY >>= 1;
	}
}

#undef CHECKMV

/**
**************************************************************************
* \brief Searches for motion vector for given block using proposed algorithm
*
*  This function searches for motion vector for given block using proposed
*  motion estimation algorithm.
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param iY                  [IN] -  vertical coordinate of the current block
* \param iX                  [IN] -  horizontal coordinate of the current block
* \param iBY                 [IN] -  vertical coordinate of the block
* \param iBX                 [IN] -  horizontal coordinate of the block
* \param BlockSize           [IN] -  size of the current block
* \param CurrentVector       [IN] -  current best motion vector
* 
* \return found motion vector
*/
cMV LocalNewMSU_MB(gmcMotionEstimator* pMEStruct, int iY, int iX, int iBY, int iBX, cMacroBlockPartitions BlockSize, cMV CurrentVector)
{
	int iCurOff;
	int iCurChr1Off, iCurChr2Off;
	int iRange;
		
	const int iSkipPels = 2;
	const int iAddY = (1<<pMEStruct->m_PrecMY)*iSkipPels;
	const int iAddX = (1<<pMEStruct->m_PrecMX)*iSkipPels;

	int flag;

	BOOL bLaunchLDP = FALSE;

	pMEStruct->m_MInfo.m_iBlX = iBX;
	pMEStruct->m_MInfo.m_iBlY = iBY;

	NEW_BLOCK_ID

	iCurOff = iY*pMEStruct->m_iExtWidth+iX + pMEStruct->m_iFirstRowOffset;

	GetBlock(pMEStruct->m_CurBlock, MBwidth[BlockSize], MBheight[BlockSize], MBwidth[BlockSize], pMEStruct->CurFrame+iCurOff, pMEStruct->m_iExtWidth<<pMEStruct->m_MInfo.m_bFieldInFrame, 0, 0, 0, 0);
	if (pMEStruct->m_bConsiderChroma)
	{
		iCurChr1Off = iY/pMEStruct->m_iChrYMod * pMEStruct->m_iExtWidth/pMEStruct->m_iChrXMod + iX/pMEStruct->m_iChrXMod + pMEStruct->m_iSecCompOff;
		pMEStruct->m_MInfo.m_pRefBlockChr1 = pMEStruct->RefFrame+iCurChr1Off;
		iCurChr2Off = iY/pMEStruct->m_iChrYMod * pMEStruct->m_iExtWidth/pMEStruct->m_iChrXMod + iX/pMEStruct->m_iChrXMod + pMEStruct->m_iThdCompOff;
		pMEStruct->m_MInfo.m_pRefBlockChr2 = pMEStruct->RefFrame+iCurChr2Off;
		GetBlock( pMEStruct->m_CurBlChr1, MBwidth[BlockSize]/pMEStruct->m_iChrXMod, MBheight[BlockSize]/pMEStruct->m_iChrYMod,
                  MBwidth[BlockSize]/pMEStruct->m_iChrXMod, pMEStruct->CurFrame+iCurChr1Off, pMEStruct->m_iExtWidth/pMEStruct->m_iChrXMod, 0, 0, 0, 0 );
		GetBlock( pMEStruct->m_CurBlChr2, MBwidth[BlockSize]/pMEStruct->m_iChrXMod, MBheight[BlockSize]/pMEStruct->m_iChrYMod,
                  MBwidth[BlockSize]/pMEStruct->m_iChrXMod, pMEStruct->CurFrame+iCurChr2Off, pMEStruct->m_iExtWidth/pMEStruct->m_iChrXMod, 0, 0, 0, 0 );
	}

	flag = FALSE;
	if ( CurrentVector.error <= (((1<<pMEStruct->m_PrecMX) + (1<<pMEStruct->m_PrecMY))/2) )
	{
		if(pMEStruct->m_MInfo.m_bFieldInFrame)
			CurrentVector.y>>=1;

		CurrentVector.error = MainDiffFunction(pMEStruct, iX, iY, CurrentVector.x, CurrentVector.y, &pMEStruct->m_MInfo, BlockSize);
		iRange = 2;

		flag = CurrentVector.error != INT_MAX;
	}
    if(!flag)
    {
        //look through filtered motion vector list to find the best candidate
        if ( CurrentVector.error <= (((4<<pMEStruct->m_PrecMX) + (4<<pMEStruct->m_PrecMY))/2) ) iRange = 4;
        else
        {
            bLaunchLDP = TRUE;
            iRange = 1;
        }

        FilterMVList( pMEStruct->m_MVPrvCand + iBY*pMEStruct->m_iMBlocksHor + iBX, iRange );
		if (pMEStruct->m_CandidatesMemoryAlloc == cmaOnFly)
		{
			// free memory of unused candidates if their number is more than given value
			// only for onfly MV candidates memory management mode
			RemoveUnusedMVCand(pMEStruct->m_MVPrvCand + iBY*pMEStruct->m_iMBlocksHor + iBX, pMEStruct->m_CandKeepPrealloc);
		}
        CurrentVector = CheckMVList( pMEStruct, pMEStruct->m_MVPrvCand + iBY*pMEStruct->m_iMBlocksHor + iBX, iX, iY, &pMEStruct->m_MInfo, BlockSize );
        if ( CurrentVector.error == INT_MAX )
        {
            CurrentVector.x = CurrentVector.y = 0;
        }
    }

    if (!LocalSDP(pMEStruct, iX, iY, &CurrentVector, iRange, &pMEStruct->m_MInfo, BlockSize) && bLaunchLDP)
    {
        LocalLDPEDP(pMEStruct, iX, iY, &CurrentVector, 1/*pMEStruct->m_iMaxMotHor + pMEStruct->m_iMaxMotVert*/, &pMEStruct->m_MInfo, BlockSize);
        LocalSDP(pMEStruct, iX, iY, &CurrentVector, 1, &pMEStruct->m_MInfo, BlockSize);
    }
    LocalSubPelRefine(pMEStruct, iX, iY, &CurrentVector, &pMEStruct->m_MInfo, BlockSize);

	return CurrentVector;
}


/**
**************************************************************************
* \brief Searches for motion vector for 4 blocks 8x8
*
*  This function searches for motion vectors for given macroblock using
*  fragmentation into 4 blocks 8x8.
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param CurrentVector_8x8   [OUT] - pointer to the array to store MVs
* \param iX                  [IN] -  horizontal coordinate of the current block
* \param iY                  [IN] -  vertical coordinate of the current block
* \param iBX                 [IN] -  horizontal coordinate of the block
* \param iBY                 [IN] -  vertical coordinate of the block
* \param MeanVector          [IN] -  mean vector of local area motion
* 
* \return sum of errors of 4 found vectors
*/
int Try_8x8(gmcMotionEstimator* pMEStruct, cMV* CurrentVector_8x8, int iX, int iY, int iBX, int iBY, cMV MeanVector)
{
	int iError = 0;
	int i;
	for (i = 0; i < 4; i++)
	{
		CurrentVector_8x8[i] = LocalNewMSU_MB(pMEStruct, iY + ((i&2)>>1)*cbt_8x8, iX + (i&1)*cbt_8x8, iBY, iBX, cbt_8x8, MeanVector);
		iError += CurrentVector_8x8[i].error;
	}
	return iError;
}

/**
**************************************************************************
* \brief Searches for motion vector for 4 blocks 4x4
*
*  This function searches for motion vectors for given 8x8 block into
*  macroblock using fragmentation into 4 blocks 4x4.
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param CurrentVector_4x4   [OUT] - pointer to the array to store MVs
* \param iX                  [IN] -  horizontal coordinate of the current block
* \param iY                  [IN] -  vertical coordinate of the current block
* \param iBX                 [IN] -  horizontal coordinate of the block
* \param iBY                 [IN] -  vertical coordinate of the block
* \param MeanVector          [IN] -  mean vector of local area motion
* 
* \return sum of errors of 4 found vectors
*/
int Try_4x4(gmcMotionEstimator* pMEStruct, cMV* CurrentVector_4x4, int iX, int iY, int iBX, int iBY, cMV MeanVector)
{
	int iError = 0;
	int i;
	for (i = 0; i < 4; i++)
	{
		CurrentVector_4x4[i] = LocalNewMSU_MB(pMEStruct, iY + ((i&2)>>1)*cbt_4x4, iX + (i&1)*cbt_4x4, iBY, iBX, cbt_4x4, MeanVector);
		iError += CurrentVector_4x4[i].error;
	}
	return iError;
}

/**
**************************************************************************
* \brief Stores debug information
*
*  This function stores debug information about level of errors into
*  corresponding map
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param iBlY                [IN] -  vertical coordinate of the block
* \param iBlX                [IN] -  horizontal coordinate of the block
* \param iBeginX             [IN] -  left border of area to store info
* \param iEndX               [IN] -  right border of area to store info
* \param iBeginY             [IN] -  down border of area to store info
* \param iEndY               [IN] -  up border of area to store info
* 
* \return none
*/
void SetHierLaunch(gmcMotionEstimator* pMEStruct, int iBlX, int iBlY, int iBeginX, int iEndX, int iBeginY, int iEndY)
{
	int g, h;
	for (g = iBeginY; g < iEndY; g++)
	{
		for (h = iBeginX; h < iEndX; h++)
		{
			pMEStruct->m_pHierLaunchMap[(iBlY * 4 + g)* pMEStruct->m_iMBlocksHor*4 + iBlX * 4 + h] = 1;
		}
	}
}

/**
**************************************************************************
* \brief This function adds motion vector candidate to the list of next frame
*
*  This function adds found motion vector in current frame in the list of
*  candidates of the next frame.
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* \param CandMV              [IN] -  candidate motion vector
* \param iX                  [IN] -  horizontal coordinate of the block
* \param iY                  [IN] -  vertical coordinate of the block
* \param iRefFrame           [IN] -  reference frame index
* \param field               [IN] -  reference field
* 
* \return none
*/
void DecideNextFrameCandidate(gmcMotionEstimator* pMEStruct, const cMV* CandMV, int iX, int iY, int iRefFrame, int field)
{
#define BLOCK_IS_OK(BX, BY) \
	(0 <= (BX) && (BX) < pMEStruct->m_iMBlocksHor && 0 <= (BY) && (BY) < pMEStruct->m_iMBlocksVert)

	int iFrX = iX + ((-CandMV->x) >> pMEStruct->m_PrecMX);
	int iFrY = iY + ((-CandMV->y) >> pMEStruct->m_PrecMY);
	int iDstBlX = iFrX / pMEStruct->m_iBlockSize;
	int iDstBlY = iFrY / pMEStruct->m_iBlockSize;

	cMVList* pList=pMEStruct->m_MVNxtCand__[iRefFrame][field];

	if (BLOCK_IS_OK(iDstBlX-1, iDstBlY-1)) AddCandidateMV(pList, CandMV, iDstBlX-1, iDstBlY-1, pMEStruct->m_iMBlocksHor);
	if (BLOCK_IS_OK(iDstBlX,   iDstBlY-1)) AddCandidateMV(pList, CandMV, iDstBlX,   iDstBlY-1, pMEStruct->m_iMBlocksHor);
	if (BLOCK_IS_OK(iDstBlX+1, iDstBlY-1)) AddCandidateMV(pList, CandMV, iDstBlX+1, iDstBlY-1, pMEStruct->m_iMBlocksHor);

	if (BLOCK_IS_OK(iDstBlX-1, iDstBlY  )) AddCandidateMV(pList, CandMV, iDstBlX-1, iDstBlY  , pMEStruct->m_iMBlocksHor);
	if (BLOCK_IS_OK(iDstBlX,   iDstBlY  )) AddCandidateMV(pList, CandMV, iDstBlX,   iDstBlY  , pMEStruct->m_iMBlocksHor);
	if (BLOCK_IS_OK(iDstBlX+1, iDstBlY  )) AddCandidateMV(pList, CandMV, iDstBlX+1, iDstBlY  , pMEStruct->m_iMBlocksHor);

	if (BLOCK_IS_OK(iDstBlX-1, iDstBlY+1)) AddCandidateMV(pList, CandMV, iDstBlX-1, iDstBlY+1, pMEStruct->m_iMBlocksHor);
	if (BLOCK_IS_OK(iDstBlX,   iDstBlY+1)) AddCandidateMV(pList, CandMV, iDstBlX,   iDstBlY+1, pMEStruct->m_iMBlocksHor);
	if (BLOCK_IS_OK(iDstBlX+1, iDstBlY+1)) AddCandidateMV(pList, CandMV, iDstBlX+1, iDstBlY+1, pMEStruct->m_iMBlocksHor);

#undef BLOCK_IS_OK
}

void GetBlockVarMean(gmcMotionEstimator* pMEStruct,BYTE* frame,int iX,int iY,int iWidth,int iHeight,int iStride,float* pVar, float* pMean)
{
	int i,j;
	float var=0,mean=0;
	
	BYTE* ptr=frame+pMEStruct->m_iFirstRowOffset+iY*pMEStruct->m_iExtWidth+iX;
	for(i=0;i<iHeight;i++)
	{
		for(j=0;j<iWidth;j++)
		{
			mean+=*ptr;
			ptr++;
		}
		ptr+=iStride-iWidth;
	}
	mean/=(iWidth*iHeight);

	ptr=frame+pMEStruct->m_iFirstRowOffset+iY*pMEStruct->m_iExtWidth+iX;
	for(i=0;i<iHeight;i++)
	{
		for(j=0;j<iWidth;j++)
		{
			var+=sqr(*ptr-mean);
			ptr++;
		}
		ptr+=iStride-iWidth;
	}
	var=(float)sqrt(var/(iWidth*iHeight));

	*pVar=var;
	*pMean=mean;
}

int GetNeighbContrast(gmcMotionEstimator* pMEStruct,BYTE* CurFrame,int iX,int iY,cMacroBlockPartitions BlockSize)
{
	int i,j, iError=0;
	BYTE block1[256], block2[256];
	GetBlock(block1, MBwidth[BlockSize], MBheight[BlockSize], MBwidth[BlockSize], CurFrame+pMEStruct->m_iFirstRowOffset+iY*pMEStruct->m_iExtWidth+iX, pMEStruct->m_iExtWidth, 0, 0, 0, 0);
	
	for(i=-2;i<=2;i+=2)
	{
		for(j=-2;j<=2;j++)
		{
			if((i && j) || (!i && !j))
				continue;
	
			GetBlock(block2, MBwidth[BlockSize], MBheight[BlockSize], MBwidth[BlockSize], CurFrame+pMEStruct->m_iFirstRowOffset+(iY+i)*pMEStruct->m_iExtWidth+(iX+j), pMEStruct->m_iExtWidth, 0, 0, 0, 0);
            switch(BlockSize)
            {
            case cbt_16x16:
                iError += pMEStruct->pGetError_16x16(block1, block2, MBwidth[BlockSize], &pMEStruct->m_MInfo);
                break;
            case cbt_8x8:
                iError += pMEStruct->pGetError_8x8(block1, block2, MBwidth[BlockSize], &pMEStruct->m_MInfo);
                break;
            case cbt_4x4:
                iError += pMEStruct->pGetError_4x4(block1, block2, MBwidth[BlockSize], &pMEStruct->m_MInfo);
                break;
            }  
        }
	}
	iError>>=2;

	return iError;
	//return GetBlockVar(pMEStruct,CurFrame,iX,iY,BlockSize);
}
	
void SwapAndCleanCands(gmcMotionEstimator* pMEStruct,int iRefFrame)
{
	cMVList* pMVListTemp;
	int i;
	
	pMVListTemp = pMEStruct->m_MVNxtCand__[iRefFrame][FT_TOP];
	pMEStruct->m_MVNxtCand__[iRefFrame][FT_TOP] = pMEStruct->m_MVPrvCand__[iRefFrame][FT_TOP];
	pMEStruct->m_MVPrvCand__[iRefFrame][FT_TOP] = pMVListTemp;

	pMVListTemp = pMEStruct->m_MVNxtCand__[iRefFrame][FT_BOTTOM];
	pMEStruct->m_MVNxtCand__[iRefFrame][FT_BOTTOM] = pMEStruct->m_MVPrvCand__[iRefFrame][FT_BOTTOM];
	pMEStruct->m_MVPrvCand__[iRefFrame][FT_BOTTOM] = pMVListTemp;

	for (i = 0; i<pMEStruct->m_iMBlocksInFrame; i++)
	{
		ClearMVCand(pMEStruct->m_MVNxtCand__[iRefFrame][FT_TOP] + i);
		ClearMVCand(pMEStruct->m_MVNxtCand__[iRefFrame][FT_BOTTOM] + i);
	}
}

/**
**************************************************************************
* \brief Adds temporal motion candidates from previous frame motion field
*
*  This function adds motion candidates from previous motion field
*  according to the motion vectors: their length and direction
*
* \param pMEStruct           [IN/OUT] -  pointer to gmcMotionEstimator structure
* \param iRefFrame           [IN] -  reference frame index
* \param field               [IN] -  reference field
* 
* \return none
*/
void AddCandidatesFromPrvFrame(gmcMotionEstimator* pMEStruct,int iRefFrame, int field)
{
	int iBlX, iBlY, iX, iY;
	int i;

	const float fCandSADThreshold = pMEStruct->m_fMeanErrorPP + 0.7f * pMEStruct->m_fDispersionPP;
	cMacroBlock* pMB;

	for (iBlY = 0,iY=0; iBlY < pMEStruct->m_iMBlocksVert; iBlY++,iY += ME_MACROBLOCK_SIZE)
	{
		for (iBlX = 0,iX=0; iBlX < pMEStruct->m_iMBlocksHor; iBlX++,iX += ME_MACROBLOCK_SIZE)
		{
			pMB=&pMEStruct->m_RefMBlocks__[iRefFrame][field][iBlY * pMEStruct->m_iMBlocksHor + iBlX];
			
			if (pMB->m_BlockPartition == cbt_16x16)
			{
				if (pMB->m_DataMVs[iRefFrame]->error < fCandSADThreshold && pMB->m_DataMVs[iRefFrame]->error != -1)
					DecideNextFrameCandidate(pMEStruct, pMB->m_DataMVs[iRefFrame], iX, iY,iRefFrame,field);
			}
			else if (pMB->m_BlockPartition == cbt_16x8)
			{
				if (pMB->m_DataMVs[iRefFrame]->error < fCandSADThreshold && pMB->m_DataMVs[iRefFrame]->error != -1)
					DecideNextFrameCandidate(pMEStruct, pMB->m_DataMVs[iRefFrame], iX, iY,iRefFrame,field);
				
				if (pMB->m_DataMVs[iRefFrame][8].error < fCandSADThreshold && pMB->m_DataMVs[iRefFrame][8].error != -1)
					DecideNextFrameCandidate(pMEStruct, &pMB->m_DataMVs[iRefFrame][8], iX, iY,iRefFrame,field);
			}
			else if (pMB->m_BlockPartition == cbt_8x8)
			{
				for (i = 0; i < 4; i++)
				{
					if (pMB->m_SubBlockPartition[i] == cbt_8x8)
					{
						if ((pMB->m_DataMVs[iRefFrame][4*i].error << 2) < fCandSADThreshold && pMB->m_DataMVs[iRefFrame][4*i].error != -1)
							DecideNextFrameCandidate(pMEStruct, &pMB->m_DataMVs[iRefFrame][4*i], iX, iY,iRefFrame,field);
					}
					else
					{
						int j;
						for (j = 0; j < 4; j++)
						{
							if ((pMB->m_DataMVs[iRefFrame][4*i + j].error << 4) < fCandSADThreshold && pMB->m_DataMVs[iRefFrame][4*i + j].error != -1)
								DecideNextFrameCandidate(pMEStruct, &pMB->m_DataMVs[iRefFrame][4*i + j], iX, iY,iRefFrame,field);
						}
					}
				}
			}
		}
	}
}

/**
**************************************************************************
* \brief Adds temporal motion candidates from previous frame motion field
*
*  This function adds motion candidates from previous motion field
*  according to the motion vectors: their length and direction
*
* \param pMEStruct           [IN/OUT] -  pointer to gmcMotionEstimator structure
* \param iRefFrame           [IN] -  reference frame index
* \param field               [IN] -  reference field
* 
* \return none
*/
// void AddCandidatesFromPrvFrame2(gmcMotionEstimator* pMEStruct,int iRefFrame, int field)
// {
//     int iBlX, iBlY, iX, iY;
//     const int iBlockRangeX = pMEStruct->m_iMaxMotHor  >> ME_MACROBLOCK_SIZE_POW;
//     const int iBlockRangeY = pMEStruct->m_iMaxMotVert >> ME_MACROBLOCK_SIZE_POW;
// 
//     for (iBlY = 0, iY = 0; iBlY < pMEStruct->m_iMBlocksVert; iBlY++, iY += ME_MACROBLOCK_SIZE)
//     {
//         for (iBlX = 0, iX = 0; iBlX < pMEStruct->m_iMBlocksHor; iBlX++, iX += ME_MACROBLOCK_SIZE)
//         {
//             int iHorBlOff, iVerBlOff;
//             const int iBegHorOff = iBlX - iBlockRangeX < 0 ? 0 : iBlX - iBlockRangeX;
//             const int iEndHorOff = iBlX + iBlockRangeX < pMEStruct->m_iBlocksHor ? iBlX + iBlockRangeX + 1 : pMEStruct->m_iBlocksHor;
//             const int iBegVerOff = iBlY - iBlockRangeY < 0 ? 0 : iBlY - iBlockRangeY;
//             const int iEndVerOff = iBlY + iBlockRangeY < pMEStruct->m_iBlocksHor ? iBlY + iBlockRangeY + 1 : pMEStruct->m_iBlocksHor;
//             cMV bestMV;
//             cInitMV( bestMV );
//             for (iVerBlOff = iBegVerOff; iVerBlOff < iEndVerOff; iVerBlOff++)
//                 for (iHorBlOff = iHorBlOff; iHorBlOff < iEndHorOff; iHorBlOff++)
//                 {
//                 }
//         }
//     }
// }

/**
**************************************************************************
* \brief Decides what macroblock fragmentation to use
*
*  This function decides what macroblock fragmentation we should use for
*  given macroblock
*
* \param pMEStruct           [IN/OUT] -  pointer to gmcMotionEstimator structure
* \param CurrentVector_16x16 [IN] -  motion vector for 16x16 fragmentation
* \param CurrentVector_8x8   [IN] -  array of motion vectors for 8x8 fragmentation
* \param iError_8x8          [IN] -  sum of error of 8x8 vectors
* \param CurrentVector_4x4   [IN] -  array of motion vectors for 4x4 fragmentation
* \param iError_4x4          [IN] -  array of sum of errors of 4x4 vectors
* \param CurrentMB           [OUT] - pointer to the current macroblock
* \param iBlX                [IN] -  horizontal coordinate of the block
* \param iBlY                [IN] -  vertical coordinate of the block
* 
* \return none
*/
void DecideMotion(gmcMotionEstimator* pMEStruct, cMV CurrentVector_16x16, cMV* CurrentVector_8x8, int iError_8x8,
                  cMV* CurrentVector_4x4, int* iError_4x4, cMacroBlock* CurrentMB, int iBlX, int iBlY)
{
	int i, j;
	
	if ( pMEStruct->m_bAccept_16x16 && (CurrentVector_16x16.error <= iError_8x8 || iError_8x8 == -1) )
	{
		CurrentMB->m_BlockPartition = cbt_16x16;
		CurrentMB->m_DataMV[0] = CurrentVector_16x16;
	}
	else if ( pMEStruct->m_bAccept_8x8 || pMEStruct->m_bAccept_4x4 )
	{
		CurrentMB->m_BlockPartition = cbt_8x8;
		for (i = 0; i < 4; i++)
		{
			if ( pMEStruct->m_bAccept_8x8 && (CurrentVector_8x8[i].error <= iError_4x4[i] || iError_4x4[i] == -1) )
			{
				CurrentMB->m_SubBlockPartition[i] = cbt_8x8;
				CurrentMB->m_DataMV[4*i] = CurrentVector_8x8[i];
			}
			else if (pMEStruct->m_bAccept_4x4)
			{
				CurrentMB->m_SubBlockPartition[i] = cbt_4x4;
				for (j = 0; j < 4; j++)
				{
					CurrentMB->m_DataMV[4*i + j] = CurrentVector_4x4[4*i + j];
				}
			}
			else
			{// we should not get in here!!!
				assert(FALSE);
			}
		}
	}
	else
	{// we should not get in here!!!
		assert(FALSE);
	}
}

/**
**************************************************************************
* \brief Applies motion estimation using proposed algorithm
*
*  This function estimates motion using proposed algorithm
*
* \param pMEStruct           [IN/OUT] -  pointer to gmcMotionEstimator structure
* 
* \return none
*/
void EstimateNewMSU_MB(gmcMotionEstimator* pMEStruct)
{
	int i;
	int iX, iY;
	int iBX, iBY;
	cMV CurrentVector_16x16;
	cMV CurrentVector_8x8[4];
	int iError_8x8 = -1;
	cMV CurrentVector_4x4[16];
	int iError_4x4[4];
	cMV MeanVector;
	//BOOL bNeedSplit;
	BOOL bGoodTry_8x8;

	const int iSkipPels = 2;
	const int iAddY = (1<<pMEStruct->m_PrecMY)*iSkipPels;
	const int iAddX = (1<<pMEStruct->m_PrecMX)*iSkipPels;

	const float fSplitTresh_16x16 = pMEStruct->m_fMeanErrorPP + 0.2f * pMEStruct->m_fDispersionPP;
	const float fSplitTresh_8x8 = fSplitTresh_16x16 / 4;

	if (pMEStruct->m_bExistPrevMV) AddCandidatesFromPrvFrame(pMEStruct,0,FT_TOP);

    LauncheNumber = 0;
    DepthSum = 0;
	SwapAndCleanCands(pMEStruct,0);
	pMEStruct->m_MVPrvCand=pMEStruct->m_MVPrvCand__[0][FT_TOP];

	for (iY = 0, iBY = 0; iY < pMEStruct->m_iHeight; iY += ME_MACROBLOCK_SIZE, iBY++)
	{
		for (iX = 0, iBX = 0; iX < pMEStruct->m_iWidth; iX += ME_MACROBLOCK_SIZE, iBX++)
		{
			CurrentVector_16x16.error = -1;
			iError_8x8 = -1;
			iError_4x4[0] = iError_4x4[1] = iError_4x4[2] = iError_4x4[3] = -1;
			bGoodTry_8x8 = FALSE;
			//			GetContrastVert(CurFrame);
			//			GetContrastHor();
			//			bNeedSplit = FALSE;
			MeanVector = LocalDecideMotion(pMEStruct, iBY, iBX, 0, FT_TOP);
			//bNeedSplit = MeanVector.error > ((4<<pMEStruct->m_PrecMX) + (4<<pMEStruct->m_PrecMY))/2;
			if (pMEStruct->m_bAccept_16x16) CurrentVector_16x16 = LocalNewMSU_MB(pMEStruct, iY, iX, iBY, iBX, cbt_16x16, MeanVector);

			if ( (CurrentVector_16x16.error > fSplitTresh_16x16/* || bNeedSplit*/ || !pMEStruct->m_bAccept_16x16) && pMEStruct->m_bAccept_8x8 )
			{
				if (pMEStruct->m_bAccept_16x16) AddCandidateMV(pMEStruct->m_MVPrvCand, &CurrentVector_16x16, iBX, iBY, pMEStruct->m_iMBlocksHor);
				iError_8x8 = Try_8x8(pMEStruct, CurrentVector_8x8, iX, iY, iBX, iBY, MeanVector);
				bGoodTry_8x8 = iError_8x8 < CurrentVector_16x16.error || !pMEStruct->m_bAccept_16x16;
			}
			if ( ((/*bNeedSplit && */bGoodTry_8x8) || (!pMEStruct->m_bAccept_16x16 && !pMEStruct->m_bAccept_8x8)) && pMEStruct->m_bAccept_4x4)
			{
				if (pMEStruct->m_bAccept_8x8)
				{
					ClearMVCand(pMEStruct->m_MVPrvCand + iBY*pMEStruct->m_iMBlocksHor + iBX);
					for (i = 0; i < 4; i++)
					{
						AddCandidateMV(pMEStruct->m_MVPrvCand, &CurrentVector_8x8[i], iBX, iBY, pMEStruct->m_iMBlocksHor);
					}
				}
				for (i = 0; i < 4; i++)
				{
					if ((CurrentVector_8x8[i].error > fSplitTresh_8x8/* && bNeedSplit*/) || iError_8x8 == -1)
					{
						iError_4x4[i] = Try_4x4(pMEStruct, CurrentVector_4x4 + i*4,
							iX + (i&1)*cbt_8x8, iY + ((i&2)>>1)*cbt_8x8, iBX, iBY, MeanVector);
					}
				}
			}
			DecideMotion(pMEStruct, CurrentVector_16x16, CurrentVector_8x8, iError_8x8, CurrentVector_4x4, iError_4x4,
				           pMEStruct->m_CurMBlocks + iBY*pMEStruct->m_iMBlocksHor + iBX, iBX, iBY);
		}
	}
	pMEStruct->m_bExistPrevMV = TRUE;
//    DepthSum = DepthSum / LauncheNumber;
}

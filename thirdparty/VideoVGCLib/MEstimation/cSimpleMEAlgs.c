/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cSimpleMEAlgs.c
*  \brief File with implimentation of simple ME algorithms
*
*  This file contains implementation of simple motion estimation
*  algorithms in video, such as full search, 3DRS, E3DRS, hierarchic
*  search, logarithmic search and so on.
*
*  $Id: cSimpleMEAlgs.c,v 1.15 2007/06/28 08:22:46 05s_gri Exp $
*  $Log: cSimpleMEAlgs.c,v $
*  Revision 1.15  2007/06/28 08:22:46  05s_gri
*  og
*  @CVS log removed
*  @
*  text
*  @d18 34
*  @
*
*
*
*  Revision 1.14  2007/06/05 12:29:16  05s_gri
*  Intel compiler v9.0 warnings removed
*
*  Revision 1.13  2007/06/04 14:40:40  05s_gri
*  Doxygen completed. Doxygen warnings are not fixed.
*
*  Revision 1.12  2007/04/22 20:15:47  05s_put
*  Add/fix doxygen comments
*
*  Revision 1.11  2007/03/05 14:19:08  05s_put
*  - interface refactoring
*  - memory fix
*
*  Revision 1.4  2006/12/20 16:56:52  05s_gri
*  Code doxygenning is continued
*
*  Revision 1.3  2006/11/13 10:24:53  05s_put
*  New version of compensation
*
*  Revision 1.5  2006/09/09 13:24:57  09k_sim
*  Began to add codec support to the library.
*  MPEG2 ME is supported.
*
*  Revision 1.4  2006/08/01 13:07:38  09k_sim
*  Some refactoring in FSA was done.
*
*  Revision 1.3  2006/07/20 00:26:41  09k_sim
*  LocalFSA parameters increased.
*  FullSearch & HierarchicSearch functions were rewritten, so now FS and HS work.
*
*  Revision 1.2  2006/07/13 07:51:30  05s_put
*  Fixed error
*
*/

#include "cMEstimation.h"
#include "cSimpleMEAlgs.h"

/**
**************************************************************************
* \brief Motion estimation using 3DRS algorithm
*
*  This function implements motion estimation, using 3DRS algorithm
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* 
* \return none
*/
void Estimate3DRS(gmcMotionEstimator* pMEStruct)
{
	int X, Y;
	int iCandQ;
	int Xbl, Ybl;
	cMV CurrentVector, RandomVector;
	int i;
	int iCurOff;
	BYTE* pRefBlock;

	memcpy(pMEStruct->MVPrev, pMEStruct->MVectors, sizeof(cMV) * pMEStruct->m_iBlocksInFrame);

	for (Y = 0, Ybl = 0; Y < pMEStruct->m_iHeight; Y += pMEStruct->m_iBlockSize, Ybl++)
		for (X = 0, Xbl = 0; X < pMEStruct->m_iWidth; X += pMEStruct->m_iBlockSize, Xbl++)
		{
			pMEStruct->m_MInfo.m_iBlX = Xbl;
			pMEStruct->m_MInfo.m_iBlY = Ybl;

			iCandQ = 0;
			// adding spatial candidates
			if (Ybl > 0)
			{
				if (Xbl > 0)
					pMEStruct->m_MVCandidates[iCandQ++] = pMEStruct->MVectors[(Ybl-1)*pMEStruct->m_iBlocksHor + Xbl-1];
				if (Xbl < pMEStruct->m_iBlocksHor - 1)
					pMEStruct->m_MVCandidates[iCandQ++] = pMEStruct->MVectors[(Ybl-1)*pMEStruct->m_iBlocksHor + Xbl+1];
			}
			// adding temporal candidate
			if (Ybl < pMEStruct->m_iBlocksVert-2 && pMEStruct->m_bExistPrevMV)
				pMEStruct->m_MVCandidates[iCandQ++] = pMEStruct->MVPrev[(Ybl+2)*pMEStruct->m_iBlocksHor + Xbl];
			// random vector
			RandomVector.x = (rand() % (pMEStruct->m_iMaxMVX+pMEStruct->m_iMaxMVX)) - pMEStruct->m_iMaxMVX;
			RandomVector.y = (rand() % (pMEStruct->m_iMaxMVY+pMEStruct->m_iMaxMVY)) - pMEStruct->m_iMaxMVY;
			pMEStruct->m_MVCandidates[iCandQ++] = RandomVector;

			iCurOff = Y*pMEStruct->m_iExtWidth+X + pMEStruct->m_iFirstRowOffset;
			pRefBlock = pMEStruct->RefFrame+iCurOff;
			GetBlock(pMEStruct->m_CurBlock, pMEStruct->m_iBlockSize, pMEStruct->m_iBlockSize, pMEStruct->m_iBlockSize, pMEStruct->CurFrame+iCurOff, pMEStruct->m_iExtWidth, 0, 0, 0, 0);

			CurrentVector.x = 0;
			CurrentVector.y = 0;
			CurrentVector.error = MainDiffFunction(pMEStruct, X, Y, CurrentVector.x, CurrentVector.y, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);

			for (i=0; i<iCandQ; i++)
			{
				pMEStruct->m_MVCandidates[i].error = MainDiffFunction(pMEStruct, X, Y, pMEStruct->m_MVCandidates[i].x,
					pMEStruct->m_MVCandidates[i].y, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);
				if (pMEStruct->m_MVCandidates[i].error<CurrentVector.error) CurrentVector = pMEStruct->m_MVCandidates[i];
			}

			pMEStruct->MVectors[Ybl*pMEStruct->m_iBlocksHor + Xbl] = CurrentVector;
		}

		pMEStruct->m_bExistPrevMV = TRUE;
}

/**
**************************************************************************
* \brief Searches motion using E3DRS
*
*  This function implements motion vector search using E3DRS algorithm
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* 
* \return none
*/
void EstimateE3DRS(gmcMotionEstimator* pMEStruct)
{
	int X, Y;
	int iCandQ;
	int Xbl, Ybl;
	cMV CurrentVector, RandomVector;
	int i, j;
	int iCurOff;
	BYTE* pRefBlock;

	memcpy(pMEStruct->MVPrev, pMEStruct->MVectors, sizeof(cMV) * pMEStruct->m_iBlocksInFrame);

	for (Y = 0, Ybl = 0; Y < pMEStruct->m_iHeight; Y += pMEStruct->m_iBlockSize, Ybl++)
		for (X = 0, Xbl = 0; X < pMEStruct->m_iWidth; X += pMEStruct->m_iBlockSize, Xbl++)
		{
			int iStepX = 1 << pMEStruct->m_PrecMX, iStepY = 1 << pMEStruct->m_PrecMY;

			pMEStruct->m_MInfo.m_iBlX = Xbl;
			pMEStruct->m_MInfo.m_iBlY = Ybl;

			iCandQ = 0;
			// adding spatial candidates
			if (Ybl > 0)
			{
				if (Xbl > 0)
				{
					pMEStruct->m_MVCandidates[iCandQ++] = pMEStruct->MVectors[(Ybl-1)*pMEStruct->m_iBlocksHor + Xbl-1];
				}
				if (Xbl < pMEStruct->m_iBlocksHor - 1)
					pMEStruct->m_MVCandidates[iCandQ++] = pMEStruct->MVectors[(Ybl-1)*pMEStruct->m_iBlocksHor + Xbl+1];
			}
			// adding temporal candidate
			if (Ybl < pMEStruct->m_iBlocksVert-2 && pMEStruct->m_bExistPrevMV)
				pMEStruct->m_MVCandidates[iCandQ++] = pMEStruct->MVPrev[(Ybl+2)*pMEStruct->m_iBlocksHor + Xbl];
			// random vector
			RandomVector.x = (rand() % (pMEStruct->m_iMaxMVX+pMEStruct->m_iMaxMVX)) - pMEStruct->m_iMaxMVX;
			RandomVector.y = (rand() % (pMEStruct->m_iMaxMVY+pMEStruct->m_iMaxMVY)) - pMEStruct->m_iMaxMVY;
			pMEStruct->m_MVCandidates[iCandQ++] = RandomVector;

			iCurOff = Y*pMEStruct->m_iExtWidth+X + pMEStruct->m_iFirstRowOffset;
			pRefBlock = pMEStruct->RefFrame+iCurOff;
			GetBlock(pMEStruct->m_CurBlock, pMEStruct->m_iBlockSize, pMEStruct->m_iBlockSize, pMEStruct->m_iBlockSize, pMEStruct->CurFrame+iCurOff, pMEStruct->m_iExtWidth, 0, 0, 0, 0);

			CurrentVector.x = 0;
			CurrentVector.y = 0;
			CurrentVector.error = MainDiffFunction(pMEStruct, X, Y, CurrentVector.x, CurrentVector.y, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);

			for (i=0; i<iCandQ; i++)
			{
				pMEStruct->m_MVCandidates[i].error = MainDiffFunction(pMEStruct, X, Y, pMEStruct->m_MVCandidates[i].x, pMEStruct->m_MVCandidates[i].y,
					&pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);
				if (pMEStruct->m_MVCandidates[i].error<CurrentVector.error) CurrentVector = pMEStruct->m_MVCandidates[i];
			}

			// enhancement - check of neighbors

			for (i=-iStepY; i<iStepY+1; i+=iStepY)
				for (j=-iStepX; j<iStepX+1; j+=iStepX)
				{
					int iError, iCurMVX, iCurMVY;

					iCurMVX = CurrentVector.x;
					iCurMVY = CurrentVector.y;

					if ((i || j) &&
						-pMEStruct->m_iMaxMVX <= iCurMVX+(j) && iCurMVX+(j) <= pMEStruct->m_iMaxMVX &&
						-pMEStruct->m_iMaxMVY <= iCurMVY+(i) && iCurMVY+(i) <= pMEStruct->m_iMaxMVY)
					{
						iError = MainDiffFunction(pMEStruct, X, Y, iCurMVX+j, iCurMVY+i, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);
						if (iError<CurrentVector.error)
						{
							CurrentVector.error = iError;
							CurrentVector.x = iCurMVX+j;
							CurrentVector.y = iCurMVY+i;
						}
					}
				}

				pMEStruct->MVectors[Ybl*pMEStruct->m_iBlocksHor + Xbl] = CurrentVector;
		}

		pMEStruct->m_bExistPrevMV = TRUE;
}

/**
**************************************************************************
* \brief Searches motion for current block using FSA
*
*  This function implements motion vector search for current block, using
*  full search algorithm with variable step
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param iX                  [IN] -  X coordinate of the current block
* \param iY                  [IN] -  Y coordinate of the current block
* \param iYStart             [IN] -  bottom of the search area
* \param iYEnd               [IN] -  top of the search area
* \param iXStart             [IN] -  left border of the search area
* \param iXEnd               [IN] -  right border of the search area
* \param iStepX              [IN] -  horizontal step of search
* \param iStepY              [IN] -  vertical   step of search
* \param CurrentVector       [IN/OUT] - current best motion vector
* \param BlockSize           [IN] -  size of current block
* \param bLocalRefinement    [IN] -  flag for local motion vector refinement to 1 pixel precision
* 
* \return none
*/
void LocalFSA(gmcMotionEstimator* pMEStruct, int iX, int iY, int iYStart, int iYEnd, int iXStart, int iXEnd, int iStepX, int iStepY,
							cMV* CurrentVector, cMacroBlockPartitions BlockSize, BOOL bLocalRefinement)
{
	int mx, my;
	int iError;

	NEW_BLOCK_ID

	GetBlock(pMEStruct->m_CurBlock, MBwidth[BlockSize], MBheight[BlockSize], MBwidth[BlockSize], pMEStruct->CurFrame+pMEStruct->m_iFirstRowOffset+iY*pMEStruct->m_iExtWidth+iX, pMEStruct->m_iExtWidth<<pMEStruct->m_MInfo.m_bFieldInFrame, 0, 0, 0, 0);
	for (my = iYStart; my<=iYEnd; my+=iStepY)
	{
		for (mx = iXStart; mx<=iXEnd; mx+=iStepX)
		{
			iError = MainDiffFunction(pMEStruct, iX, iY, mx, my, &pMEStruct->m_MInfo, BlockSize);
			
			if (iError < CurrentVector->error)
			{
				CurrentVector->x = mx;
				CurrentVector->y = my;
				CurrentVector->error = iError;
			}
		}
	}

	if(bLocalRefinement)
	{
		int x=CurrentVector->x, y=CurrentVector->y;
		for (my=y-1; my<=y+1; my++)
		{
			for (mx=x-1; mx<=x+1; mx++)
			{
				iError = MainDiffFunction(pMEStruct, iX, iY, mx, my, &pMEStruct->m_MInfo, BlockSize);
				
				if (iError < CurrentVector->error)
				{
					CurrentVector->x = mx;
					CurrentVector->y = my;
					CurrentVector->error = iError;
				}
			}
		}
	}
}

/**
**************************************************************************
* \brief Searches motion using FSA
*
*  This function implements motion vector search using full search
*  algorithm with variable step
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* 
* \return none
*/
void EstimateFSA(gmcMotionEstimator* pMEStruct)
{
	int X, Y,Xmbl, Ymbl, i,j;
	cMV CurrentVector;

	for (Y = 0, Ymbl = 0; Y < pMEStruct->m_iHeight; Y += ME_MACROBLOCK_SIZE, Ymbl++)
	{
		for (X = 0, Xmbl = 0; X < pMEStruct->m_iWidth; X += ME_MACROBLOCK_SIZE, Xmbl++)
		{
			switch(pMEStruct->m_iBlockSize)
			{
			case b_16x16:
				CurrentVector.error=MAXLONG;
				LocalFSA(pMEStruct,X,Y,-pMEStruct->m_iMaxMVY,pMEStruct->m_iMaxMVY,-pMEStruct->m_iMaxMVX,pMEStruct->m_iMaxMVX,1,1,&CurrentVector, (cMacroBlockPartitions)pMEStruct->m_iBlockSize,FALSE);
				pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iBlocksHor + Xmbl].m_BlockPartition = cbt_16x16;
				pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iBlocksHor + Xmbl].m_DataMV[0] = CurrentVector;
				break;

			case b_8x8:
				pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_BlockPartition = cbt_8x8;
				for(i=0;i<4;i++)
				{
					CurrentVector.error=MAXLONG;
					pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_SubBlockPartition[i] = cbt_8x8;
					LocalFSA(pMEStruct,X+(i&1)*8, Y+(i&2)*4,-pMEStruct->m_iMaxMVY,pMEStruct->m_iMaxMVY,-pMEStruct->m_iMaxMVX,pMEStruct->m_iMaxMVX,1,1,&CurrentVector,(cMacroBlockPartitions)pMEStruct->m_iBlockSize,FALSE);
					pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_DataMV[i*4] = CurrentVector;
				}
				break;

			case b_4x4:
				pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_BlockPartition = cbt_8x8;
				for(i=0;i<4;i++)
				{
					pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_SubBlockPartition[i] = cbt_4x4;
					for(j=0;j<4;j++)
					{
						CurrentVector.error=MAXLONG;
						LocalFSA(pMEStruct,X+(i&1)*8+(j&1)*4, Y+(i&2)*4+(j&2)*2,-pMEStruct->m_iMaxMVY,pMEStruct->m_iMaxMVY,-pMEStruct->m_iMaxMVX,pMEStruct->m_iMaxMVX,1,1,&CurrentVector,(cMacroBlockPartitions)pMEStruct->m_iBlockSize,FALSE);
						pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_DataMV[i*4+j] = CurrentVector;
					}
				}
				break;
			}
		}
	}
}

/**
**************************************************************************
* \brief Calculates mean value of the given block pixels
*
*  This function calculates mean value of given block pixels
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param pBlock              [IN] -  pointer to the block
* \param iBlockWidth         [IN] -  block width
* \param iBlockHeight        [IN] -  block height
* \param iStride             [IN] -  block stride
* 
* \return mean value of block pixels
*/
BYTE GetBlockMeanValue(gmcMotionEstimator* pMEStruct, BYTE* pBlock, int iBlockWidth, int iBlockHeight, int iStride)
{
	int i, j;
	int iMean = 0;
	BYTE* pCurStride = pBlock;

	if (pMEStruct->m_bIsInterlaced)
	{
		pCurStride = pCurStride + (pMEStruct->m_MInfo.m_bBotBase?iStride:0);
		iStride = iStride + iStride;
		iBlockHeight = iBlockHeight/2;
	}

	for (i=0; i<iBlockHeight; i++)
	{
		for (j=0; j<iBlockWidth; j++)
		{
			iMean += pCurStride[j];
		}
		pCurStride += iStride;
	}

	return iMean / (iBlockWidth*iBlockHeight);
}

/*! \def CHECKMV(mvy,mvx)
*   \brief Check if the given vector has lower error then the best current one
*
*  This is a macro that checks if the current vector (\a mvy, \a mvx) has
*  lower error then the current best one. If it has then this vector is
*  stored as the best current one
*/
#define CHECKMV(mvy,mvx)                                                                                \
	if (-pMEStruct->m_iMaxMVX <= iCurMVX+(mvx) && iCurMVX+(mvx) <= pMEStruct->m_iMaxMVX &&                          \
	    -pMEStruct->m_iMaxMVY <= iCurMVY+(mvy) && iCurMVY+(mvy) <= pMEStruct->m_iMaxMVY)                            \
	{                                                                                                     \
		iError = MainDiffFunction(pMEStruct, iX, iY, iCurMVX+(mvx), iCurMVY+(mvy), Minfo, BlockSize); \
		if (iError<CurrentVector->error)                                                                    \
		{                                                                                                   \
			bMVChanged = TRUE;                                                                                \
			CurrentVector->error = iError;                                                                    \
			CurrentVector->x = iCurMVX+(mvx);                                                                 \
			CurrentVector->y = iCurMVY+(mvy);                                                                 \
		}                                                                                                   \
	}

/**
**************************************************************************
* \brief Applies SDP search around given center and sub-pel refinement
*
*  This function applies refining search of motion vector using small
*  diamond pattern. Start point is current best motion vector. Quantity
*  of competent steps is limited. After that sub-pixel refinement of the
*  vector is performed
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param pRefBlock           [IN] -  pointer to the current block in reference frame
* \param iX                  [IN] -  horizontal coordinate of the current block
* \param iY                  [IN] -  vertical coordinate of the current block
* \param CurrentVector       [IN/OUT] - current best motion vector
* \param iRange              [IN] -  number of competent steps
* \param Minfo               [IN/OUT] - pointer to the structure with motion information about current block
* \param BlockSize           [IN] -  size of the current block
* 
* \return TRUE if the best motion vector was found in the middle of the pattern within competent steps, FALSE otherwise
*/
void FinishingSearch(gmcMotionEstimator* pMEStruct, BYTE* pRefBlock, int iX, int iY, cMV* CurrentVector, int iRange, MInfoStruct* Minfo, cMacroBlockPartitions BlockSize)
{
	int iStep;
	BOOL bMVChanged = TRUE;
	int iError, iCurMVX, iCurMVY;
	int iStepX = 1 << pMEStruct->m_PrecMX, iStepY = 1 << pMEStruct->m_PrecMY;

	for (iStep = 0; (iStep<iRange) && bMVChanged; iStep++)
	{
		iCurMVX = CurrentVector->x;
		iCurMVY = CurrentVector->y;
		bMVChanged= FALSE;


		CHECKMV(iStepY,0);
		CHECKMV(-iStepY,0);
		CHECKMV(0,-iStepX);
		CHECKMV(0,iStepX);
	}

	iStepX >>= 1;
	iStepY >>= 1;
	if (iStepX || iStepY)
	{
		iCurMVX = CurrentVector->x;
		iCurMVY = CurrentVector->y;
		CHECKMV(iStepY,0);
		CHECKMV(-iStepY,0);
		CHECKMV(0,-iStepX);
		CHECKMV(0,iStepX);
	}

	iStepX >>= 1;
	iStepY >>= 1;
	if (iStepX || iStepY)
	{
		iCurMVX = CurrentVector->x;
		iCurMVY = CurrentVector->y;
		CHECKMV(iStepY,0);
		CHECKMV(-iStepY,0);
		CHECKMV(0,-iStepX);
		CHECKMV(0,iStepX);
	}
}
#undef CHECKMV

/**
**************************************************************************
* \brief This function performs hierarchic search of motion vector for given block
*
*  This function searches motion vector of current block using hierarchic
*  search algorithm
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param X                   [IN] -  current block horizontal coordinate
* \param Y                   [IN] -  current block vertical coordinate
* \param BestVector          [OUT] - the best motion vector found
* \param iBlockSize			     [IN] -  block size 
* 
* \return none
*/
void LocalHierarchicSearch(gmcMotionEstimator* pMEStruct, int X, int Y, cMV* BestVector, int iBlockSize)
{
	int i,j,ii,jj, iError;
    int iHierBlockSize=iBlockSize/pMEStruct->m_iHierarchicReduction;
	int iAreaVertSize=2*pMEStruct->m_iHierarchicMaxMotVert+iHierBlockSize, iAreaHorSize=2*pMEStruct->m_iHierarchicMaxMotHor+iHierBlockSize;
	
	cMV CurrentVector;
	
	cMVList* pCandList=malloc(sizeof(cMVList));
	cMVList* pCurCand=pCandList;
	cInitMVListNode(pCurCand);
	
	BestVector->error=CurrentVector.error=MAXLONG;
	//current block linear reduction
	for(i=0,ii=Y;i<iHierBlockSize;i++,ii+=pMEStruct->m_iHierarchicReduction)
	{
		for(j=0,jj=X;j<iHierBlockSize;j++,jj+=pMEStruct->m_iHierarchicReduction)
		{
			pMEStruct->m_pCurrentHierarchicBlock[i*iHierBlockSize+j]=GetBlockMeanValue(pMEStruct,pMEStruct->CurFrame+ii*pMEStruct->m_iExtWidth+jj+pMEStruct->m_iFirstRowOffset,pMEStruct->m_iHierarchicReduction,pMEStruct->m_iHierarchicReduction,pMEStruct->m_iExtWidth);
		}
	}

	//reference frame area linear reduction
	for(i=0,ii=Y-pMEStruct->m_iMaxMotVert; i<iAreaVertSize; i++,ii+=pMEStruct->m_iHierarchicReduction)
	{
		for(j=0,jj=X-pMEStruct->m_iMaxMotHor; j<iAreaHorSize; j++,jj+=pMEStruct->m_iHierarchicReduction)
		{
			pMEStruct->m_pHierarchicPrevFrameArea[i*iAreaHorSize+j]=GetBlockMeanValue(pMEStruct,pMEStruct->RefFrame+ii*pMEStruct->m_iExtWidth+jj+pMEStruct->m_iFirstRowOffset,pMEStruct->m_iHierarchicReduction,pMEStruct->m_iHierarchicReduction,pMEStruct->m_iExtWidth);
		}
	}

	NEW_BLOCK_ID
	
	for(i=-pMEStruct->m_iHierarchicMaxMotVert; i<=pMEStruct->m_iHierarchicMaxMotVert; i++)
	{
		for(j=-pMEStruct->m_iHierarchicMaxMotHor; j<=pMEStruct->m_iHierarchicMaxMotHor; j++)
		{
			iError=GetErrorSAD_C(pMEStruct->m_pCurrentHierarchicBlock,pMEStruct->m_pHierarchicPrevFrameArea+(i+pMEStruct->m_iHierarchicMaxMotVert)*iAreaHorSize+(j+pMEStruct->m_iHierarchicMaxMotHor),iHierBlockSize,iAreaHorSize,(cMacroBlockPartitions)iHierBlockSize,&pMEStruct->m_MInfo);
			
			if(iError<=CurrentVector.error)
			{
				if(iError<CurrentVector.error)
					pCurCand=pCandList;

				CurrentVector.error=iError;
				CurrentVector.x=j;
				CurrentVector.y=i;
				
				pCurCand->m_DataMV=CurrentVector;
				pCurCand->m_bUsed=TRUE;
				if(!pCurCand->m_pNext)
				{
					pCurCand->m_pNext=malloc(sizeof(cMVList));
					pCurCand=pCurCand->m_pNext;
					cInitMVListNode(pCurCand);
				}
				else
				{
					pCurCand=pCurCand->m_pNext;
					pCurCand->m_bUsed=FALSE;
				}
			}
		}
	}

	pCurCand=pCandList;
	GetBlock(pMEStruct->m_CurBlock, iBlockSize, iBlockSize, iBlockSize, pMEStruct->CurFrame+pMEStruct->m_iFirstRowOffset+Y*pMEStruct->m_iExtWidth+X, pMEStruct->m_iExtWidth, 0, 0, 0, 0);

	NEW_BLOCK_ID
	
	while(pCurCand->m_bUsed)
	{
		CurrentVector=pCurCand->m_DataMV;
		CurrentVector.error*=sqr(pMEStruct->m_iHierarchicReduction);
		CurrentVector.x=(CurrentVector.x*pMEStruct->m_iHierarchicReduction)<<pMEStruct->m_PrecMX;
		CurrentVector.y=(CurrentVector.y*pMEStruct->m_iHierarchicReduction)<<pMEStruct->m_PrecMY;
    
		//FS
		LocalFSA(pMEStruct,X,Y,CurrentVector.y,CurrentVector.y+pMEStruct->m_iHierarchicReduction*pMEStruct->m_PrecMY,CurrentVector.x,CurrentVector.x+pMEStruct->m_iHierarchicReduction*pMEStruct->m_PrecMX,1,1,&CurrentVector,(cMacroBlockPartitions)iBlockSize,FALSE);
		if(CurrentVector.error < BestVector->error)
			*BestVector=CurrentVector;

		pCurCand=pCurCand->m_pNext;
	}

	pCurCand=pCandList;
	while(pCurCand)
	{
		cMVList* tmp=pCurCand;
		pCurCand=pCurCand->m_pNext;
		free(tmp);
	}
}

/**
**************************************************************************
* \brief This function performs hierarchic motion estimation
*
*  This function performs motion estimation using hierarchic search
*  algorithm
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* 
* \return none
*/
void EstimateHierarchic(gmcMotionEstimator* pMEStruct)
{
	int X, Y,Xmbl, Ymbl, i,j;
	cMV CurrentVector;
	cInitMV(&CurrentVector);

	for (Y = 0, Ymbl = 0; Y < pMEStruct->m_iHeight; Y += ME_MACROBLOCK_SIZE, Ymbl++)
		for (X = 0, Xmbl = 0; X < pMEStruct->m_iWidth; X += ME_MACROBLOCK_SIZE, Xmbl++)
		{
			switch(pMEStruct->m_iBlockSize)
			{
			case b_16x16:
				LocalHierarchicSearch(pMEStruct, X, Y, &CurrentVector,pMEStruct->m_iBlockSize);
				pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iBlocksHor + Xmbl].m_BlockPartition = cbt_16x16;
				pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iBlocksHor + Xmbl].m_DataMV[0] = CurrentVector;
				break;

			case b_8x8:
				pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_BlockPartition = cbt_8x8;
				for(i=0;i<4;i++)
				{
					pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_SubBlockPartition[i] = cbt_8x8;
					LocalHierarchicSearch(pMEStruct, X+(i&1)*8, Y+(i&2)*4, &CurrentVector,pMEStruct->m_iBlockSize);
					pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_DataMV[i*4] = CurrentVector;
				}
				break;

			case b_4x4:
				pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_BlockPartition = cbt_8x8;
				for(i=0;i<4;i++)
				{
					pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_SubBlockPartition[i] = cbt_4x4;
					for(j=0;j<4;j++)
					{
						LocalHierarchicSearch(pMEStruct, X+(i&1)*8+(j&1)*4, Y+(i&2)*4+(j&2)*2, &CurrentVector,pMEStruct->m_iBlockSize);
						pMEStruct->m_CurMBlocks[Ymbl*pMEStruct->m_iMBlocksHor + Xmbl].m_DataMV[i*4+j] = CurrentVector;
					}
				}
				break;
			}
		}
}

/**
**************************************************************************
* \brief Applies motion estimation using diamond search algorithm
*
*  This function estimates motion using diamond pattern with decreasing
*  its size at every step from the half of motion search area to 1 pixel
*
* \param pMEStruct           [IN/OUT] -  pointer to gmcMotionEstimator structure
* 
* \return none
*/
void EstimateDS(gmcMotionEstimator* pMEStruct)
{
	cMV CurrentVector;
	BOOL bPredictNewMotion = FALSE;
	int X, Y;
	int Xbl, Ybl;
	int iStepX, iStepY;
	int iError;
	int iCurMVX, iCurMVY;
	int iLeftUntouched;
	int iCurOff;
	BYTE* pRefBlock;

	for (Y = 0, Ybl = 0; Y < pMEStruct->m_iHeight; Y += pMEStruct->m_iBlockSize, Ybl++)
		for (X = 0, Xbl = 0; X < pMEStruct->m_iWidth; X += pMEStruct->m_iBlockSize, Xbl++)
		{
			pMEStruct->m_MInfo.m_iBlX = Xbl;
			pMEStruct->m_MInfo.m_iBlY = Ybl;

			iCurOff = Y*pMEStruct->m_iExtWidth+X + pMEStruct->m_iFirstRowOffset;
			iLeftUntouched = 0;
			iStepX = (pMEStruct->m_iMaxMVX+1)/2;
			iStepY = (pMEStruct->m_iMaxMVY+1)/2;

			pRefBlock = pMEStruct->RefFrame+iCurOff;
			GetBlock(pMEStruct->m_CurBlock, pMEStruct->m_iBlockSize, pMEStruct->m_iBlockSize, pMEStruct->m_iBlockSize, pMEStruct->CurFrame+iCurOff, pMEStruct->m_iExtWidth, 0, 0, 0, 0);

			CurrentVector.x = 0;
			CurrentVector.y = 0;
			CurrentVector.error = MainDiffFunction(pMEStruct, X, Y, CurrentVector.x, CurrentVector.y, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);

			for (;iStepX > 0 || iStepY > 0; iStepX=(iStepX+1)/2, iStepY=(iStepY+1)/2)
			{
				iLeftUntouched++;
				iCurMVX = CurrentVector.x;
				iCurMVY = CurrentVector.y;

				iError = MainDiffFunction(pMEStruct, X, Y, iCurMVX, iCurMVY-iStepY, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);
				if (iError < CurrentVector.error)
				{
					CurrentVector.x = iCurMVX;
					CurrentVector.y = iCurMVY - iStepY;
					CurrentVector.error = iError;
					iLeftUntouched = 0;
				}

				iError = MainDiffFunction(pMEStruct, X, Y, iCurMVX, iCurMVY+iStepY, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);
				if (iError < CurrentVector.error)
				{
					CurrentVector.x = iCurMVX;
					CurrentVector.y = iCurMVY + iStepY;
					CurrentVector.error = iError;
					iLeftUntouched = 0;
				}

				iError = MainDiffFunction(pMEStruct, X, Y, iCurMVX-iStepX, iCurMVY, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);
				if (iError < CurrentVector.error)
				{
					CurrentVector.x = iCurMVX - iStepX;
					CurrentVector.y = iCurMVY;
					CurrentVector.error = iError;
					iLeftUntouched = 0;
				}

				iError = MainDiffFunction(pMEStruct, X, Y, iCurMVX+iStepX, iCurMVY, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);
				if (iError < CurrentVector.error)
				{
					CurrentVector.x = iCurMVX + iStepX;
					CurrentVector.y = iCurMVY;
					CurrentVector.error = iError;
					iLeftUntouched = 0;
				}

				iError = MainDiffFunction(pMEStruct, X, Y, iCurMVX+iStepX, iCurMVY-iStepY, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);
				if (iError < CurrentVector.error)
				{
					CurrentVector.x = iCurMVX + iStepX;
					CurrentVector.y = iCurMVY - iStepY;
					CurrentVector.error = iError;
					iLeftUntouched = 0;
				}

				iError = MainDiffFunction(pMEStruct, X, Y, iCurMVX-iStepX, iCurMVY-iStepY, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);
				if (iError < CurrentVector.error)
				{
					CurrentVector.x = iCurMVX - iStepX;
					CurrentVector.y = iCurMVY - iStepY;
					CurrentVector.error = iError;
					iLeftUntouched = 0;
				}

				iError = MainDiffFunction(pMEStruct, X, Y, iCurMVX+iStepX, iCurMVY+iStepY, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);
				if (iError < CurrentVector.error)
				{
					CurrentVector.x = iCurMVX + iStepX;
					CurrentVector.y = iCurMVY + iStepY;
					CurrentVector.error = iError;
					iLeftUntouched = 0;
				}

				iError = MainDiffFunction(pMEStruct, X, Y, iCurMVX-iStepX, iCurMVY+iStepY, &pMEStruct->m_MInfo, (cMacroBlockPartitions)pMEStruct->m_iBlockSize);
				if (iError < CurrentVector.error)
				{
					CurrentVector.x = iCurMVX - iStepX;
					CurrentVector.y = iCurMVY + iStepY;
					CurrentVector.error = iError;
					iLeftUntouched = 0;
				}
				// we should not perform the same calculation twice
				if (iStepX == 1) iStepX = 0;
				if (iStepY == 1) iStepY = 0;
			}
			pMEStruct->MVectors[Ybl*pMEStruct->m_iBlocksHor + Xbl] = CurrentVector;
		}
		// end of vector calculation
		pMEStruct->m_bExistPrevMV = TRUE; 
}

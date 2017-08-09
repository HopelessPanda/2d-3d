/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cMEstimation.h
*  \brief Performs motion estimation
*
*  This file contains declarations of the functions that performs
*  motion estimation in video and types, used by this functions.
*/

#ifndef _C_MESTIMATION_H_
#define _C_MESTIMATION_H_

#include <assert.h>
#if ( _MSC_VER < 1210 )
    #undef __cplusplus // for VS6 purposes
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "../../VideoGMLib/Common/CommonTypes.h"
#include "cMEStaff.h"
#include "cGetError.h"
#include "MEParams/cMEParams.h"

/*! \def ME_MACROBLOCK_SIZE
*   \brief Defines macro block size
*
*  Defines macro block size
*/
#define ME_MACROBLOCK_SIZE 16

/*! \def ME_MACROBLOCK_SIZE_POW
*   \brief Defines macro block size
*
*  Defines macro block size
*/
#define ME_MACROBLOCK_SIZE_POW 4

/*! \def ME_MACROBLOCK_SIZE_M1
*   \brief Defines macro block size minus 1
*
*  Defines macro block size minus 1; need it to perform some remainder
*  calculations
*/
#define ME_MACROBLOCK_SIZE_M1 15

/*! \def FUNCTIONS_QUANTITY
*   \brief Defines quantity of implemented ME algorithms
*
*  Defines quantity of implemented ME algorithms
*/
#define FUNCTIONS_QUANTITY 6

/*! \def NUM_REF_FRAMES
*   \brief Reference frames quantity
*
*  Quantity of reference frames used in motion estimation
*/

#define NUM_REF_FRAMES 2

/*! \struct cMacroBlock
*   \brief Structure for containing macro block information
*
*  This structure contains macro block information. The numeration of
*  blocks always goes  from left to right, from down to up (in the way
*  of increasing vertical coordinate), like this (for cbt_4x4):
*
* 12 13 14 15
*  8  9 10 11
*  4  5  6  7
*  0  1  2  3
*
*/
typedef struct
{
	cMacroblockTypes m_type;
	cMacroBlockPartitions m_BlockPartition;                     //!< indicates if macroblock is subdivided into 4 8x8 blocks
	cMacroBlockPartitions m_SubBlockPartition[4];               //!< indicates if some of 8x8 blocks subdivided into 4x4 blocks
	cMV *m_DataMV;                                             //!< array of motion vectors of the macroblock
	cMV m_DataMVs[NUM_REF_FRAMES][16];
} cMacroBlock;

/**
**************************************************************************
* \brief Initialization of cMacroBlock
*
*  This function initializes cMacroBlock structure with default values
*
* \param cMacroBlockStruct   [IN/OUT] - pointer to cMacroBlock structure
* 
* \return none
*/
__inline void cInitMacroBlock(cMacroBlock* cMacroBlockStruct)
{
	int i,j;
	cMacroBlockStruct->m_BlockPartition = cbt_16x16;
	cMacroBlockStruct->m_type=MBT_FORWARD;
	for (i = 0; i < 4; i++) cMacroBlockStruct->m_SubBlockPartition[i] = cbt_8x8;
	
	for(j=0;j<NUM_REF_FRAMES;j++)
	{
		for (i = 0; i < 16; i++)
		{
			cInitMV(&cMacroBlockStruct->m_DataMVs[j][i]);
		}
	}
	
	cMacroBlockStruct->m_DataMV=cMacroBlockStruct->m_DataMVs[0];
}

/*! \enum AcceptableBlockSizes
*   \brief Types of blocks, used for ME and MC
*
*  Describes types of blocks, used for ME and MC
*/
typedef enum
{
	b_32x32 = 32,
	b_16x16 = 16,
	b_8x8 = 8,
	b_4x4 = 4
} AcceptableBlockSizes;

struct taggmcMotionEstimator;

/*! \typedef EstimationFuntion
*   \brief Function, that performs motion estimation
*
*  Type definition of function, that performs motion estimation
*/	
typedef void (*EstimationFuntion)(struct taggmcMotionEstimator*);

/*! \struct taggmcMotionEstimator
*   \brief Structure for performing ME
*
*  This structure contains all data, needed for implementation of
*  motion estimation in video
*/
typedef struct taggmcMotionEstimator
{
	cMEParams*		m_pParams;
	BYTE *CurFrame, *RefFrame, *RefFrames[NUM_REF_FRAMES][2]; //!< Current & reference frames 
	cMV *MVPrev;                                              //!< Motion vectors from previous frame motion estimation
	cMV *MVectors;                                            //!< Motion vectors of current frame motion estimation
	cMacroBlock* m_CurMBlocks;                                //!< Macro blocks of current frame
	cMacroBlock* m_RefMBlocks;                                //!< Macro blocks of reference frame
	cMacroBlock* m_RefMBlocks__[NUM_REF_FRAMES][2];           //!< Macro blocks of reference frame
	cMV* m_MeanMVs;                                           //!< Mean motion vectors, used for cMV smoothing
	int m_iWidth, m_iHeight;                                  //!< Width and height of video frames
	int m_iBorderSize;                                        //!< Size of border in pixels, needed to be present around the frame
	int m_q;                                                  //!< Quality of estimation (not used)
	int m_iExtWidth, m_iExtHeight;                            //!< Width and height of frames with border added
	int m_iBlocksInFrame;                                     //!< Quantity of motion vectors
	int m_iBlocksHor, m_iBlocksVert;                          //!< Horizontal and vertical quantity of blocks
	int m_iMBlocksInFrame;                                    //!< Quantity of macro blocks in frame
	int m_iMBlocksHor, m_iMBlocksVert;                        //!< Horizontal and vertical quantity of macro blocks
	int m_iFirstRowOffset;                                    //!< Offset of real frame data in frame with borders
	int m_iFirstRowOffsetChr;                                 //!< Offset of real chroma frame data in frame with borders
	int m_iSecCompOff;                                        //!< Offset of second pixel component real frame data in frame with borders
	int m_iThdCompOff;                                        //!< Offset of third pixel component real frame data in frame with borders
	int m_PostSteps;                                          //!< Quantity of post search steps
	MInfoStruct m_MInfo;                                      //!< Information about currently processed motion block and so on
	BOOL m_bIsInterlaced;                                     //!< Estimation over interlaced video or not

	BOOL m_bExistPrevMV;                                      //!< Indicates, if we can use motion vectors from previously processed frame

	BYTE* m_bShiftsMemory;                                    //!< Pointer to the memory where shifted Y frames are situated
	BYTE* m_pRefShiftsY[4];                                   //!< Pointer to the frames with definite shifted Y frames
	BYTE* m_pRefShiftsChr1[4];                                //!< Pointer to the frames with definite shifted U frames
	BYTE* m_pRefShiftsChr2[4];                                //!< Pointer to the frames with definite shifted V frames
	BYTE* m_pRefShiftsYMem;                                   //!< Pointer to the memory of shifted Y frames
	BYTE* m_pRefShiftsChr1Mem;                                //!< Pointer to the memory of shifted U frames
	BYTE* m_pRefShiftsChr2Mem;                                //!< Pointer to the memory of shifted V frames
	BOOL m_bUsePreparedShifts;                                //!< Indicates if ME should use given shifted frames
	BOOL m_bUsePreparedChromaShifts;                          //!< Indicates if ME should use given shifted chroma planes
	
	float m_fMeanErrorPP;                                     //!< mean SAD value per pixel
	float m_fMeanErrRenew;                                    //!< coefficient used to renew mean per pixel error
	float m_fDispersionPP;                                    //!< "dispersion" of SAD values per pixel in frame
	float m_fDispersionRenew;                                 //!< coefficient used to renew "dispersion" of SAD values pixel error

	BlockDiffFunction pGetError_16x16;                        //!< Pointer to the currently used difference function for 16x16 blocks
	BlockDiffFunction pGetError_16x8;                         //!< Pointer to the currently used difference function for 16x8 blocks
	BlockDiffFunction pGetError_8x8;                          //!< Pointer to the currently used difference function for 8x8 blocks
	BlockDiffFunction pGetError_4x4;                          //!< Pointer to the currently used difference function for 4x4 blocks
	BlockDiffFunction pGetErrorChr_16x16;                     //!< Pointer to the currently used difference function for 16x16 blocks
	BlockDiffFunction pGetErrorChr_8x8;                       //!< Pointer to the currently used difference function for 8x8 blocks
	BlockDiffFunction pGetErrorChr_4x4;                       //!< Pointer to the currently used difference function for 4x4 blocks

	int m_iMaxMotVert, m_iMaxMotHor;                          //!< vertical and horizontal dimensions of search area in pixels
	int m_iMaxMVX, m_iMaxMVY;                                 //!< maximum motion vector coordinates in horizontal and vertical direction
    int m_iCacheSizeVert, m_iCacheSizeHor;                    //!< vertical and horizontal dimensions of block ID's cache; are equal to MAX MV coordinates in the entire video
	EstimationPrecision m_PrecMX, m_PrecMY;                   //!< vertical and horizontal precision of motion estimation
	AcceptableBlockSizes m_iBlockSize;                        //!< block size for algorithms with fixed block sizes; MAX block size for algorithms with variable block sizes
	int m_iPelsInBlock;                                       //!< quantity of pixels in block
	cMV* m_MVCandidates;                                      //!< motion vector candidates
	int m_iMeanError;                                         //!< mean motion error
	int m_iMaxError;                                          //!< MAX suitable motion error
	int m_iDropNearVectorThresh;                              //!< threshold how close should be motion vector to not include it into candidates
	int m_iSteps;                                             //!< quantity of post search algorithm steps
	int m_iMEAlg;                                             //!< type of motion estimation used
	int m_iMEMetricAlg;                                       //!< type of metric used
	float m_fSequnceAverageSADsPerPixel;                      //!< average SADs quantity per pixel for entire sequence
	float m_fSequnceAverageMEErrorPerPixel;                   //!< average SAD value per pixel for entire sequence
	float m_fFrameAverageMEErrorPerBlock;                     //!< average SAD value per block 16x16 for one frame
	float m_fPrvFrameAverageMEErrorPerBlock;                  //!< average SAD value per block 16x16 for previous frame
	float m_fPrvFrameMEErrorDeviationPerBlock;                //!< dispersion of SAD value per block 16x16 for previous frame
	//float m_fFrameAverageSADsPerPixel;
	int m_iFrameCount;                                        //!< counter of frames for values averaging
	int m_sConfigFileNameMem;                                 //!< allocated amount of memory to store file name
	char* m_sConfigFileName;                                  //!< name of file to store configuration variables and statistic information
	
	BYTE* m_pCurrentHierarchicBlock;                          //!< pointer to the current hierarchic block
	BYTE* m_pHierarchicPrevFrameArea;                         //!< pointer to the downsampled area of previous frame for HS
	int m_iHierarchicMaxMotVert;                              //!< vertical dimension of MV search area during hierarchic search
	int m_iHierarchicMaxMotHor;                               //!< horizontal dimension of MV search area during hierarchic search
	int m_iHierarchicReduction;                               //!< reduction of block size during hierarchic search
	
	BOOL m_bSaveStatistic;                                    //!< shows whether we should save statistic or not

	BYTE* m_CurBlock;                                         //!< current block
	BYTE* m_CurBlChr1,* m_CurBlChr2;                          //!< current block, chroma components
	BYTE* m_RefBlock;                                         //!< reference block
	BYTE* m_RefBlChr1,* m_RefBlChr2;                          //!< reference block, chroma components

	EstimationFuntion m_AvailableMEFunctions[FUNCTIONS_QUANTITY]; //!< array of different motion estimation functions
	EstimationFuntion m_pEstimate;                                //!< pointer to the currently used function of motion estimation

	cMVList* m_MVPrvCand__[NUM_REF_FRAMES][2];                //!< motion vector candidates for previous frame macroblocks
	cMVList* m_MVNxtCand__[NUM_REF_FRAMES][2];                //!< motion vector candidates for the next frame macroblocks

	cMVList* m_MVPrvCand;                                     //!< motion vector candidates for previous frame macroblocks
	cMVList* m_MVNxtCand;                                     //!< motion vector candidates for the next frame macroblocks

	BYTE* m_pHierLaunchMap;                                   //!< map of hierarchic launches
	BOOL m_bAccept_16x16;                                     //!< 16x16 macroblock type is allowed
	BOOL m_bAccept_8x8;                                       //!<  8x8  macroblock type is allowed
	BOOL m_bAccept_4x4;                                       //!<  4x4  macroblock type is allowed

	BOOL m_bConsiderChroma;                                   //!< TRUE if we should consider chroma
	int m_iChrXMod, m_iChrYMod;                               //!< colorspace resampling down of chroma components horizontally and vertically

	BOOL m_bConsiderVectorLength;                             //!< TRUE if we should add vector length to blocks' difference

	DWORD *m_blockIDs;										  //!< block ID's cache to prevent repetitive SAD calculation
	DWORD m_curBlockID;										  //!< current block ID

	float m_fNoiseVar;										  //!< Noise variance
	
	BOOL m_bBordersAllowed;									  //!< True, if MC from frame borders is allowed
	InterpolationAlgorithm m_interAlg;						  //!< Halfpel interpolation algorithm

	cPictureStructure m_picStructure;
	cPictureType m_picType;

	CandMemoryAllocation m_CandidatesMemoryAlloc;             //!< memory allocation for MV candidates
	int m_CandKeepPrealloc;
} gmcMotionEstimator;

long MainDiffFunction(gmcMotionEstimator* pMEStruct, int iX, int iY, int iMX, int iMY, MInfoStruct* pMInfo, cMacroBlockPartitions BlockSize);

/**
**************************************************************************
* \brief Motion estimation using 3DRS algorithm
*
*/
void Estimate3DRS(gmcMotionEstimator* pMEStruct);

/**
**************************************************************************
* \brief Searches motion using E3DRS
*
*/
void EstimateE3DRS(gmcMotionEstimator* pMEStruct);

/**
**************************************************************************
* \brief Searches motion using FSA
* 
*/
void EstimateFSA(gmcMotionEstimator* pMEStruct);

/**
**************************************************************************
* \brief This function performs hierarchic motion estimation
*
*/
void EstimateHierarchic(gmcMotionEstimator* pMEStruct);

/**
**************************************************************************
* \brief Applies motion estimation using diamond search algorithm
*
*/
void EstimateDS(gmcMotionEstimator* pMEStruct);

/**
**************************************************************************
* \brief Applies motion estimation using proposed algorithm
*
*/
void EstimateNewMSU_MB(gmcMotionEstimator* pMEStruct);

/**
**************************************************************************
* \brief Gets motion vector for specified pixel
*
*  Gets motion vector for specified pixel on the frame with pixel accuracy
*
* \param cMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* \param x                   [IN]  -  horizontal coordinate of pixel
* \param y                   [IN]  -  vertical coordinate of pixel
* \param motionx             [OUT] -  first coordinate of corresponding vector
* \param motiony             [OUT] -  second coordinate of corresponding vector
* \param motion_dif          [OUT] -  local error of corresponding vector per pixel
* \param iCurBlockSize       [OUT] -  size of current block with the given pixel
*
* \return none
*/
__inline void cGetMotionVector(gmcMotionEstimator* cMEStruct, int x, int y, int *motionx, int *motiony, int *motion_dif, int *iCurBlockSize)
{
	
		cMacroBlock* cMB = cMEStruct->m_CurMBlocks + (y/ME_MACROBLOCK_SIZE)*cMEStruct->m_iMBlocksHor + (x/ME_MACROBLOCK_SIZE);
		if (cMB->m_BlockPartition == cbt_16x16)
		{
			*motionx = cMB->m_DataMV[0].x;
			*motiony = cMB->m_DataMV[0].y;
			*motion_dif = cMB->m_DataMV[0].error;
			*iCurBlockSize = 16;
		}
		else if (cMB->m_BlockPartition == cbt_8x8)
		{
			int iIndex = ((y&ME_MACROBLOCK_SIZE_M1)/cMB->m_BlockPartition) * (ME_MACROBLOCK_SIZE/cMB->m_BlockPartition) +
			             ((x&ME_MACROBLOCK_SIZE_M1)/cMB->m_BlockPartition);
			if (cMB->m_SubBlockPartition[iIndex] == cbt_8x8)
			{
				*motionx = cMB->m_DataMV[iIndex*4].x;
				*motiony = cMB->m_DataMV[iIndex*4].y;
				*motion_dif = cMB->m_DataMV[iIndex*4].error << 2;
				*iCurBlockSize = 8;
			}
			else if (cMB->m_SubBlockPartition[iIndex] == cbt_4x4)
			{
				iIndex *= 4;
				iIndex += ((y&7)/cbt_4x4) * 2 +
			            ((x&7)/cbt_4x4);
				
				*motionx = cMB->m_DataMV[iIndex].x;
				*motiony = cMB->m_DataMV[iIndex].y;
				*motion_dif = cMB->m_DataMV[iIndex].error << 4;
				*iCurBlockSize = 4;
			}
		}
		else
		{
			assert(FALSE);
		}
	
}

/**
**************************************************************************
* \brief Returns size of border needed around the frame
*
*  This function returns the size of border around the frame needed to
*  motion estimation function to perform motion estimation
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
*
* \return Border size
*/
__inline int cGetBorderSise(gmcMotionEstimator* pMEStruct)
{
	return pMEStruct->m_iBorderSize;
}

/**
**************************************************************************
* \brief Sets motion vectors for previous frame for candidate enhancement
*
*  This function replaces motion vectors for previous frame to make good
*  candidates for the next frame processing
*
* \param pMEStruct           [IN/OUT] - pointer to gmcMotionEstimator structure
* \param pmx                 [IN] -  horizontal components of vectors
* \param pmy                 [IN] -  vertical components of vectors
* \param iBlockSize          [IN] -  block sizes
*
* \return FALSE on error, TRUE on success
*/
__inline BOOL cSetPrvMotionVectors(gmcMotionEstimator* pMEStruct, int *pmx, int *pmy, int iBlockSize)
{
	int i;
	int* pLineX[4];
	int* pLineY[4];
	switch(iBlockSize)
	{
	case cbt_16x16:
		for (i = 0; i < pMEStruct->m_iMBlocksInFrame; i++)
		{
			pMEStruct->m_RefMBlocks[i].m_BlockPartition = cbt_16x16;
			pMEStruct->m_RefMBlocks[i].m_DataMV[0].error = 0;
			pMEStruct->m_RefMBlocks[i].m_DataMV[0].x = *pmx;
			pMEStruct->m_RefMBlocks[i].m_DataMV[0].y = *pmy;
			pmx++;pmy++;
		}
		return TRUE;
		break;
	case cbt_8x8:
		pLineX[0] = pmx; pLineX[1] = pmx + pMEStruct->m_iMBlocksHor;
		pLineY[0] = pmy; pLineY[1] = pmy + pMEStruct->m_iMBlocksHor;
		for (i = 0; i < pMEStruct->m_iMBlocksVert; i++)
		{
			int iRow = i * pMEStruct->m_iMBlocksHor;
			int j;
			for (j = 0; j < pMEStruct->m_iMBlocksHor; j++)
			{
				int k;
				register int iIndex = iRow + j;
				pMEStruct->m_RefMBlocks[iIndex].m_BlockPartition = cbt_8x8;
				for (k = 0; k < 4; k++)
					pMEStruct->m_RefMBlocks[iIndex].m_SubBlockPartition[k] = cbt_8x8;

				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[0].error  = 0;
				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[0].x      = *pLineX[0];
				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[0].y      = *pLineY[0];
				pLineX[0]++;pLineY[0]++;

				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[4].error  = 0;
				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[4].x      = *pLineX[0];
				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[4].y      = *pLineY[0];
				pLineX[0]++;pLineY[0]++;

				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[8].error  = 0;
				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[8].x      = *pLineX[1];
				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[8].y      = *pLineY[1];
				pLineX[1]++;pLineY[1]++;

				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[12].error = 0;
				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[12].x     = *pLineX[1];
				pMEStruct->m_RefMBlocks[iIndex].m_DataMV[12].y     = *pLineY[1];
				pLineX[1]++;pLineY[1]++;
			}
			pLineX[0] += pMEStruct->m_iMBlocksHor << 1;
			pLineX[1] += pMEStruct->m_iMBlocksHor << 1;
			pLineY[0] += pMEStruct->m_iMBlocksHor << 1;
			pLineY[1] += pMEStruct->m_iMBlocksHor << 1;
		}
		return TRUE;
		break;
	case cbt_4x4:
		pLineX[0] = pmx; pLineY[0] = pmy;
		for (i = 1; i < 4; i++)
		{
			pLineX[i] = pLineX[i - 1] + pMEStruct->m_iMBlocksHor;
			pLineY[i] = pLineY[i - 1] + pMEStruct->m_iMBlocksHor;
		}
		for (i = 0; i < pMEStruct->m_iMBlocksVert; i++)
		{
			int iRow = i * pMEStruct->m_iMBlocksHor;
			int j, k;
			for (j = 0; j < pMEStruct->m_iMBlocksHor; j++)
			{
				register int iIndex = iRow + j;
				pMEStruct->m_RefMBlocks[iIndex].m_BlockPartition = cbt_8x8;

				for (k = 0; k < 16; k++)
				{
					int iLineIndex = ((k >> 3) << 1) + ((k & 3) >> 1);
					pMEStruct->m_RefMBlocks[iIndex].m_DataMV[k].error = 0;
					pMEStruct->m_RefMBlocks[iIndex].m_DataMV[k].x     = *pLineX[iLineIndex];
					pMEStruct->m_RefMBlocks[iIndex].m_DataMV[k].y     = *pLineY[iLineIndex];
					pLineX[iLineIndex]++;
					pLineY[iLineIndex]++;
				}

				for (k = 0; k < 4; k++)
					pMEStruct->m_RefMBlocks[iIndex].m_SubBlockPartition[k] = cbt_8x8;
				}
			for (k = 0; k < 4; k++)
			{
				pLineX[k] += pMEStruct->m_iMBlocksHor << 2;
				pLineY[k] += pMEStruct->m_iMBlocksHor << 2;
			}
		}
		return TRUE;
		break;
	default:
		return FALSE;
	}
}


/**
**************************************************************************
* \brief Adding motion candidate
*
*/
void cAddMotionCandidate(gmcMotionEstimator* cMEStruct, int x, int y);

/**
**************************************************************************
* \brief Returns maximal size of buffer needed to contain all MVs
*
*  This function returns maximal size of buffer needed to store motion
*  vectors
*
* \param iBlocksInFrame      [IN} -  number of blocks in frame
*
* \return none
*/
__inline int cGetMVBufferSize(int iBlocksInFrame)
{
	return iBlocksInFrame * 2;
}
/**
**************************************************************************
* \brief Fills user buffer with motion information
*
*/
int cGetMVs(BYTE* dst, cMV* MVectors, int iBlocksInFrame);
/**
**************************************************************************
* \brief Sets motion information to motion estimation object
*
*/
void cSetMVs(signed char* src, cMV* MVectors, int iBlocksInFrame);
/**
**************************************************************************
* \brief Sets pointers to the externally created shifts
*
*/
void cGiveShifts(gmcMotionEstimator* pMEStruct, BYTE** pShifts);
/**
**************************************************************************
* \brief Sets pointers to the externally created chroma shifts
*
*/
void cGiveChromaShifts(gmcMotionEstimator* pMEStruct, BYTE** pShiftsChr1, BYTE** pShiftsChr2);

/**
**************************************************************************
* \brief Initialization for gmcMotionEstimator structure
*
*/
void cInitME(gmcMotionEstimator* cMEStruct);

/**
**************************************************************************
* \brief Initializes internal variables used by ME process
*
*/
int cStartME(gmcMotionEstimator* cMEStruct, int h, int w, cMEParams* pMEParams);

/**
**************************************************************************
* \brief Applies motion estimation using chosen algorithm
*
*/
void cEstimate(gmcMotionEstimator* pMEStruct, BYTE* CurFrame, BYTE* RefFrame, BOOL bBotBase);

/**
**************************************************************************
* \brief Prints configuration of gmcMotionEstimator after processing
*
*/
void PrintConfig(gmcMotionEstimator* cMEStruct, char* sFileName);

/**
**************************************************************************
* \brief Frees resources had been used by ME
*
*/
void cStopME(gmcMotionEstimator* cMEStruct);

/**
**************************************************************************
* \brief Change the size of MV search area
*
*/
void cChangeMotionArea(gmcMotionEstimator* cMEStruct, int iMaxMotHor, int iMaxMotVert);


void CalculateError(gmcMotionEstimator* pMEStruct);


/**
**************************************************************************
* \brief Returns the flag indicating if ME uses chroma.
*
*  This function returns the flag indicating if ME uses chroma.
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
*
* \return TRUE, if chroma is used in ME, FALSE otherwise
*/
__inline BOOL cGetConsiderChromaME(gmcMotionEstimator* pMEStruct)
{
    return pMEStruct->m_pParams->m_advMEParams.m_bConsiderChroma;
}

/**
**************************************************************************
* \brief Sets ME chroma using state
*
*  This function returns the state of ME chroma using.
*
* \param pMEStruct           [IN] -  pointer to gmcMotionEstimator structure
* \param bUseChroma          [IN] -  flag indicating if ME should use chroma
*
* \return none
*/
__inline void cSetConsiderChroma(gmcMotionEstimator* pMEStruct, BOOL bUseChroma)
{
    pMEStruct->m_pParams->m_advMEParams.m_bConsiderChroma = bUseChroma;
}

#endif

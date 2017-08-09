/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cGetError.h
*  \brief File with calculation of block difference functions
*
*  This file contains difinitions of the functions that performs
*  calculations of difference between blocks and data structures,
*  used by this functions.
*
*  $Id: cGetError.h,v 1.16.10.2.2.1 2008/08/01 13:35:27 10a_moi Exp $
*  $Log: cGetError.h,v $
*  Revision 1.16.10.2.2.1  2008/08/01 13:35:27  10a_moi
*  ly shifts calculation implemente*
*  Revision 1.16.10.3  2008/07/24 18:04:27  10i_tsv
*  UVsoft headers are added in some files
*  @
*  text
*  @d10 62
*  @
*
*
*  1.16.*
*  Revision 1.16.10.1  2008/07/12 18:09:05  10i_tsv
*  UVsoft headers are added in some files
*  @
*  text
*  @d10 62
*  @
*
*
*  1.16.6.2
*  log
*  @Y*
*  Revision 1.16  2007/06/28 08:22:46  05s_gri
*  Headers are changed on YUVsoft headers
*
*  Revision 1.15  2007/06/05 12:29:15  05s_gri
*  Intel compiler v9.0 warnings removed
*
*  Revision 1.14  2007/06/04 14:40:40  05s_gri
*  Doxygen completed. Doxygen warnings are not fixed.
*
*  Revision 1.13  2007/04/22 20:15:47  05s_put
*  Add/fix doxygen comments
*
*  Revision 1.12  2007/04/12 07:31:48  05s_gri
*  MMX version of 4x4 block error function added.
*
*  Revision 1.11  2007/03/05 14:19:07  05s_put
*  - interface refactoring
*  - memory fix
*
*  Revision 1.3  2006/11/14 16:12:06  09k_sim
*  GetErrorSAD_l_16x8_MMX was added.
*
*  Revision 1.2  2006/11/13 10:24:53  05s_put
*  New version of compensation
*
*  Revision 1.10  2006/10/31 18:33:24  09k_sim
*  Refactoring of MInfoStruct: cCodecInfo struct was added
*
*  Revision 1.9  2006/10/30 21:55:02  09k_sim
*  m_iFrameNum* was renamed to m_iPicNum*
*
*  Revision 1.8  2006/10/30 21:50:23  09k_sim
*  m_iPicNumProcOrder member was added to MInfoStruct
*
*  Revision 1.7  2006/10/13 17:46:56  09k_sim
*  m_MEDir was added to MInfoStruct
*
*  Revision 1.6  2006/10/08 20:22:54  09k_sim
*  MV predictor was added to MInfoStruct
*
*  Revision 1.5  2006/10/01 17:00:23  09k_sim
*  Current block coordinates (MInfoStruct) initialization added.
*
*  Revision 1.4  2006/09/12 01:46:00  09k_sim
*  Pointer to RDO data was added to library data structures.
*
*  Revision 1.3  2006/09/09 13:24:57  09k_sim
*  Began to add codec support to the library.
*  MPEG2 ME is supported.
*
*  Revision 1.2  2006/07/13 07:51:30  05s_put
*  Fixed error
*
*/

#ifndef __C_GET_ERROR_H__
#define __C_GET_ERROR_H__

#include "cMV.h"

/*! \enum EstimationPrecision
*   \brief Precision of ME and MC
*
*  Describes precision of ME and MC
*/
typedef enum
{
	OnePixel = 0,
	HalfPixel = 1,
	QuarterPixel = 2
} EstimationPrecision;

/*! \enum cPictureStructure
*   \brief Structures of pictures
*
*  Describes structures of pictures
*/
typedef enum
{
  PS_TOP_FIELD     = 1,
  PS_BOTTOM_FIELD  = 2,
  PS_FRAME = 3
} cPictureStructure;

/*! \enum cPictureType
*   \brief Types of pictures
*
*  Describes types of pictures
*/
typedef enum 
{
	PT_I_PICTURE=1,
	PT_P_PICTURE=2,
	PT_B_PICTURE=3
} cPictureType;

/*! \enum cMacroblockTypes
*   \brief Types of macroblocks
*
*  Describes types of macroblocks
*/
typedef enum
{
	MBT_SKIP=0,
	MBT_INTRA=1,
	MBT_PATTERN=2,
	MBT_BACKWARD=4,
	MBT_FORWARD=8,
	MBT_QUANT=16
} cMacroblockTypes;


#define ME_FWD 0
#define ME_BWD 1

/*! \struct cCodecInfo
*   \brief Structure of codec information
*
*  This structure contains information from the codec for advanced functions of
*  motion error computation (RDO)
*/
typedef struct
{
	void* m_pRDOinfo;                                           //!< Pointer to additional info used by RDO error functions
	int m_iPicNumProcOrder;                                     //!< currently processed picture number in the codec processing order
	cMV* m_pMVPredictor;                                        //!< Pointer to the motion vector difference with which is coded (for gmcCodecME)
	cPictureStructure m_PicStruct;                              //!< currently processed picture structure
	cPictureType m_PicType;                                     //!< currently processed picture type

} cCodecInfo;

/*! \struct MInfoStruct
*   \brief Structure of information for advanced error functions
*
*  This structure contains information for advanced functions of
*  motion error computation
*/	
typedef struct
{
	int m_iPicNum;                                              //!< currently processed picture number in the display order
	cMV m_CurMV;                                                //!< currently processed motion vector
	cMV m_MeanMV;                                               //!< mean motion vector of currently processed area
	cMV* pPrvMVs;                                               //!< array of motion vectors of previous frame
	cMV* pCurMVs;                                               //!< array of motion vectors of current frame
	int m_iBlocksHor;                                           //!< quantity of blocks in the line of frame
	int m_iBlocksVert;                                          //!< quantity of blocks in the column of frame
	int m_iX;                                                   //!< horizontal coordinate of currently processed block
	int m_iY;                                                   //!< vertical coordinate of currently processed block
	int m_iBlX;                                                 //!< column of currently processed block
	int m_iBlY;                                                 //!< row of currently processed block
	BOOL m_bBotBase;                                            //!< TRUE if bottom lines contains previous field during interlaced processing
	BYTE* m_pRefBlockChr1;                                      //!< pointer to the current block first chroma component in reference frame
	BYTE* m_pRefBlockChr2;                                      //!< pointer to the current block second chroma component in reference frame
	cFieldType m_refField;                                      //!< Reference field type in the case of interlaced scan
	//cFieldType m_curField;                                    //!< Current field type in the case of interlaced scan
	EstimationPrecision m_PrecMX, m_PrecMY;                     //!< precision of horizontal and vertical motion
	BOOL m_bFieldInFrame;                                       //!< TRUE if field prediction in frame picture is performed, MPEG2 standard
	float m_fFrameAverageSADsPerPixel;                          //!< Average SAD value per pixel in the current frame
	int m_MEDir;                                                //!< Current ME direction ME_FWD/ME_BWD

	cCodecInfo m_CodecInfo;                                     //!< Information from codec
	
} MInfoStruct;

#ifdef USE_ASM64

#define GetErrorSAD_16x16_MMX	GetErrorSAD_16x16_MMX64
#define GetErrorSAD_16x8_MMX	GetErrorSAD_16x8_MMX64
#define GetErrorSAD_8x8_MMX		GetErrorSAD_8x8_MMX64
#define GetErrorSAD_4x4_MMX		GetErrorSAD_4x4_MMX64

#define GetErrorSAD_16x16_H264_MMX	GetErrorSAD_16x16_H264_MMX64
#define GetErrorSAD_16x8_H264_MMX	GetErrorSAD_16x8_H264_MMX64
#define GetErrorSAD_8x8_H264_MMX	GetErrorSAD_8x8_H264_MMX64
#define GetErrorSAD_4x4_H264_MMX	GetErrorSAD_4x4_H264_MMX64

#define GetErrorSAD_Int_16x16_MMX	GetErrorSAD_Int_16x16_MMX64
#define GetErrorSAD_Int_8x8_MMX		GetErrorSAD_Int_8x8_MMX64

#else

#ifndef USE_ASM32
#define USE_ASM32
#endif

#define GetErrorSAD_16x16_MMX	GetErrorSAD_16x16_MMX32
#define GetErrorSAD_16x8_MMX	GetErrorSAD_16x8_MMX32
#define GetErrorSAD_8x8_MMX		GetErrorSAD_8x8_MMX32
#define GetErrorSAD_4x4_MMX		GetErrorSAD_4x4_MMX32

#define GetErrorSAD_16x16_H264_MMX	GetErrorSAD_16x16_H264_MMX32
#define GetErrorSAD_16x8_H264_MMX	GetErrorSAD_16x8_H264_MMX32
#define GetErrorSAD_8x8_H264_MMX	GetErrorSAD_8x8_H264_MMX32
#define GetErrorSAD_4x4_H264_MMX	GetErrorSAD_4x4_H264_MMX32

#define GetErrorSAD_Int_16x16_MMX	GetErrorSAD_Int_16x16_MMX32
#define GetErrorSAD_Int_8x8_MMX		GetErrorSAD_Int_8x8_MMX32

#endif // USE_ASM64


#ifdef USE_ASM64

/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks
*
*/
long GetErrorSAD_16x16_MMX64(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo);

/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks with on fly H264-interpolation
*
*/
long GetErrorSAD_16x16_H264_MMX64(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY);

/**
**************************************************************************
* \brief Calculates SAD between 16x8 blocks
*
*/
long GetErrorSAD_16x8_MMX64(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo);

/**
**************************************************************************
* \brief Calculates SAD between 16x8 blocks with on fly H264-interpolation
*
*/
long GetErrorSAD_16x8_H264_MMX64(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY);

/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks
*
*/
long GetErrorSAD_8x8_MMX64(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);

/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks with on fly H264-interpolation
*
*/
long GetErrorSAD_8x8_H264_MMX64(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY);

/**
**************************************************************************
* \brief Calculates SAD between 4x4 blocks
*
*/
long GetErrorSAD_4x4_MMX64(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);

/**
**************************************************************************
* \brief Calculates SAD between 4x4 blocks with on fly H264-interpolation
*
*/
long GetErrorSAD_4x4_H264_MMX64(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY);

/**
**************************************************************************
* \brief Calculates SAD between 16x16 interlaced blocks
*
*/
long GetErrorSAD_Int_16x16_MMX64(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 16x16 interlaced blocks
*
*/
//long GetErrorSAD_Int_16x8_MMX64(const BYTE* block1, const BYTE* block2, const int stride, const MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 8x8 interlaced blocks
*
*/
long GetErrorSAD_Int_8x8_MMX64(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);

#endif // USE_ASM64

#ifdef USE_ASM32

/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks
*
*/
long GetErrorSAD_16x16_MMX32(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo);

/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks with on fly H264-interpolation
*
*/
long GetErrorSAD_16x16_H264_MMX32(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY);

/**
**************************************************************************
* \brief Calculates SAD between 16x8 blocks
*
*/
long GetErrorSAD_16x8_MMX32(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo);

/**
**************************************************************************
* \brief Calculates SAD between 16x8 blocks with on fly H264-interpolation
*
*/
long GetErrorSAD_16x8_H264_MMX32(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY);

/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks
*
*/
long GetErrorSAD_8x8_MMX32(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);

/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks with on fly H264-interpolation
*
*/
long GetErrorSAD_8x8_H264_MMX32(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY);

/**
**************************************************************************
* \brief Calculates SAD between 4x4 blocks
*
*/
long GetErrorSAD_4x4_MMX32(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);

/**
**************************************************************************
* \brief Calculates SAD between 4x4 blocks with on fly H264-interpolation
*
*/
long GetErrorSAD_4x4_H264_MMX32(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY);

/**
**************************************************************************
* \brief Calculates SAD between 16x16 interlaced blocks
*
*/
long GetErrorSAD_Int_16x16_MMX32(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 16x16 interlaced blocks
*
*/
//long GetErrorSAD_Int_16x8_MMX32(const BYTE* block1, const BYTE* block2, const int stride, const MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 8x8 interlaced blocks
*
*/
long GetErrorSAD_Int_8x8_MMX32(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);

#endif // USE_ASM32

/**
**************************************************************************
* \brief Calculates SAD between 32x32 blocks
*
*/
long GetErrorSAD_32x32_MMX(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo);

/**
**************************************************************************
* \brief Calculates SAD between 32x32 blocks
*
*/
long GetErrorSADS_32x32_MMX(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks
*
*/
long GetErrorSADS_16x16_MMX(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks
*
*/
long GetErrorSADS_8x8_MMX(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);

/**
**************************************************************************
* \brief Calculates SAD between 32x32 blocks
*
*/
long GetErrorSAD_32x32_C(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks
*
*/
long GetErrorSAD_16x16_C(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks
*
*/
long GetErrorSAD_8x8_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 4x4 blocks
*
*/
long GetErrorSAD_4x4_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 2x2 blocks
*
*/
long GetErrorSAD_2x2_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 2x2 blocks with on fly H264-interpolation
*
*/
long GetErrorSAD_2x2_H264_C(const BYTE* blockRef, const int strideRef, BYTE** pSrcShifts, int iFirstRowOffset, int iStride, int iX, int iY, int iMX, int iMY);


/**
**************************************************************************
* \brief Calculates SM between 16x16 blocks
*
*/
long GetErrorSM_16x16_C(const BYTE *block1, const BYTE *block2, const int stride, MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SM between 8x8 blocks
*
*/
long GetErrorSM_8x8_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);


/**
**************************************************************************
* \brief Calculates mixed error between 16x16 blocks
*
*  This function implements calculating of mixed error between two 16x16
*  blocks in two equal frames
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
*
* \return mixed difference between two blocks
*/
__inline long GetErrorMix_16x16(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	return GetErrorSAD_16x16_MMX(block1, block2, stride, pMInfo) + 2*GetErrorSM_16x16_C(block1, block2, stride, pMInfo);
}
/**
**************************************************************************
* \brief Calculates mixed error between 8x8 blocks
*
*  This function implements calculating of mixed error between two 8x8
*  blocks in two equal frames
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
*
* \return mixed difference between two blocks
*/
__inline long GetErrorMix_8x8(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	return GetErrorSAD_8x8_MMX(block1, block2, stride, pMInfo) + 2*GetErrorSM_8x8_C(block1, block2, stride, pMInfo);
}


/**
**************************************************************************
* \brief  Calculates SAD between 16x16 blocks, using logo mask
*
*/
long GetErrorSADLogo_16x16_C(const BYTE* block1, const BYTE* block2, const int stride, const BYTE* pMask, int MaskW);
/**
**************************************************************************
* \brief  Calculates SAD between 8x8 blocks, using logo mask
*
*/
long GetErrorSADLogo_8x8_C(const BYTE* block1, const BYTE* block2, const int stride, const BYTE* pMask, int MaskW);
/**
**************************************************************************
* \brief Calculates SAD between 8x8 blocks for color components, using logo mask
*
*/
long GetErrorSADLogoColor_8x8_C(const BYTE* block1, const BYTE* block2, const int stride, const BYTE* pMask, int MaskW);


/**
**************************************************************************
* \brief Calculates SAD between 32x32 interlaced blocks
*
*/
long GetErrorSAD_Int_32x32_MMX(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);

/**
**************************************************************************
* \brief Calculates SAD between 16x16 interlaced blocks
*
*/
long GetErrorSAD_Int_16x16_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);
/**
**************************************************************************
* \brief Calculates SAD between 8x8 interlaced blocks
*
*/
long GetErrorSAD_Int_8x8_C(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo);


/**
**************************************************************************
* \brief Calculates SAD between blocks of different sizes
*
*/
long GetErrorSAD_C(const BYTE* pBlock1, const BYTE* pBlock2, const int iStride1, const int iStride2, cMacroBlockPartitions BlockSize, MInfoStruct* pMInfo);


/**
**************************************************************************
* \brief Calculates SAD between 16x16 blocks and accounts vector length
*
*  This function implements calculating of sun of absolute differences
*  between two 16x16 blocks in two equal frames and also takes in account
*  motion vector length
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
*
* \return mixed difference between two blocks
*/
__inline long GetErrorSAD_l_16x16_MMX(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	return GetErrorSAD_16x16_MMX(block1, block2, stride, pMInfo) +
	       (abs(pMInfo->m_CurMV.x << 2) >> (pMInfo->m_PrecMX)) +
	       (abs(pMInfo->m_CurMV.y << 2) >> (pMInfo->m_PrecMY));
}

/**
**************************************************************************
* \brief Calculates SAD between 16x8 blocks and accounts vector length
*
*  This function implements calculating of sun of absolute differences
*  between two 16x8 blocks in two equal frames and also takes in account
*  motion vector length
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
*
* \return mixed difference between two blocks
*/
__inline long GetErrorSAD_l_16x8_MMX(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	return GetErrorSAD_16x8_MMX(block1, block2, stride, pMInfo) +
	       (abs(pMInfo->m_CurMV.x << 2) >> (pMInfo->m_PrecMX + 1)) +
	       (abs(pMInfo->m_CurMV.y << 2) >> (pMInfo->m_PrecMY + 1));
}

/**
**************************************************************************
* \brief Calculates mixed error between 8x8 blocks
*
*  This function implements calculating of mixed error between two 8x8
*  blocks in two equal frames
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
*
* \return mixed difference between two blocks
*/
__inline long GetErrorSAD_l_8x8_MMX(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	return GetErrorSAD_8x8_MMX(block1, block2, stride, pMInfo) +
	       (abs(pMInfo->m_CurMV.x << 2) >> (pMInfo->m_PrecMX + 2)) +
	       (abs(pMInfo->m_CurMV.y << 2) >> (pMInfo->m_PrecMY + 2));
}
/**
**************************************************************************
* \brief Calculates mixed error between 4x4 blocks
*
*  This function implements calculating of mixed error between two 8x8
*  blocks in two equal frames
*
* \param block1           [IN] -  pointer to the first block
* \param block2           [IN] -  pointer to the second block
* \param stride           [IN] -  length of frame line in bytes
* \param pMInfo           [IN] -  pointer to the structure with additional information
*
* \return mixed difference between two blocks
*/
__inline long GetErrorSAD_l_4x4_MMX(const BYTE* block1, const BYTE* block2, const int stride, MInfoStruct* pMInfo)
{
	return GetErrorSAD_4x4_C(block1, block2, stride, pMInfo) +
	       (abs(pMInfo->m_CurMV.x << 2) >> (pMInfo->m_PrecMX + 4)) +
	       (abs(pMInfo->m_CurMV.y << 2) >> (pMInfo->m_PrecMY + 4));
}

/*! \typedef BlockDiffFunction
*   \brief Function, that calculate difference between blocks
*
*  So called block difference measure function type definition, computes
*  difference between two blocks
*/	
typedef long (*BlockDiffFunction) (const BYTE*, const BYTE*, const int, MInfoStruct*);

#endif // __C_GET_ERROR_H__

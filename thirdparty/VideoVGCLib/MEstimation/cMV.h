/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cMV.h
*  \brief File with implementation of motion vector
*
*  This file contains implementation of motion vector data
*  structure, data structures, formed from it and some definitions
*  and implementations of function of processing this data
*  structures
*
*/

#ifndef __C_MV_H__
#define __C_MV_H__

#include <windows.h>
#include <limits.h>

/*! \enum cFieldType
*   \brief Types of fields
*
*  Describes types of fields in the case of interlaced scan
*/
typedef enum
{
    FT_TOP,
    FT_BOTTOM
} cFieldType;

/*! \struct cMV
*   \brief Structure for containing motion vector
*
*  This structure contains motion vector data
*/	
typedef struct
{
    cFieldType field;											//!< indicates the field to compensate from in the case of interlaced scan
    int x, y;                                                   //!< coordinates of vector
    long error;                                                 //!< error of the vector
} cMV;

/*! \var NULL_MV
*   \brief Null motion vector
*
*  Null motion vector, pointing to the top field, with maximum error value. 
*/
static const cMV NULL_MV = {FT_TOP,0,0,INT_MAX};

/*! \enum cMacroBlockPartitions
*   \brief Types of macro blocks
*
*  Describes types of macro blocks, used for ME and MC
*/
typedef enum			//compatible with old code
{
    cbt_16x8,
    cbt_16x16 = 16,
    cbt_8x8 = 8,
    cbt_4x4 = 4
} cMacroBlockPartitions;

/*! \var MBwidth
*   \brief Macroblock widths
*
*  Array of macroblock widths. 
*/
static const int MBwidth[]={16,0,0,0,4,0,0,0,8,0,0,0,0,0,0,0,16};

/*! \var MBheight
*   \brief Macroblock heights
*
*  Array of macroblock heights. 
*/
static const int MBheight[]={8,0,0,0,4,0,0,0,8,0,0,0,0,0,0,0,16};

/**
**************************************************************************
* \brief Initialization of cMV
*
*  This function initializes cMV structure with default values
*
* \param cMVStruct   [IN/OUT] - pointer to cMacroBlock structure
* 
* \return none
*/
__inline void cInitMV(cMV* cMVStruct)
{
    cMVStruct->field = FT_TOP;
    cMVStruct->x = cMVStruct->y = 0;
    cMVStruct->error = MAXLONG;
}

/**
**************************************************************************
* \brief Calculates distance between two vectors
*
*  This function calculates distance between two vectors as sum of
*  absolute differences of their components
*
* \param mv1                 [IN] -  pointer to the first block
* \param mv2                 [IN] -  pointer to the second block
* 
* \return distance between two vectors
*/
__inline int MVABS(const cMV* mv1, const cMV* mv2)
{
    return abs(mv1->x - mv2->x) + abs(mv1->y - mv2->y);
}

/*! \struct tagMVList
*   \brief Structure for making list of motion vectors
*
*  This structure allows to construct list of motion vectors
*/	
typedef struct tagMVList
{
    cMV m_DataMV;                                               //!< data
    struct tagMVList* m_pNext;                                  //!< pointer to the next element of the list
    BOOL m_bUsed;                                               //!< TRUE if data is correctly filled
} cMVList;

/*! \struct cBlock
*   \brief Structure for storing block data
*
*  This structure contains block data
*/    
typedef struct cBlock
{
    cMacroBlockPartitions m_Size;  //!< block size
    BOOL m_bSplit;              //!< TRUE if block is split into sub-blocks
    struct cBlock *m_subBlocks; //!< array of sub-blocks
    cMV m_MV;                   //!< block motion vector
    cMVList m_MVList;           //!< block candidate vector list
    float m_fVar;               //!< block luminance variance
    INT32 m_iX;                 //!< block horizontal coordinate
    INT32 m_iY;                 //!< block vertical coordinate
} cBlock;

/**
**************************************************************************
* \brief Initialization of cMVList (one node)
*
*  This function initializes cMVlist structure (one node) with default
*  values
*
* \param cMVListStruct       [IN/OUT] - pointer to cMVList structure
* 
* \return none
*/
__inline void cInitMVListNode(cMVList* cMVListStruct)
{
    cMVListStruct->m_pNext = NULL;
    cMVListStruct->m_bUsed = FALSE;
    cInitMV(&cMVListStruct->m_DataMV);
}

/**
**************************************************************************
* \brief Initialization of cMVList
*
*/
int cInitMVList( cMVList* pList, int iCount );

/**
**************************************************************************
* \brief De-initialization of cMVList
*
*/
void cDeInitMVList(cMVList* cMVListStruct);

#endif // __C_MV_H__
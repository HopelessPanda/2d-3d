/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cMV.c
*  \brief File with implementation of motion vector
*
*  This file contains implementation of motion vector data
*  structure, data structures, formed from it and some definitions
*  and implementations of function of processing this data
*  structures
*
*/

#include "cMV.h"


/**
**************************************************************************
* \brief Initialization of cMVList
*
*  This function initializes cMVlist structure with default values and
*  iCount of elements
*
* \param pList               [IN/OUT] - pointer to cMVList structure
* \param iCount              [IN]   - quantity of elements to add to the list
* 
* \return 0 if no errors, -1 otherwise
*/
int cInitMVList( cMVList* pList, int iCount )
{
    cMVList* pCurNode = pList;
    int iCurNode = 0;
    for (iCurNode = 0; iCurNode < iCount; iCurNode++)
    {
        pCurNode->m_pNext = ( cMVList* )malloc( sizeof( cMVList ) );
        if (!pCurNode->m_pNext) return -1;
        cInitMVListNode( pCurNode->m_pNext );
        pCurNode = pCurNode->m_pNext;
    }

    return 0;
}

/**
**************************************************************************
* \brief De-initialization of cMVList node by node
*
*  This function de-initializes cMVlist structure with default values
*
* \param cMVListStruct       [IN/OUT] - pointer to cMVList structure
* 
* \return none
*/
void cDeInitMVList(cMVList* cMVListStruct)
{
	cMVList* pCurNode = cMVListStruct->m_pNext;
	while (pCurNode != NULL)
	{
		cMVList* pNodeToFree = pCurNode;
		pCurNode = pCurNode->m_pNext;
		free( pNodeToFree );
	}
	cInitMVListNode(cMVListStruct);
}
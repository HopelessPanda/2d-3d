/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cSimpleMEAlgs.h
*  \brief File File with definitions of simple ME algorithms
*
*  This file contains definitions of simple motion estimation
*  algorithms functions, such as full search, 3DRS, E3DRS,
*  hierarchic search, logarithmic search and so on.
*
*  $Id: cSimpleMEAlgs.h,v 1.9 2007/06/28 08:22:46 05s_gri Exp $
*  $Log: cSimpleMEAlgs.h,v $
*  Revision 1.9  2007/06/28 08:22:46  05s_gri
*  Headers are changed on YUVsoft headers
*
*  Revision 1.8  2007/06/04 14:40:41  05s_gri
*  Doxygen completed. Doxygen warnings are not fixed.
*
*  Revision 1.7  2007/04/22 20:15:47  05s_put
*  Add/fix doxygen comments
*
*  Revision 1.6  2007/03/05 14:19:08  05s_put
*  - interface refactoring
*  - memory fix
*
*  Revision 1.3  2006/12/20 16:56:52  05s_gri
*  Code doxygenning is continued
*
*  Revision 1.2  2006/11/13 10:24:53  05s_put
*  New version of compensation
*
*  Revision 1.3  2006/09/09 13:24:57  09k_sim
*  Began to add codec support to the library.
*  MPEG2 ME is supported.
*
*  Revision 1.2  2006/09/04 15:25:19  09k_sim
*  Bugs fixed
*
*  Revision 1.1  2006/09/04 14:31:29  09k_sim
*  Simple ME functions' definitions
*
*
*
*/

#ifndef __C_SIMPLE_ME_ALGS_H__
#define __C_SIMPLE_ME_ALGS_H__

void LocalFSA(gmcMotionEstimator* pMEStruct, int iX, int iY, int iYStart, int iYEnd, int iXStart, int iXEnd, int iStepX, int iStepY,
							cMV* CurrentVector, cMacroBlockPartitions BlockSize, BOOL bLocalRefinement);

void LocalHierarchicSearch(gmcMotionEstimator* pMEStruct, int X, int Y, cMV* BestVector, int iBlockSize);

#endif // __C_SIMPLE_ME_ALGS_H__
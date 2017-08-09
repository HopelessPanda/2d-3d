/*
********************************************************************
(c) YUVsoft Corp., 2007, www.yuvsoft.com

This code can be distributed and used only with WRITTEN PERMISSION of
YUVsoft.
********************************************************************
*/

/**
*  \file cProposedME.h
*  \brief File with definitions of proposed ME functions
*
*  This file contains difinitions of the functions that performs
*  motion estimation using proposed (MSU) algorithm
*
*  $Id: cProposedME.h,v 1.8 2007/06/28 08:22:46 05s_gri Exp $
*  $Log: cProposedME.h,v $
*  Revision 1.8  2007/06/28 08:22:46  05s_gri
*  Headers are changed on YUVsoft headers
*
*  Revision 1.7  2007/06/04 14:40:40  05s_gri
*  Doxygen completed. Doxygen warnings are not fixed.
*
*  Revision 1.6  2007/03/05 14:19:08  05s_put
*  - interface refactoring
*  - memory fix
*
*  Revision 1.1.2.1  2006/12/22 09:56:31  05s_put
*  Calculation of means and variances was added.
*  More*
*  Revision 1.3  2006/12/20 16:56:52  05s_gri
*  Code doxygenning is continued
*
*  Revision 1.2  2006/12/18 13:46:12  05s_put
*  - doxygen comments fixed
*
*  Revision 1.1  2006/11/13 10:29:12  05s_put
*  adding new file
*
*  Revision 1.5  2006/10/13 17:48:20  09k_sim
*  Began to add field processing.
*
*  Revision 1.4  2006/10/08 18:30:12  09k_sim
*  Refactoring.
*
*  Revision 1.3  2006/09/24 10:29:59  09k_sim
*  Some refactoring concerning inertia MV candidates was done (needed by gmCodecME).
*
*  Revision 1.2  2006/09/22 10:48:22  09k_sim
*  Better multiframe support was added.
*
*  Revision 1.1  2006/09/09 13:26:56  09k_sim
*  Began to add codec support to the library.
*  MPEG2 ME is supported.
*
*
*/

cMV LocalDecideMotion(gmcMotionEstimator* pMEStruct, int iBY, int iBX, int iRefFrame, int field);
cMV LocalNewMSU_MB(gmcMotionEstimator* pMEStruct, int iY, int iX, int iBY, int iBX, cMacroBlockPartitions BlockSize, cMV CurrentVector);
void ClearMVCand(cMVList* pMVList);
void RemoveUnusedMVCand(cMVList* pMVList, int keepPrealloc);
void AddCandidatesFromPrvFrame(gmcMotionEstimator* pMEStruct, int iRefFrame, int field);
void AddCandidateMV(cMVList* MVCandArr, const cMV* CandMV, int iBlX, int iBlY, int iBlocksHor);
void SwapAndCleanCands(gmcMotionEstimator* pMEStruct,int iRefFrame);
void DecideNextFrameCandidate(gmcMotionEstimator* pMEStruct, const cMV* CandMV, int iX, int iY, int iRefFrame, int field);

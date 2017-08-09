/*
********************************************************************
(c) MSU Video Group 2003-2005, http://compression.ru/video/
This source code is property of MSU Graphics and Media Lab

This code can be distributed and used only with WRITTEN PERMISSION.
********************************************************************
*/

/**
*  \file MEstimation.h
*  \brief Performs motion estimation
*
*  This file contains declarations of the class that performs
*  motion estimation in video.
*
*  $Id: MEstimation.h,v 1.49 2007/04/22 20:15:47 05s_put Exp $
*  $Log: MEstimation.h,v $
*  Revision 1.49  2007/04/22 20:15:47  05s_put
*  Add/fix doxygen comments
*
*  Revision 1.48  2007/03/05 14:19:07  05s_put
*  - interface refactoring
*  - memory fix
*
*  Revision 1.44.2.4  2007/02/07 17:43:57  05s_put
*   std::string usage *
*  Revision 1.44.2.3  2006/12/22 09:56:31  05s_put
*   std::string usage in ME instead of CString
*  @
*  text*
*  Revision 1.47  2006/12/20 17:36:07  05s_put
*  - doxygen comments fix
*  - small code cleanup
*
*  Revision 1.46  2006/12/18 13:46:12  05s_put
*  - doxygen comments fixed
*
*  Revision 1.45  2006/11/23 10:59:59  05s_put
*  - fixed cSetPrvMVs for 4x4 blocks
*  - made function, that makes possible give ME externally made half-pixel shifts
*  - fixed negative error values
*  - fixed warning with unreferenced parameter
*
*  Revision 1.44  2006/11/13 10:24:53  05s_put
*  New version of compensation
*
*  Revision 1.4  2006/09/09 13:24:57  09k_sim
*  Began to add codec support to the library.
*  MPEG2 ME is supported.
*
*  Revision 1.3  2006/07/19 23:49:41  09k_sim
*  PrintConfig is now called from gmMotionEstimator::Stop
*
*  Revision 1.2  2006/07/13 07:51:30  05s_put
*  Fixed error
*
*  Revision 1.47  2006/06/13 10:46:08  05s_put
*  Some vector field enhancements
*
*  Revision 1.46  2006/06/12 23:38:38  05s_put
*  Another variance of dispersion calculation
*
*  Revision 1.45  2006/06/08 17:30:03  05s_put
*  Made H264-like shifts calculations, made separate project file for
*  ME core written on "C"
*
*  Revision 1.44  2006/05/25 17:42:55  05s_put
*  Made chroma consideration for YV12 colorspace
*
*  Revision 1.43  2006/05/19 17:34:24  05s_put
*  Began to make variable block sizes
*
*  Revision 1.42  2006/05/18 17:05:51  05s_put
*  Saving current state: began to make variable block sizes - 16x16, 8x8 and 4x4
*
*  Revision 1.41  2006/05/17 12:59:08  05s_put
*  Made C version of MEstimation
*
*  Revision 1.40  2006/05/16 13:46:46  05s_put
*  Remaking project system
*
*  Revision 1.39  2006/05/11 05:32:48  05s_put
*  Made almost everything for codec
*
*  Revision 1.38  2006/04/27 15:17:51  05s_put
*  All algorithms fixed, made some project files changes
*
*  Revision 1.37  2006/04/13 17:11:36  05s_put
*  First version of YV12 compensation
*
*  Revision 1.36  2006/04/06 13:33:08  05s_put
*  Made another one smooth function
*
*  Revision 1.35  2006/04/05 13:36:21  05s_put
*  Saving version with color support
*
*  Revision 1.34  2006/03/21 18:36:46  05s_put
*  saving current version
*
*  Revision 1.33  2006/03/15 18:19:48  05s_put
*  saving current version of MV candidates filtering
*
*  Revision 1.32  2006/03/14 14:26:16  05s_put
*  Made smarter candidates.
*
*  Revision 1.31  2006/03/13 18:29:58  05s_put
*  Current version save
*
*  Revision 1.30  2006/03/11 17:03:17  05s_put
*  Begining of candidates propogation implementation
*  Smoothing vector field using previous frame
*
*  Revision 1.29  2006/03/09 14:45:49  05s_put
*  - made interlaced processing correct with new smooth vector field functions
*  - uncheked smooth vector function is now uncommented
*
*  Revision 1.28  2006/03/07 15:21:11  05s_put
*  Saving current changes
*
*  Revision 1.27  2006/03/07 07:54:23  05s_put
*  Made a smooth motion vector field function
*
*  Revision 1.26  2006/03/03 07:37:13  05s_put
*  Begin to make MV field dependent error functions.
*
*  Revision 1.25  2006/03/01 13:23:19  05s_put
*  Interlaced processing is ON
*
*  Revision 1.24  2006/02/28 20:01:59  05s_put
*  Commit of interlaced "supporting" version. Still bugs remaining.
*
*  Revision 1.23  2006/02/24 16:06:00  05s_put
*  Made quarter-pixel precision
*
*  Revision 1.22  2006/02/24 10:59:32  05s_put
*  Made quarter pixel GetBlock functuion,
*  made precision settings in dialog window
*
*  Revision 1.21  2006/02/22 17:14:31  05s_put
*  Made a getBlock function to maintain quarter-pixel precision
*
*  Revision 1.20  2006/02/17 15:11:05  05s_put
*  Some bug fixes
*
*  Revision 1.19  2006/02/17 13:34:10  05s_put
*  Made hierarchic post search
*
*  Revision 1.18  2006/02/15 08:01:01  05s_put
*  I've begun to make bad compensated blocks detection
*
*  Revision 1.17  2006/02/09 10:25:38  05s_put
*  Made 32x32 block sizes, made "FRC performance"
*
*  Revision 1.16  2006/02/08 08:18:35  05s_put
*  Saving current state
*
*  Revision 1.15  2006/02/01 11:12:52  05s_put
*  Saving current state
*
*  Revision 1.14  2006/01/31 01:22:47  05s_put
*  Made MV showing as a parameter in configuration window,
*  made hierarchic search function
*
*  Revision 1.13  2006/01/30 20:51:27  05s_put
*  Made motion vector visualization.
*
*  Revision 1.12  2006/01/26 12:48:36  05s_put
*  Done with "dispersion" calculation
*
*  Revision 1.11  2006/01/24 21:21:24  05s_put
*  Saving current state
*
*  Revision 1.10  2006/01/24 01:20:32  05s_put
*  Saving current state
*
*  Revision 1.9  2006/01/20 23:59:23  05s_put
*  Correct statistic gathering
*
*  Revision 1.8  2006/01/18 18:28:22  05s_put
*  Almost done statistic gathering
*
*  Revision 1.7  2006/01/18 17:00:51  05s_put
*  Compiled with MFC in MEstimation
*
*  Revision 1.6  2006/01/18 15:20:37  05s_put
*  Added MFC support to MEstimation project
*
*  Revision 1.5  2006/01/18 15:10:40  05s_put
*  Just saving current state
*
*  Revision 1.4  2006/01/16 14:53:01  05s_put
*  My algorithm is working
*
*  Revision 1.3  2006/01/12 18:28:31  05s_put
*  Made normal interface for 3DRS and E3DRS realizations
*
*  Revision 1.2  2006/01/10 17:01:31  05s_put
*  Made 3DRS and E3DRS algoritms
*
*  Revision 1.1  2006/01/09 17:41:37  05s_put
*  First commit
*
*  Revision 1.37  2005/11/16 15:12:10  05m_mah
*  Some Bugs fixed
*
*  Revision 1.36  2005/11/14 19:55:29  05m_mah
*  IYV12 estimation/compensation added
*
*  Revision 1.35  2005/07/04 12:20:26  05s_put
*  Fixed doxygen comments
*
*  Revision 1.34  2005/07/04 12:04:34  05s_put
*  Doxygen comments added
*
*  Revision 1.33  2005/07/04 11:12:17  05s_put
*  Added doxygen comments
*
*  Revision 1.32  2005/07/04 08:12:30  05s_put
*  Fixed licenses
*
*  Revision 1.31  2005/07/03 13:16:33  05m_mah
*  Permissions header added
*
*  Revision 1.30  2005/06/14 07:30:02  05s_put
*  Fixed bug in candidates search (MVectors used instead of MVPrev)
*
*  Revision 1.29  2005/06/07 17:01:54  05s_put
*  Last version
*
*  Revision 1.28  2005/05/26 12:00:10  05s_put
*  Now counting SAD operations
*
*  Revision 1.27  2005/05/25 18:50:30  05m_mah
*  Some features emplemented
*
*  Revision 1.26  2005/05/24 12:38:03  05s_put
*  Added color components usage during estimation of logo
*
*  Revision 1.25  2005/05/23 17:45:17  05s_put
*  Begining of color SAD realization to remove logo
*
*  Revision 1.24  2005/05/16 13:53:14  05s_put
*  Fixed bug with float motion vectors work
*
*  Revision 1.23  2005/05/14 07:44:46  05s_put
*  Some fixes performed, quality improved.
*
*  Revision 1.22  2005/05/11 15:35:22  05s_put
*  Added work with float motion vector coordinates.
*  During SetMotionVector function coordinates aproximated according
*  to current motion compensation accuracy.
*
*  Revision 1.21  2005/04/29 14:04:03  05s_put
*  Some code improvements readability.
*
*  Revision 1.20  2005/04/28 15:04:43  05s_put
*  Removed STL vector usage :(((
*
*  Revision 1.19  2005/04/27 09:53:13  05s_put
*  Made some architecture changes, added C SAD functions
*
*  Revision 1.18  2005/04/26 08:17:06  05s_put
*  Half-pixel for logo removal
*  Fixed colors in YV12
*
*  Revision 1.17  2005/04/25 12:13:28  05s_put
*  COmpensation now working in YV12.
*
*  Revision 1.16  2005/04/25 08:27:44  05s_put
*  Some interface changes, according to "delogo" project requirements.
*
*  Revision 1.15  2005/04/21 12:00:27  05s_put
*  Now we estimates motion for logo using logo mask.
*
*  Revision 1.14  2005/04/21 09:10:16  05s_put
*  FSA for logo works with SAD function without mask for now.
*
*  Revision 1.13  2005/04/18 12:24:59  05s_put
*  Made some "const" declarations
*
*  Revision 1.12  2005/04/18 09:31:32  05s_put
*  Some changes in project files
*  Restored SAD function call in Motion Estimation
*
*  Revision 1.11  2005/04/14 14:35:11  05s_put
*  Added logo work
*
*  Revision 1.10  2005/04/12 17:21:10  05s_put
*  First iteration logo removal support added.
*  Fixed bug, that MVectors was not initialized by 0 in class constructor.
*
*  Revision 1.9  2005/04/08 17:07:33  05s_put
*  Added some more descriptions
*
*  Revision 1.8  2005/04/08 13:14:33  05s_put
*  Some comments added.
*  Added needed interface functionality.
*
*  Revision 1.7  2005/04/08 07:16:33  05s_put
*  Added headers to files of motion estimation and denoising,
*  added support of VS 6.0 to MSUOldFilmRestore and MSUTunerTVRecover
*
*/

#ifndef _MESTIMATION_H_
#define _MESTIMATION_H_

#include <string>

extern "C"
{
#include "cMEstimation.h"
}
#include "MEParams.h"

/*! \class gmMotionEstimator
*   \brief Class for performing ME
*
*  This class implements motion estimation in video
*/	
class gmMotionEstimator : protected gmcMotionEstimator
{
public:

    gmMotionEstimator() 
	{
		cInitME((gmcMotionEstimator*)this);
	};

/**
**************************************************************************
* \brief Initializes internal variables used by ME process
*
*  Initializes all internal variables and allocates necessary memory used
*  by ME process
*
* \param h                   [IN] -  height of frames to be processed (should be multiple of 16)
* \param w                   [IN] -  width of frames to be processed (should be multiple of 16)
* \param pMEParams           [IN] -  parameters of motion estimation algorithm
*
* \return none
*/
	inline void Start(int h, int w, MEParams* pMEParams)
	{
		cStartME((gmcMotionEstimator*)this, h, w, (cMEParams*)pMEParams);
	};

/**
**************************************************************************
* \brief Function of motion estimation
*
*/
	inline void Estimate(BYTE* CurFrame, BYTE* PrevFrame, BOOL bBotBase)
	{
		cEstimate((gmcMotionEstimator*)this, CurFrame, PrevFrame, bBotBase);
	}

/**
**************************************************************************
* \brief Gets motion vector for specified pixel
*
*  Gets motion vector for specified pixel on the frame with pixel accuracy
*
* \param x                   [IN]  -  horizontal coordinate of pixel
* \param y                   [IN]  -  vertical coordinate of pixel
* \param motionx             [OUT] -  first coordinate of corresponding vector
* \param motiony             [OUT] -  second coordinate of corresponding vector
* \param motion_dif          [OUT] -  local error of corresponding vector
* \param iCurBlockSize       [OUT] -  returns block size of corresponding block
*
* \return none
*/
	inline void GetMotionVector(int x, int y, int &motionx, int &motiony, int &motion_dif, int &iCurBlockSize)
	{
		cGetMotionVector((gmcMotionEstimator*)this, x, y, &motionx, &motiony, &motion_dif, &iCurBlockSize);
	}

/**
**************************************************************************
* \brief Sets motion vectors for previous frame for candidate enhancement
*
*  This function replaces motion vectors for previous frame to make good
*  candidates for the next frame processing
*
* \param pmx                 [IN] -  horizontal components of vectors
* \param pmy                 [IN] -  vertical components of vectors
* \param iBlockSize          [IN] -  block sizes
*
* \return FALSE on error, TRUE on success
*/
	inline BOOL SetPrvMotionVectors(int *pmx, int *pmy, int iBlockSize)
	{
		return cSetPrvMotionVectors((gmcMotionEstimator*)this, pmx, pmy, iBlockSize);
	}
	


/**
**************************************************************************
* \brief Returns maximum size of buffer needed to store MVs
*
*/
	inline int GetMVBufferSize()
	{
		return cGetMVBufferSize(m_iBlocksInFrame);
	}
/**
**************************************************************************
* \brief Fills user buffer with motion information
*
*/
	int GetMVs(BYTE* dst)
	{
		return cGetMVs(dst, MVectors, m_iBlocksInFrame);
	}
/**
**************************************************************************
* \brief Sets motion information to motion estimation object
*
*/
	void SetMVs(signed char* src)
	{
		cSetMVs(src, MVectors, m_iBlocksInFrame);
	}

/**
**************************************************************************
* \brief Returns pointer to the motion vectors array of the current frame
*
*  This function returns pointer to the motion vectors array of the
*  current frame
*
* \return Pointer to the motion vector array
*/
	inline cMV* GetMVsArray()
	{
		return MVectors;
	};

/**
**************************************************************************
* \brief Returns pointer to the motion vectors array of the previous frame
*
*  This function returns pointer to the motion vectors array of the
*  previous frame
*
* \return Pointer to the motion vector array
*/
	inline cMV* GetPrvMVsArray()
	{
		return MVPrev;
	};

	inline int GetBlockSize()
	{
		if (m_bAccept_4x4)   return 4;
		if (m_bAccept_8x8)   return 8;
		if (m_bAccept_16x16) return 16;
		return m_iBlockSize;
	}

	inline int GetBorderSize()
	{
		return m_iBorderSize;
	}

	inline BYTE* GetHierMap()
	{
		return m_pHierLaunchMap;
	}

	inline BYTE** GetPlanesY()
	{
		return m_pRefShiftsY;
	}

	inline InterpolationAlgorithm GetInterAlg()
	{
		return m_interAlg;
	}

/**
**************************************************************************
* \brief Adding motion candidate
*
*  This function adds motion vector to candidate set.
*
* \param x                   [IN] -  pointer to the currently processed block in current frame
* \param y                   [IN] -  pointer to the currently processed block in current frame
* 
* \return None
*/
	inline void AddMotionCandidate(int x, int y)
	{
		cAddMotionCandidate((gmcMotionEstimator*)this, x, y);
	}


/**
**************************************************************************
* \brief Frees resources had been used by ME
*
*  Deallocates all memory had been used for ME
*
* \return None
*/
	void Stop()
	{
		if (m_bSaveStatistic)
			PrintConfig((gmcMotionEstimator*)this, m_sConfigFileName);
		cStopME((gmcMotionEstimator*)this);
	}

/**
**************************************************************************
* \brief Frees resources had been used by ME (class destructor)
*
*  Deallocates all memory had been used for ME (class destructor)
*
* \return None
*/
	virtual ~gmMotionEstimator()
	{
		Stop();
	}
};

#endif

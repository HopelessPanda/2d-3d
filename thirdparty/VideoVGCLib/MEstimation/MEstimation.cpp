/*
********************************************************************
(c) MSU Video Group 2003-2005, http://compression.ru/video/
This source code is property of MSU Graphics and Media Lab

This code can be distributed and used only with WRITTEN PERMISSION.
********************************************************************
*/

/**
*  \file MEstimation.cpp
*  \brief Performs motion estimation
*
*  This file contains implementation of the class that performs
*  motion estimation in video.
*
*  $Id: MEstimation.cpp,v 1.38 2006/11/13 10:24:52 05s_put Exp $
*  $Log: MEstimation.cpp,v $
*  Revision 1.38  2006/11/13 10:24:52  05s_put
*  New version of compensation
*
*  Revision 1.2  2006/07/13 07:51:30  05s_put
*  Fixed error
*
*  Revision 1.44  2006/05/17 12:59:08  05s_put
*  Made C version of MEstimation
*
*  Revision 1.43  2006/05/16 13:46:46  05s_put
*  Remaking project system
*
*  Revision 1.42  2006/05/11 05:32:48  05s_put
*  Made almost everything for codec
*
*  Revision 1.41  2006/04/27 15:17:51  05s_put
*  All algorithms fixed, made some project files changes
*
*  Revision 1.40  2006/04/26 06:21:51  05s_put
*  Some modifications
*
*  Revision 1.39  2006/04/20 17:17:35  05s_put
*  Motion inertia bug fix
*
*  Revision 1.38  2006/04/13 17:11:36  05s_put
*  First version of YV12 compensation
*
*  Revision 1.37  2006/04/06 13:33:08  05s_put
*  Made another one smooth function
*
*  Revision 1.36  2006/04/05 13:36:21  05s_put
*  Saving version with color support
*
*  Revision 1.35  2006/03/22 16:50:13  05s_put
*  saving current version
*
*  Revision 1.34  2006/03/21 18:36:46  05s_put
*  saving current version
*
*  Revision 1.33  2006/03/16 13:42:36  05s_put
*  Made candidate vectors filtering
*
*  Revision 1.32  2006/03/15 18:19:48  05s_put
*  saving current version of MV candidates filtering
*
*  Revision 1.31  2006/03/15 16:49:32  05s_put
*  Current version
*
*  Revision 1.30  2006/03/14 14:26:16  05s_put
*  Made smarter candidates.
*
*  Revision 1.29  2006/03/13 18:29:57  05s_put
*  Current version save
*
*  Revision 1.28  2006/03/11 17:03:17  05s_put
*  Begining of candidates propogation implementation
*  Smoothing vector field using previous frame
*
*  Revision 1.27  2006/03/09 14:45:49  05s_put
*  - made interlaced processing correct with new smooth vector field functions
*  - uncheked smooth vector function is now uncommented
*
*  Revision 1.26  2006/03/07 15:21:11  05s_put
*  Saving current changes
*
*  Revision 1.25  2006/03/07 07:54:23  05s_put
*  Made a smooth motion vector field function
*
*  Revision 1.24  2006/03/03 07:37:13  05s_put
*  Begin to make MV field dependent error functions.
*
*  Revision 1.23  2006/03/01 13:23:19  05s_put
*  Interlaced processing is ON
*
*  Revision 1.22  2006/02/28 20:01:59  05s_put
*  Commit of interlaced "supporting" version. Still bugs remaining.
*
*  Revision 1.21  2006/02/24 16:05:59  05s_put
*  Made quarter-pixel precision
*
*  Revision 1.20  2006/02/24 10:59:32  05s_put
*  Made quarter pixel GetBlock functuion,
*  made precision settings in dialog window
*
*  Revision 1.19  2006/02/22 17:14:31  05s_put
*  Made a getBlock function to maintain quarter-pixel precision
*
*  Revision 1.18  2006/02/17 15:11:05  05s_put
*  Some bug fixes
*
*  Revision 1.17  2006/02/17 13:34:10  05s_put
*  Made hierarchic post search
*
*  Revision 1.16  2006/02/15 16:56:02  05s_put
*  Support for interlaced processing is made.
*
*  Revision 1.15  2006/02/09 10:25:38  05s_put
*  Made 32x32 block sizes, made "FRC performance"
*
*  Revision 1.14  2006/02/08 08:18:34  05s_put
*  Saving current state
*
*  Revision 1.13  2006/02/01 16:29:59  05s_put
*  Bug fixed
*
*  Revision 1.12  2006/02/01 11:12:51  05s_put
*  Saving current state
*
*  Revision 1.11  2006/01/31 01:22:47  05s_put
*  Made MV showing as a parameter in configuration window,
*  made hierarchic search function
*
*  Revision 1.10  2006/01/30 20:51:27  05s_put
*  Made motion vector visualization.
*
*  Revision 1.9  2006/01/26 12:48:36  05s_put
*  Done with "dispersion" calculation
*
*  Revision 1.8  2006/01/24 21:21:24  05s_put
*  Saving current state
*
*  Revision 1.7  2006/01/24 01:20:32  05s_put
*  Saving current state
*
*  Revision 1.6  2006/01/20 23:59:23  05s_put
*  Correct statistic gathering
*
*  Revision 1.5  2006/01/18 18:28:22  05s_put
*  Almost done statistic gathering
*
*  Revision 1.4  2006/01/16 14:53:00  05s_put
*  My algorithm is working
*
*  Revision 1.3  2006/01/12 18:28:31  05s_put
*  Made normal interface for 3DRS and E3DRS realizations
*
*  Revision 1.2  2006/01/10 17:01:31  05s_put
*  Made 3DRS and E3DRS algoritms
*
*  Revision 1.1  2006/01/09 17:41:36  05s_put
*  First commit
*
*  Revision 1.32  2005/11/14 19:55:29  05m_mah
*  IYV12 estimation/compensation added
*
*  Revision 1.31  2005/07/04 12:20:26  05s_put
*  Fixed doxygen comments
*
*  Revision 1.30  2005/07/04 11:12:17  05s_put
*  Added doxygen comments
*
*  Revision 1.29  2005/07/04 08:12:30  05s_put
*  Fixed licenses
*
*  Revision 1.28  2005/07/03 13:16:33  05m_mah
*  Permissions header added
*
*  Revision 1.27  2005/05/26 12:00:10  05s_put
*  Now counting SAD operations
*
*  Revision 1.26  2005/05/25 18:50:30  05m_mah
*  Some features emplemented
*
*  Revision 1.25  2005/05/24 12:38:03  05s_put
*  Added color components usage during estimation of logo
*
*  Revision 1.24  2005/05/23 17:45:17  05s_put
*  Begining of color SAD realization to remove logo
*
*  Revision 1.23  2005/05/18 13:30:21  05s_put
*  We have defeated dandruff (10 mask numbers bugs was removed.. i think..)
*
*  Revision 1.22  2005/05/14 07:44:46  05s_put
*  Some fixes performed, quality improved.
*
*  Revision 1.21  2005/05/11 15:35:22  05s_put
*  Added work with float motion vector coordinates.
*  During SetMotionVector function coordinates aproximated according
*  to current motion compensation accuracy.
*
*  Revision 1.20  2005/04/29 14:04:03  05s_put
*  Some code improvements readability.
*
*  Revision 1.19  2005/04/28 15:14:47  05s_put
*  Bug with quantity of motion candidates fixed
*
*  Revision 1.18  2005/04/28 15:04:43  05s_put
*  Removed STL vector usage :(((
*
*  Revision 1.17  2005/04/27 09:53:13  05s_put
*  Made some architecture changes, added C SAD functions
*
*  Revision 1.16  2005/04/26 08:17:06  05s_put
*  Half-pixel for logo removal
*  Fixed colors in YV12
*
*  Revision 1.15  2005/04/25 12:13:28  05s_put
*  COmpensation now working in YV12.
*
*  Revision 1.14  2005/04/25 08:27:44  05s_put
*  Some interface changes, according to "delogo" project requirements.
*
*  Revision 1.13  2005/04/21 12:00:26  05s_put
*  Now we estimates motion for logo using logo mask.
*
*  Revision 1.12  2005/04/18 12:24:59  05s_put
*  Made some "const" declarations
*
*  Revision 1.11  2005/04/14 14:35:11  05s_put
*  Added logo work
*
*  Revision 1.10  2005/04/13 14:54:38  05s_put
*  Made some changes in colorspaces handling
*
*  Revision 1.9  2005/04/12 17:21:10  05s_put
*  First iteration logo removal support added.
*  Fixed bug, that MVectors was not initialized by 0 in class constructor.
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

#include "MEstimation.h"


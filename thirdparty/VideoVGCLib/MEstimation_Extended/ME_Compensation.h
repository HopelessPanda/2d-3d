#pragma once

#include <VideoVGCLib/MEstimation/cMEstimation.h>
//#include "../DepthFromStereo/TFExtendedME.h"

void Motion_Compensation_Simple(cMV *motion_vectors, BYTE *frm_reference, BYTE *dst, int width, int height, BOOL pixel_precision);

void Motion_Compensation_cMV(cMV *motion_vectors, cMV *src, cMV *dst, int width, int height);
//void Motion_Compensation_Smart(gmcMotionEstimator *me, RefShiftsData *me_shifts, BYTE *src, BYTE *dst, BYTE *buf_extended, int width, int height);
//void Motion_Compensation_Smart_3planes(gmcMotionEstimator *me, RefShiftsData *me_shifts, BYTE *src, BYTE *dst, BYTE *buf_extended, int width, int height);

void CompensateFrame_3Planes( BYTE * dst, cMV * source_to_reference_motion, BYTE * reference, int width, int height );
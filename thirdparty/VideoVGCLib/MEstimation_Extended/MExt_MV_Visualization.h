#pragma once

#include <VideoGMLib/Common/CommonTypes.h>
#include <VideoVGCLib/MEstimation/cMV.h>

int FindColorWheelRadius( cMV * motion_field, int width, int height );

void DrawMotionField(cMV *motion_field, BYTE * rgb_output, int width, int height, int radius);

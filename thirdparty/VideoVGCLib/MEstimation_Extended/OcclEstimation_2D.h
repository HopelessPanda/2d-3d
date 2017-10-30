#pragma once
#include "Calculation_Buffers.h"

void Make_Dilation_Erosion_Mask ( BYTE * dst_mask, BYTE * depth_gradient_mask, BYTE * lrc, int width, int height );

void EstimateOcclusions2D ( Buffer_Storage * frame_buffers, BYTE * source_frame, BYTE * reference_frame  );
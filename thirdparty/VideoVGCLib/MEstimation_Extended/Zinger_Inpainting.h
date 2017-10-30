#pragma once

#include <VideoVGCLib/MEstimation/cMV.h>

int choose_maximum(float v1, float v2);
int choose_maximum_MV(cMV v1, cMV v2);
int choose_minimum (float v1, float v2);
int choose_all(float v1, float v2);

void ZingerInpainting_1plane(BYTE*dst, BYTE*alpha, BYTE * depth, int w, int h, int (*d_comparator) (float, float) );
void ZingerInpainting_MVField(cMV*dst_field, BYTE*alpha, cMV * src_field, int w, int h, int (*d_comparator) (cMV, cMV) );
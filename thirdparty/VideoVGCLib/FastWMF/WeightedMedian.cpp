#include "WeightedMedian.h"
#include <iostream>

void WeightedMedian::init(int _w, int _h, wm_type _src_type, bool _RGB_guide, double _sigma,
						  int _rad, weight_function _wf, int _num_auxiliary_planes, int _nF, int _nI)
{
	w = _w;
	h = _h;
	src_type = _src_type;
	RGB_guide = _RGB_guide;
	sigma = _sigma;
	rad = _rad;
	wf = _wf;
	num_auxiliary_planes = _num_auxiliary_planes;
	nF = _nF;
	nI = _nI;
}

void WeightedMedian::filter(void* dst, void* src, BYTE* guide, BYTE* confidence, void** auxiliary_src_planes, 
							BYTE** auxiliary_guide_planes, BYTE** auxiliary_confidence_planes)
{
	if (src_type != wm_sc16)
	{
		WeightedMedianFilter(dst, src, guide, w, h, u8bit, 1, RGB_guide, confidence, sigma, rad, wf, num_auxiliary_planes, 
							 auxiliary_src_planes, auxiliary_guide_planes, auxiliary_confidence_planes, nF, nI);
	}
	else
	{
		int* src_int = new int[w * h];
		int* dst_int = new int[w * h];
		
		short* src_sp = (short*)(src);
		for (int y = 0; y < h; ++y)
		{
			int y_shift = y * w;
			for (int x = 0; x < w; ++x)
			{
				src_int[y_shift + x] = (int)(src_sp[y_shift + x]);
			}
		}

		int** auxiliary_src_planes_int = NULL;
		if (num_auxiliary_planes != 0)
		{
			auxiliary_src_planes_int = new int*[num_auxiliary_planes];
			short** auxiliary_src_planes_sp = (short**)(auxiliary_src_planes);
			for (int i = 0; i < num_auxiliary_planes; ++i)
			{
				auxiliary_src_planes_int[i] = new int[w * h];
				for (int y = 0; y < h; ++y)
				{
					int y_shift = y * w;
					for (int x = 0; x < w; ++x)
					{
						auxiliary_src_planes_int[i][y_shift + x] = (int)(auxiliary_src_planes_sp[i][y_shift + x]);
					}
				}

			}
		}
		
		WeightedMedianFilter((void*)(dst_int), (void*)(src_int), guide, w, h, s32bit, 1, RGB_guide, confidence, sigma, rad, wf, num_auxiliary_planes, 
							 (void**)(auxiliary_src_planes_int), auxiliary_guide_planes, auxiliary_confidence_planes, nF, nI);

		short* dst_sp = (short*)(dst);
		for (int y = 0; y < h; ++y)
		{
			int y_shift = y * w;
			for (int x = 0; x < w; ++x)
			{
				dst_sp[y_shift + x] = (short)(dst_int[y_shift + x]);
			}
		}

		delete [] src_int;
		delete [] dst_int;

		if (num_auxiliary_planes != 0)
		{
			for (int i = 0; i < num_auxiliary_planes; ++i)
			{
				delete [] auxiliary_src_planes_int[i];
			}

			delete [] auxiliary_src_planes_int;
		}
	}
}
#include "WMF.h"

enum wm_type
{
	wm_uc8,  //BYTE
	wm_sc16, //short
};

class WeightedMedian
{
	int w;
	int h;
	wm_type src_type;
	bool RGB_guide;
	double sigma;
	int rad;
	weight_function wf;
	int num_auxiliary_planes;
	int nF;
	int nI;
public:
	WeightedMedian(){}
	~WeightedMedian(){}

	void init(int _w, int _h, wm_type _src_type, bool _RGB_guide=true, double _sigma = 1.0,
		      int _rad=25, weight_function _wf=INV1, int _num_auxiliary_planes=0, int _nF=256, int _nI=256);
	void filter(void* dst, void* src, BYTE* guide, BYTE* confidence=NULL, void** auxiliary_src_planes=NULL, 
		        BYTE** auxiliary_guide_planes=NULL, BYTE** auxiliary_confidence_planes=NULL);
};

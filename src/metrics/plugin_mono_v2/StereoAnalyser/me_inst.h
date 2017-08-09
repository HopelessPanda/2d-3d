#pragma once
extern "C" {
#include <VideoVGCLib/MEstimation/cMEstimation.h>
}
#include <VideoGMLib\Common\CommonTypes.h>
#include <VideoVGCLib/PNG_Image/png_image.h>
#include "vec_pair.h"
enum FIELD_TYPE { FIRST = 0, SECOND = 1 };

#define TR_VAR 5 //very low threshold
#define TR_LRC 46 //anyway I use RANSAC

class ME_Instance
{
public:
	ME_Instance();
	~ME_Instance();
	void Init(int width, int height, bool isMono);
	void Load(PNG_Image &first, PNG_Image &second);
	void Process();
	void CalcQuality();
	void FilterPairs(int tr_var = TR_VAR, int tr_lrc = TR_LRC);
	void FillMiscX(vecPairArray src, int f);
	void GetDepthMap(BYTE* dst, int f);
	cMV* Field(int f);
	BYTE* Quality(int f);
	vecPairArray Pairs();
private:
	int m_width, m_height;
	bool isInit;
	void FillQCP(int f);
	cMV* field_f_to_s;
	cMV* field_s_to_f;
	BYTE* q_first;
	BYTE* q_second;
	int ChooseBestPairInBlock(int bx, int by, vecPair &dst, int lrc);
	PNG_Image m_first;
	PNG_Image m_second;
	cMEParams meParams_f_to_s;
	gmcMotionEstimator meEstimator_f_to_s;
	cMEParams meParams_s_to_f;
	gmcMotionEstimator meEstimator_s_to_f;
	vecPairArray m_pairs;
};
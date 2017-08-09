#include <VideoVGCLib/PNG_Image/png_image.h>
#include "me_inst.h"
extern "C" {
#include <VideoVGCLib/MEstimation/cMEstimation.h>
}

struct tri_buf
{
	float ys;
	float ms;
	float ds;
};

struct me_data
{
	PNG_Image* frame;
	ME_Instance* inst;
	int field;
};

class MiddleGen
{
private:
	void AddValue(float x, int y, BYTE p, BYTE m);
	tri_buf* m_array;
	BYTE* m_image;
	BYTE* m_mask;
	int m_width;
	bool isInited;
	int m_height;
public:
	void Init(int w, int h);
	void MakeView(me_data left, me_data right, float p);
	BYTE* GetImage();
	BYTE* GetMask();
	MiddleGen();
	~MiddleGen();
};
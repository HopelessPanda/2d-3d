#include <metrics/common_algorithms/PNG_Image/png_image.h>
#include <metrics/common_algorithms/GeometryAnalyser/me_inst.h>
#define TO_BYTE(a)  (BYTE(CLIP((a), 0, 255)))
extern "C" {
#include <VideoVGCLib/MEstimation/cMEstimation.h>
}

struct tri_buf
{
	double ys;
	double ms;
	double ds;
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
	void AddValue(double x, int y, BYTE p, BYTE m);
	tri_buf* m_array;
	BYTE* m_image;
	BYTE* m_mask;
	int m_width;
	bool isInited;
	int m_height;
public:
	void Init(int w, int h);
	void MakeView(me_data left, me_data right, double p);
	BYTE* GetImage();
	BYTE* GetMask();
	MiddleGen();
	~MiddleGen();
};
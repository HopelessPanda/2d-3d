#include <VideoVGCLib/PNG_Image/png_image.h>
#include "me_inst.h"
#include "ransac_ap.h"

enum SAVE_TYPE { O_LEFT = 0, O_RIGHT = 1, O_MONO = 2, M_STEREO = 3, M_MONO = 4, M_SSIM};
enum RES_TYPE { GEOM = 0, POS = 1 };
enum POSITION {LEFT = 0, RIGHT = 1, MONO = 2};
enum RESULT { G_LR = 0, G_LM = 1, P_LM = 2, P_RM = 3, G_BM = 4};

class StereoAnalyser
{
public:
	StereoAnalyser();
	void Init(int width, int height);
	void InitStereo(PNG_Image * left, PNG_Image * right);
	void InitMono(PNG_Image * mono);
	void Analyse();
	void Load(PNG_Image * src, int t);
	int SaveVis(int vis_type);
	float GetTotal(int t);
	float GetFit(int t);
	ME_Instance *  GetME(int t);
	aTransform GetResults(int t);
	~StereoAnalyser();
private:
	int m_width;
	int m_height;
	PNG_Image *o_left;
	PNG_Image *o_right;
	PNG_Image *o_mono;
	BYTE* m_ssim;
	void TransformMono();
	ME_Instance L_TO_R;
	ME_Instance L_TO_M;
	ME_Instance R_TO_M;
	Ransac TR_G_S;
	Ransac TR_G_M;
	Ransac TR_P_ML;
	Ransac TR_P_MR;
	bool isStereoInit;
	bool isMonoInit;
};
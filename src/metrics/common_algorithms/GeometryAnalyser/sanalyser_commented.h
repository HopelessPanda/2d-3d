#include "me_inst.h"
#include "ransac_ap.h"

//�� �������
enum SAVE_TYPE { O_LEFT = 0, O_RIGHT = 1, O_MONO = 2, M_STEREO = 3, M_MONO = 4, M_SSIM };
enum RES_TYPE { GEOM = 0, POS = 1 };
//��� ��� �� ����
enum POSITION { LEFT = 0, RIGHT = 1, MONO = 2 };
//��� ��� �� �������������� (Geometry_Left-Right / Geometry_Left-Mono / Position_Left-Mono...)
enum RESULT { G_LR = 0, G_LM = 1, P_LM = 2, P_RM = 3 };

//�� ����� �������
class StereoAnalyser
{
public:
	StereoAnalyser(); //�����������
	void Init(int width, int height); //�������������� �������
	void InitStereo(PNG_Image * left, PNG_Image * right); //������ ������� ME ��� ������
	void InitMono(PNG_Image * mono, bool disable_stereo); //�� ��, �� ��� ���� (���� �� �������� ������, ����� ���� �� ����� - ������ ��� ��������)
	void Analyse(); //��������� ������
	void Load(PNG_Image * src, POSITION t); //��������� �������� ���������� ����
	int SaveVis(int vis_type); //�� �������
	double GetTotal(int t); //������� ����� �������� ����� ���������� (� ��������� 0.0 ... 1.0)
	double GetFit(int t); //������� ������� � ���������� �������������� (0.0 ... 1.0)
	ME_Instance *  GetME(int t); //������� �����
	aTransform GetResults(RESULT t); //������� ��� ���������
	~StereoAnalyser(); //����������
private:
	int m_width; //��������
	int m_height; //������
	bool m_process_stereo; //������� �� ������ (���� ��� ������� ���� ������ �� ������ ������)
	PNG_Image *o_left; //��������
	PNG_Image *o_right;
	PNG_Image *o_mono;
	BYTE* m_ssim; //����� ��� ������� ������� 2D / 3D
	void TransformMono(); //������� 2D ���� ����������� ������� �� 3D
	ME_Instance L_TO_R; //�����
	ME_Instance L_TO_M;
	ME_Instance R_TO_M;
	Ransac TR_G_S; //�������� RANSAC
	Ransac TR_G_M;
	Ransac TR_P_ML;
	Ransac TR_P_MR;
	bool isStereoInit; //�������� �� ����� ��� ������
	bool isMonoInit; // �� ��, �� ��� ����
};
#pragma once
extern "C" {
#include <VideoVGCLib/MEstimation/cMEstimation.h>
}
#include <VideoGMLib\Common\CommonTypes.h>
#include <metrics/common_algorithms/PNG_Image/png_image.h>
#include "vec_pair.h"
enum FIELD_TYPE { FIRST = 0, SECOND = 1 }; //��� ����, ������ ��� ��������. ������������ � ���������� �������

//������ ��� ����������
#define TR_VAR 5 
#define TR_LRC 46

//������ ��� ����� ������� ������ ������� ������� � �����
#define AR_LRC 10
#define AR_DIF 2

//������ ����� ����������
//�� ����, �� ����� 24*24 ����� ������ ������ ���� ������
#define FI_BLOCK 24

//���� ������ ��� ������� ME
class ME_Instance
{
public:
	ME_Instance(); //�����������
	~ME_Instance(); //����������
	void Init(int width, int height, bool isMono); //�������������� ���������� ������� � ����������
	void Load(PNG_Image &first, PNG_Image &second); //��������� �������� �����
	void Process(); //��������� ME �� ����������
	void CalcQuality(); //����������� ������� � ME, LRC - Left-Right Consistency Check, ���������� ���� "����" � "�������"
	void FilterPairs(int tr_var = TR_VAR, int tr_lrc = TR_LRC); //����������� �������, ������� ������ �� �������� �������, ��������� m_pairs
	void FillMiscX(vecPairArray src, int f); //��������� "�����" ������ ���� � �������� "������" �������. ����� ���� ��� ������� 2D,
											 //����� ��� ���������� ������� ������� 2D-L ����� ���� �������� ������ � ������� � L-R
	void GetDepthMap(BYTE* dst, int f); //��������� dst ������ ����������� (������ ��� ��������)
	cMV* Field(int f); //������� ����� ���� �������� ����� ME
	BYTE* Quality(int f); //������ ����������� LRC
	vecPairArray Pairs(); //������ ����� ��������������� �������� ��� ������������ �������
private:
	int m_width, m_height; //�����, ������...
	bool isInit; //��������������� ��.
	void FillQCP(int f); //����� ��������� ������� LRC
	cMV* field_f_to_s; //����� ���� �������� ���������� � ME
	cMV* field_s_to_f; //first_to_second, second_to_first
	BYTE* q_first; //������� LRC
	BYTE* q_second;
	int ChooseBestPairInBlock(int bx, int by, vecPair &dst, int lrc); //�� ������ �����, ��������� ����� ���� � ���� ������
	PNG_Image m_first; //���� �������� ���
	PNG_Image m_second;
	cMEParams meParams_f_to_s; //��������� ��� ����� ME
	gmcMotionEstimator meEstimator_f_to_s; //���� ME
	cMEParams meParams_s_to_f;
	gmcMotionEstimator meEstimator_s_to_f;
	vecPairArray m_pairs; //��������������� ���� �������� ����� ��� ����� FilterPairs
};
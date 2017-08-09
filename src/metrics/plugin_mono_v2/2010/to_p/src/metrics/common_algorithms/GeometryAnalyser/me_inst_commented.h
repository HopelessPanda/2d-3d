#pragma once
extern "C" {
#include <VideoVGCLib/MEstimation/cMEstimation.h>
}
#include <VideoGMLib\Common\CommonTypes.h>
#include <metrics/common_algorithms/PNG_Image/png_image.h>
#include "vec_pair.h"
enum FIELD_TYPE { FIRST = 0, SECOND = 1 }; //тип поля, прямое или обратное. используется в параметрах функций

//пороги для сортировки
#define TR_VAR 5 
#define TR_LRC 46

//пороги для новой функции выбора лучшего вектора в блоке
#define AR_LRC 10
#define AR_DIF 2

//размер блока фильтрации
//то есть, из блока 24*24 будет выбран только один вектор
#define FI_BLOCK 24

//своя обёртка для анализа ME
class ME_Instance
{
public:
	ME_Instance(); //конструктор
	~ME_Instance(); //деструктор
	void Init(int width, int height, bool isMono); //инициализирует внутренние массивы с картинками
	void Load(PNG_Image &first, PNG_Image &second); //загружает картинки извне
	void Process(); //запускает ME на вычисление
	void CalcQuality(); //анализирует доверие к ME, LRC - Left-Right Consistency Check, совпадение поля "туда" и "обратно"
	void FilterPairs(int tr_var = TR_VAR, int tr_lrc = TR_LRC); //прореживает вектора, отбирая лучшие по заданным порогам, заполняет m_pairs
	void FillMiscX(vecPairArray src, int f); //заполняет "чужой" массив инфо о векторах "своими" данными. нужно было для анализа 2D,
											 //когда для вычисления позиции ракурса 2D-L нужно было вытащить данные о глубине с L-R
	void GetDepthMap(BYTE* dst, int f); //заполняет dst картой диспаритета (прямой или обратной)
	cMV* Field(int f); //достать сырое поле векторов после ME
	BYTE* Quality(int f); //достаёт посчитанный LRC
	vecPairArray Pairs(); //достаёт набор отфильтрованных векторов для последующего анализа
private:
	int m_width, m_height; //длина, ширина...
	bool isInit; //инициализирован ли.
	void FillQCP(int f); //здесь считается доверие LRC
	cMV* field_f_to_s; //сырое поле векторов полученное с ME
	cMV* field_s_to_f; //first_to_second, second_to_first
	BYTE* q_first; //доверие LRC
	BYTE* q_second;
	int ChooseBestPairInBlock(int bx, int by, vecPair &dst, int lrc); //оч важная штука, фильтрует целый блок в один вектор
	PNG_Image m_first; //сама картинка тут
	PNG_Image m_second;
	cMEParams meParams_f_to_s; //параметры для нашей ME
	gmcMotionEstimator meEstimator_f_to_s; //сама ME
	cMEParams meParams_s_to_f;
	gmcMotionEstimator meEstimator_s_to_f;
	vecPairArray m_pairs; //отфильтрованные пары векторов будут тут после FilterPairs
};
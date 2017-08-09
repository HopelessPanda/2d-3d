#include "sanalyser.h"
#include "ransac_ap.h"

//начальная инициализация.
void StereoAnalyser::Init(int width, int height)
{
	m_width = width; //запомнить ширину и высоту
	m_height = height;
	m_ssim = (BYTE*)malloc(m_width*m_height*sizeof(BYTE)); //выделить память под SSIM
	//во блин, а SSIM я уже давно не использую! убрать фигню, заменить на bool !!!
}

//инициализировать ME_Instance и запомнить указатели на картинки с ракурсами
void StereoAnalyser::InitStereo(PNG_Image * left, PNG_Image * right)
{
	if (m_ssim == NULL) //исполняет роль isInited ... блин, криво!
		Init(left->width(), left->height());
	L_TO_R.Init(m_width, m_height, false);
	isStereoInit = true;
	o_left = left;
	o_right = right;
	TR_G_S.Init(GEOMETRY); //запустить ransac
}
//аналогично, но для мого
void StereoAnalyser::InitMono(PNG_Image * mono, bool disable_stereo)
{
	if(disable_stereo) //чтоб не считать поле L-R 
		m_process_stereo = false;
	if (m_ssim == NULL) 
		Init(mono->width(), mono->height());
	L_TO_M.Init(m_width, m_height, true); // запустить ещё два ME
	R_TO_M.Init(m_width, m_height, true); //
	o_mono = mono; //запомнить картинку
	isMonoInit = true;
	TR_G_M.Init(GEOMETRY); //запустить три ранзака
	TR_P_ML.Init(POSITION);
	TR_P_MR.Init(POSITION);
}

//заменить картинки на какие-нибудь другие. на всякий случай
void StereoAnalyser::Load(PNG_Image * src, int t)
{
	switch (t)
	{
	case(LEFT) : o_left = src; break;
	case(RIGHT) : o_right = src; break;
	case(MONO) : o_mono = src; break;
	}
}

//конструктор
StereoAnalyser::StereoAnalyser()
{
	m_process_stereo = true; //просто дефолтные значения
	isStereoInit = false;
	isMonoInit = false;
	m_width = m_height = 0;
	o_left = o_right = o_mono = NULL;
	m_ssim = NULL;
}

//запустить анализ
void StereoAnalyser::Analyse()
{
	if (!isStereoInit)
		return; //не инициализировано стерео ? без него никуда. выходим!
	if (isStereoInit && isMonoInit)
	{
#pragma omp parallel sections
		{
#pragma omp section
		o_left->ConvertToType(YUV_P); //обязательно сначала конвертировать в YUV_P (внутри ME тоже это умеет, но это доп. расходы)
#pragma omp section
		o_right->ConvertToType(YUV_P);
#pragma omp section
		o_mono->ConvertToType(YUV_P);
		}
#pragma omp parallel sections
		{
#pragma omp section //prepare LR ME data
			{
				L_TO_R.Load(*o_left, *o_right); //загружаем картинки в ME
				L_TO_R.Process(); //считаем ME
				L_TO_R.CalcQuality(); //считаем доверие
				L_TO_R.FilterPairs(); //фильтруем вектора
			}
#pragma omp section //prepare LM ME data
			{
				L_TO_M.Load(*o_left, *o_mono);
				L_TO_M.Process();
				L_TO_M.CalcQuality();
				L_TO_M.FilterPairs();
			}
#pragma omp section //prepare RM ME data
			{
				R_TO_M.Load(*o_right, *o_mono);
				R_TO_M.Process();
				R_TO_M.CalcQuality();
				R_TO_M.FilterPairs();
			}
		}
		
		//тут анализируем вектора ранзаком.
#pragma omp parallel sections
		{
#pragma omp section //analyse L->M geometry (R->M isn't really necessary)
			{
				TR_G_M.Load(L_TO_M.Pairs()); //загрузить вектора в ранзак
				TR_G_M.Analyse(); //проанализировать их
			}
#pragma omp section //analyse stero geometry (if needed)
			if (m_process_stereo)
			{
				TR_G_S.Load(L_TO_R.Pairs());
				TR_G_S.Analyse();
			}
#pragma omp section //analyse L->M position
			{
				L_TO_R.FillMiscX(L_TO_M.Pairs(), FIRST); //а вод перед анализом позиции, надо сначала заполнить инфо о глубине
				TR_P_ML.Load(L_TO_M.Pairs());
				TR_P_ML.Analyse();
			}
#pragma omp section //analyse R->M position
			{
				L_TO_R.FillMiscX(R_TO_M.Pairs(), SECOND);
				TR_P_MR.Load(R_TO_M.Pairs());
				TR_P_MR.Analyse();
			}
		}
	}
	else if (isStereoInit) //считаем только стерео
	{
#pragma omp parallel sections
		{
#pragma omp section
		o_left->ConvertToType(YUV_P); 
#pragma omp section
		o_right->ConvertToType(YUV_P);
		}
		L_TO_R.Load(*o_left, *o_right);
		L_TO_R.Process();
		L_TO_R.CalcQuality();
		L_TO_R.FilterPairs();
		TR_G_S.Load(L_TO_R.Pairs());
		TR_G_S.Analyse();
	}
}

//удаляем всё... остальное очистится само через свои деструкторы
StereoAnalyser::~StereoAnalyser()
{
	if (m_ssim != NULL)
		free(m_ssim);
}

//получить результаты от ранзаков
aTransform StereoAnalyser::GetResults(int t)
{
	switch (t)
	{
	default: case (G_LR) : return TR_G_S.Result(); break;
	case (G_LM) : return TR_G_M.Result(); break;
	case (P_LM) : return TR_P_ML.Result(); break;
	case (P_RM) : return TR_P_MR.Result(); break;
	}
}

//сколько векторов отфильтровано (в процентах, от максимума)
double StereoAnalyser::GetTotal(int t)
{
	Ransac * source;
	switch (t)
	{
	default: case (G_LR) : source = &TR_G_S; break;
	case (G_LM) : source = &TR_G_M; break;
	case (P_LM) : source = &TR_P_ML; break;
	case (P_RM) : source = &TR_P_MR; break;
	}
	int vectors = source->GetTotal();
	int total_amount = (m_width / FI_BLOCK)*(m_height / FI_BLOCK);
	return double(vectors) / total_amount;
}

//и сколько подошло под преобразование
double StereoAnalyser::GetFit(int t)
{
	Ransac * source;
	switch (t)
	{
	default: case (G_LR) : source = &TR_G_S; break;
	case (G_LM) : source = &TR_G_M; break;
	case (P_LM) : source = &TR_P_ML; break;
	case (P_RM) : source = &TR_P_MR; break;
	}
	int vectors_fit = source->GetFit();
	int vectors_total = source->GetTotal();
	if(vectors_total == 0)
		return 0;
	else
		return double(vectors_fit) / vectors_total;
}

//вернуть нужный ME_inst
ME_Instance *  StereoAnalyser::GetME(int t)
{
	ME_Instance * source;
	switch (t)
	{
	default: case (0) : source = &L_TO_R; break;
	case (1) : source = &L_TO_M; break;
	case (2) : source = &R_TO_M; break;
	}
	return source;
}
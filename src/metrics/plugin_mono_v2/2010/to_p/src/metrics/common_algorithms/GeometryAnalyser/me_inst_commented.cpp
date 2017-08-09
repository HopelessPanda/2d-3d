#include "me_inst.h"

//процедура извлечения cMV поля из ME. не моя и ладно.
void scExtractMVFPixmap(gmcMotionEstimator* me, cMV* MVF)
{
	int width = me->m_iWidth;
	int height = me->m_iHeight;
	int x, y;
	int bx0, by0;
	int iCompStep;

	iCompStep = me->m_pParams->m_MinBlSize;

	for (by0 = 0; by0 < me->m_iHeight; by0 += iCompStep)
	{
		for (bx0 = 0; bx0 < me->m_iWidth; bx0 += iCompStep)
		{
			int motionx;
			int motiony;
			int motion_dif;
			int iCurBlockSize;
			cGetMotionVector(me, bx0, by0, &motionx, &motiony, &motion_dif, &iCurBlockSize);
			for (y = by0; y < by0 + iCompStep; ++y)
			{
				for (x = bx0; x < bx0 + iCompStep; ++x)
				{
					MVF[y * width + x].x = motionx;
					MVF[y * width + x].y = motiony;
					MVF[y * width + x].error = motion_dif;
					MVF[y * width + x].field = FT_TOP;
				}
			}
		}
	}
}

//конструктор
ME_Instance::ME_Instance()
{
	m_width = 0; //дефолтные значения
	m_height = 0;
	isInit = false;
	cInitMEParams(&meParams_f_to_s); //инициализировать настройки и инстантсы ME
	cInitMEParams(&meParams_s_to_f);
	cInitME(&meEstimator_f_to_s);
	cInitME(&meEstimator_s_to_f);
}

//запускает всё дело, выделяет память под основные массивы и т.д.
void ME_Instance::Init(int width, int height, bool isMono)
{
	m_width = width; //ну с этим понятно
	m_height = height;
	//сюда будут сохраняться фильтрованные вектора. почему * 2 - не помню
	m_pairs.array = (vecPair*)malloc(m_width*m_height*sizeof(vecPair) * 2);
	m_pairs.count = 0; //пока там векторов нет.
	//прямое и обратное поля сырых векторов
	field_f_to_s = (cMV*)malloc(m_width*m_height*sizeof(cMV));
	field_s_to_f = (cMV*)malloc(m_width*m_height*sizeof(cMV));
	//LRC доверие векторов туда и обратно. 
	q_first = (BYTE*)malloc(m_width*m_height*sizeof(BYTE));
	q_second = (BYTE*)malloc(m_width*m_height*sizeof(BYTE));
	//четвертьпиксельная точность
	meParams_f_to_s.m_PrecMX = QuarterPixel;
	meParams_f_to_s.m_PrecMY = QuarterPixel;
	//блоки от 4*4 до 16*16
	meParams_f_to_s.m_MaxBlSize = cbt_16x16;
	meParams_f_to_s.m_MinBlSize = cbt_4x4;
	//другого вроде нет
	meParams_f_to_s.m_MEAlgorithm = MSU;
	if (isMono)
	{
		//для моно - цветонезависимая метрика и большие расстояния поиска
		meParams_f_to_s.m_MEMetricAlgorithm = METRIC_COLORINDEPENDENT;
		meParams_f_to_s.m_iMaxMotHor = MAX(40, (int)(0.12 * m_width));
		meParams_f_to_s.m_iMaxMotVert = MAX(40, (int)(0.12 * m_height));
	}
	else
	{
		//для стерео - чтоб побыстрее и покачественнее
		meParams_f_to_s.m_MEMetricAlgorithm = METRIC_SAD;
		meParams_f_to_s.m_iMaxMotHor = MAX(40, (int)(0.12 * m_width));
		meParams_f_to_s.m_iMaxMotVert = MAX(32, (int)(0.06 * m_height));
	}
	//чот странное, было в первой версии, так и оставил. вроде чтоб не уменьшался исходник с цветом 
	//(в YUV формате часто цвет уменьшают вчетверо)
	meParams_f_to_s.m_advMEParams.m_bConsiderChroma = TRUE;
	meParams_f_to_s.m_advMEParams.m_ChrXMod = crNoReduction;
	meParams_f_to_s.m_advMEParams.m_ChrYMod = crNoReduction;
	//копируем всё это в параметры обратного МЕ
	cCopyMEParams(&meParams_s_to_f, &meParams_f_to_s);

	isInit = true; //уря, инициализировано

	//запускаем выделение памяти внутренних структур ME
	cStartME(&meEstimator_f_to_s, m_height, m_width, &meParams_f_to_s);
	cStartME(&meEstimator_s_to_f, m_height, m_width, &meParams_s_to_f);
}

//загрузить новые кадры в мяшку
void ME_Instance::Load(PNG_Image &first, PNG_Image &second)
{
	int border = meEstimator_f_to_s.m_iBorderSize; //добавляем границы картинкам. 
	m_first = first;
	m_second = second;
	m_first.AddBorders(border, true); //моя функция. медленная. можно поменять на что-то побыстрее.
	m_second.AddBorders(border, true); //true = значит с протягиванием крайних пикселей 
	m_first.ConvertToType(YUV_P); //ME требует картинки в формате YUV_P
	m_second.ConvertToType(YUV_P);
}

//заполняем значения глубины для чужого массива инфо о векторах
void ME_Instance::FillMiscX(vecPairArray src, int f)
{
	cMV* field = f == FIRST ? field_f_to_s : field_s_to_f; //прямое или обратное поле
#pragma omp parallel for
	for (int i = 0; i < src.count; i++)
	{
		//не забываем центрирование координат..
		//по сути - просто выдергиваем и запоминаем глубину в точке sX / sY
		int new_i = CLIP((int)floor(src.array[i].sX + 0.5 + m_width/2), 0, m_width - 1);
		int new_j = CLIP((int)floor(src.array[i].sY + 0.5 + m_height/2), 0, m_height - 1);
		src.array[i].mX = field[new_i + new_j * m_width].x;
	}
}

//типичный компаратор для сортировки
enum CO_TYPE { CO_X, CO_Y, VE_X, VE_Y, M_X };

template <CO_TYPE letter>
int Comparator(const void* left, const void* right)
{
	int result = 0;
	double diff;
	CO_TYPE thing = letter;
	const vecPair* vp_left, *vp_right;
	vp_left = (const vecPair*)left;
	vp_right = (const vecPair*)right;
	switch (letter)
	{
	case CO_X: diff = vp_left->sX - vp_right->sX; break;
	case CO_Y: diff = vp_left->sY - vp_right->sY; break;
	case VE_X: diff = vp_left->vX - vp_right->vX; break;
	case VE_Y: diff = vp_left->vY - vp_right->vY; break;
	case M_X: diff = vp_left->mX - vp_right->mX; break;
	}
	if (diff > 0) result = 1;
	else if (diff < 0) result = -1;
	else result = 0;

	return result;
}

//первейший кандидат на замену.
//выбирает один вектор из блока
//тем самым прорежая вектора
int ME_Instance::ChooseBestPairInBlock(int bx, int by, vecPair &pair, int lrc)
{
	vecPair * lrc_sorted;
	int max_x = MIN((bx + 1)*FI_BLOCK, m_width) - bx*FI_BLOCK; //откуда докуда идём.
	int max_y = MIN((by + 1)*FI_BLOCK, m_height) - by*FI_BLOCK; //определяем по номеру блока
	int max_xy = max_x*max_y;
	if (max_x*3 < FI_BLOCK || max_y*3 < FI_BLOCK) //типа если блок сильно мелкий, то выбирать нечего. что за бред..
		return -1;
	lrc_sorted = (vecPair*)malloc(max_xy*sizeof(vecPair)); //здесь будут сортироваться вектора. медианой
	int added_cnt = 0; //сколько добавлено
	for (int i = bx*FI_BLOCK; i < MIN((bx + 1)*FI_BLOCK, m_width); i++) //идём по всем пикселям блока
		for (int j = by*FI_BLOCK; j < MIN((by + 1)*FI_BLOCK, m_height); j++)
		{
			int index = i + j*m_width; //индекс пикселя в массиве
			//не учитываем, если lrc меньше порога.
			//добавить сюда ограничение на var ? не, лучше всё переписать
			if (q_first[index] < lrc )
			{
				vecPair temp = {0};
				temp.sX = i - m_width/2; //центрирование координат
				temp.sY = j - m_height/2; //чтоб сдвиг считался относительно центра кадра
				temp.vX = field_f_to_s[index].x; //длина вектора*4
				temp.vY = field_f_to_s[index].y; //потому что четвертьпиксельная точность
				lrc_sorted[added_cnt++] = temp;
			}
		}
	if ( (added_cnt * 100) / max_xy  < AR_LRC) //типа мало векторов отобралось. тогда блок типа плохой
	{
		free (lrc_sorted);
		return -1;
	}
	qsort(lrc_sorted, added_cnt, sizeof(vecPair), Comparator<VE_Y>); //сортируем по Y
	vecPair mosty = lrc_sorted[added_cnt / 2]; //выбираем медианой

	int besty_cnt = 0;

	//а здесь находим среди них медианой координату по X
	for (int i = 0; i < added_cnt; i++)
	{
		if ( abs(mosty.vY - lrc_sorted[i].vY) <= AR_DIF ) //добавляем все рядом с найденным Y
			lrc_sorted[besty_cnt++] = lrc_sorted[i];
	}
	qsort(lrc_sorted, besty_cnt, sizeof(vecPair), Comparator<VE_X>); //фильтруем по X
	pair = lrc_sorted[besty_cnt / 2]; //выбираем медианой
	free (lrc_sorted);
	return 0;
}

//получаем набор векторов для анализа
void ME_Instance::FilterPairs(int var, int lrc)
{
	BYTE* var_first = m_first.CalcVar(); //считаем дисперсию картинок.
	BYTE* var_second = m_second.CalcVar(); 
	m_pairs.count = 0; //сколько пар отобрали
	int x_blocks = m_width / FI_BLOCK; //делим картинку на блоки
	int y_blocks = m_height / FI_BLOCK;

#pragma omp	parallel for
	for (int j = 0; j < y_blocks; j++) //проходим по все блокам и отбираем по вектору из каждого
		for (int i = 0; i < x_blocks; i++)
		{
			vecPair pair;
			int index = i*FI_BLOCK + j*FI_BLOCK*m_width;
			if (var_first[index] < var || ChooseBestPairInBlock(i, j, pair, lrc) != 0) //если в блоке плохая дисперсия, пропускаем его
			//это работало, пока FI_BLOCK был равен AN_BLOCK... попробовать поставить оба на 16!!
				continue;
#pragma omp critical
			m_pairs.array[m_pairs.count++] = pair;
		}
}

//подсчёт LRC - доверия к векторам ME
void ME_Instance::FillQCP(int f)
{
	cMV* first, *second;
	BYTE* dst;
	if (f == FIRST) //выбираем что куда анализируем
	{
		first = field_f_to_s;
		second = field_s_to_f;
		dst = q_first;
	}
	else
	{
		first = field_s_to_f;
		second = field_f_to_s;
		dst = q_second;
	}
	double norm_koef = 4.0; //на сколько делить длину вектора чтоб получить расстояние
	int frame_size = m_width * m_height; //размер кадра
	memset(dst, 255, m_width * m_height * sizeof(BYTE)); //заполняем всё "плохим" значением, чтоб там, куда не попал вектор, оно и осталось!

#pragma omp parallel for //проходимся по всему кадру
	for (int ind_y = 0; ind_y < m_height; ind_y++) 
	{
		for (int ind_x = 0; ind_x < m_width; ind_x++)
		{
			int index = ind_y*m_width + ind_x;
			// получаем вектор из первого поля во второе
			double first_vector_x = first[index].x / norm_koef;
			double first_vector_y = first[index].y / norm_koef;

			// находим координаты конечной точки этого вектора
			int second_x = int(floor(ind_x + first_vector_x + 0.5f));
			int second_y = int(floor(ind_y + first_vector_y + 0.5f));
			// координата плохая ? вектор отбрасываем !
			if (CLIP(second_x, 0, m_width - 1) != second_x || CLIP(second_y, 0, m_height - 1) != second_y)
				continue;
			int second_index = second_y * m_width + second_x;

			// находим вектор из второго поля в первое
			double second_vector_x = second[second_index].x / norm_koef;
			double second_vector_y = second[second_index].y / norm_koef;

			//подсчитываем, насколько он не совпадаем с первым и запоминаем
			dst[index] = (BYTE)CLIP(sqrt((double)sqr(second_vector_x + first_vector_x) + (double)sqr(second_vector_y + first_vector_y)) * 10, 0, 255);
		}
	}
}

//одновременно заполняем оба поля
void ME_Instance::CalcQuality()
{
#pragma omp parallel sections
	{
#pragma omp section
		{
			FillQCP(FIRST);
		}
#pragma omp section
		{
			FillQCP(SECOND);
		}
	}
}

//вернуть LRC поля
BYTE* ME_Instance::Quality(int f)
{
	if (f == FIRST)
		return q_first;
	else
		return q_second;
}

//подсчитать ME поля
void ME_Instance::Process()
{
#pragma omp parallel sections
	{
#pragma omp section
		{
			cEstimate(&meEstimator_f_to_s, m_first.GetData(), m_second.GetData(), TRUE); //вычисление 
			scExtractMVFPixmap(&meEstimator_f_to_s, field_f_to_s); //вытащить cMV поле
		}
#pragma omp section
		{
			cEstimate(&meEstimator_s_to_f, m_second.GetData(), m_first.GetData(), TRUE);
			scExtractMVFPixmap(&meEstimator_s_to_f, field_s_to_f);
		}
	}
}

//вернуть поле
cMV* ME_Instance::Field(int f)
{
	if (f == FIRST)
		return field_f_to_s;
	else
		return field_s_to_f;
}

//вернуть фильтрованные вектора
vecPairArray ME_Instance::Pairs()
{
	return m_pairs;
}


//посчитать и заполнить карту диспаритете
void ME_Instance::GetDepthMap(BYTE* dst, int f)
{
	int wh = m_width*m_height;
	cMV* field;
	if (f == FIRST) //выбрать поле
		field = field_f_to_s;
	else
		field = field_s_to_f;
#pragma omp parallel for
	for (int j = 0; j < m_height; j++) //пройтись по всему
		for (int i = 0; i < m_width; i++)
		{
			int index = i + j*m_width;
			dst[index] = CLIP(128 + 3 * field[index].x, 0, 255); //вывести глубину * 12
		}
}

//деструктор. просто очистить все массивы.
ME_Instance::~ME_Instance()
{
	if (isInit)
	{
		free(field_f_to_s);
		free(field_s_to_f);
		free(q_first);
		free(q_second);
		cStopME(&meEstimator_f_to_s);
		cStopME(&meEstimator_s_to_f);
		cDeInitMEParams(&meParams_f_to_s);
		cDeInitMEParams(&meParams_s_to_f);
		free(m_pairs.array);
	}
}
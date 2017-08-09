#include "me_inst.h"

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

ME_Instance::ME_Instance()
{
	m_width = 0;
	m_height = 0;
	isInit = false;
	cInitMEParams(&meParams_f_to_s);
	cInitMEParams(&meParams_s_to_f);
	cInitME(&meEstimator_f_to_s);
	cInitME(&meEstimator_s_to_f);
}

void ME_Instance::Init(int width, int height, bool isMono)
{
	m_width = width;
	m_height = height;
	m_pairs.array = (vecPair*)malloc(m_width*m_height*sizeof(vecPair)*2);
	m_pairs.count = 0;
	field_f_to_s = (cMV*)malloc(m_width*m_height*sizeof(cMV));
	field_s_to_f = (cMV*)malloc(m_width*m_height*sizeof(cMV));
	q_first = (BYTE*)malloc(m_width*m_height*sizeof(BYTE));
	q_second = (BYTE*)malloc(m_width*m_height*sizeof(BYTE));
	meParams_f_to_s.m_PrecMX = QuarterPixel;
	meParams_f_to_s.m_PrecMY = QuarterPixel;
	meParams_f_to_s.m_MaxBlSize = cbt_16x16;
	meParams_f_to_s.m_MinBlSize = cbt_8x8;
	meParams_f_to_s.m_MEAlgorithm = MSU;
	if (isMono)
	{
		meParams_f_to_s.m_MEMetricAlgorithm = METRIC_COLORINDEPENDENT;
		meParams_f_to_s.m_iMaxMotHor = MAX(40, (int)(0.12 * m_width));
		meParams_f_to_s.m_iMaxMotVert = MAX(40, (int)(0.12 * m_height));
	}
	else
	{
		meParams_f_to_s.m_MEMetricAlgorithm = METRIC_SAD;
		meParams_f_to_s.m_iMaxMotHor = MAX(40, (int)(0.12 * m_width));
		meParams_f_to_s.m_iMaxMotVert = MAX(32, (int)(0.06 * m_height));
	}
	meParams_f_to_s.m_advMEParams.m_bConsiderChroma = TRUE;
	meParams_f_to_s.m_advMEParams.m_ChrXMod = crNoReduction;
	meParams_f_to_s.m_advMEParams.m_ChrYMod = crNoReduction;
	cCopyMEParams(&meParams_s_to_f, &meParams_f_to_s);

	isInit = true;

	cStartME(&meEstimator_f_to_s, m_height, m_width, &meParams_f_to_s);
	cStartME(&meEstimator_s_to_f, m_height, m_width, &meParams_s_to_f);
}

void ME_Instance::Load(PNG_Image &first, PNG_Image &second)
{
	int border = meEstimator_f_to_s.m_iBorderSize;
	m_first = first;
	m_second = second;
	m_first.AddBorders(border,  true);
	m_second.AddBorders(border, true);
	m_first.ConvertToType(YUV_P);
	m_second.ConvertToType(YUV_P);
}

void ME_Instance::FillMiscX(vecPairArray src, int f)
{
	cMV* field = f == FIRST ? field_f_to_s : field_s_to_f;
	#pragma omp parallel for
	for (int i = 0; i < src.count; i++)
	{
		int new_i = CLIP(floorf(src.array[i].sX + 0.5), 0, m_width-1);
		int new_j = CLIP(floorf(src.array[i].sY + 0.5), 0, m_height-1);
		src.array[i].mX = field[new_i + new_j * m_width].x;
	}
}

int ME_Instance::ChooseBestPairInBlock(int bx, int by, vecPair &pair, int lrc)
{
	int best_i = 0, best_j = 0, best_lrc = lrc;
	for (int i = bx*AN_BLOCK; i < MIN((bx + 1)*AN_BLOCK, m_width); i++)
		for (int j = by*AN_BLOCK; j < MIN((by + 1)*AN_BLOCK, m_height); j++)
		{
			int index = i + j*m_width;
			if (q_first[index] < best_lrc)
			{
				best_lrc = q_first[index];
				best_i = i;
				best_j = j;
			}
		}
	if (best_lrc >= lrc)
		return -1;
	int index = best_i + best_j*m_width;
	pair.sX = best_i; pair.sY = best_j; pair.vX = field_f_to_s[index].x; pair.vY = field_f_to_s[index].y; pair.mX = 0;
	return 0;
}

void ME_Instance::FilterPairs(int var, int lrc)
{
	BYTE* var_first = m_first.CalcVar();
	BYTE* var_second = m_second.CalcVar();
	m_pairs.count = 0;
	int x_blocks = m_width / AN_BLOCK;
	int y_blocks = m_height / AN_BLOCK;

	for (int j = 0; j < y_blocks; j++)
		for (int i = 0; i < x_blocks; i++)
		{
			vecPair pair;
			int index = i*AN_BLOCK + j*AN_BLOCK*m_width;
			if (var_first[index] < var || ChooseBestPairInBlock(i, j, pair, lrc) != 0)
				continue;
			m_pairs.array[m_pairs.count++] = pair;
		}
}

void ME_Instance::FillQCP(int f)
{
	cMV* first, *second;
	BYTE* dst;
	if (f == FIRST)
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
	float norm_koef = 4.0;
	int frame_size = m_width * m_height;
	memset(dst, 255, m_width * m_height * sizeof(BYTE));

#pragma omp parallel for
	for (int ind_y = 0; ind_y < m_height; ind_y++)
	{
		for (int ind_x = 0; ind_x < m_width; ind_x++)
		{
			int index = ind_y*m_width + ind_x;
			// first vector
			float first_vector_x = first[index].x / norm_koef;
			float first_vector_y = first[index].y / norm_koef;

			// coordinate in second vector field
			int second_x = int(floorf(ind_x + first_vector_x + 0.5f));
			int second_y = int(floorf(ind_y + first_vector_y + 0.5f));
			//bad coord -> skip vector
			if (CLIP(second_x, 0, m_width-1) != second_x || CLIP(second_y, 0, m_height-1) != second_y)
				continue;
			int second_index = second_y * m_width + second_x;

			// second vector
			float second_vector_x = second[second_index].x / norm_koef;
			float second_vector_y = second[second_index].y / norm_koef;

			dst[index] = (BYTE)CLIP(sqrt((float)sqr(second_vector_x + first_vector_x) + (float)sqr(second_vector_y + first_vector_y))*10, 0, 255);
		}
	}
}

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

BYTE* ME_Instance::Quality(int f)
{
	if (f == FIRST)
		return q_first;
	else
		return q_second;
}

void ME_Instance::Process()
{
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			cEstimate(&meEstimator_f_to_s, m_first.GetData(), m_second.GetData(), TRUE);
			scExtractMVFPixmap(&meEstimator_f_to_s, field_f_to_s);
		}
		#pragma omp section
		{
			cEstimate(&meEstimator_s_to_f, m_second.GetData(), m_first.GetData(), TRUE);
			scExtractMVFPixmap(&meEstimator_s_to_f, field_s_to_f);
		}
	}
}

cMV* ME_Instance::Field(int f)
{
	if (f == FIRST)
		return field_f_to_s;
	else
		return field_s_to_f;
}

vecPairArray ME_Instance::Pairs()
{
	return m_pairs;
}

void ME_Instance::GetDepthMap(BYTE* dst, int f)
{
	int wh = m_width*m_height;
	cMV* field;
	if (f == FIRST)
		field = field_f_to_s;
	else
		field = field_s_to_f;
	#pragma omp parallel for
	for (int j = 0; j < m_height; j++)
		for (int i = 0; i < m_width; i++)
		{
			int index = i + j*m_width;
			dst[index] = CLIP(128 + field[index].x, 0,255);
		}
}

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
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
    m_pairs.array = (vecPair*)malloc(m_width*m_height*sizeof(vecPair) * 2);
    m_pairs.count = 0;
    field_f_to_s = (cMV*)malloc(m_width*m_height*sizeof(cMV));
    field_s_to_f = (cMV*)malloc(m_width*m_height*sizeof(cMV));
    q_first = (BYTE*)malloc(m_width*m_height*sizeof(BYTE));
    q_second = (BYTE*)malloc(m_width*m_height*sizeof(BYTE));
    meParams_f_to_s.m_PrecMX = QuarterPixel;
    meParams_f_to_s.m_PrecMY = QuarterPixel;
    meParams_f_to_s.m_MaxBlSize = cbt_16x16;
    meParams_f_to_s.m_MinBlSize = cbt_4x4;
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
    m_pfirst = &first;
    m_psecond = &second;
    m_first_me = first;
    m_second_me = second;
    m_first_me.AddBorders(border, true);
    m_second_me.AddBorders(border, true);
    m_first_me.ConvertToType(YUV_P);
    m_second_me.ConvertToType(YUV_P);
}

void ME_Instance::FillMiscX(vecPairArray src, int f)
{
    cMV* field = f == FIRST ? field_f_to_s : field_s_to_f;
#pragma omp parallel for
    for (int i = 0; i < src.count; i++)
    {
        int new_i = CLIP((int)floor(src.array[i].sX + 0.5 + m_width/2), 0, m_width - 1);
        int new_j = CLIP((int)floor(src.array[i].sY + 0.5 + m_height/2), 0, m_height - 1);
        src.array[i].mX = field[new_i + new_j * m_width].x;
    }
}

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

int ME_Instance::ChooseBestPairInBlock(int bx, int by, vecPair &pair, int lrc)
{
    vecPair * lrc_sorted;
    int max_x = MIN((bx + 1)*FI_BLOCK, m_width) - bx*FI_BLOCK;
    int max_y = MIN((by + 1)*FI_BLOCK, m_height) - by*FI_BLOCK;
    int max_xy = max_x*max_y;
    if (max_x*3 < FI_BLOCK || max_y*3 < FI_BLOCK)
        return -1;
    lrc_sorted = (vecPair*)malloc(max_xy*sizeof(vecPair));
    int added_cnt = 0;
    for (int i = bx*FI_BLOCK; i < MIN((bx + 1)*FI_BLOCK, m_width); i++)
        for (int j = by*FI_BLOCK; j < MIN((by + 1)*FI_BLOCK, m_height); j++)
        {
            int index = i + j*m_width;
            if (q_first[index] < lrc && abs(field_f_to_s[index].x) + abs(field_f_to_s[index].y) != 0)
            {
                vecPair temp = {0};
                temp.sX = i - m_width/2;
                temp.sY = j - m_height/2;
                temp.vX = field_f_to_s[index].x;
                temp.vY = field_f_to_s[index].y;
                lrc_sorted[added_cnt++] = temp;
            }
        }
    if ( (added_cnt * 100) / max_xy  < AR_LRC)
    {
        free (lrc_sorted);
        return -1;
    }
    qsort(lrc_sorted, added_cnt, sizeof(vecPair), Comparator<VE_Y>);
    vecPair mosty = lrc_sorted[added_cnt / 2];

    int besty_cnt = 0;

    for (int i = 0; i < added_cnt; i++)
    {
        if ( abs(mosty.vY - lrc_sorted[i].vY) <= AR_DIF )
            lrc_sorted[besty_cnt++] = lrc_sorted[i];
    }
    qsort(lrc_sorted, besty_cnt, sizeof(vecPair), Comparator<VE_X>);
    pair = lrc_sorted[besty_cnt / 2];
    free (lrc_sorted);
    return 0;
}

void ME_Instance::FilterPairs(int var, int lrc)
{
    BYTE* var_first = m_pfirst->CalcVar();
    BYTE* var_second = m_psecond->CalcVar();
    m_pairs.count = 0;
    int x_blocks = m_width / FI_BLOCK;
    int y_blocks = m_height / FI_BLOCK;

#pragma omp	parallel for
    for (int j = 0; j < y_blocks; j++)
        for (int i = 0; i < x_blocks; i++)
        {
            vecPair pair;
            int index = i*FI_BLOCK + j*FI_BLOCK*m_width;
            if (var_first[index] < var || ChooseBestPairInBlock(i, j, pair, lrc) != 0)
                continue;
#pragma omp critical
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
    double norm_koef = 4.0;
    int frame_size = m_width * m_height;
    memset(dst, 255, m_width * m_height * sizeof(BYTE));

#pragma omp parallel for
    for (int ind_y = 0; ind_y < m_height; ind_y++)
    {
        for (int ind_x = 0; ind_x < m_width; ind_x++)
        {
            int index = ind_y*m_width + ind_x;
            // first vector
            double first_vector_x = first[index].x / norm_koef;
            double first_vector_y = first[index].y / norm_koef;

            // coordinate in second vector field
            int second_x = int(floor(ind_x + first_vector_x + 0.5f));
            int second_y = int(floor(ind_y + first_vector_y + 0.5f));
            //bad coord -> skip vector
            if (CLIP(second_x, 0, m_width - 1) != second_x || CLIP(second_y, 0, m_height - 1) != second_y)
                continue;
            int second_index = second_y * m_width + second_x;

            // second vector
            double second_vector_x = second[second_index].x / norm_koef;
            double second_vector_y = second[second_index].y / norm_koef;

            dst[index] = (BYTE)CLIP(sqrt((double)sqr(second_vector_x + first_vector_x) + (double)sqr(second_vector_y + first_vector_y)) * 10, 0, 255);
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

#include <iostream>
void ME_Instance::Process()
{
#pragma omp parallel sections
    {
#pragma omp section
        {
            cEstimate(&meEstimator_f_to_s, m_first_me.GetData(), m_second_me.GetData(), TRUE);
            scExtractMVFPixmap(&meEstimator_f_to_s, field_f_to_s);
        }
#pragma omp section
        {
            cEstimate(&meEstimator_s_to_f, m_second_me.GetData(), m_first_me.GetData(), TRUE);
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
            dst[index] = CLIP(128 + 3 * field[index].x, 0, 255);
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
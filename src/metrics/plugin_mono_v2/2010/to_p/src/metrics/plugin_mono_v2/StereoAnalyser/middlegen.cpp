#include "middlegen.h"
extern "C"
{
#include <StereoWarpLib/include/StereoWarp.h>
}

MiddleGen::MiddleGen()
{
	m_array = NULL;
	m_image = m_mask = NULL;
	isInited = false;
}

void MiddleGen::Init(int w, int h)
{
	m_width = w;
	m_height = h;
	m_array = (tri_buf*)malloc(w*h*sizeof(tri_buf));
	m_image = (BYTE*)malloc(w*h);
	m_mask = (BYTE*)malloc(w*h);
	isInited = true;
}

MiddleGen::~MiddleGen()
{
	if(isInited) 
	{
		free(m_array);
		free(m_image);
		free(m_mask);
	}
}

void MiddleGen::AddValue(float x, int y, BYTE p, BYTE m)
{
	int l_x = floorf(x);
	int r_x = floorf(x+0.99);
	if(l_x < 0 || y < 0 || r_x > m_width - 1 || y > m_height - 1)
		return;
	float l_dist = CLIP(1-(x - l_x), 0, 1);
	float r_dist = CLIP(1-(r_x - x), 0, 1);
	int l_ind = l_x + y*m_width;
	int r_ind = r_x + y*m_width;
	m_array[l_ind].ys += p*l_dist;
	m_array[l_ind].ms += m*l_dist;
	m_array[l_ind].ds += l_dist;
	m_array[r_ind].ys += p*r_dist;
	m_array[r_ind].ms += m*r_dist;
	m_array[r_ind].ds += r_dist;
}

void MiddleGen::MakeView(me_data left, me_data right, float pos)
{
	BYTE* l_mask = left.inst->Quality(left.field);
	BYTE* r_mask = right.inst->Quality(right.field);
	cMV* l_vec = left.inst->Field(left.field);
	cMV* r_vec = right.inst->Field(right.field);

	for(int i = 0; i < m_width*m_height; i++)
	{
		m_array[i].ys = 0;
		m_array[i].ms = 0;
		m_array[i].ds = 0;
	}
	for(int j = 0; j < m_height; j++)
		for(int i = 0; i < m_width; i++)
		{
			int index = i+j*m_width;
			float new_xl = i + l_vec[index].x*pos/4.0f;
			AddValue(new_xl, j, left.frame->y(i,j), l_mask[index]);
			float new_xr = i + r_vec[index].x*(1.0-pos)/4.0f;
			AddValue(new_xr, j, right.frame->y(i,j), r_mask[index]);
		}
	for(int j = 0; j < m_height; j++)
		for(int i = 0; i < m_width; i++)
		{
			int index = i+j*m_width;
			tri_buf cur = m_array[index];
			if(cur.ds < 1.25 || cur.ms / cur.ds > 40)
			{
				m_image[index] = 0;
				m_mask[index] = 255;
			}
			else
			{
				m_image[index] = cur.ys / cur.ds;
				m_mask[index] = cur.ms / cur.ds;
			}
		}
}

BYTE* MiddleGen::GetImage()
{
	return m_image;
}
BYTE* MiddleGen::GetMask()
{
	return m_mask;
}
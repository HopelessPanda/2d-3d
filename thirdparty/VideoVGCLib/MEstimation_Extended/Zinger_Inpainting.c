#include "Zinger_Inpainting.h"
#include <math.h>

#define dir_num 30
#define PI 3.1415926

int choose_maximum(float v1, float v2)
{
	return v1 > v2;
}
int choose_maximum_MV(cMV v1, cMV v2)
{
	return v1.x > v2.x;
}

int choose_minimum (float v1, float v2)
{
	return v1 < v2;
}

int choose_all(float v1, float v2)
{
	return 0;
}

typedef struct 
{
	int x;
	int y;
	float w;
	float d;
} ppixel;

typedef struct 
{
	int x;
	int y;
	float w;
	cMV mv;
} mvpixel;

float NBRWeight(int dx, int dy)
{
	float res = (float)(dx*dx+dy*dy); 
	res=sqrtf(res);
	return 1.0f/res;
}

void GetNBRPixel(BYTE*alpha, BYTE * depth, int x, int y, float sx, float sy, ppixel*dst, int w, int h)
{
	BYTE max_alpha = 255;
	int px = x;
	int py = y;
	int index = 0;
	BYTE alpha_bool = 0;
	int n=0;
	BYTE last = 0;
	while(1){
		++n;
		px = (int)(x+sx*n);
		py = (int)(y+sy*n);

		if (px<0 || py<0 || px>=w || py>=h) {dst->w=0; return;}

		index = py*w + px;
		alpha_bool = (alpha[index]-max_alpha == 0);

		if ( alpha_bool && last ){
			dst->x = px;
			dst->y = py;
			dst->d = depth[index];
			dst->w = NBRWeight(px-x, py-y);
			return;
		}
		last = alpha_bool;
	}
}

void GetNBR_MV(BYTE*alpha, cMV * field, int x, int y, float sx, float sy, mvpixel * dst, int w, int h)
{
	BYTE max_alpha = 255;
	int px = x;
	int py = y;
	int index = 0;
	BYTE alpha_bool = 0;
	int n=0;
	BYTE last = 0;
	while(1){
		++n;
		px = (int)(x+sx*n);
		py = (int)(y+sy*n);

		if (px<0 || py<0 || px>=w || py>=h) {dst->w=0; return;}

		index = py*w + px;
		alpha_bool = (alpha[index]-max_alpha == 0);

		if ( alpha_bool && last ){
			dst->x = px;
			dst->y = py;
			dst->mv = field[index];
			dst->w = NBRWeight(px-x, py-y);
			return;
		}
		last = alpha_bool;
	}
}

void ZingerInpainting_1plane(BYTE*dst, BYTE*alpha, BYTE * depth, int w, int h, int (*d_comparator) (float, float) )
{
	float sin_values[dir_num];
	float cos_values[dir_num];

	int posX, posY, i;
	int step_mult = 2;

	for( i = 0; i < dir_num; ++i)
	{
		sin_values[i] = step_mult * (float)sin(i*2*PI/dir_num);
		cos_values[i] = step_mult * (float)cos(i*2*PI/dir_num);
	}

	if(d_comparator == NULL)
		d_comparator = choose_maximum;

#pragma omp parallel for private(posX, i)
	for (posY = 0; posY < h; ++ posY )
	{
		ppixel nbr[dir_num];
		for (posX = 0; posX < w; ++ posX )
		{
			int xyw = posX+posY*w;
			int yw = posY*w;
			if(alpha[xyw]!=255){
				float r=0, g=0, b=0, a=0, ww=0;
				float average_depth = 0, average_depth_count = 0;

				for(i=0;i<dir_num;i++){
					GetNBRPixel(alpha, depth, posX, posY, sin_values[i], cos_values[i], &(nbr[i]), w, h);
					if (nbr[i].w != 0)
					{
						average_depth += nbr[i].d;
						++ average_depth_count;
					}
				}
				average_depth /= average_depth_count;

				for(i=0;i<dir_num;i++){
					if(nbr[i].w == 0) continue;
					if( d_comparator(nbr[i].d, average_depth) ) continue;
					ww += nbr[i].w;
					r += nbr[i].w * dst[nbr[i].x + nbr[i].y*w]; 					
					//g += nbr[i].w * dst[nbr[i].x + nbr[i].y*w+w*h]; 					
					//b += nbr[i].w * dst[nbr[i].x + nbr[i].y*w+w*h*2]; 										
				}
				dst[xyw] = (BYTE)(r/ww);
				//dst[xyw+w*h] = (BYTE)(g/ww);
				//dst[xyw+w*h*2] = (BYTE)(b/ww);
			}
		}
	}
}

void ZingerInpainting_MVField(cMV*dst_field, BYTE*alpha, cMV * src_field, int w, int h, int (*d_comparator) (cMV, cMV) )
{
	float sin_values[dir_num];
	float cos_values[dir_num];

	int posX, posY, i;
	int step_mult = 2;

	for( i = 0; i < dir_num; ++i)
	{
		sin_values[i] = step_mult * (float)sin(i*2*PI/dir_num);
		cos_values[i] = step_mult * (float)cos(i*2*PI/dir_num);
	}

	if(d_comparator == NULL)
		d_comparator = choose_maximum_MV;

#pragma omp parallel for private(posX, i)
	for (posY = 0; posY < h; ++ posY )
	{
		mvpixel nbr[dir_num];
		for (posX = 0; posX < w; ++ posX )
		{
			int xyw = posX+posY*w;
			int yw = posY*w;
			if(alpha[xyw]!=255){
				float mv_x = 0, mv_y = 0, ww=0;
				int average_depth_count = 0;
				cMV average_vector;
				average_vector.x = average_vector.y = 0;

				for(i=0;i<dir_num;i++){
					GetNBR_MV(alpha, src_field, posX, posY, sin_values[i], cos_values[i], &(nbr[i]), w, h);
					if (nbr[i].w != 0)
					{
						average_vector.x += nbr[i].mv.x;
						average_vector.y += nbr[i].mv.y;
						++ average_depth_count;
					}
				}
				average_vector.x /= average_depth_count;
				average_vector.y /= average_depth_count;

				for(i=0;i<dir_num;i++){
					if(nbr[i].w == 0) continue;
					if( d_comparator(nbr[i].mv, average_vector) ) continue;
					ww += nbr[i].w;
					mv_x += nbr[i].w * dst_field[nbr[i].x + nbr[i].y*w].x; 					
					mv_y += nbr[i].w * dst_field[nbr[i].x + nbr[i].y*w].y; 										
				}
				dst_field[xyw].x = (int)(mv_x/ww);
				dst_field[xyw].y = (int)(mv_y/ww);
			}
		}
	}
}

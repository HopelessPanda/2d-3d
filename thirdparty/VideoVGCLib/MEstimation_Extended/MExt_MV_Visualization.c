
#define _USE_MATH_DEFINES
#include <math.h>

#include "MExt_MV_Visualization.h"

#define MAXCOLS 120 //the number which specifies color gradations

/** \brief class for motion vectors visualization*/
typedef struct 
{
	int ncols;/**< \brief number of columns*/
	int colorwheel[MAXCOLS][3]; /**< \brief color information*/
} ColorWheel;

//=====
void Set_Cols(ColorWheel * wheel, int r, int g, int b, int k)
{
	wheel->colorwheel[k][0] = r;
	wheel->colorwheel[k][1] = g;
	wheel->colorwheel[k][2] = b;
}

void Init_ColorWheel(ColorWheel * wheel)
{
	int k = 0, i;

	// relative lengths of color transitions:
	// these are chosen based on perceptual similarity
	// (e.g. one can distinguish more shades between red and yellow 
	//  than between yellow and green)
	int RY = (int)(15 * (double)(MAXCOLS / 60.0));
	int YG = (int)(6  *	(double)(MAXCOLS / 60.0));
	int GC = (int)(4  *	(double)(MAXCOLS / 60.0));
	int CB = (int)(11 * (double)(MAXCOLS / 60.0));
	int BM = (int)(13 * (double)(MAXCOLS / 60.0));
	int MR = (int)(6  *	(double)(MAXCOLS / 60.0));

	wheel->ncols = RY + YG + GC + CB + BM + MR;

	if (wheel->ncols > MAXCOLS) 
	{
		return;
	}

	for ( i = 0; i < YG; i++) Set_Cols(wheel, 255-255*i/YG,	255,			0,			k++);
	for ( i = 0; i < GC; i++) Set_Cols(wheel, 0,				255,			255*i/GC,	k++);
	for ( i = 0; i < CB; i++) Set_Cols(wheel, 0,				255-255*i/CB,	255,		k++);
	for ( i = 0; i < BM; i++) Set_Cols(wheel, 255*i/BM,		0,				255,		k++);
	for ( i = 0; i < MR; i++) Set_Cols(wheel, 255,			0,				255-255*i/MR, k++);
	for ( i = 0; i < RY; i++) Set_Cols(wheel, 255,			255*i/RY,		0,			k++);
}

/** \brief compute visualization for one vector
* \param [out] rgb output rgb color
* \param [in] cFx, cFy vector coordinates
*/
void ComputeColorRGB_Pix(ColorWheel * wheel, unsigned char *rgb, const double cFx, const double cFy, int maxRad /*= 20*/)
{
	double fx = cFx / (double)(maxRad);
	double fy = cFy / (double)(maxRad);

	double rad = sqrt(fx * fx + fy * fy);
	double a = atan2(-fy, -fx) / M_PI;
	double fk = (a + 1.0) / 2.0 * (wheel->ncols-1);
	int k0 = (int)fk; 
	int k1 = (k0 + 1) % wheel->ncols;
	int b;
	double f = fk - k0;
	
	//f = 0; // uncomment to see original color wheel
	for ( b = 0; b < 3; b++ ) 
	{
		double col0 = wheel->colorwheel[k0][b] / 255.0;
		double col1 = wheel->colorwheel[k1][b] / 255.0;
		double col = (1 - f) * col0 + f * col1;
		if (rad <= 1) 
		{
			col = 1 - rad * (1 - col); // increase saturation with radius
		} 
		else 
		{
			col *= .75; // out of range
		}
		rgb[2 - b] = (int)(255.0 * col / 2);
		rgb[2 - b] = CLIP ( (int) (rgb[2 - b]) * 2 - 128, 0, 255) * 2;
	}
}

int FindColorWheelRadius( cMV * motion_field, int width, int height )
{
	int MAX_RADIUS = 1;

	int ind_x, ind_y;

	for(ind_y = 0; ind_y < height; ++ ind_y )
	{
		for(ind_x = 0; ind_x < width; ++ ind_x)
		{
			int index = ind_y * width + ind_x;

			if(MAX_RADIUS < motion_field[index].x)
				MAX_RADIUS = motion_field[index].x;

			if(MAX_RADIUS < motion_field[index].y)
				MAX_RADIUS = motion_field[index].y;
		}
	}

	return MAX_RADIUS;
}

void DrawMotionField(cMV *motion_field, BYTE * rgb_output, int width, int height, int radius)
{
	int MAX_RADIUS = 1;

	int ind_x, ind_y;
	int plane_size = width * height;

	ColorWheel wheel;
	unsigned char rgb[3];
	rgb[0] = rgb[1] = rgb[2] = 0;

	Init_ColorWheel(&wheel);

	if(radius != 0)
	{
		MAX_RADIUS = radius;
	}
	else
	{
		MAX_RADIUS = FindColorWheelRadius ( motion_field, width, height );
	}

	for(ind_y = 0; ind_y < height; ++ ind_y )
	{
		for(ind_x = 0; ind_x < width; ++ ind_x)
		{
			int index = ind_y * width + ind_x;
			int coord;

			ComputeColorRGB_Pix(&wheel, rgb, motion_field[index].x, motion_field[index].y, MAX_RADIUS);

			for (coord = 0; coord < 3; coord++)
			{
				rgb_output[index + coord * plane_size] = rgb[coord];
			}
		}
	}
}

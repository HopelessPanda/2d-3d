/* this class provides convenient image processing and was originally made for stereo correction tool
	it allows to convert the color type, crop, resize, save the image to PNG file, etc.
*/
#pragma once
#include "stdlib.h"
#include "string.h"
#include <libpng/libpng/png.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "windows.h"

#ifndef fullTransform
struct fullTransform
{
	double aX;
	double aY;
	double bX;
	double bY;
	double cX;
	double cY;
};
#endif

#define AN_BLOCK 8
#define  CLIP(a,_min_,_max_)  (((a) > (_max_)) ? (_max_) : MAX(a,_min_))
typedef unsigned char BYTE;
//BGR_I - interlaced image, pixels placed as "bgrbgrbgr"
//BGR_P - planed image, pixels placed as "bbbgggrrr"
//YV12_C - corrupted read YV12 frame from avifile.lib
//YUV_I / YUV_P - same as BGR, but YUV
enum IMG_TYPE { BGR_I = 0, BGR_P = 1, YV12_C = 2, YUV_I = 3, YUV_P = 4, RGB_P = 5, RGB_I = 6, GRAY = 7};

class PNG_Image
{
public:
	//should be very useful for debug, saves any BYTE array to PNG image
	static int SaveArrayToPNG(BYTE* data, int width, int height, const char* filename); 
	PNG_Image & operator = (const PNG_Image & other);
	//returns internal BYTE array
	BYTE* GetData();
	PNG_Image(int w=0, int h=0, BYTE*d=NULL, int t=0);
	PNG_Image(PNG_Image &i);
	~PNG_Image();
	//chage the internal image representation
	int ConvertToType(int type);
	//save image to PNG file (using BGR_P conversion)
	int SaveToPNG(const char* filename);
	//set image data from some external array, don't copy data
	int Set(int w, int h, int t, BYTE*d);
	//same as above, but with allocating space and copying data
	int Init(int w, int h, int t, const BYTE*d = NULL);
	//allows to set new data + type
	int Set(BYTE*d, int t);
	//set new type only
	int Set(int t);
	//calculate blocks variance
	BYTE* CalcVar();
	//get calculated variance, calculate if not available
	BYTE* GetVar();
	//transform the picture using special transform structure
	void Transform(fullTransform tr, bool invert = false);
	//transform using perspective 3x3 matrix
	void Transform(cv::Mat trans);
	//transform using known geometrical parameters
	void Transform(double rot_angle, double zoom_x, double zoom_y, double shift_x, double shift_y);
	//flip the picture vertically
	void Flip();
	//crop the picture by given values. negative values will add the borders
	int Crop(int left, int top, int right, int bottom);
	//resize using opencv
	int Resize(int width, int height, int interp = cv::INTER_AREA);
	//add borders using Crop, then apply stretch, if needed
	int AddBorders(int left, int top, int right, int bottom, bool stretch = false);
	//add the same borders to all edges
	int AddBorders(int border, bool stretch = false);
	//get width
	int width();
	//get height
	int height();
	//get type
	int type();
	//get pixel values. only values.
	inline BYTE r(int sx, int sy);	inline BYTE g(int sx, int sy);	inline BYTE b(int sx, int sy);
	inline BYTE y(int sx, int sy);	inline BYTE u(int sx, int sy);	inline BYTE v(int sx, int sy);
	//there is no way to set pixel values, so use GetData() and access with pointer
private:
	int m_width;
	//was the data allocated
	bool to_free;
	//is there any ready-to-use variance data
	bool is_var;
	//internal picture data array
	BYTE* m_data;
	//variance array
	BYTE* m_var_data;
	int m_height;
	//picture changed? clean variance! it's not up-to-date anymore
	void CleanVar();
	int m_type;
};
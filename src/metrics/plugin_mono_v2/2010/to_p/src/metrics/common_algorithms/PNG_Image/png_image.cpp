#include "png_image.h"

PNG_Image::PNG_Image(int w, int h, BYTE* d, int t)
{
	to_free = false;
	is_var = false;
	m_var_data = NULL;
	Set(w, h, t, d);
}

PNG_Image::PNG_Image(PNG_Image &i)
{
	to_free = false;
	is_var = false;
	Set(i.width(), i.height(), i.type(), NULL);
	BYTE* temp = (BYTE*)malloc(m_width*m_height*3*sizeof(BYTE));
	memcpy(temp, i.GetData(), m_width*m_height * 3 * sizeof(BYTE));
	Set(temp, m_type);
	to_free = true;
	m_var_data = NULL;
}

PNG_Image & PNG_Image:: operator = (const PNG_Image & other)
{
	if (to_free)
		free(m_data);
	to_free = false;
	m_width = other.m_width;
	m_height = other.m_height;
	m_type = other.m_type;
	int wh = m_width*m_height;
	if(other.m_data == NULL)
	{
		m_data = NULL;
		to_free = false;
	}
	else
	{
		m_data = (BYTE*)malloc(wh * 3);
		memcpy(m_data, other.m_data, wh * 3);
		to_free = true;
	}
	CleanVar();
	return *this;
}

PNG_Image::~PNG_Image()
{
	if (to_free)
		free(m_data);
	CleanVar();
}

int PNG_Image::Set(int w, int h, int t, BYTE* d)
{
	if (to_free)
		free(m_data);
	to_free = false;
	m_width = w;
	m_height = h;
	if(t != GRAY)
	{
		m_data = d;
		m_type = t;
		CleanVar();
		return 0;
	}
	else
		return -1; //use Init for GRAY images
}

int PNG_Image::Init(int w, int h, int t, const BYTE*d)
{
	m_width = w;
	m_height = h;
	m_type = t;
	int wh = w*h;
	if (to_free)
		free(m_data);
	m_data = (BYTE*)calloc(wh * 3, 1);
	to_free = true;
	if(t == GRAY)
	{
		if(d != NULL)
			memcpy(m_data, d, wh);
		memset(m_data+wh,128,wh*2);
		m_type = YUV_P;
	}
	else
		if(d != NULL)
			memcpy(m_data, d, wh * 3);
	CleanVar();
	return 0;
}

void PNG_Image::CleanVar()
{
	if (is_var)
		free(m_var_data);
	is_var = false;
	m_var_data = NULL;
}

int PNG_Image::Set(BYTE* d, int t)
{
	if (m_width > 0 && m_height > 0 && t != GRAY)
	{
		if (to_free)
			free(m_data);
		to_free = false;
		m_type = t;
		m_data = d;
		CleanVar();
		return 0;
	}
	else
		return -1;
}

int PNG_Image::AddBorders(int border, bool stretch)
{
	return AddBorders(border, border, border, border, stretch);
}

int PNG_Image::AddBorders(int left, int top, int right, int bottom, bool stretch)
{
	if (left < 0 || right < 0 || top < 0 || bottom < 0)
		return -1;
	else
	{
		int result = Crop(-left, -top, -right, -bottom);
		if (result != 0 || !stretch)
			return result;
		#pragma omp parallel for 
		for (int j = 0; j < m_height; j++)
			for (int i = 0; i < m_width; i++)
			{
				int old_x = CLIP(i, left, m_width - right - 1);
				int old_y = CLIP(j, top, m_height - bottom - 1);
				int oldind = old_x + old_y*m_width;
				int ind = i + j*m_width;
				int wh = m_width*m_height;
				if (oldind == ind)
					continue;
				if (m_type == BGR_I || m_type == YUV_I)
					memcpy(m_data + ind * 3, m_data + oldind * 3, 3);
				else if (m_type == BGR_P || m_type == YUV_P)
				{
					m_data[ind] = m_data[oldind];
					m_data[ind+wh] = m_data[oldind+wh];
					m_data[ind+wh*2] = m_data[oldind+wh*2];
				}
			}
	}
	return 0;
}

int PNG_Image::Resize(int width, int height, int interp)
{
	ConvertToType(BGR_I);
	cv::Mat image(m_height, m_width, CV_8UC3, m_data);
	cv::Mat result;
	cv::resize(image, result, cv::Size(width, height), 0, 0, interp);
	free(m_data);
	m_data = (BYTE*)malloc(3*width*height);
	memcpy(m_data, result.data, 3*width*height);
	m_width = width;
	m_height = height;
	CleanVar();
	return 0;
}

int PNG_Image::Crop(int left, int top, int right, int bottom)
{
	//if (left < 0 || top < 0 || right < 0 || bottom < 0)
		//return -1;
	if (m_width - left - right <= 0 || m_height - top - bottom <= 0 || m_type == YV12_C)
		return -1;
	int new_width = m_width - left - right;
	int new_height = m_height - top - bottom;
	int new_wh = new_width * new_height;
	int old_wh = m_width * m_height;
	BYTE* temp = (BYTE*)calloc(new_width*new_height * 3, sizeof(BYTE));
	if (temp == NULL)
		return -1;
	#pragma omp parallel for 
	for (int j = 0; j < new_height; j++)
		for (int i = 0; i < new_width; i++)
		{
			int new_index = i + j*new_width;
			if (CLIP(i + left, 0, m_width - 1) != i + left)
				continue;
			if (CLIP(j + top, 0, m_height - 1) != j + top)
				continue;
			int old_index = (i + left) + (j + top)*m_width;
			if (m_type == BGR_I || m_type == YUV_I || m_type == RGB_I)
				memcpy(&temp[new_index * 3], &m_data[old_index * 3], 3);
			else
			{
				temp[new_index] = m_data[old_index];
				temp[new_index + new_wh] = m_data[old_index + old_wh];
				temp[new_index + new_wh*2] = m_data[old_index + old_wh*2];
			}
		}
	free(m_data);
	m_data = temp;
	m_width = new_width;
	m_height = new_height;
	CleanVar();
	return 0;
}

void PNG_Image::Transform(double angle, double zx, double zy, double sx, double sy)
{
	fullTransform tr;
	tr.aX = sin(angle);
	tr.aY = -sin(angle);
	tr.bY = zy;
	tr.bX = zx;
	tr.cX = sx;
	tr.cY = sy;
	Transform(tr);
}

void PNG_Image::Transform(fullTransform tr, bool invert)
{
	ConvertToType(BGR_I);
	cv::Mat affine(3, 3, CV_64F, cv::Scalar(0));
	cv::Mat tozero(3, 3, CV_64F, cv::Scalar(0));
	cv::Mat fromzero(3, 3, CV_64F, cv::Scalar(0));
	cv::Mat picture(m_height, m_width, CV_8UC3, m_data);
	cv::Mat new_picture(m_height, m_width, CV_8UC3, cv::Scalar::all(0));
	affine.at<double>(0, 0) = tr.bX; tozero.at<double>(0, 0) = 1;				fromzero.at<double>(0, 0) = 1;
	affine.at<double>(0, 1) = tr.aX; tozero.at<double>(0, 1) = 0;				fromzero.at<double>(0, 1) = 0;
	affine.at<double>(0, 2) = tr.cX; tozero.at<double>(0, 2) = -m_width / 2;	fromzero.at<double>(0, 2) = m_width / 2;
	affine.at<double>(1, 2) = tr.cY; tozero.at<double>(1, 2) = -m_height / 2;	fromzero.at<double>(1, 2) = m_height / 2;
	affine.at<double>(1, 1) = tr.bY; tozero.at<double>(1, 1) = 1;				fromzero.at<double>(1, 1) = 1;
	affine.at<double>(1, 0) = tr.aY; tozero.at<double>(1, 0) = 0;				fromzero.at<double>(1, 0) = 0;
	affine.at<double>(2, 2) = 1;	 tozero.at<double>(2, 2) = 1;				fromzero.at<double>(2, 2) = 1;
	cv::Mat temp = affine;
	if (invert)
		cv::invert(temp, affine);
	affine = fromzero*affine*tozero;
	cv::warpPerspective(picture, new_picture, affine, cv::Size(m_width, m_height), CV_INTER_LANCZOS4, IPL_BORDER_REPLICATE);
	memcpy(m_data, new_picture.data, m_width*m_height * 3 * sizeof(BYTE));
	CleanVar();
}

void PNG_Image::Transform(cv::Mat trans)
{
	ConvertToType(BGR_I);
	cv::Mat picture(m_height, m_width, CV_8UC3, m_data);
	cv::Mat new_picture(m_height, m_width, CV_8UC3, cv::Scalar::all(0));
	cv::warpPerspective(picture, new_picture, trans, cv::Size(m_width, m_height));
	memcpy(m_data, new_picture.data, m_width*m_height * 3 * sizeof(BYTE));
	CleanVar();
}

int PNG_Image::Set(int t)
{
	if(t == GRAY)
		return -1;
	m_type = t;
	return 0;
}

int PNG_Image::width()
{
	return m_width;
}

BYTE* PNG_Image::GetData()
{
	return m_data;
}

BYTE* PNG_Image::GetVar()
{
	if(!is_var)
		CalcVar();
	return m_var_data;
}

BYTE* PNG_Image::CalcVar()
{
	if (!is_var)
	{
		m_var_data = (BYTE*)malloc(m_width*m_height*sizeof(BYTE));
		is_var = true;
	}
	int block = AN_BLOCK;

	//prepare the Y plane
	PNG_Image temp = *this;
	int addright = (block - temp.width() % block) % block;
	int addbottom = (block - temp.height() % block) % block;
	temp.AddBorders(0, 0, addright, addbottom, true);
	int mw = temp.m_width / block;
	int mh = temp.m_height / block;
	int mwh = mw*mh;
	double* mid = (double*)calloc(mwh, sizeof(double));
	double* dif = (double*)calloc(mwh, sizeof(double));
	#pragma omp parallel for
	for (int j = 0; j < temp.height(); j++)
		for (int i = 0; i < temp.width(); i++)
		{
			int ni = i / block;
			int nj = j / block;
			int mind = ni + nj * mw;
			mid[mind] += temp.y(i, j) / (double)(block*block);
		}
	#pragma omp parallel for
	for (int j = 0; j < temp.height(); j++)
		for (int i = 0; i < temp.width(); i++)
		{
			int ni = i / block;
			int nj = j / block;
			int mind = ni + nj * mw;
			dif[mind] += abs(temp.y(i, j) - mid[mind]) / (double)(block*block);
		}
	#pragma omp parallel for
	for (int j = 0; j < m_height; j++)
		for (int i = 0; i < m_width; i++)
		{
			int ni = i / block;
			int nj = j / block;
			int mind = ni + nj * mw;
			m_var_data[i + j*m_width] = BYTE(y(i,j) == 0 ? 0 :CLIP(dif[mind]*10, 0, 255));
		}
	free(mid);
	free(dif);
	return m_var_data;
}

int PNG_Image::height()
{
	return m_height;
}

int PNG_Image::type()
{
	return m_type;
}

//to bypass the opencv compilation error -_-
inline int cv::_interlockedExchangeAdd(int * a, int b)
{
	return (int)InterlockedExchangeAdd((volatile long*)a, b);
}

//main function, omg
int PNG_Image::ConvertToType(int new_type)
{
	if (new_type == m_type)
		return 0;
	BYTE* temp = (BYTE*)malloc(m_width*m_height * 3 * sizeof(BYTE));
	if (temp == NULL)
		return -1;
	if (m_type == YV12_C)
	{
		int wh4 = m_width*m_height / 4;
		int wh = m_width*m_height;
		int w2 = m_width / 2;
		BYTE* fixed = (BYTE*)malloc(wh4 * 8);
		//magic. thanks to our avi library
		for (int i = 0; i < 6 * m_height / 2; i += 3)
		{
			memcpy(fixed + w2*(i + 0), m_data + w2*(i + 2), w2);
			memcpy(fixed + w2*(i + 1), m_data + w2*(i + 1), w2);
			memcpy(fixed + w2*(i + 2), m_data + w2*(i + 0), w2);
		}
		BYTE* start = fixed + wh4 * 2;
		BYTE* temp = (BYTE*)malloc(w2);
		//magic again. did somebody ever check YUV in avifile?
		for (int i = 0; i < m_height; i++)
		{
			memcpy(temp, start + i*m_width, w2);
			memcpy(start + i*m_width, start + i*m_width + w2, w2);
			memcpy(start + i*m_width + w2, temp, w2);
		}
		BYTE* input = (BYTE*)malloc(wh4 * 8);
		//relocating Y, U and V, for opencv
		memcpy(input, fixed + wh4 * 2, wh);
		memcpy(input + wh, fixed, wh / 2);
		cv::Mat Y_col(m_height + m_height / 2, m_width, CV_8UC1, input);
		cv::Mat dst;
		//our videoimage YV12 -> RGB conversion is bad. using opencv.
		cv::cvtColor(Y_col, dst, CV_YUV2RGB_YV12);
		memcpy(m_data, dst.data, wh * 3);
		m_type = BGR_I;
		free(input);
		free(temp);
		free(fixed);
		//frame must be flipped now. last thanks to our awesome avi library.
		Flip();
		ConvertToType(new_type);
	}
	else if (new_type == YUV_P)
	{
		#pragma omp parallel for
		for (int j = 0; j < m_height; j++)
			for (int i = 0; i < m_width; i++)
			{
				temp[j*m_width + i] = y(i, j);
				temp[j*m_width + i + m_width*m_height] = u(i, j);
				temp[j*m_width + i + m_width*m_height * 2] = v(i,j);
			}
		free(m_data);
		m_data = temp;
	}
	else if (new_type == BGR_P)
	{
	#pragma omp parallel for
		for (int j = 0; j < m_height; j++)
			for (int i = 0; i < m_width; i++)
			{
				temp[j*m_width + i] = b(i, j);
				temp[j*m_width + i + m_width*m_height] = g(i, j);
				temp[j*m_width + i + m_width*m_height * 2] = r(i, j);
			}
		free(m_data);
		m_data = temp;
	}
	else if (new_type == RGB_P)
	{
	#pragma omp parallel for
		for (int j = 0; j < m_height; j++)
			for (int i = 0; i < m_width; i++)
			{
				temp[j*m_width + i] = r(i, j);
				temp[j*m_width + i + m_width*m_height] = g(i, j);
				temp[j*m_width + i + m_width*m_height * 2] = b(i, j);
			}
		free(m_data);
		m_data = temp;
	}
	else if (new_type == BGR_I)
	{
	#pragma omp parallel for 
		for (int j = 0; j < m_height; j++)
			for (int i = 0; i < m_width; i++)
			{
				temp[(j*m_width + i) * 3] = b(i, j);
				temp[(j*m_width + i) * 3 + 1] = g(i, j);
				temp[(j*m_width + i) * 3 + 2] = r(i, j);
			}
		free(m_data);
		m_data = temp;
	}
	else if (new_type == RGB_I)
	{
	#pragma omp parallel for 
		for (int j = 0; j < m_height; j++)
			for (int i = 0; i < m_width; i++)
			{
				temp[(j*m_width + i) * 3] = r(i, j);
				temp[(j*m_width + i) * 3 + 1] = g(i, j);
				temp[(j*m_width + i) * 3 + 2] = b(i, j);
			}
		free(m_data);
		m_data = temp;
	}
	else if (new_type == YUV_I)
	{
	#pragma omp parallel for 
		for (int j = 0; j < m_height; j++)
			for (int i = 0; i < m_width; i++)
			{
				temp[(j*m_width + i) * 3] = y(i, j);
				temp[(j*m_width + i) * 3 + 1] = u(i, j);
				temp[(j*m_width + i) * 3 + 2] = v(i, j);
			}
		free(m_data);
		m_data = temp;
	}
	else
		return -1;
	m_type = new_type;
	return 0;
}

/*void PNG_Image::Grey()
{
	ConvertToType(BGR_P);
	int wh = m_width*m_height;
	memcpy(m_data + wh, m_data, wh);
	memcpy(m_data + wh*2, m_data, wh);
	CleanVar();
}*/

void PNG_Image::Flip()
{
	if (m_type == BGR_I || m_type == YUV_I || m_type == RGB_I)
	{
		BYTE* temp = (BYTE*)malloc(m_width * 3);
		for (int j = 0; j < (m_height - 1) / 2 + 1; j++)
		{
			memcpy(temp, &m_data[j*m_width * 3], m_width * 3);
			memcpy(&m_data[j*m_width * 3], &m_data[(m_height-j-1)*m_width * 3], m_width * 3);
			memcpy(&m_data[(m_height - j - 1)*m_width * 3], temp, m_width * 3);
		}
		free(temp);
	}
	else if (m_type == BGR_P || m_type == YUV_P || m_type == RGB_P)
	{
		BYTE* temp = (BYTE*)malloc(m_width);
		int wh = m_width*m_height;
		for (int k = 0; k < 3; k ++)
			for (int j = 0; j < (m_height - 1) / 2 + 1; j++)
			{
				memcpy(temp, &m_data[j*m_width + wh*k], m_width);
				memcpy(&m_data[j*m_width + wh*k], &m_data[(m_height - j - 1)*m_width + wh*k], m_width);
				memcpy(&m_data[(m_height - j - 1)*m_width + wh*k], temp, m_width);
			}
		free(temp);
	}
	CleanVar();
}

int PNG_Image::SaveToPNG(const char *path)
{
	PNG_Image temp = *this;
	temp.ConvertToType(BGR_P);
	int height = m_height;
	int planes = 3;
	int width = m_width;
	BYTE* pict = temp.GetData();
	FILE *fp = NULL;
	fopen_s(&fp, path, "wb");
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	int x, y;
	png_uint_32 bytes_per_row;
	png_byte **row_pointers = NULL;

	if (fp == NULL) return -1;

	/* Initialize the write struct. */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		return -1;
	}

	/* Initialize the info struct. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, NULL);
		fclose(fp);
		return -1;
	}

	/* Set image attributes. */
	png_set_IHDR(png_ptr,
		info_ptr,
		width,
		height,
		8,
		PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	/* Initialize rows of PNG. */
	bytes_per_row = width * 3;
	row_pointers = (png_byte**)png_malloc(png_ptr, height * sizeof(png_byte *));
	for (y = 0; y < height; ++y) {
		BYTE *row = (BYTE*)png_malloc(png_ptr, sizeof(BYTE) * bytes_per_row);
		row_pointers[y] = (png_byte *)row;
		if (planes == 3)
		{
			for (x = 0; x < width; ++x) {
				*row++ = pict[x + y*width + width*height * 2];
				*row++ = pict[x + y*width + width*height];
				*row++ = pict[x + y*width];
			}
		}
		else
		{
			for (x = 0; x < width; ++x) {
				*row++ = pict[x + y*width];
				*row++ = pict[x + y*width];
				*row++ = pict[x + y*width];
			}
		}
	}

	/* Actually write the image data. */
	png_init_io(png_ptr, fp);
	png_set_rows(png_ptr, info_ptr, row_pointers);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	/* Cleanup. */
	for (y = 0; y < height; y++) {
		png_free(png_ptr, row_pointers[y]);
	}
	png_free(png_ptr, row_pointers);

	/* Finish writing. */
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
	return 0;
}

int PNG_Image::SaveArrayToPNG(BYTE* data, int width, int height, const char* filename)
{
	PNG_Image temp;
	temp.Init(width, height, GRAY, data);
	return temp.SaveToPNG(filename);;
}

BYTE PNG_Image::r(int sx, int sy)
{
	switch (m_type)
	{
	case(BGR_P) : return m_data[sx + m_width*sy + m_width*m_height*2]; break;
	case(BGR_I) : return m_data[(sx + m_width*sy)*3 + 2]; break;
	case(RGB_P) : return m_data[sx + m_width*sy]; break;
	case(RGB_I) : return m_data[(sx + m_width*sy)*3]; break;
	case(YUV_I) : case(YUV_P) : return (BYTE)floorf(CLIP(y(sx, sy) + 1.13983f*(v(sx, sy) - 128), 0, 255) + 0.5f); break;
	default: return 0;
	}
}

BYTE PNG_Image::g(int sx, int sy)
{
	switch (m_type)
	{
	case(RGB_P) : case(BGR_P) : return m_data[sx + m_width*sy + m_width*m_height]; break;
	case(RGB_I) : case(BGR_I) : return m_data[(sx + m_width*sy) * 3 + 1]; break;
	case(YUV_I) : case(YUV_P) : return (BYTE)floorf(CLIP(y(sx, sy) - 0.39465f*(u(sx, sy) - 128) - 0.58060f*(v(sx, sy) - 128), 0, 255) + 0.5f); break;
	default: return 0;
	}
}

BYTE PNG_Image::b(int sx, int sy)
{
	switch (m_type)
	{
	case(RGB_P) : return m_data[sx + m_width*sy + m_width*m_height*2]; break;
	case(RGB_I) : return m_data[(sx + m_width*sy)*3 + 2]; break;
	case(BGR_P) : return m_data[sx + m_width*sy]; break;
	case(BGR_I) : return m_data[(sx + m_width*sy) * 3]; break;
	case(YUV_I) : case(YUV_P) : return (BYTE)floorf(CLIP(y(sx, sy) + 2.03211f*(u(sx, sy) - 128), 0, 255) + 0.5f); break;
	default: return 0;
	}
}

BYTE PNG_Image::y(int sx, int sy)
{
	switch (m_type)
	{
	case(YUV_P) : return m_data[sx + m_width*sy]; break;
	case(YUV_I) : return m_data[(sx + m_width*sy) * 3]; break;
	case(RGB_I) : case(RGB_P) : case(BGR_I) : case(BGR_P) : return (BYTE)floorf(CLIP(r(sx, sy)*0.299f + g(sx, sy)*0.587f + b(sx, sy)*0.114f, 0, 255) + 0.5f); break;
	default: return 0;
	}
}

BYTE PNG_Image::u(int sx, int sy)
{
	switch (m_type)
	{
	case(YUV_P) : return m_data[sx + m_width*sy + m_width*m_height]; break;
	case(YUV_I) : return m_data[(sx + m_width*sy) * 3 + 1]; break;
	case(RGB_I) : case(RGB_P) : case(BGR_I) : case(BGR_P) : return (BYTE)floorf(CLIP(-r(sx, sy)* 0.14713f - g(sx, sy)*0.28886f + b(sx, sy)*0.436f + 128, 0, 255) + 0.5f); break;
	default: return 0;
	}
}

BYTE PNG_Image::v(int sx, int sy)
{
	switch (m_type)
	{
	case(YUV_P) : return m_data[sx + m_width*sy + m_width*m_height*2]; break;
	case(YUV_I) : return m_data[(sx + m_width*sy) * 3 + 2]; break;
	case(RGB_I) : case(RGB_P) : case(BGR_I) : case(BGR_P) : return (BYTE)floorf(CLIP(r(sx, sy)*0.615f - g(sx, sy)*0.51499f - b(sx, sy)*0.10001f + 128, 0, 255) + 0.5f); break;
	default: return 0;
	}
}
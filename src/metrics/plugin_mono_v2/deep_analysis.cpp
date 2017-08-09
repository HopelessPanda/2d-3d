#include "deep_analysis.h"

cv::Mat CalculateIntegral (cv::Mat &image)
{
	int width = image.cols;
	int height = image.rows;
	cv::Mat integral(height, width, CV_64FC1, cv::Scalar::all(0));
	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
		{
			integral.at<double>(j,i) = image.at<unsigned char>(j,i);
			if(i > 0)
				integral.at<double>(j,i) += integral.at<double>(j,i - 1);
			if(j > 0)
				integral.at<double>(j,i) += integral.at<double>(j - 1,i);
			if(j > 0 && i > 0)
				integral.at<double>(j,i) -= integral.at<double>(j - 1,i - 1);
		}
	return integral;
}

cv::Mat CalculateMulIntegral (cv::Mat &image, cv::Mat &image2)
{
	int width = image.cols;
	int height = image.rows;
	cv::Mat integral(height, width, CV_64FC1, cv::Scalar::all(0));
	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
		{
			integral.at<double>(j,i) = double(image.at<unsigned char>(j,i)*image2.at<unsigned char>(j,i));
			if(i > 0)
				integral.at<double>(j,i) += integral.at<double>(j,i - 1);
			if(j > 0)
				integral.at<double>(j,i) += integral.at<double>(j - 1,i);
			if(j > 0 && i > 0)
				integral.at<double>(j,i) -= integral.at<double>(j - 1,i - 1);
		}
	return integral;
}

cv::Mat CalculateMaskIntegral (cv::Mat &image, cv::Mat &mask)
{
	int width = image.cols;
	int height = image.rows;
	cv::Mat integral(height, width, CV_64FC1, cv::Scalar::all(0));
	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
		{
			integral.at<double>(j,i) = image.at<unsigned char>(j,i)*(1-(double)mask.at<unsigned char>(j,i)/255.0f);
			if(i > 0)
				integral.at<double>(j,i) += integral.at<double>(j,i - 1);
			if(j > 0)
				integral.at<double>(j,i) += integral.at<double>(j - 1,i);
			if(j > 0 && i > 0)
				integral.at<double>(j,i) -= integral.at<double>(j - 1,i - 1);
		}
	return integral;
}

cv::Vec3b getColorSubpix(const cv::Mat& img, cv::Point2f pt)
{
	if(pt.y < 0 || pt.y >= img.rows || pt.x < 0 || pt.x >= img.cols)
		return cv::Vec3b(0,0,0);
    cv::Mat patch;
    cv::getRectSubPix(img, cv::Size(1,1), pt, patch);
    return patch.at<cv::Vec3b>(0,0);
}

int MaskBlur (cv::Mat &image, cv::Mat &mask, cv::Mat &result, int rad)
{
	int width = image.cols;
	int height = image.rows;
	cv::Mat res(height, width, CV_8UC1, cv::Scalar::all(0));
	if (width != mask.cols || height != mask.rows)
		return -1;
	cv::Mat im_int, ma_int;
#pragma omp parallel sections
{
	#pragma omp section
	im_int = CalculateMaskIntegral(image, mask);
	#pragma omp section
	ma_int = CalculateIntegral(mask);
}
#pragma omp parallel for
	for(int i = rad; i < width-rad; i++)
		for(int j = rad; j < height-rad; j++)
		{
			double value = (double)(im_int.at<double>(j+rad,i+rad) + im_int.at<double>(j-rad,i-rad) -
				im_int.at<double>(j+rad,i-rad) - im_int.at<double>(j-rad,i+rad))/sqr(2*rad);
			double div = (double)(ma_int.at<double>(j+rad,i+rad) + ma_int.at<double>(j-rad,i-rad) -
				ma_int.at<double>(j+rad,i-rad) - ma_int.at<double>(j-rad,i+rad))/sqr(2*rad);
			div = 1 - div/255.0f;
			if(abs(div) > 0.05)
				res.at<unsigned char>(j, i) = BYTE(value / div);
			else
				res.at<unsigned char>(j, i) = 0;
		}
	result = res;
	return 0;
}

void FilterLetterbox(cv::Mat &image, cv::Mat &mask, int depth, int type)
{
	int i, j, b_top, b_left, b_right, b_bottom, stop = 0;
	int m_height = image.rows, m_width = image.cols;

#pragma omp parallel sections private(i,j, stop)
{
	#pragma omp section
	{
	stop = 0;
	for(j = 0; j < m_height && stop == 0; j++)
		for(i = 0; i < m_width; i++)
			if(image.at<unsigned char>(j,i) > 5)
				{stop = 1; break;}
	b_top = j-1; stop = 0;
	}
	#pragma omp section
	{
	stop = 0;
	for(j = m_height-1; j >=0 && stop == 0; j--)
		for(i = 0; i < m_width; i++)
			if(image.at<unsigned char>(j,i) > 5)
				{stop = 1; break;}
	b_bottom = j+1; stop = 0;
	}
	#pragma omp section
	{
	stop = 0;
	for(i = 0; i < m_width && stop == 0; i++)
		for(j = 0; j < m_height; j++)
			if(image.at<unsigned char>(j,i) > 5)
				{stop = 1; break;}
	b_left = i-1; stop = 0;
	}
	#pragma omp section
	{
	stop = 0;
	for(i = m_width-1; i >=0 && stop == 0; i--)
		for(j = 0; j < m_height; j++)
			if(image.at<unsigned char>(j,i) > 5)
				{stop = 1; break;}
	b_right = i+1;
	}
}
	if (type == 1)
	{
		if(b_top >= 0 && b_top <= m_height-1)
		{for (j = 0; j <= Min(b_top+depth, m_height-1); j++) for(i = 0; i < m_width; i++) mask.at<unsigned char>(j,i) = 255;}
		if(b_bottom >= 0 && b_bottom <= m_height-1)
		{for (j = m_height-1; j >= Max(0, b_bottom-depth); j--) for(i = 0; i < m_width; i++) mask.at<unsigned char>(j,i) = 255;}
		if(b_left >= 0 && b_left <= m_width-1)
		{for (i = 0; i <= Min(b_left+depth, m_width-1); i++) for(j = 0; j < m_height; j++) mask.at<unsigned char>(j,i) = 255;}
		if(b_right >= 0 && b_right <= m_width-1)
		{for (i = m_width-1; i >= Max(0, b_right-depth); i--) for(j = 0; j < m_height; j++) mask.at<unsigned char>(j,i) = 255;}
	}
	else
	{
		if(b_top >= 0 && b_top <= m_height-1)
		{for (j = b_top; j <= Min(b_top+depth, m_height-1); j++) for(i = 0; i < m_width; i++) image.at<unsigned char>(j,i) = 0;}
		if(b_bottom >= 0 && b_bottom <= m_height-1)
		{for (j = b_bottom; j >= Max(0, b_bottom-depth); j--) for(i = 0; i < m_width; i++) image.at<unsigned char>(j,i) = 0;}
		if(b_left >= 0 && b_left <= m_width-1)
		{for (i = b_left; i <= Min(b_left+depth, m_width-1); i++) for(j = 0; j < m_height; j++) image.at<unsigned char>(j,i) = 0;}
		if(b_right >= 0 && b_right <= m_width-1)
		{for (i = b_right; i >= Max(0, b_right-depth); i--) for(j = 0; j < m_height; j++) image.at<unsigned char>(j,i) = 0;}
	}
}

double CalculateSSIM(cv::Mat &dst, cv::Mat &src1, cv::Mat &src2, int rad)
{
	int width = src1.cols;
	int height = src1.rows;
	if (width != src2.cols || height != src2.rows)
		return 0;
	cv::Mat int_1, int_2, int_12, int_11, int_22;
	#pragma omp parallel sections
	{
	#pragma omp section
		int_1 = CalculateIntegral(src1);
	#pragma omp section
		int_2 = CalculateIntegral(src2);
	#pragma omp section
		int_12 = CalculateMulIntegral(src1, src2);
	#pragma omp section
		int_11 = CalculateMulIntegral(src1, src1);
	#pragma omp section
		int_22 = CalculateMulIntegral(src2, src2);
	}

	double sum = 0;
	int count = 0;

	cv::Mat res(height, width, CV_8UC1, cv::Scalar::all(0));
	int i;
#pragma omp parallel for private (i) reduction(+:sum) reduction(+:count)
	for(int j = rad; j < height - rad; j++)
		for(i = rad; i < width - rad; i++)
		{
			int x = 0;
			double mean1 = (double)(int_1.at<double>(j+rad,i+rad) + int_1.at<double>(j-rad,i-rad) -
				int_1.at<double>(j+rad,i-rad) - int_1.at<double>(j-rad,i+rad))/sqr(2*rad);
			double mean2 = (double)(int_2.at<double>(j+rad,i+rad) + int_2.at<double>(j-rad,i-rad) -
				int_2.at<double>(j+rad,i-rad) - int_2.at<double>(j-rad,i+rad))/sqr(2*rad);
			double mul11 = (double)(int_11.at<double>(j+rad,i+rad) + int_11.at<double>(j-rad,i-rad) -
				int_11.at<double>(j+rad,i-rad) - int_11.at<double>(j-rad,i+rad))/sqr(2*rad);
			double mul22 = (double)(int_22.at<double>(j+rad,i+rad) + int_22.at<double>(j-rad,i-rad) -
				int_22.at<double>(j+rad,i-rad) - int_22.at<double>(j-rad,i+rad))/sqr(2*rad);
			double mul12 = (double)(int_12.at<double>(j+rad,i+rad) + int_12.at<double>(j-rad,i-rad) -
				int_12.at<double>(j+rad,i-rad) - int_12.at<double>(j-rad,i+rad))/sqr(2*rad);
			if(mean2 > 10)
				x = 1;
			double der1  = abs(mul11 - mean1*mean1);
			double der2  = abs(mul22 - mean2*mean2);
			double der12 = mul12 - mean1*mean2;
			double c1 = sqr(0.01f*255);
			double c2 = sqr(0.03f*255);
			//double val = (2*mean1*mean2 + c1)*(2*der12 + c2)/(der1 + der2 + c2)/(sqr(mean1)+sqr(mean2)+c1);
			double val = double(255 - CLIP((der12+c2/2)/(sqrt(der1*der2) + c2/2)*255, 0, 255));
			double val_clip = double(255 - CLIP((der12+c2/2)/(sqrt(der1*der2) + c2/2)*255*SSIM_TR, 0, 255));
			sum += val_clip;
			if (abs(val_clip) > 0.1) 
				count += 1 ;
			res.at<unsigned char>(j,i) = BYTE(CLIP(val, 0, 255));
		}
	//	difference power	*	difference field size
	dst = res;
	if (count == 0)
		return 0;
	else
		return (sum / count) * (10.0f * CLIP(count, 0, width*height/10) / width / height);
}

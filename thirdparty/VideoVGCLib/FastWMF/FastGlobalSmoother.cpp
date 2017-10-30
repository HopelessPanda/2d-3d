#include "FastGlobalSmoother.h"

using namespace cv;

#define EPS 1e-43f

void FastGlobalSmoother::init(BYTE* guide, int _w, int _h, double lambda, double sigma_color, double lambda_attenuation, int num_iter)
{
	w = _w;
	h = _h;

	guide_mat = Mat(h, w, CV_8UC3);
	int green_shift = w * h;
	int red_shift = 2 * w * h;
	for (int y = 0; y < h; ++y)
	{
		int y_shift = y * w;
		for (int x = 0; x < w; ++x)
		{
			guide_mat.at<Vec3b>(y, x) = Vec3b(guide[y_shift + x], guide[green_shift + y_shift + x], guide[red_shift + y_shift + x]);
		}
	}

	fgsf = ximgproc::createFastGlobalSmootherFilter(guide_mat, lambda, sigma_color, lambda_attenuation, num_iter);
}

void FastGlobalSmoother::filter(void* src_map, BYTE* src_conf, void* dst_map, fgs_type type)
{
	Mat src_map_mat_x, src_map_mat_y;
	Mat src_conf_mat = Mat(h, w, CV_8UC1, src_conf).clone();
    Mat dst_map_mat_x, dst_map_mat_y;

	if (type == fgs_uc8)
	{
		src_map_mat_x = Mat(h, w, CV_8UC1, (BYTE*)(src_map)).clone();
		src_map_mat_y = Mat(h, w, CV_8UC1, (BYTE*)(src_map) + w * h).clone();
	}
	else
	{
		src_map_mat_x = Mat(h, w, CV_16SC1, (short*)(src_map)).clone();
		src_map_mat_y = Mat(h, w, CV_16SC1, (short*)(src_map) + w * h).clone();
	}

	Mat map_x_mul_conf, map_y_mul_conf, src_conf_f;
	src_map_mat_x.convertTo(map_x_mul_conf, CV_32F);
	src_map_mat_y.convertTo(map_y_mul_conf, CV_32F);
	src_conf_mat.convertTo(src_conf_f, CV_32F);
	map_x_mul_conf = src_conf_f.mul(map_x_mul_conf);
	map_y_mul_conf = src_conf_f.mul(map_y_mul_conf);
	Mat conf_filtered;
	fgsf->filter(map_x_mul_conf, map_x_mul_conf);
	fgsf->filter(map_y_mul_conf, map_y_mul_conf);
	fgsf->filter(src_conf_f, conf_filtered);
    Mat inverse_conf = 1 / (conf_filtered + EPS);
	map_x_mul_conf = map_x_mul_conf.mul(inverse_conf);
	map_y_mul_conf = map_y_mul_conf.mul(inverse_conf);

	if (type == fgs_uc8)
	{
		map_x_mul_conf.convertTo(dst_map_mat_x, CV_8UC1);
		map_y_mul_conf.convertTo(dst_map_mat_y, CV_8UC1);
		memcpy((BYTE*)(dst_map),         (BYTE*)(dst_map_mat_x.data), w * h);
		memcpy((BYTE*)(dst_map) + w * h, (BYTE*)(dst_map_mat_y.data), w * h);
	}
	else
	{
		map_x_mul_conf.convertTo(dst_map_mat_x, CV_16SC1);
		map_y_mul_conf.convertTo(dst_map_mat_y, CV_16SC1);
		memcpy((short*)(dst_map),         (short*)(dst_map_mat_x.data), sizeof(short) * w * h);
		memcpy((short*)(dst_map) + w * h, (short*)(dst_map_mat_y.data), sizeof(short) * w * h);
	}
}

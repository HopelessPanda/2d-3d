//#define _USE_MATH_DEFINES
//#include <cmath>

#include "depth_estimator.hpp"
#include "metric.h"

#define M_PI 3.14
#define BORDER 0
#define BLOCK_SIZE 8
#define NORMALIZER 10

static unsigned previous_frame_histogram[256];
static int wext;

double distr(double x, double sigma_sq)
{
	return exp(-(x*x) / (2.0 * sigma_sq)) / std::sqrt(2 * M_PI * sigma_sq);
}

double distr2(double x, double y, double sigma_sq)
{
	return exp(-(x*x + y*y) / (2.0 * sigma_sq)) / (2 * M_PI * sigma_sq);
}

template<typename T>
T get_with_bounds(int x, int y, int width, int height, const T* map)
{
	x = std::min(std::max(x, 0), width - 1);
	y = std::min(std::max(y, 0), height - 1);
	return map[y * width + x];
}

#define DEPTH(x, y) get_with_bounds(x, y, width, height, depth_map)
#define DEPTH_PREV(x, y, t) get_with_bounds(x, y, width, height, previous_depths_compensated[t - 1])
#define CUR_Y(x, y) get_with_bounds(x + BORDER, y + BORDER, wext, height + wext * BORDER * 2, cur_Y)
#define CUR_U(x, y) get_with_bounds(x, y, width, height, cur_U)
#define CUR_V(x, y) get_with_bounds(x, y, width, height, cur_V)

template<int d>
BYTE filtered_depth(
	int x,
	int y,
	double sqsigma_depth,
	double sqsigma_color,
	int width,
	int height,
	const BYTE* depth_map,
	const BYTE* cur_Y,
	const short* cur_U,
	const short* cur_V)
{
	double sum = 0.0, sum_k = 0.0;
	for (int j = -d / 2; j <= d / 2; ++j) {
		for (int i = -d / 2; i <= d / 2; ++i) {
			double color_dist =
				std::sqrt(std::pow(CUR_Y(x, y) - CUR_Y(x + i, y + j), 2)
					+ std::pow(CUR_U(x, y) - CUR_U(x + i, y + j), 2)
					+ std::pow(CUR_V(x, y) - CUR_V(x + i, y + j), 2));
			double k =
				distr(std::hypot(i, j), sqsigma_depth)
				* distr(color_dist, sqsigma_color);
			sum += k * DEPTH(x + i, y + j);
			sum_k += k;
		}
	}

	return sum / sum_k;
}

template<int d>
BYTE median_depth(
	int x,
	int y,
	int width,
	int height,
	const BYTE* depth_map)
{
	std::array<BYTE, d * d> depths;
	for (int j = -d / 2; j <= d / 2; ++j) {
		for (int i = -d / 2; i <= d / 2; ++i) {
			depths[(j + d / 2) * d + i + d / 2] = DEPTH(x + i, y + j);
		}
	}

	std::sort(depths.begin(), depths.end());
	return depths[depths.size() / 2];
}

void histogram(int width, int height, const BYTE* depth_map, unsigned hist[256])
{
	std::memset(hist, 0, sizeof(*hist) * 256);

	for (int y = 0; y < height; y += 4) {
		for (int x = 0; x < width; ++x) {
			++hist[depth_map[y * width + x]];
		}
	}
}

void histogram_match(int width, int height, BYTE* depth_map, const unsigned hist[256], const unsigned hist_prev[256])
{
	BYTE replace[256];

	// Build CDFs. Pure black is not included.
	double f[256], f_prev[256];
	f[0] = 0;
	f_prev[0] = 0;
	for (int i = 1; i < 256; ++i) {
		f[i] = f[i - 1] + hist[i];
		f_prev[i] = f_prev[i - 1] + hist_prev[i];
	}
	for (int i = 0; i < 256; ++i) {
		f[i] /= f[255];
		f_prev[i] /= f_prev[255];
	}

	// Build the replacement table.
	int j = 0;
	for (int i = 0; i < 256; ++i) {
		while (f_prev[j] < f[i])
			++j;
		if (j != 0 && (f[i] - f_prev[j - 1]) < (f_prev[j] - f[i]))
			--j;
		replace[i] = j;
	}

	// Replace the depth values.
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			depth_map[y * width + x] = replace[depth_map[y * width + x]];
		}
	}
}

DepthEstimator::DepthEstimator(int width, int height, uint8_t quality)
	: width(width)
	, height(height)
	, quality(quality)
	, width_ext(width + 2 * BORDER)
	, num_blocks_hor((width + BLOCK_SIZE - 1) / BLOCK_SIZE)
	, num_blocks_vert((height + BLOCK_SIZE - 1) / BLOCK_SIZE)
	, first_row_offset(width_ext *  BORDER + BORDER)
{
	frames_processed = 0;
}

DepthEstimator::~DepthEstimator() {
	// PUT YOUR CODE HERE
}

#include <iostream>
void DepthEstimator::Estimate(const uint8_t* cur_Y,
	const int16_t* cur_U,
	const int16_t* cur_V,
	const MV* MVectors,
	uint8_t* depth_map) {
	cout << "in estimate\n";
	//int mv_w = width / 4;
	//int mv_h = height / 4;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			//int mv_x = x / 4, mv_y = y / 4;
			//const MV& mv = MVectors[mv_y * mv_w + mv_x];
			const MV& mv = MVectors[y * width + x];
			depth_map[y * width + x] = std::min(255.0, std::hypot(mv.x, mv.y) * 10);
		}
	}

	// Filtering.
	//std::unique_ptr<BYTE[]> depth_map_copy(depth_map);

	BYTE* new_depth_map = new BYTE[width * height];
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			new_depth_map[y * width + x] = depth_map[y * width + x];
		}
	}
#pragma omp parallel for
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			depth_map[y * width + x] = filtered_depth<10>(x, y, 50, 50, width, height, new_depth_map, cur_Y, cur_U, cur_V);
		}
	}

	/* More filtering.
	std::unique_ptr<BYTE[]> depth_map_copy_2(depth_map);
	depth_map = new BYTE[width * height];
	#pragma omp parallel for
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			depth_map[y * width + x] = median_depth<3>(x, y, width, height, depth_map_copy_2.get());
		}
	}
	PNG_Image::SaveArrayToPNG(depth_map, width, height, "DEPTH_median.png");
	*/
	histogram(width, height, depth_map, previous_frame_histogram);
}

static int frames_processed;
static BYTE* previous_depths_compensated[5];

template<int d>
BYTE filtered_depth_withtime(
	int x,
	int y,
	double sqsigma_depth,
	double sqsigma_color,
	double sqsigma_time,
	int width,
	int height,
	const BYTE* depth_map,
	const BYTE* cur_Y,
	const short* cur_U,
	const short* cur_V,
	int depths_available)
{
	double sum = 0.0, sum_k = 0.0;
	for (int j = -d / 2; j <= d / 2; ++j) {
		for (int i = -d / 2; i <= d / 2; ++i) {
			double color_dist =
				std::sqrt(std::pow(CUR_Y(x, y) - CUR_Y(x + i, y + j), 2)
					+ std::pow(CUR_U(x, y) - CUR_U(x + i, y + j), 2)
					+ std::pow(CUR_V(x, y) - CUR_V(x + i, y + j), 2));

			double k =
				distr(std::hypot(i, j), sqsigma_depth)
				* distr(color_dist, sqsigma_color);

			double d = distr(0, sqsigma_time);
			sum += k * d * DEPTH(x + i, y + j);
			sum_k += k * d;
			for (int t = 1; t <= depths_available; ++t) {
				d = distr(t, sqsigma_time);
				sum += k * d * DEPTH_PREV(x + i, y + j, t);
				sum_k += k * d;
			}
		}
	}

	return sum / sum_k;
}

void DepthEstimator::Estimate_with_time(const uint8_t * cur_Y, const int16_t * cur_U, const int16_t * cur_V, const MV * mvectors, uint8_t * depth_map)
{
	//std::unique_ptr<BYTE[]> previous_depths[5];
	/*
	if (frames_processed >= 2) {
		previous_depths[0].reset(depth_map);
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				previous_depths[0][y * width + x] = depth_map[y * width + x];
			}
		}
		for (int i = 1; i < frames_processed - 1; ++i) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					previous_depths[i][y * width + x] = previous_depths_compensated[i - 1][y * width + x];
				}
			}
			previous_depths_compensated[i - 1] = new BYTE[width * height];
		}

		if (frames_processed == 6)
			delete[] previous_depths_compensated[4];

		previous_depths_compensated[frames_processed - 2] = new BYTE[width * height];
		//depth_map = new BYTE[width * height];
	}
	cout << "!!!\n";
	*/
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			const MV& mv = mvectors[y * width + x];
			depth_map[y * width + x] = std::min(255.0, std::hypot(mv.x, mv.y) * 10);

			// Motion compensation for the previous depth maps.
			/*if (frames_processed >= 2) {
				for (int i = 0; i < frames_processed - 1; ++i) {
					previous_depths_compensated[i][y * width + x] =
						get_with_bounds(x + mv.x, y + mv.y, width, height, previous_depths[i].get());
				}
			}*/
		}
	}
	// Filtering.
	if (frames_processed > 0) {
		BYTE* new_depth_map = new BYTE[width * height];
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				new_depth_map[y * width + x] = depth_map[y * width + x];
			}
		}

		#pragma omp parallel for
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				//if (frames_processed >= 2)
				//	depth_map[y * width + x] = filtered_depth_withtime<15>(x, y, 50, 50, 10, width, height, new_depth_map, cur_Y, cur_U, cur_V, frames_processed - 1);
				//else
					depth_map[y * width + x] = filtered_depth<10>(x, y, 50, 50, width, height, new_depth_map, cur_Y, cur_U, cur_V);
			}
		}
	}

		BYTE* new_depth_map = new BYTE[width * height];
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				new_depth_map[y * width + x] = depth_map[y * width + x];
			}
		}

		// More filtering.
		#pragma omp parallel for
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				depth_map[y * width + x] = median_depth<3>(x, y, width, height, new_depth_map);
			}
		}

	// Even more filtering.
	if (quality == 100) {
		if (frames_processed == 1) {
			histogram(width, height, depth_map, previous_frame_histogram);
		}
		else if (frames_processed > 1) {
			unsigned hist[256];
			histogram(width, height, depth_map, hist);
			histogram_match(width, height, depth_map, hist, previous_frame_histogram);
			std::memcpy(previous_frame_histogram, hist, sizeof(hist));
		}
	}

	if (frames_processed < 6)
		++frames_processed;
}

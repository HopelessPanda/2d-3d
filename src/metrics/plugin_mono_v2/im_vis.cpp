#include "im_vis.h"

void show_image(const cv::Mat image) {
	cv::namedWindow("im_vis(show_image)");
	cv::imshow("im_vis(show_image)", image);
	cv::waitKey(0);
}

void imagesc(const cv::Mat& image) {
	int height = image.rows;
	int width = image.cols;
	cv::Mat result(height, width, CV_8UC3);
	double max = 0.0;
	double min = 0.0;
	cv::minMaxLoc(image, &min, &max);
	double len = max - min;
	double bit_len = len / (256 * 2);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			double bit_num = (image.at<float>(i, j) - min) / bit_len;
			cv::Vec3b &pixel = result.at<cv::Vec3b>(i, j);
			if (bit_num < 256) {
				pixel[2] = 0;
				pixel[1] = uchar(bit_num);
				pixel[0] = uchar(255.0 - bit_num);
			}
			else {
				pixel[2] = uchar(bit_num - 256.0);
				pixel[1] = uchar(255.0 - (bit_num - 256.0));
				pixel[0] = 0;
			}
		}
	}
	//cv::namedWindow("im_vis(imagesc)");
	//cv::imshow("im_vis(imagesc)", result);
	//cv::waitKey(0);
}

void imagesc_save(const cv::Mat& image, const std::string filename, BYTE* mask) {
	int height = image.rows;
	int width = image.cols;
	cv::Mat result(height, width, CV_8UC3);
	double max = 0.0;
	double min = 0.0;
	cv::minMaxLoc(image, &min, &max);
	double len = max - min;
	double bit_len = len / (256 * 2);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int index = i*width + j;
			double bit_num = (image.at<unsigned char>(i, j) - min) / bit_len;
			cv::Vec3b &pixel = result.at<cv::Vec3b>(i, j);
			if(0) {	
				if (bit_num < 256) {
					pixel[2] = image.at<unsigned char>(i, j);//0;
					pixel[1] =image.at<unsigned char>(i, j);//uchar(bit_num);
					pixel[0] =image.at<unsigned char>(i, j);//uchar(255.0 - bit_num);
				}
				else {
					pixel[2] = image.at<unsigned char>(i, j);//uchar(bit_num - 256.0);
					pixel[1] = image.at<unsigned char>(i, j);//uchar(255.0 - (bit_num - 256.0));
					pixel[0] = image.at<unsigned char>(i, j);//0;
				}
			} else {
				if (bit_num < 256) {
					pixel[2] = 255;
					pixel[1] = uchar(bit_num);
					pixel[0] = uchar(255.0 - bit_num);
				}
				else {
					pixel[2] = uchar(bit_num - 256.0);
					pixel[1] = uchar(255.0 - (bit_num - 256.0));
					pixel[0] = 0;
				}
			}
		}
	}
	IplImage ipl = result;
	cvSaveImage(filename.c_str(), &ipl);
	//cv::imwrite(filename, result);
	//std::cout << filename << "  saved" << std::endl;
}

void imagesc2(const cv::Mat& image1, const cv::Mat& image2) {
	int height = image1.rows;
	int width = image1.cols;
	cv::Mat result1(height, width, CV_8UC3);
	double max = 0.0;
	double min = 0.0;
	cv::minMaxLoc(image1, &min, &max);
	double len = max - min;
	double bit_len = len / (256 * 2);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			double bit_num = (image1.at<float>(i, j) - min) / bit_len;
			cv::Vec3b &pixel = result1.at<cv::Vec3b>(i, j);
			if (bit_num < 256) {
				pixel[2] = 0;
				pixel[1] = uchar(bit_num);
				pixel[0] = uchar(255.0 - bit_num);
			}
			else {
				pixel[2] = uchar(bit_num - 256.0);
				pixel[1] = uchar(255.0 - (bit_num - 256.0));
				pixel[0] = 0;
			}
		}
	}

	//cv::namedWindow("im_vis(imagesc21)");
	//cv::imshow("im_vis(imagesc21)", result1);
	cv::Mat result2(height, width, CV_8UC3);
	max = 0.0;
	min = 0.0;
	cv::minMaxLoc(image2, &min, &max);
	len = max - min;
	bit_len = len / (256 * 2);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			double bit_num = (image2.at<float>(i, j) - min) / bit_len;
			cv::Vec3b &pixel = result2.at<cv::Vec3b>(i, j);
			if (bit_num < 256) {
				pixel[2] = 0;
				pixel[1] = uchar(bit_num);
				pixel[0] = uchar(255.0 - bit_num);
			}
			else {
				pixel[2] = uchar(bit_num - 256.0);
				pixel[1] = uchar(255.0 - (bit_num - 256.0));
				pixel[0] = 0;
			}
		}
	}
	//cv::namedWindow("im_vis(imagesc22)");
	//cv::imshow("im_vis(imagesc22)", result2);
	//cv::waitKey(0);
}
﻿#pragma once

#include <opencv2/opencv.hpp>
typedef	unsigned char BYTE;

/**

IM_VIS
Данные функции предназначены для визуализирования изображений в формате OCV(C++)

*/




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//input:	image with any type elements
//
//Совмещение двух операций OCV: namedWindow+imshow
//Ожидает ввода для закрытия
//Тип элементов изображения неважен
void show_image(const cv::Mat image);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//input:	image with CV_32F elements type
//
//Вывод тепловой карты изображения с элементами типа CV_32F. Значения возрастают от синего к красному.
//Ожидает ввода для закрытия
//Аналог imagesc() из Matlab
void imagesc(const cv::Mat& image, const cv::Mat&);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//input:	two images with CV_32F elements type
//
//Вывод двух тепловых карт в разных окнах для двух изображений с элементами типа CV_32F. Значения возрастают от синего к красному.
//Ожидает ввода для закрытия
void imagesc2(const cv::Mat& image1, const cv::Mat& image2);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//input:	image with CV_32F elements type
//			name of file to save visualisation
//
//Сохранение тепловой карты в файл 
void imagesc_save(const cv::Mat& image, const std::string filename, BYTE*);
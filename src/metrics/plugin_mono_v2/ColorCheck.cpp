#include "metric.h"
#include "im_vis.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <algorithm>

using namespace std;
using namespace cv;

// Compute histogram and CDF for an image with mask
// -----
void do1ChnHist(const cv::Mat image, double* hist, double* cdf)
{
    //cout << "A";
    std::cout << "AAAAAAAAA:   " << image.size().width << " x " << image.size().height << " x " << image.channels() << std::endl;
    cv::Mat t = image.reshape(1, 1);

    for (int p = 0; p < t.cols; p++) {
        uchar c = t.at<uchar>(0, p);
        hist[c] += 1.0; // Histogram                                  
    }

    //normalize hist to a max value of 1.0
    cv::Mat _tmp(1, 256, CV_64FC1, hist);
    double minVal, maxVal;
    cv::minMaxLoc(_tmp, &minVal, &maxVal);
    _tmp = _tmp / maxVal;

    // Calculate the Cumulative Distribution Function
    cdf[0] = hist[0];
    for (int j = 1; j < 256; j++)
    {
        cdf[j] = cdf[j - 1] + hist[j];
    }

    //normalize CDF to a max value of 1.0
    _tmp.data = (uchar*)cdf;
    cv::minMaxLoc(_tmp, &minVal, &maxVal);
    _tmp = _tmp / maxVal;
}

// Match Red, Green and Blue histograms of ‘src’ to that of ‘dst’, according to both masks.
Mat histMatchRGB(cv::Mat &src, cv::Mat &dst, PNG_Image m_Left, PNG_Image m_Mono, int type)
{
    int m_height = src.rows;
    int m_width = src.cols;
    bool log = true;

    if (log) {
        cout << "\nin histmatchRGB\n";
    }

    Mat b(src.size().height, src.size().width, CV_8UC1);
    Mat g(src.size().height, src.size().width, CV_8UC1);
    Mat r(src.size().height, src.size().width, CV_8UC1);

    Mat b1(src.size().height, src.size().width, CV_8UC1);
    Mat g1(src.size().height, src.size().width, CV_8UC1);
    Mat r1(src.size().height, src.size().width, CV_8UC1);
    Mat chns[3] = { b, g, r };
    Mat chns1[3] = { b1, g1, r1 };

    m_Mono.ConvertToType(type);
    m_Left.ConvertToType(type);
    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++)
        {
            //cout << j << ' ' << i << endl;
            b.at<uchar>(j, i) = m_Mono.GetData()[(j*m_width + i) * 3];
            g.at<uchar>(j, i) = m_Mono.GetData()[(j*m_width + i) * 3 + 1];
            r.at<uchar>(j, i) = m_Mono.GetData()[(j*m_width + i) * 3 + 2];
            b1.at<uchar>(j, i) = m_Left.GetData()[(j*m_width + i) * 3];
            g1.at<uchar>(j, i) = m_Left.GetData()[(j*m_width + i) * 3 + 1];
            r1.at<uchar>(j, i) = m_Left.GetData()[(j*m_width + i) * 3 + 2];
        }
    }

    //imshow("b", b);
    //imshow("g", g);
    //imshow("r", r);
    //waitKey(0);
    if (log) {
        cout << "split\n";
    }

    cv::Mat src_hist = cv::Mat::zeros(1, 256, CV_64FC1);
    cv::Mat dst_hist = cv::Mat::zeros(1, 256, CV_64FC1);
    cv::Mat src_cdf = cv::Mat::zeros(1, 256, CV_64FC1);
    cv::Mat dst_cdf = cv::Mat::zeros(1, 256, CV_64FC1);
    cv::Mat Mv(1, 256, CV_8UC1);
    uchar* M = Mv.ptr();

    for (int i = 0; i < 3; i++)
    {
        
        if (log) {
            cout << "computing channel " << i << endl;
        }
        src_hist.setTo(0);
        dst_hist.setTo(0);
        src_cdf.setTo(0);
        dst_cdf.setTo(0);

        double* _src_cdf = (double*)src_cdf.data;
        double* _dst_cdf = (double*)dst_cdf.data;
        double* _src_hist = (double*)src_hist.data;
        double* _dst_hist = (double*)dst_hist.data;

        cout << "doing 1hist\n";
        do1ChnHist(chns[i], _src_hist, _src_cdf);
        do1ChnHist(chns1[i], _dst_hist, _dst_cdf);
        cout << "done 1hist\n";

        uchar last = 0;

        double HISTMATCH_EPSILON = 0.0000000001;
        for (int j = 0; j < 256; j++)
        {
            double F1j = _src_cdf[j];

            for (int k = last; k < 256; k++)
            {
                if (abs(F1j - _dst_cdf[k]) < HISTMATCH_EPSILON) {
                    //cout << F1j << ' ' << _dst_cdf[k] << '\n';
                    last = k;
                    break;
                }
                else {
                    if (k < 255 && _dst_cdf[k] <= F1j && F1j <= _dst_cdf[k + 1])
                        cout << _dst_cdf[k] << ' ' << F1j << ' ' << _dst_cdf[k + 1] << endl;
                    last = (abs(_dst_cdf[k] - F1j) > abs(_dst_cdf[k + 1] - F1j)) ? k + 1 : k;
                    break;
                }
            }
            cout << (int)last << ' ';
            M[j] = last;
            last;
        }

        //if (type == 3)for (int i = 0; i < 256; i++)
        //    cout << i << ' ' << (int)M[i] << endl;

        cv::Mat lut(1, 256, CV_8UC1, M);
        //imshow("d", chns[i]);
        cv::LUT(chns[i], lut, chns[i]);

        //imshow("sf", chns[i]);

    }

    if (log) {
        cout << "time to merge!\n";
    }

    Mat res(chns[0].size().height, chns[0].size().width, CV_8UC3);
    for (int i = 0; i < chns[0].size().width; i++) {
        for (int j = 0; j < chns[0].size().height; j++) {
            Vec3b &pixel = res.at<Vec3b>(j, i);
            pixel[0] = b.at<uchar>(j, i);
            pixel[1] = g.at<uchar>(j, i);
            pixel[2] = r.at<uchar>(j, i);
        }
    }
    return res;
}

PNG_Image brightMatch(PNG_Image &dst, PNG_Image &src)
{
    int dst_sum = 0;
    int src_sum = 0;
    dst.ConvertToType(7);
    src.ConvertToType(7);
    cout << "src type " << src.type() << endl;
    cout << "dst type " << dst.type() << endl;
    for (int i = 0; i < dst.height(); i++) {
        for (int j = 0; j < dst.width(); j++) {
            int index = i*dst.width() + j;
            dst_sum += dst.GetData()[index];
            src_sum += src.GetData()[index];
        }
    }
    double dst_av = (double)dst_sum / (dst.width() * dst.height());
    double src_av = (double)src_sum / (dst.width() * dst.height());
    cout << dst_sum << ' ' << src_sum << endl;
    cout << dst_av << ' ' << src_av << endl;
    double k = dst_av / (double)src_av;
    cout << "k " << k << endl;

    Mat m_src(src.height(), src.width(), CV_8UC3, src.GetData());
    m_src.convertTo(m_src, -1, k, 0);
    return src;
}

int CMonoPlugin::Brightness_Color_check(PNG_Image *mono, PNG_Image *left)
{
    /*
    PNG_Image tmp = m_Left;
    tmp.ConvertToType(0);
    Mat left(m_height, m_width, CV_8UC3, tmp.GetData());

    tmp = m_Mono;
    tmp.ConvertToType(0);
    Mat mono(m_height, m_width, CV_8UC3, tmp.GetData());
    m_Mono.SaveToPNG("mono_.png");
    m_Left.SaveToPNG("left_.png");

    Mat res = histMatchRGB(left, mono, m_Left, m_Mono, 6); //bgr_i
    PNG_Image result;
    result.Set(res.size().width, res.size().height, 6, res.data);
    result.SaveToPNG("result_bgr_i.png");

    PNG_Image res1 = brightMatch(m_Left, result);
    res1.SaveToPNG("res_bright.png");
      */

    int sum = 0;
    int total = left->width() * left->height();
    BYTE *lh = left->GetData();
    for (int i = 0; i < left->height(); i++) {
        for (int j = 0; j < left->width(); j++) {
            int index = i*left->width() + j;
            sum += lh[index];
        }
    }
    double left_average = (double)sum / total;
    sum = 0;

    BYTE *mh = mono->GetData();
    for (int i = 0; i < left->height(); i++) {
        for (int j = 0; j < left->width(); j++) {
            int index = i*left->width() + j;
            sum += mh[index];
        }
    }
    double mono_average = (double)sum / total;
    int incr = left_average - mono_average;

    incr = 10;
    for (int i = 0; i < left->height(); i++) {
        for (int j = 0; j < left->width(); j++) {
            int index = i*left->width() + j;
            if (mono->GetData()[index] + incr > 255) {
                mono->GetData()[index] = 255;
            }
            else {
                mono->GetData()[index] += incr;
            }
        }
    }
    
    mono->SaveToPNG("old.png");
    
    return incr;

    return 0;
}
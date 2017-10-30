#include "log.h"
#include <iostream>

using namespace cv;

void LOG::print(string txt, int mode)
{
    if (mode <= detail_level) {
        cout << txt << endl;
    }
}

void LOG::vis_segments(BYTE * segmentation_map, int height, int width, string name, int framenum)
{
    if (true) {
        BYTE *tmp = (BYTE*)malloc(height*width*sizeof(BYTE));
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                tmp[i*width + j] = segmentation_map[i*width + j];
            }
        }
        Mat seg_map(height, width, CV_8UC1, tmp);
        double min, max;
        double newMin = 50, newMax = 255;
        Point minLoc, maxLoc;
        minMaxLoc(seg_map, &min, &max, &minLoc, &maxLoc);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                tmp[i*width + j] *= (newMax - newMin) / (max - min);
                tmp[i*width + j] += newMin;
            }
        }
		stringstream sss;
		sss << framenum << "_" << name;
        PNG_Image::SaveArrayToPNG(seg_map.data, width, height, sss.str().c_str());
        free(tmp);
   }
}

void LOG::print_byte_array(BYTE * arr, int height, int width, int mode)
{
    if (mode <= detail_level) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cout << (int)arr[i*width + j] << ' ';
            }
            cout << endl;
        }
    }
}


void LOG::vis_grey_image(BYTE *data, int height, int width, string name, int framenum) // (full name).png | framenumber_(m|l|r|smth).png
{
    if (visualization) {
        if (framenum == -1) {
            PNG_Image::SaveArrayToPNG(data, width, height, name.c_str());
        }
        else {
            stringstream sss;
            sss << framenum;
            string cust_name = sss.str() + name;
            PNG_Image::SaveArrayToPNG(data, width, height, cust_name.c_str());
        }
    }
}

void LOG::vis_color_image(BYTE * data, int height, int width, string name)
{
}

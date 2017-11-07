#include "log.h"
#include <iostream>

using namespace cv;

void LOG::print(string txt, int mode)
{
    if (mode <= detail_level) {
        cout << txt << endl;
    }
}

void LOG::vis_normalized(BYTE * segmentation_map, int height, int width, string name, int framenum, double newMin, double newMax)
{
    if (visualization) {
        BYTE *tmp = (BYTE*)malloc(height*width*sizeof(BYTE));
		double min = 10000, max = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                tmp[i*width + j] = segmentation_map[i*width + j];
				if (tmp[i*width + j] < min) min = tmp[i*width + j];
				if (tmp[i*width + j] > max) max = tmp[i*width + j];
            }
        }        
		double coef = (newMax - newMin) / (max - min);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
				tmp[i*width + j] *= coef;
                tmp[i*width + j] += newMin;
            }
        }
		stringstream sss;
		sss << framenum << "_" << name;
        PNG_Image::SaveArrayToPNG(tmp, width, height, sss.str().c_str());
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


void LOG::vis_grey_image(BYTE *data, int height, int width, string name, int framenum) // (full name).png | framenum_(m|l|r|smth).png
{
    if (visualization) {
        if (framenum == -1) {
            PNG_Image::SaveArrayToPNG(data, width, height, name.c_str());
        }
        else {
            stringstream sss;
            sss << framenum;
            string cust_name = sss.str() + '_' + name;
            PNG_Image::SaveArrayToPNG(data, width, height, cust_name.c_str());
        }
    }
}

void LOG::vis_color_image(BYTE * data, int height, int width, string name)
{
}

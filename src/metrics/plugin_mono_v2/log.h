#pragma once
#include <string>
#include "metric.h"

using namespace std;

enum log {
    NONE = 0,
    BASIC = 1,
    PANIC = 2
};

class LOG
{  
public:
    bool log;
    int detail_level;
    bool visualization;
    LOG(bool l = false, int level = 1, bool vis = false) {
        detail_level = level;  
        visualization = vis; 
        log = l;
    }

    void print(string txt, int mode = 2);
    template<typename T> void print(string txt, T n, int mode = 2);
    template<typename T> void print_arr(string txt, T *n, int length, int mode = 2);
    void print_byte_array(BYTE *arr, int height, int width, int mode = 2);

    void vis_segments(BYTE* dst_segmentation_map, int height, int width, string name, int framenum); // save image with normalization
    void vis_grey_image(BYTE *data, int height, int width, string name, int framenum = -1);  // (full name).png | framenumber_(m|l|r|smth).png
    void vis_color_image(BYTE *data, int height, int width, string name); // not done
};

template<typename T>
inline void LOG::print(string txt, T n, int mode)
{
    if (mode <= detail_level) {
        cout << txt << ": " << n << endl;
    }
}

template<typename T>
inline void LOG::print_arr(string txt, T *n, int length, int mode)
{
    if (mode <= detail_level) {
        cout << txt << ": " << endl;
        for (int i = 0; i < length; i++) {
            cout << n[i] << endl;
        }
    }
}
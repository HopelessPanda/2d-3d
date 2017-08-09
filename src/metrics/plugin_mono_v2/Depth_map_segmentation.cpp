#include "Depth_map_segmentation.h"
#include <opencv2/core/core.hpp>
//#include "Useful_OpenCV_functions.h"
#include <math.h>
#include <string>
#include <iostream>
#define PI 3.1415

double g(int x,double sigma)
{
    return 1/(sqrt(2*PI)*sigma) * exp(-x*x/(2*sigma*sigma));
}
void DepthSegmentationEngine::Init(int _w,int _h,int _prefiltering_radius,bool _use_adaptive_strength,double _static_strength)
{
    w = _w;
    h = _h;
    prefiltering_radius = _prefiltering_radius;
    use_adaptive_strength = _use_adaptive_strength;
    strength = _static_strength;

    gauss_kernel = new double[2*256+1];
    depth_histogram = new int[256];
    extended_depth_histogram = new int[3*256];
    smoothed_depth_histogram = new double[256];
    filtered_depth_map = new BYTE[w*h];
    histogram_labeling = new int[256];
    flood_fill_list = new pixel[w*h];
}

void DepthSegmentationEngine::Deinit()
{
    delete[] gauss_kernel;
    delete[] depth_histogram;
    delete[] extended_depth_histogram;
    delete[] smoothed_depth_histogram;
    delete[] filtered_depth_map;
    delete[] histogram_labeling;
    delete[] flood_fill_list;
}
int DepthSegmentationEngine::TraceHistogramLocalMaximum(double* hist,int hist_size,int startpos)
{
    double max_diff;
    int shift;
    
    int x = startpos;
    while(1)
    {
        max_diff = 0;
        if(x>0)
        {
            if(hist[x-1] - hist[x]>max_diff)
            {
                max_diff = hist[x-1] - hist[x];
                shift = -1;
            }
        }
        if(x<hist_size-1)
        {
            if(hist[x+1] - hist[x]>max_diff)
            {
                max_diff = hist[x+1] - hist[x];
                shift = 1;
            }
        }

        if(max_diff<0.01) // reached a local maximum
            break;
        else
            x+=shift;
    }
    return x;
}

void DepthSegmentationEngine::ExtendHistogram(int* src_hist,int* dst_hist,int hist_size,int border_size)
{
    for(int i=0;i<border_size;i++)
    {
        dst_hist[i] = src_hist[0];
    }
    for(int i=border_size;i<hist_size+border_size;i++)
    {
        dst_hist[i] = src_hist[i-border_size];
    }
    for(int i=hist_size+border_size;i<hist_size+2*border_size;i++)
    {
        dst_hist[i] = src_hist[255];
    }
}
void DepthSegmentationEngine::SmoothHistogram(int* src_hist,double* dst_hist,int hist_size,double sigma)
{
    //prepare kernel and add borders to the histogram:
    int rad = (int)(3*sigma+1);
    int sz = 2*rad+1;
    double sum_coef = 0;

    for(int i=0;i<sz;i++)
    {
        gauss_kernel[i] = g(i-rad,sigma);
        sum_coef+=gauss_kernel[i];
    }

    int border = rad+1; 
    ExtendHistogram(src_hist,extended_depth_histogram,hist_size,border);

    //apply gaussian filter to the histogram:
    double cur_sum;
    for(int i=0;i<hist_size;i++)
    {
        cur_sum = 0;
        for(int j=0;j<sz;j++)
        {
            cur_sum+= gauss_kernel[j]*extended_depth_histogram[border+i+j-rad];
        }
        cur_sum/=sum_coef;
        dst_hist[i] = cur_sum;
    }
}

double DepthSegmentationEngine::GetDepthRange(BYTE* depth_map)
{
    double mean=0,lower_mean=0,upper_mean=0;
    int lower_num=0,upper_num=0;
    int sz = w*h;

    for(int i=0;i<sz;i++)
    {
        mean += depth_map[i];
    }
    mean/= sz;

    for(int i=0;i<sz;i++)
    {
        if(depth_map[i]>mean)
        {
            upper_mean+=depth_map[i];
            upper_num++;
        }
        else
        {
            lower_mean+=depth_map[i];
            lower_num++;
        }
    }

    if(upper_num==0 || lower_num==0)
        return 0;
    else
    {
        upper_mean/=upper_num;
        lower_mean/=lower_num;
    }
    return (upper_mean-lower_mean);
}

void DepthSegmentationEngine::LabelHistogram(double* src_hist,int* dst_hist_labeling,int hist_size)
{
    for(int i=0;i<hist_size;i++)
        dst_hist_labeling[i] = -1;
    int local_max_index;
    int cur_segment = -1;
    for(int i=0;i<hist_size;i++)
    {
        if(src_hist[i]>0.01)
        {
            local_max_index = TraceHistogramLocalMaximum(src_hist,hist_size,i);
            if(dst_hist_labeling[local_max_index]==-1)
            {
                cur_segment++;
                dst_hist_labeling[local_max_index] = cur_segment;
                dst_hist_labeling[i] = cur_segment;
            }
            else
                dst_hist_labeling[i] = dst_hist_labeling[local_max_index];
        }
    }
}

void DepthSegmentationEngine::FloodFill(BYTE* src,pixel starting_point,int target_label)
{
    int cur_x,cur_y;
    flood_fill_list[0] = starting_point;
    int list_start=0,list_end=1;
    int source_label = src[starting_point.y*w+starting_point.x];
    src[starting_point.y*w+starting_point.x] = target_label;

    while(list_end-list_start>0)
    {
        cur_x = flood_fill_list[list_start].x;
        cur_y = flood_fill_list[list_start].y;
        list_start++;

        // checking neighbors:
        for(int i=-1;i<=1;i++)
            for(int j=-1;j<=1;j++)
            {
                if(i==0&&j==0)
                    continue;
                if((cur_x+j)>=0 && (cur_x+j)<=w-1 && (cur_y+i)>=0 && (cur_y+i)<=h-1)
                {
                    if(src[(cur_y+i)*w+(cur_x+j)]==source_label)
                    {
                        src[(cur_y+i)*w+(cur_x+j)] = target_label;
                        flood_fill_list[list_end] = pixel(cur_x+j,cur_y+i);
                        list_end++;
                    }
                }
            }
    }
}

int DepthSegmentationEngine::ConnectedComponentSegmentation(BYTE* segmentation_map)
{
    int src_max_segment = 0;
    for(int i=0;i<w*h;i++)
        if(segmentation_map[i]>src_max_segment)
            src_max_segment = segmentation_map[i];
    int cur_segment = src_max_segment+1;

    for(int i=0;i<h;i++)
        for(int j=0;j<w;j++)
        {
            if(segmentation_map[i*w+j]<=src_max_segment)
            {
                FloodFill(segmentation_map,pixel(j,i),cur_segment);
                cur_segment++;
            }
        }
    
    int dst_max_segment = 0;
    for(int i=0;i<w*h;i++)
    {
        segmentation_map[i] -= (src_max_segment+1);
        if(segmentation_map[i]>dst_max_segment)
            dst_max_segment = segmentation_map[i];
    }
    
    return (dst_max_segment+1);
}

#include <metrics/common_algorithms/PNG_Image/png_image.h>
int DepthSegmentationEngine::GetSegmentationMap(BYTE* src_depth_map, BYTE* dst_segmentation_map)
{
    memcpy(filtered_depth_map,src_depth_map,w*h);
    if(use_adaptive_strength)
    {
        double depth_range = GetDepthRange(filtered_depth_map);
        strength = depth_range/6.0;
    }
    
    if(strength<0.1)
    {
        memset(dst_segmentation_map,0,w*h);
        return 1;
    }

    memset(depth_histogram,0,256*sizeof(int));
    for(int i=0;i<w*h;i++)
        depth_histogram[filtered_depth_map[i]]++;
        
    SmoothHistogram(depth_histogram,smoothed_depth_histogram,256,strength);
    LabelHistogram(smoothed_depth_histogram,histogram_labeling,256);

    for(int i=0;i<w*h;i++) {
        dst_segmentation_map[i] = histogram_labeling[filtered_depth_map[i]];
    }            
    return ConnectedComponentSegmentation(dst_segmentation_map);
}


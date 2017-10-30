#include "auxiliary_functions.h"
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <algorithm>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

inline double exp_RGB(double r1,double g1,double b1,double r2,double g2,double b2,double sigma)
{
	return exp(-((r1-r2)*(r1-r2)+(g1-g2)*(g1-g2)+(b1-b2)*(b1-b2))/(2*sigma*sigma));
}
inline double inv1_RGB(double r1,double g1,double b1,double r2,double g2,double b2,double sigma)
{
	return 1.0/((abs(r1-r2)+abs(g1-g2)+abs(b1-b2))+sigma);
}
inline double inv2_RGB(double r1,double g1,double b1,double r2,double g2,double b2,double sigma)
{
	return 1.0/(((r1-r2)*(r1-r2)+(g1-g2)*(g1-g2)+(b1-b2)*(b1-b2))+(sigma*sigma));
}

inline double exp_grayscale(double v1,double v2,double sigma)
{
	return exp(-((v1-v2)*(v1-v2))/(2*sigma*sigma));
}
inline double inv1_grayscale(double v1,double v2,double sigma)
{
	return 1.0/((abs(v1-v2))+sigma);
}
inline double inv2_grayscale(double v1,double v2,double sigma)
{
	return 1.0/(((v1-v2)*(v1-v2))+(sigma*sigma));
}

void ApplyKMeansClustering(int* dst_label_map,double* dst_cluster_centers,BYTE* RGB_hist_mask,int num_points,int k,int sz)
{
	if(num_points<=k)
	{
		int label=0;
		for(int i=0;i<sz;i++)
			for(int j=0;j<sz;j++)
				for(int k=0;k<sz;k++)
				{
					if(RGB_hist_mask[i*sz*sz+j*sz+k]>0)
					{
						dst_label_map[i*sz*sz+j*sz+k]  = label;
						dst_cluster_centers[3*label]   = (double)i;
						dst_cluster_centers[3*label+1] = (double)j;
						dst_cluster_centers[3*label+2] = (double)k;
						label++;
					}
				}
		return;
	}

	srand(101);
	int* points_array = new int[3*num_points];
	int* points_label_map = new int[num_points];
	int pos = 0;
	for(int i=0;i<sz;i++)
		for(int j=0;j<sz;j++)
			for(int k=0;k<sz;k++)
			{
				if(RGB_hist_mask[i*sz*sz+j*sz+k]>0)
				{
					points_array[pos]   = i;
					points_array[pos+1] = j;
					points_array[pos+2] = k;
					pos+=3;
				}
			}

	int selected_point;
	for(int i=0;i<k;i++)
	{
		selected_point = rand()%num_points;
		dst_cluster_centers[3*i]   = points_array[3*selected_point];
		dst_cluster_centers[3*i+1] = points_array[3*selected_point+1];
		dst_cluster_centers[3*i+2] = points_array[3*selected_point+2];
	}

	int max_iter = 10;
	int cur_iter = 0;
	bool changed = true;
	double min_dist;
	double cur_dist;

	int* num_points_array = new int[k];
	double* cluster_centers_new = new double[3*k];
	double eps = 1;
	double max_changed;
	double cur_changed;
	int i_pos,j_pos;

	while(changed && cur_iter<max_iter)
	{
		changed = false;
#pragma omp parallel for private(min_dist) private(cur_dist) private(i_pos) private(j_pos)
		for(int i=0;i<num_points;i++)
		{
			min_dist = 10000;
			i_pos = 3*i;
			j_pos = 0;
			for(int j=0;j<k;j++)
			{
				cur_dist = (points_array[i_pos]-dst_cluster_centers[j_pos])    *(points_array[i_pos]-dst_cluster_centers[j_pos])+
					       (points_array[i_pos+1]-dst_cluster_centers[j_pos+1])*(points_array[i_pos+1]-dst_cluster_centers[j_pos+1])+
						   (points_array[i_pos+2]-dst_cluster_centers[j_pos+2])*(points_array[i_pos+2]-dst_cluster_centers[j_pos+2]);
				if(cur_dist<min_dist)
				{
					min_dist = cur_dist;
					points_label_map[i] = j;
				}
				j_pos+=3;
			}
		}
		memset(cluster_centers_new,0,3*k*sizeof(double));
		memset(num_points_array,0,k*sizeof(int));		
		for(int i=0;i<num_points;i++)
		{
			cluster_centers_new[3*points_label_map[i]]+=points_array[3*i];
			cluster_centers_new[3*points_label_map[i]+1]+=points_array[3*i+1];
			cluster_centers_new[3*points_label_map[i]+2]+=points_array[3*i+2];
			num_points_array[points_label_map[i]]++;
		}

		max_changed = 0;
		for(int i=0;i<k;i++)
		{
			cluster_centers_new[3*i]/=num_points_array[i];
			cluster_centers_new[3*i+1]/=num_points_array[i];
			cluster_centers_new[3*i+2]/=num_points_array[i];
			cur_changed = (cluster_centers_new[3*i]-dst_cluster_centers[3*i])    *(cluster_centers_new[3*i]-dst_cluster_centers[3*i])+
				          (cluster_centers_new[3*i+1]-dst_cluster_centers[3*i+1])*(cluster_centers_new[3*i+1]-dst_cluster_centers[3*i+1])+
						  (cluster_centers_new[3*i+2]-dst_cluster_centers[3*i+2])*(cluster_centers_new[3*i+2]-dst_cluster_centers[3*i+2]);
			if(cur_changed>max_changed)
				max_changed=cur_changed;
		}
		memcpy(dst_cluster_centers,cluster_centers_new,3*k*sizeof(double));
		if(max_changed>eps)
			changed = true;
		cur_iter++;
	}

	for(int i=0;i<num_points;i++)
		dst_label_map[points_array[3*i]*sz*sz+points_array[3*i+1]*sz+points_array[3*i+2]] = points_label_map[i];

	delete[] points_array;
	delete[] points_label_map;
	delete[] num_points_array;
	delete[] cluster_centers_new;
}

template<typename T>
void PreprocessRGBGuideImage(T* dst,double* dst_affinity_map,weight_function f, double sigma,BYTE* src_guide,int w,int h,int nF,int num_auxiliary_planes,BYTE** auxiliary_guide_planes)
{
	int q = 2;
	int hist_sz = 256>>q;
	BYTE* hist_mask = new BYTE[hist_sz*hist_sz*hist_sz];
	int* label_map = new int[hist_sz*hist_sz*hist_sz];
	double* cluster_centers = new double[3*nF];

	memset(hist_mask,0,hist_sz*hist_sz*hist_sz);
	int sz = w*h;
	int r,g,b;
	int num_points = 0;

	for(int i=0;i<sz;i++)
	{
		r = src_guide[i]>>q;
		g = src_guide[sz+i]>>q;
		b = src_guide[2*sz+i]>>q;
		if(hist_mask[r*hist_sz*hist_sz+g*hist_sz+b]==0)
		{
			num_points++;
			hist_mask[r*hist_sz*hist_sz+g*hist_sz+b] = 255;
		}
	}

	BYTE* cur_guide;
	for(int n=0;n<num_auxiliary_planes;n++)
	{
		cur_guide = auxiliary_guide_planes[n];
		for(int i=0;i<sz;i++)
		{
			r = cur_guide[i]>>q;
			g = cur_guide[sz+i]>>q;
			b = cur_guide[2*sz+i]>>q;
			if(hist_mask[r*hist_sz*hist_sz+g*hist_sz+b]==0)
			{
				num_points++;
				hist_mask[r*hist_sz*hist_sz+g*hist_sz+b] = 255;
			}
		}
	}

	ApplyKMeansClustering(label_map,cluster_centers,hist_mask,num_points,nF,hist_sz);

	for(int i=0;i<sz;i++)
	{
		r = src_guide[i]>>q;
		g = src_guide[sz+i]>>q;
		b = src_guide[2*sz+i]>>q;
		dst[i] = label_map[r*hist_sz*hist_sz+g*hist_sz+b];
	}

	for(int n=0;n<num_auxiliary_planes;n++)
	{
		cur_guide = auxiliary_guide_planes[n];
		for(int i=0;i<sz;i++)
		{
			r = cur_guide[i]>>q;
			g = cur_guide[sz+i]>>q;
			b = cur_guide[2*sz+i]>>q;
			dst[(n+1)*w*h+i] = label_map[r*hist_sz*hist_sz+g*hist_sz+b];
		}
	}

	switch(f)
	{
	case EXP:
		for(int i=0;i<nF;i++)
			for(int j=0;j<nF;j++)
			{
				dst_affinity_map[i*nF+j] = exp_RGB(cluster_centers[3*i],cluster_centers[3*i+1],cluster_centers[3*i+2],cluster_centers[3*j],cluster_centers[3*j+1],cluster_centers[3*j+2],sigma);
			}
		break;
	case INV1:
		for(int i=0;i<nF;i++)
			for(int j=0;j<nF;j++)
			{
				dst_affinity_map[i*nF+j] = inv1_RGB(cluster_centers[3*i],cluster_centers[3*i+1],cluster_centers[3*i+2],cluster_centers[3*j],cluster_centers[3*j+1],cluster_centers[3*j+2],sigma);
			}
		break;
	case INV2:
		for(int i=0;i<nF;i++)
			for(int j=0;j<nF;j++)
			{
				dst_affinity_map[i*nF+j] = inv2_RGB(cluster_centers[3*i],cluster_centers[3*i+1],cluster_centers[3*i+2],cluster_centers[3*j],cluster_centers[3*j+1],cluster_centers[3*j+2],sigma);
			}
		break;
	}

	delete[] hist_mask;
	delete[] label_map;
	delete[] cluster_centers;
}
template void PreprocessRGBGuideImage<BYTE>(BYTE* dst,double* dst_affinity_map,weight_function f, double sigma,BYTE* src_guide,int w,int h,int nF,int num_auxiliary_planes,BYTE** auxiliary_guide_planes);
template void PreprocessRGBGuideImage<WORD>(WORD* dst,double* dst_affinity_map,weight_function f, double sigma,BYTE* src_guide,int w,int h,int nF,int num_auxiliary_planes,BYTE** auxiliary_guide_planes);

void PreprocessGrayscaleGuideImage(BYTE* dst,double* dst_affinity_map,weight_function f,double sigma,BYTE* src_guide,int w,int h,int num_auxiliary_planes,BYTE** auxiliary_guide_planes)
{
	int sz = w*h;
	memcpy(dst,src_guide,sz);
	for(int n=0;n<num_auxiliary_planes;n++)
		memcpy(dst+(n+1)*sz,auxiliary_guide_planes[n],sz);

	switch(f)
	{
	case EXP:
		for(int i=0;i<256;i++)
			for(int j=0;j<256;j++)
			{
				dst_affinity_map[i*256+j] = exp_grayscale(i,j,sigma);
			}
			break;
	case INV1:
		for(int i=0;i<256;i++)
			for(int j=0;j<256;j++)
			{
				dst_affinity_map[i*256+j] = inv1_grayscale(i,j,sigma);
			}
			break;
	case INV2:
		for(int i=0;i<256;i++)
			for(int j=0;j<256;j++)
			{
				dst_affinity_map[i*256+j] = inv2_grayscale(i,j,sigma);
			}
			break;
	}

}

template<typename T>
struct px
{
	int pos;
	T val;
	bool operator <(const px& rhs)
	{
		return val<rhs.val;
	}
};


template<typename T>
void AdaptiveQuantization(WORD* dst,T* dst_mapping,T* src,int w,int h,int nI,int num_auxiliary_planes,T** auxiliary_src_planes)
{
	int full_sz = (1+num_auxiliary_planes)*w*h;
	px<T> *data = new px<T>[full_sz];

	for(int i=0;i<w*h;i++)
	{
		data[i].pos = i;
		data[i].val = src[i];
	}
	for(int n=0;n<num_auxiliary_planes;n++)
	{
		for(int i=0;i<w*h;i++)
		{
			data[(n+1)*w*h+i].pos = (n+1)*w*h+i;
			data[(n+1)*w*h+i].val = auxiliary_src_planes[n][i];
		}
	}
	std::sort(data,data+full_sz);

	double min_val,max_val;
	min_val = data[0].val;
	max_val = data[full_sz-1].val;

	double maxRange = max_val-min_val;
	int accuracy = 256;

	double l = 0,r = maxRange*2.0/nI,m;
	double thresh = r/accuracy;

	bool decrease_error_bound;
	double base;
	int cnt;

	while(r-l > thresh)
	{
		m = (r+l)/2;
		decrease_error_bound = true;
		base = min_val;
		cnt=0;

		for(int i=0;i<full_sz;i++)
		{
			if(data[i].val>base+m)
			{
				cnt++;
				base = data[i].val;
				if(cnt==nI)
				{
					decrease_error_bound = false;
					break;
				}
			}
		}
		if(decrease_error_bound)
			r=m;
		else 
			l=m;
	}

	base = min_val;
	int base_idx = 0;
	cnt = 0;
	for(int i=0;i<=full_sz;i++)
	{
		if(data[i].val>base+r || i==full_sz)
		{
			dst_mapping[cnt] = data[(base_idx+i-1)>>1].val; //median of current cluster
			if(i==full_sz) break;
			cnt++;
			base = data[i].val;
			base_idx = i;
		}
		dst[data[i].pos] = cnt;
	}
	
	delete[] data;
}
template void AdaptiveQuantization<float>(WORD* dst,float* dst_mapping,float* src,int w,int h,int nI,int num_auxiliary_planes,float** auxiliary_src_planes);
template void AdaptiveQuantization<int>  (WORD* dst,int*   dst_mapping,int*   src,int w,int h,int nI,int num_auxiliary_planes,int**   auxiliary_src_planes);

template<typename T>
void AddBorders(T* dst,T* src,int w,int h,int border_sz)
{
	int wext = w+2*border_sz;
	int hext = h+2*border_sz;
	int i_pos,j_pos;

	for(int i=0;i<hext;i++)
		for(int j=0;j<wext;j++)
		{
			i_pos = min(max(0,i-border_sz),h-1);
			j_pos = min(max(0,j-border_sz),w-1);
			dst[i*wext+j] = src[i_pos*w+j_pos];
		}
}


template void AddBorders<BYTE>(BYTE* dst,BYTE* src,int w,int h,int border_sz);
template void AddBorders<WORD>(WORD* dst,WORD* src,int w,int h,int border_sz);

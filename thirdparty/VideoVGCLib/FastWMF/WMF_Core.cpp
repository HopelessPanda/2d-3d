#include "auxiliary_functions.h"
#include "omp.h"
#include <assert.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct necklace_table
{
	int* val;
	short* next;
	short* prev;
	void Init(int sz)
	{
		val = new int[sz];
		next = new short[sz];
		prev = new short[sz];
	}
	void Deinit()
	{
		delete[] val;
		delete[] next;
		delete[] prev;
	}
};
inline void NecklaceInsertElement(short* prev_array,short* next_array,int pos)
{
	next_array[pos] = next_array[0];
	prev_array[next_array[pos]] = pos;
	next_array[0] = pos;
	prev_array[pos] = 0;
}
inline void NecklaceDeleteElement(short* prev_array,short* next_array,int pos)
{
	next_array[prev_array[pos]] = next_array[pos];
	prev_array[next_array[pos]] = prev_array[pos];
}

template<typename T,typename T_guide>
inline void UpdateTables(necklace_table joint_histogram,necklace_table BCB, int cut_point,T* src,T_guide* guide,int w,int h,int rad,int j,int i,int j_prev,int i_prev,BYTE* confidence,int nF,int num_planes)
{
	int j_shift = j-j_prev;
	int i_shift = i-i_prev;
	int i_pos,j_pos;
	int i_offset,j_offset;
	int val;
	int idx;
	int cur_pos;
	int start_pos,end_pos;
	int* cur_val_array;
	int fNegate;

	if(i_shift!=0)
	{
		start_pos = (i_prev - i_shift*rad)*w+j-rad;
		end_pos = start_pos+2*rad;

		for(int n=0;n<num_planes;n++)
			for(int pos=start_pos;pos<=end_pos;pos++)
			{
				cur_pos = n*w*h+pos;
				i_offset = src[cur_pos]*(nF+1);
				j_offset = guide[cur_pos]+1;
				fNegate = (src[cur_pos] <= cut_point);
				val = (((int)confidence[cur_pos]) ^ -fNegate) + fNegate; 
				if(BCB.val[j_offset]==0)
					NecklaceInsertElement(BCB.prev,BCB.next,j_offset);
				BCB.val[j_offset]+=val;
				if(BCB.val[j_offset]==0)
					NecklaceDeleteElement(BCB.prev,BCB.next,j_offset);

				cur_val_array = joint_histogram.val+i_offset;
				cur_val_array[j_offset]-=confidence[cur_pos];
				if(cur_val_array[j_offset]==0)
					NecklaceDeleteElement(joint_histogram.prev+i_offset,joint_histogram.next+i_offset,j_offset);
			}

		start_pos = (i + i_shift*rad)*w+j-rad;
		end_pos = start_pos+2*rad;

		for(int n=0;n<num_planes;n++)
			for(int pos=start_pos;pos<=end_pos;pos++)
			{
				cur_pos = n*w*h+pos;
				i_offset = src[cur_pos]*(nF+1);
				j_offset = guide[cur_pos]+1;
				fNegate = (src[cur_pos] > cut_point);
				val = (((int)confidence[cur_pos]) ^ -fNegate) + fNegate; 
				if(BCB.val[j_offset]==0)
					NecklaceInsertElement(BCB.prev,BCB.next,j_offset);
				BCB.val[j_offset]+=val;
				if(BCB.val[j_offset]==0)
					NecklaceDeleteElement(BCB.prev,BCB.next,j_offset);

				cur_val_array = joint_histogram.val+i_offset;
				if(cur_val_array[j_offset]==0)
					NecklaceInsertElement(joint_histogram.prev+i_offset,joint_histogram.next+i_offset,j_offset);
				cur_val_array[j_offset]+=confidence[cur_pos];
			}
	}
	else
	{
		j_pos = j_prev - j_shift*rad;
		for(int n=0;n<num_planes;n++)
			for(i_pos=i-rad;i_pos<=i+rad;i_pos++)
			{
				idx = i_pos*w+j_pos;
				i_offset = src[n*w*h+idx]*(nF+1);
				j_offset = guide[n*w*h+idx]+1;
				fNegate = (src[n*w*h+idx] <= cut_point);
				val = (((int)confidence[n*w*h+idx]) ^ -fNegate) + fNegate; 
				if(BCB.val[j_offset]==0)
					NecklaceInsertElement(BCB.prev,BCB.next,j_offset);
				BCB.val[j_offset]+=val;
				if(BCB.val[j_offset]==0)
					NecklaceDeleteElement(BCB.prev,BCB.next,j_offset);

				cur_val_array = joint_histogram.val+i_offset;
				cur_val_array[j_offset]-=confidence[n*w*h+idx];
				if(cur_val_array[j_offset]==0)
					NecklaceDeleteElement(joint_histogram.prev+i_offset,joint_histogram.next+i_offset,j_offset);
			}

		j_pos = j + j_shift*rad;
		for(int n=0;n<num_planes;n++)
			for(i_pos=i-rad;i_pos<=i+rad;i_pos++)
			{
				idx = i_pos*w+j_pos;
				i_offset = src[n*w*h+idx]*(nF+1);
				j_offset = guide[n*w*h+idx]+1;
				fNegate = (src[n*w*h+idx] > cut_point);
				val = (((int)confidence[n*w*h+idx]) ^ -fNegate) + fNegate; 
				if(BCB.val[j_offset]==0)
					NecklaceInsertElement(BCB.prev,BCB.next,j_offset);
				BCB.val[j_offset]+=val;
				if(BCB.val[j_offset]==0)
					NecklaceDeleteElement(BCB.prev,BCB.next,j_offset);

				cur_val_array = joint_histogram.val+i_offset;
				if(cur_val_array[j_offset]==0)
					NecklaceInsertElement(joint_histogram.prev+i_offset,joint_histogram.next+i_offset,j_offset);
				cur_val_array[j_offset]+=confidence[n*w*h+idx];
			}
	}
}


inline void MoveCutPoint(necklace_table joint_histogram,necklace_table BCB,int cut_point,int cut_point_prev,double& balance,int center_label,double* affinity_map,int nF)
{
	int* pos_val_array;
	short* pos_next_array;
	int cur_idx;
	double* affinity_map_cur = affinity_map+center_label*nF;
	int val;
	if(cut_point>cut_point_prev)
	{
		pos_val_array = &joint_histogram.val[cut_point*(nF+1)];
		pos_next_array = &joint_histogram.next[cut_point*(nF+1)];
		cur_idx = pos_next_array[0];

		while(cur_idx!=0)
		{
			if(BCB.val[cur_idx]==0)
				NecklaceInsertElement(BCB.prev,BCB.next,cur_idx);
			val = pos_val_array[cur_idx]<<1;
			BCB.val[cur_idx] += val;
			balance += val*affinity_map_cur[cur_idx-1];
			if(BCB.val[cur_idx]==0)
				NecklaceDeleteElement(BCB.prev,BCB.next,cur_idx);
			cur_idx = pos_next_array[cur_idx];
		}
	}
	else
	{
		pos_val_array = &joint_histogram.val[cut_point_prev*(nF+1)];
		pos_next_array = &joint_histogram.next[cut_point_prev*(nF+1)];
		cur_idx = pos_next_array[0];

		while(cur_idx!=0)
		{
			if(BCB.val[cur_idx]==0)
				NecklaceInsertElement(BCB.prev,BCB.next,cur_idx);
			val = pos_val_array[cur_idx]<<1;
			BCB.val[cur_idx] -= val;
			balance -= val*affinity_map_cur[cur_idx-1];
			if(BCB.val[cur_idx]==0)
				NecklaceDeleteElement(BCB.prev,BCB.next,cur_idx);
			cur_idx = pos_next_array[cur_idx];
		}
	}
}

template<typename T,typename T_guide>
void InitializeTables(necklace_table joint_histogram,necklace_table BCB,T* src,T_guide* guide,int w,int h,int rad,int j,int i,BYTE* confidence,int nF, int nI, int num_planes)
{
	memset(joint_histogram.val,0,nI*(nF+1)*sizeof(int));
	for(int i_pos=0;i_pos<nI;i_pos++)
	{
		joint_histogram.next[i_pos*(nF+1)] = 0;
		joint_histogram.prev[i_pos*(nF+1)] = 0;
	}
	memset(BCB.val,0,(nF+1)*sizeof(int));
	BCB.next[0] = 0;
	BCB.prev[0] = 0;

	for(int i_pos=i-rad;i_pos<=i+rad;i_pos++)
		for(int j_pos=j-rad;j_pos<=j+rad;j_pos++)
		{
			for(int n=0;n<num_planes;n++)
			{
				if(joint_histogram.val[src[n*w*h+i_pos*w+j_pos]*(nF+1)+guide[n*w*h+i_pos*w+j_pos]+1]==0)
					NecklaceInsertElement(joint_histogram.prev+src[n*w*h+i_pos*w+j_pos]*(nF+1),joint_histogram.next+src[n*w*h+i_pos*w+j_pos]*(nF+1),guide[n*w*h+i_pos*w+j_pos]+1);
				joint_histogram.val[src[n*w*h+i_pos*w+j_pos]*(nF+1)+guide[n*w*h+i_pos*w+j_pos]+1]+=confidence[n*w*h+i_pos*w+j_pos];
			}
		}
	int prev_val;
	for(int i_pos=0;i_pos<nI;i_pos++)
		for(int j_pos=1;j_pos<(nF+1);j_pos++)
		{
			prev_val = BCB.val[j_pos];
			BCB.val[j_pos] -= joint_histogram.val[i_pos*(nF+1)+j_pos];
			if(prev_val==0 && BCB.val[j_pos]!=0)
				NecklaceInsertElement(BCB.prev,BCB.next,j_pos);
		}
}


inline double ComputeBalance(int center_label,necklace_table BCB,double* affinity_map,int nF)
{
	int cur_idx = BCB.next[0];
	double balance = 0;
	double* affinity_map_cur = affinity_map+center_label*nF;
	while(cur_idx!=0)
	{
		balance += BCB.val[cur_idx]*affinity_map_cur[cur_idx-1];
		cur_idx = BCB.next[cur_idx];
	}
	return balance;
}

template<typename T,typename T_guide>
void WMFCore(T* dst,T* src,T_guide* guide,double* affinity_map,int w,int h,int rad,BYTE* confidence, int nF, int nI,int num_planes)
{
	bool null_confidence = false;
	if(confidence==NULL)
	{
		null_confidence = true;
		confidence = new BYTE[num_planes*w*h];
		memset(confidence,0,num_planes*w*h);
	}
	for(int i=0;i<num_planes*w*h;i++)
		confidence[i] = min(confidence[i]+1,255);

	int wext = w+2*rad;
	int hext = h+2*rad;
	T* src_ext   = new T[num_planes*wext*hext];
	for(int n=0;n<num_planes;n++)
		AddBorders(src_ext+n*wext*hext,src+n*w*h,w,h,rad);
	T_guide* guide_ext = new T_guide[num_planes*wext*hext];
	for(int n=0;n<num_planes;n++)
		AddBorders(guide_ext+n*wext*hext,guide+n*w*h,w,h,rad);
	BYTE* confidence_ext = new BYTE[num_planes*wext*hext];
	for(int n=0;n<num_planes;n++)
		AddBorders(confidence_ext+n*wext*hext,confidence+n*w*h,w,h,rad);

#pragma omp parallel
{
	necklace_table joint_histogram;
	joint_histogram.Init(nI*(nF+1));
	necklace_table BCB;
	BCB.Init(nF+1);
	int id=omp_get_thread_num(),nthrds=omp_get_num_threads();
	int start_j = rad + (w/nthrds)*id;
	int end_j = rad + (w/nthrds)*(id+1);
	if(id==nthrds-1)
		end_j = rad+w;
	int cut_point = -1;
	double balance;
	int center_label;
	InitializeTables(joint_histogram,BCB,src_ext,guide_ext,wext,hext,rad,start_j,rad,confidence_ext,nF,nI,num_planes);

	for(int j=start_j;j<end_j;j++)
	{
		if((j-start_j)%2==0)
		{
			for(int i=rad;i<h+rad;i++)
			{
				center_label = guide_ext[i*wext+j];
				balance = ComputeBalance(center_label,BCB,affinity_map,nF);

				if(balance>0)
				{
					while(balance>0)
					{
						cut_point--;
						MoveCutPoint(joint_histogram,BCB,cut_point,cut_point+1,balance,center_label,affinity_map,nF);
					}
				}

				if(balance<0)
				{
					while(balance<0)
					{
						cut_point++;
						MoveCutPoint(joint_histogram,BCB,cut_point,cut_point-1,balance,center_label,affinity_map,nF);
					}
				}

				dst[(i-rad)*w+(j-rad)] = cut_point;
				if(i<h+rad-1)
					UpdateTables(joint_histogram,BCB,cut_point,src_ext,guide_ext,wext,hext,rad,j,i+1,j,i,confidence_ext,nF,num_planes);
				else
					UpdateTables(joint_histogram,BCB,cut_point,src_ext,guide_ext,wext,hext,rad,j+1,i,j,i,confidence_ext,nF,num_planes);	
			}
		}
		else
		{
			for(int i=h+rad-1;i>=rad;i--)
			{
				center_label = guide_ext[i*wext+j];
				balance = ComputeBalance(center_label,BCB,affinity_map,nF);

				if(balance>0)
				{
					while(balance>0)
					{
						cut_point--;
						MoveCutPoint(joint_histogram,BCB,cut_point,cut_point+1,balance,center_label,affinity_map,nF);
					}
				}

				if(balance<0)
				{
					while(balance<0)
					{
						cut_point++;
						MoveCutPoint(joint_histogram,BCB,cut_point,cut_point-1,balance,center_label,affinity_map,nF);
					}
				}

				dst[(i-rad)*w+(j-rad)] = cut_point;
				if(i>rad)
					UpdateTables(joint_histogram,BCB,cut_point,src_ext,guide_ext,wext,hext,rad,j,i-1,j,i,confidence_ext,nF,num_planes);
				else
					UpdateTables(joint_histogram,BCB,cut_point,src_ext,guide_ext,wext,hext,rad,j+1,i,j,i,confidence_ext,nF,num_planes);
			}
		}
	}
	joint_histogram.Deinit();
	BCB.Deinit();
}
	if(null_confidence)
		delete[] confidence;

	delete[] src_ext;
	delete[] guide_ext;
	delete[] confidence_ext;
}


void WeightedMedianFilter(void* dst,void* src,BYTE* guide,int w,int h,img_type src_type,int src_num_channels,bool RGB_guide,BYTE* confidence,double sigma,int rad,weight_function f,int num_auxiliary_planes,void** auxiliary_src_planes,BYTE** auxiliary_guide_planes, BYTE** auxiliary_confidence_planes,int nF,int nI)
{
	if(!RGB_guide)
		nF=256;
	if(src_type==u8bit)
		nI=256;

	// prepare guide image:
	WORD* quantized_guide_WORD;
	BYTE* quantized_guide_BYTE;
	double* affinity_map = new double[nF*nF];
	if(nF<=256)
	{
		quantized_guide_BYTE = new BYTE[(num_auxiliary_planes+1)*w*h];
		if(RGB_guide)
			PreprocessRGBGuideImage(quantized_guide_BYTE,affinity_map,f,sigma,guide,w,h,nF,num_auxiliary_planes,auxiliary_guide_planes);
		else
			PreprocessGrayscaleGuideImage(quantized_guide_BYTE,affinity_map,f,sigma,guide,w,h,num_auxiliary_planes,auxiliary_guide_planes);
	}
	else
	{
		quantized_guide_WORD = new WORD[(num_auxiliary_planes+1)*w*h];
		PreprocessRGBGuideImage(quantized_guide_WORD,affinity_map,f,sigma,guide,w,h,nF,num_auxiliary_planes,auxiliary_guide_planes);
	}

	//allocate some stuff:
	WORD* quantized_src;
	WORD* quantized_dst;
	if(src_type==s32bit || src_type==f32bit)
	{
		quantized_src = new WORD[(num_auxiliary_planes+1)*w*h];
		quantized_dst = new WORD[w*h];
	}
	float* float_map;
	int* int_map;
	if(src_type==s32bit)
		int_map = new int[nI];
	else if(src_type==f32bit)
		float_map = new float[nI];

	BYTE* src_with_aux_planes;
	BYTE* confidence_with_aux_planes;
	if(src_type==u8bit && num_auxiliary_planes>0)
		src_with_aux_planes = new BYTE[(num_auxiliary_planes+1)*w*h];
	if(confidence!=NULL && num_auxiliary_planes>0)
		confidence_with_aux_planes = new BYTE[(num_auxiliary_planes+1)*w*h];

	for(int n=0;n<src_num_channels;n++)
	{
		// prepare src image:
		if(src_type==s32bit)
		{
			int** cur_auxiliary_src_planes = new int*[num_auxiliary_planes];
			for(int k=0;k<num_auxiliary_planes;k++)
				cur_auxiliary_src_planes[k] = (int*)auxiliary_src_planes[k] + n*w*h;
			AdaptiveQuantization(quantized_src,int_map,(int*)src+n*w*h,w,h,nI,num_auxiliary_planes,cur_auxiliary_src_planes);
			delete[] cur_auxiliary_src_planes;
		}
		else if(src_type==f32bit)
		{
			float** cur_auxiliary_src_planes = new float*[num_auxiliary_planes];
			for(int k=0;k<num_auxiliary_planes;k++)
				cur_auxiliary_src_planes[k] = (float*)auxiliary_src_planes[k] + n*w*h;
			AdaptiveQuantization(quantized_src,float_map,(float*)src+n*w*h,w,h,nI,num_auxiliary_planes,cur_auxiliary_src_planes);
			delete[] cur_auxiliary_src_planes;
		}

		//perform filtering for current channel:
		if(src_type==u8bit)
		{
			if(num_auxiliary_planes>0)
			{
				memcpy(src_with_aux_planes,(BYTE*)src+n*w*h,w*h);
				for(int i=0;i<num_auxiliary_planes;i++)
					memcpy(src_with_aux_planes+(i+1)*w*h,(BYTE*)auxiliary_src_planes[i]+n*w*h,w*h);
				if(confidence!=NULL)
				{
					memcpy(confidence_with_aux_planes,confidence,w*h);
					for(int i=0;i<num_auxiliary_planes;i++)
						memcpy(confidence_with_aux_planes+(i+1)*w*h,auxiliary_confidence_planes[i],w*h);
					if(nF<=256)
						WMFCore((BYTE*)dst+n*w*h,src_with_aux_planes,quantized_guide_BYTE,affinity_map,w,h,rad,confidence_with_aux_planes,nF,nI,1+num_auxiliary_planes);
					else
						WMFCore((BYTE*)dst+n*w*h,src_with_aux_planes,quantized_guide_WORD,affinity_map,w,h,rad,confidence_with_aux_planes,nF,nI,1+num_auxiliary_planes);
				}
				else
				{
					if(nF<=256)
						WMFCore((BYTE*)dst+n*w*h,src_with_aux_planes,quantized_guide_BYTE,affinity_map,w,h,rad,NULL,nF,nI,1+num_auxiliary_planes);
					else
						WMFCore((BYTE*)dst+n*w*h,src_with_aux_planes,quantized_guide_WORD,affinity_map,w,h,rad,NULL,nF,nI,1+num_auxiliary_planes);
				}

			}
			else
			{
				if(nF<=256)
					WMFCore((BYTE*)dst+n*w*h,(BYTE*)src+n*w*h,quantized_guide_BYTE,affinity_map,w,h,rad,confidence,nF,nI,1);
				else
					WMFCore((BYTE*)dst+n*w*h,(BYTE*)src+n*w*h,quantized_guide_WORD,affinity_map,w,h,rad,confidence,nF,nI,1);
			}		
		}
		else
		{
			if(num_auxiliary_planes>0)
			{
				if(confidence!=NULL)
				{
					memcpy(confidence_with_aux_planes,confidence,w*h);
					for(int i=0;i<num_auxiliary_planes;i++)
						memcpy(confidence_with_aux_planes+(i+1)*w*h,auxiliary_confidence_planes[i],w*h);
					if(nF<=256)
						WMFCore(quantized_dst,quantized_src,quantized_guide_BYTE,affinity_map,w,h,rad,confidence_with_aux_planes,nF,nI,1+num_auxiliary_planes); 
					else
						WMFCore(quantized_dst,quantized_src,quantized_guide_WORD,affinity_map,w,h,rad,confidence_with_aux_planes,nF,nI,1+num_auxiliary_planes); 
				}
				else
				{
					if(nF<=256)
						WMFCore(quantized_dst,quantized_src,quantized_guide_BYTE,affinity_map,w,h,rad,NULL,nF,nI,1+num_auxiliary_planes); 
					else
						WMFCore(quantized_dst,quantized_src,quantized_guide_WORD,affinity_map,w,h,rad,NULL,nF,nI,1+num_auxiliary_planes); 
				}
			}
			else
			{
				if(nF<=256)
					WMFCore(quantized_dst,quantized_src,quantized_guide_BYTE,affinity_map,w,h,rad,confidence,nF,nI,1); 
				else
					WMFCore(quantized_dst,quantized_src,quantized_guide_WORD,affinity_map,w,h,rad,confidence,nF,nI,1); 
			}
			
			if(src_type==s32bit)
			{
				int* cur_dst_plane = (int*)dst+n*w*h;
				for(int i=0;i<w*h;i++)
					cur_dst_plane[i] = int_map[quantized_dst[i]];
				int a = int_map[0];
				int b = int_map[1];
			}
			else if(src_type==f32bit)
			{
				float* cur_dst_plane = (float*)dst+n*w*h;
				for(int i=0;i<w*h;i++)
					cur_dst_plane[i] = float_map[quantized_dst[i]];
			}
		}
	}

	if(nF<=256)
		delete[] quantized_guide_BYTE;
	else
		delete[] quantized_guide_WORD;
	delete[] affinity_map;
	if(src_type==s32bit || src_type==f32bit)
	{
		delete[] quantized_src;
		delete[] quantized_dst;
	}
	if(src_type==s32bit)
		delete[] int_map;
	else if(src_type==f32bit)
		delete[] float_map;

	if(src_type==u8bit && num_auxiliary_planes>0)
		delete[] src_with_aux_planes;
	if(confidence!=NULL && num_auxiliary_planes>0)
		delete[] confidence_with_aux_planes;
}

#include <VideoGMLib/Common/CommonTypes.h>
#include "WMF.h"

/**
**************************************************************************
* \brief Function for 3 channel guide preprocessing, that performs color quantization into <=nF discrete levels
*
* \param dst preallocated buffer where the quantized result will be stored
* \param dst_affinity_map 2D array where all precomputed pairwise distances are stored
* \param f weight function used for affinity map computation
* \param sigma sigma value of the weight function
* \param src_guide input 3 channel guide image (planar format is assumed)
* \param w width of the guide image
* \param h height of the guide image
* \param nF maximum number of different colors in quantized dst image
* \param num_auxiliary_planes number of additional guide planes
* \param auxiliary_guide_planes (optional) additional guide planes (this planes will be processed with the main guide and the result will be written in dst after the main quantized guide)
*/
template<typename T>
void PreprocessRGBGuideImage(T* dst,double* dst_affinity_map,weight_function f, double sigma,BYTE* src_guide,int w,int h,int nF,int num_auxiliary_planes=0,BYTE** auxiliary_guide_planes=NULL);

/**
**************************************************************************
* \brief Function for 1 channel guide preprocessing, no quantization is needed
*
* \param dst preallocated buffer where the result will be stored
* \param dst_affinity_map 2D array where all precomputed pairwise distances are stored
* \param f weight function used for affinity map computation
* \param sigma sigma value of the weight function
* \param src_guide input 1 channel guide image 
* \param w width of the guide image
* \param h height of the guide image
* \param num_auxiliary_planes number of additional guide planes
* \param auxiliary_guide_planes (optional) additional guide planes (this planes will be processed with the main guide and the result will be written in dst after the main guide)
*/
void PreprocessGrayscaleGuideImage(BYTE* dst,double* dst_affinity_map,weight_function f,double sigma,BYTE* src_guide,int w,int h,int num_auxiliary_planes=0,BYTE** auxiliary_guide_planes=NULL);

/**
**************************************************************************
* \brief Function performing adaptive quantization (minimizing quantizing error) of 1 channel image into <=nI discrete levels
*
* \param dst preallocated buffer where the quantized result will be stored
* \param dst_mapping mapping between quantized values of dst and source image values
* \param src source image for quantizing (1 channel)
* \param w width of the source image
* \param h height of the source image
* \param nI maximum number of different levels in quantized dst image
* \param num_auxiliary_planes number of additional src planes
* \param auxiliary_guide_planes (optional) additional src planes (this planes will be processed with the main src and the result will be written in dst after the main src)
*/
template<typename T>
void AdaptiveQuantization(WORD* dst,T* dst_mapping,T* src,int w,int h,int nI,int num_auxiliary_planes=0,T** auxiliary_src_planes=NULL);


/**
**************************************************************************
* \brief Helper function for adding repeated borders to images
*
* \param dst preallocated buffer where the result will be stored
* \param src source image that lacks desired borders
* \param w width of the source image
* \param h height of the source image
* \param border_sz size of the border, that will be added to all sides of the source image
*/
template<typename T>
void AddBorders(T* dst,T* src,int w,int h,int border_sz);




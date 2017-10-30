#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef NULL
#define NULL 0
#endif

enum weight_function
{
	EXP, 
	INV1,
	INV2
};

enum img_type
{
	u8bit,  //BYTE
	s32bit, //int
	f32bit  //float
};


/**
**************************************************************************
* \brief Main method for performing fast weighted median filtering. Supports arbitrary user-provided confidence maps and can use additional image planes to improve filtering (useful for temporal filtering)
*
* \param dst preallocated buffer where the result will be stored
* \param src source image for filtering (BYTE*, int*, float* are supported), any number of channels (stored in planar format)  
* \param guide image to guide the filtering process, 1 or 3 channels (planar format)
* \param w image width
* \param h image height
* \param src_type type of src and dst images
* \param src_num_channels number of channels in the source image for filtering
* \param RGB_guide flag indicating whether guide is a 3 channel RGB image (otherwise 1 channel image is assumed)
* \param confidence (optional, NULL can be passed instead) some confidence map for additional filtering process guidance
* \param sigma parameter of weight function 
* \param rad filtering radius
* \param f type of weight function which is used for filtering
* \param num_auxiliary_planes number of additional planes that can be used to improve filtering (size of auxiliary_src_planes)
* \param auxiliary_src_planes (optional, NULL can be passed instead) array of additional source images, that will be used in filtering (useful, for instance, for temporal filtering)
* \param auxiliary_guide_planes (optional, NULL can be passed instead) array of additional guide images, corresponding to auxiliary_src_planes
* \param auxiliary_confidence_planes (optional, NULL can be passed instead) array of additional confidence maps, corresponding to auxiliary_src_planes
* \param nF guide image image quantizing parameter, maximum number of different values (<2^16), applicable only for 3 channel guide images
* \param nI source image image quantizing parameter, maximum number of different values (<2^16), applicable only when src_type is s32bit or f32bit
*/
void WeightedMedianFilter(void* dst,void* src,BYTE* guide,int w,int h,img_type src_type,int src_num_channels=1,bool RGB_guide = true,BYTE* confidence=NULL,double sigma = 1.0,int rad=25,weight_function f=INV1,int num_auxiliary_planes=0,void** auxiliary_src_planes=NULL,BYTE** auxiliary_guide_planes=NULL, BYTE** auxiliary_confidence_planes=NULL,int nF=256,int nI=256);

     

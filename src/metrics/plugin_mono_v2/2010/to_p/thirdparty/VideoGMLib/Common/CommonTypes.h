/*
********************************************************************
(c) MSU Video Group 2003-2005, http://compression.ru/video/
This source code is property of MSU Graphics and Media Lab

This code can be distributed and used only with WRITTEN PERMISSION.
********************************************************************
*/

/**
*  \file ./Common/CommonTypes.h 
*  \brief Declaration of some common used entities
*
*  This file contains description of some common used entities
*/

#pragma once

#include <malloc.h> 
#include <memory.h>
 
/**
**************************************************************************
* \brief Square macro
*
*/
#define sqr(x) ((x)*(x))

/**
**************************************************************************
* \brief Fast implementation of abs function for integers
*
*/
#ifdef ABS
#undef ABS
#endif

#define ABS(x) ((((x)>>(sizeof(int)*8-1)) & ((x)^(-(x)))) ^ (x))

#ifndef uint
/*! \var uint
*   \brief Defines unsigned integer type
*
*/	
typedef unsigned int  uint;
#endif

#ifndef ushort
/*! \var ushort
*   \brief Defines unsigned short type
*
*/	
typedef unsigned short  ushort;
#endif

#ifndef BYTE
/*! \var BYTE
*   \brief Defines unsigned char type as BYTE
*
*/	
typedef	unsigned char BYTE;
#endif

#ifndef sBYTE
/*! \var sBYTE
*   \brief Defines signed char type as sBYTE
*
*/	
typedef	signed char sBYTE;
#endif

#ifndef BOOL
/*! \var BOOL
*   \brief Defines boolean type for C
*
*/	
typedef int BOOL;
#endif

#ifndef FALSE
/*! \var FALSE
*   \brief Defines boolean value "false" for C
*
*/	
#define FALSE   0
#endif

#ifndef TRUE
/*! \var TRUE
*   \brief Defines boolean value "true" for C
*
*/	
#define TRUE    1
#endif

#ifndef Pixel32
/*! \var ushort
*   \brief Defines unsigned long type as Pixel32
*
*/	
typedef unsigned long	Pixel32;
#endif

#ifndef gmcColorSpace
/*! \var gmcColorSpace
*   \brief Defines type of color space
*
*  Defines enumeration type for indicating of color space
*/	
    typedef enum { Y, YV12, UYVY, IYUV, YUV, RGB24, YCbCr, UNKNOWN} gmcColorSpace;
#endif

/*! \def Align16(a)
*   \brief Defines ceiling to nearest multiple of 16
*
*  Defines a macro that ceiling an integer number to the nearest
*  multiple of 16
*/	
#ifndef Align16
    #define Align16(a) (((a)+15)&~15)
#endif

#ifndef MAX
/*! \def MAX(a,b)
*   \brief Calculates maximum of two numbers
*
* \param a       [IN] -  1-st number
* \param b       [IN] -  2-nd number
* 
*/
#define  MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef NULL
/*! \def NULL
*   \brief Defines NULL constant as 0
*
*/
#define  NULL 0
#endif

#ifndef MIN
/*! \def MIN(a,b)
*   \brief Calculates minimum of two numbers
*
* \param a       [IN] -  1-st number
* \param b       [IN] -  2-nd number
* 
*/
#define  MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef CLIP
/*! \def CLIP(a,_min_,_max_)
*   \brief Clips a number from below and from above
*
* \param a			[IN] -  input number
* \param _min_      [IN] -  low bound
* \param _max_      [IN] -  high bound
* 
*/
#define  CLIP(a,_min_,_max_)  (((a) > (_max_)) ? (_max_) : MAX(a,_min_))
#endif

/*! \def INSIDE(x, a, b)
*   \brief Defines whether x belongs to the segment [a, b]
*
*/
#define INSIDE(x, a, b) (((x) >= (a)) && ((x) <= (b)))

/*! \def INNER_POINT(y,x,h,w)
*   \brief Checks if the specified point is located in the inner area of the frame
*
*  Defines the condition of that specified coordinates define the point located
*  in the frame which has specified height and width.
*
* \param y        [IN] -  y coordinate of pixel
* \param x        [IN] -  x coordinate of pixel
* \param h        [IN] -  height of the frame
* \param w        [IN] -  width of the frame
*
*/
#define INNER_POINT(y, x, h, w) (INSIDE(y, 0, (h)-1) && INSIDE(x, 0, (w)-1))

/*! \def INNER_POINT_1D(y,x,h,w)
*   \brief Checks if the specified point is located in the inner area of the interval
*
*  Defines the condition of that specified coordinates define the point located
*  in the interval which has specified width.
*
* \param x        [IN] -  x coordinate of the point
* \param w        [IN] -  width of the interval
*
*/
#define INNER_POINT_1D(x, w) (INSIDE((x), 0, (w)-1))


#ifndef INIT_MEMORY_C
/*! \def INIT_MEMORY_C(_mem, _val, _size, _type)
*   \brief Allocates memory
*
* \param _mem		[OUT] -  pointer to memory to be allocated
* \param _val		[IN] -  memory initializer
* \param _size      [IN] -  number of units in memory
* \param _type      [IN] -  unit type
* 
*/
#define INIT_MEMORY_C(_mem, _val, _size, _type) {_mem = (_type *)malloc((_size) * sizeof(_type)); memset(_mem, _val, (_size) * sizeof(_type));}
#endif

#ifndef DEINIT_MEMORY_C
/*! \def DEINIT_MEMORY_C(_val)
*   \brief Frees memory referred by the specified pointer
*
* \param _val		[IN/OUT] -  pointer to memory
* 
*/
#define DEINIT_MEMORY_C(_val) {if (_val != NULL) free(_val); _val = NULL;}
#endif

#ifndef INIT_ALIGNED16_MEMORY_C
/*! \def INIT_ALIGNED16_MEMORY_C(_mem, _val, _size, _type)
*   \brief Allocates memory at 16 byte boundary
*
* \param _mem		[OUT] -  pointer to memory to be allocated 
* \param _val		[IN] -  memory initializer
* \param _size      [IN] -  number of units in memory
* \param _type      [IN] -  unit type
* 
*/
#define INIT_ALIGNED16_MEMORY_C(_mem, _val, _size, _type)	{						\
    _type* temp = (_type*)malloc((16 + (_size)) * sizeof(_type));							\
    _mem = (_type*)( (((char*)temp - (char*)NULL) + 16)&~15 );						\
    *((void**)_mem - 1) = temp;														\
    memset(_mem, _val, (_size) * sizeof(_type));									\
    }
#endif

#ifndef DEINIT_ALIGNED16_MEMORY_C
/*! \def DEINIT_ALIGNED16_MEMORY_C(_val)
*   \brief Frees memory referred by the specified pointer
*
* \param _val		[IN/OUT] -  pointer to memory
* 
*/
#define DEINIT_ALIGNED16_MEMORY_C(_val)	{											\
    if (_val != NULL)  {															\
    free( *((void**)_val - 1) );									    			\
    _val = NULL;														    		\
    }																				\
    } 
#endif


#ifndef CLIPCROP
#define CLIPCROP(x, _min_, _max_, _min_v_, _max_v_, _def_v_) ((x) < (_min_) ? (_min_v_) : (x) > (_max_) ? (_max_v_) : (_def_v_))
#endif


/*

#ifndef INIT_MEMORY
/ *! \def INIT_MEMORY(_mem, _val, _size, _type)
*   \brief Allocates memory
*
* \param _mem		[OUT] -  pointer to memory to be allocated
* \param _val		[IN] -  memory initializer
* \param _size      [IN] -  number of units in memory
* \param _type      [IN] -  unit type
* 
* /
#define INIT_MEMORY(_mem, _val, _size, _type) {_mem = new _type [_size]; memset(_mem, _val, (_size) * sizeof(_type));}
#endif

#ifndef DEL
	#define DEL(_val) {if (_val != NULL) delete [] _val;}
#endif


*/

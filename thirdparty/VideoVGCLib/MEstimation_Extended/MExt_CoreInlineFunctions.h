#ifndef _MEXT_COREINLINEFUNCS_H
#define _MEXT_COREINLINEFUNCS_H

#include <VideoGMLib/Common/CommonTypes.h>

/**
**************************************************************************
* \brief Sets buffer block to specified value
* 
* \param buf				[IN/OUT]  -  pointer to the source buffer
* \param x,y				[IN] -  coordinates of left corner of the block
* \param pitch				[IN] -  number of bytes between adjacent lines
* \param value				[IN] -  value to fill the block in buf frame
* \param blocksize			[IN] -  width of block to fill with value
* 
* \return None
*/
static __inline void Set_Block_Value(BYTE *buf, int x, int y, int pitch, int value, int blocksize)
{
	int i;
	buf += y * pitch + x;
	for(i = 0; i < blocksize; i++ )
	{
		memset(buf, value, blocksize);
		buf += pitch;
	}
}

/**
**************************************************************************
* \brief Sets buffer block to specified value with bound check
* 
* \param buf				[IN/OUT]  -  pointer to the source buffer
* \param x,y				[IN] -  coordinates of left corner of the block
* \param pitch				[IN] -  number of bytes between adjacent lines
* \param value				[IN] -  value to fill the block in buf frame
* \param blocksize			[IN] -  width of block to fill with value
* 
* \return None
*/
static __inline void Set_Block_Value_Secure(BYTE *buf, int x, int y, int width, int height, int value, int blocksize)
{
	int i;
	int horizontal_block_size = MIN(blocksize, width - x);

	buf += y * width + x;
	for(i = 0; i < blocksize; i++ )
	{
		memset(buf, value, horizontal_block_size);
		buf += width;

		// height check
		++y;
		if(y >= height)
			break;
	}
}

/**
**************************************************************************
* \brief Sets buffer block (buffer type int) to specified value
* 
* \param buf				[IN/OUT]  -  pointer to the source buffer
* \param x,y				[IN] -  coordinates of left corner of the block
* \param pitch				[IN] -  number of bytes between adjacent lines
* \param value				[IN] -  value to fill the block in buf frame
* \param blocksize			[IN] -  width of block to fill with value
* 
* \return None
*/
static __inline void Set_Block_Value_Int(int *buf, int x, int y, int pitch, int value, int blocksize)
{
	int i, j;
	buf += y * pitch + x;
	for(i = 0; i < blocksize; i++ )
	{
		for(j = 0; j < blocksize; j++)
		{
			buf[j] = value;
		}
		buf += pitch;
	}
}

/**
**************************************************************************
* \brief Sets buffer block (buffer type int) to specified value with inbound check
* 
* \param buf				[IN/OUT]  -  pointer to the source buffer
* \param x,y				[IN] -  coordinates of left corner of the block
* \param pitch				[IN] -  number of bytes between adjacent lines
* \param value				[IN] -  value to fill the block in buf frame
* \param blocksize			[IN] -  width of block to fill with value
* 
* \return None
*/
static __inline void Set_Block_Value_Int_Secure(int *buf, int x, int y, int width, int height, int value, int blocksize)
{
	int i, j;
	int horizontal_block_size = MIN(blocksize, width - x);

	buf += y * width + x;
	for(i = 0; i < blocksize; i++ )
	{
		for(j = 0; j < horizontal_block_size; j++)
		{
			buf[j] = value;
		}
		buf += width;

		// height check
		++y;
		if(y >= height)
			break;
	}
}

static __inline void Copy_Block(BYTE *plane, int x0, int y0, BYTE *block, int size, int stride) //block is 64 bytes
{
	int y, s, d;

	for(y = 0; y < size; y++) 
	{
		s = (y0 + y) * stride + x0;
		d = y * size;
		memcpy(&plane[s], &block[d], size);
	}
}

static __inline int Get_Linear_Function_Value(int value, int low_thresh, int high_thresh)
{
	int result;

	if(high_thresh == low_thresh)
		high_thresh = low_thresh + 1;

	result = (value - low_thresh) * 255 / (high_thresh - low_thresh) ;

	return CLIP( result, 0, 255 );
}

static __inline int * GenerateCircleKernel ( int radius )
{
	int index_y, index_x;
	int pitch = 2*radius + 1;

	int *kernel ;

	INIT_MEMORY_C ( kernel, 0, pitch * pitch, int );

	// initializing
	for ( index_y = -radius; index_y <= radius; ++ index_y )
	{
		for ( index_x = -radius; index_x <= radius; ++index_x )
		{
			int index = (index_y+radius) * pitch + index_x + radius;
			if ( sqr(index_x) + sqr(index_y) <= sqr(radius) )
			{
				kernel[ index ] = 1;
			}
			else
			{
				kernel[ index ] = 0;
			}
		}
	}

	return kernel;
}

static __inline void DestroyCircleKernel ( int ** kernel )
{
	DEINIT_MEMORY_C( *kernel );
}

static __inline BYTE * GenerateCircleKernel_BYTE ( BYTE radius )
{
	short index_y, index_x;
	BYTE pitch = 2*radius + 1;

	BYTE *kernel ;

	INIT_MEMORY_C ( kernel, 0, pitch * pitch, BYTE );

	// initializing
	for ( index_y = -radius; index_y <= radius; ++ index_y )
	{
		for ( index_x = -radius; index_x <= radius; ++index_x )
		{
			short index = (index_y+radius) * pitch + index_x + radius;
			if ( sqr(index_x) + sqr(index_y) <= sqr(radius) )
			{
				kernel[ index ] = 1;
			}
			else
			{
				kernel[ index ] = 0;
			}
		}
	}

	return kernel;
}

static __inline void DestroyCircleKernel_BYTE ( BYTE ** kernel )
{
	DEINIT_MEMORY_C( *kernel );
}

static __inline void MapIntBufferToByteBuffer ( int * source, BYTE * dst, int width, int height)
{
	int index;
	for(index = 0; index < width*height; ++ index)
	{
		dst[index] = CLIP (source[index], 0, 255);
	}
}

#endif

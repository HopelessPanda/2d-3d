#include <VideoGMLib/Common/CommonTypes.h>
#include "Calculation_Buffers.h"

void Buffer_Storage_Allocate(Buffer_Storage * storage, int width, int height)
{
	long frame_size = width*height;
	int index = 0;

	storage->width = width;
	storage->height = height;
	storage->frame_size = frame_size;

	for(index = 0; index < BYTE_buffer_type_capacity; ++index)
	{
		INIT_MEMORY_C(storage->BYTE_buffers[index], 0, frame_size, BYTE);
	}
	for(index = 0; index < BYTE3_buffer_type_capacity; ++index)
	{
		INIT_MEMORY_C(storage->BYTE_3plane_buffers[index], 0, frame_size*3, BYTE);
	}
	for (index = 0; index < cMV_buffer_type_capacity; ++index)
	{
		INIT_MEMORY_C(storage->cMV_buffers[index], 0, frame_size, cMV);
	}
}

void Buffer_Storage_Deallocate(Buffer_Storage * storage)
{
	int index = 0;

	for(index = 0; index < BYTE_buffer_type_capacity; ++index)
	{
		DEINIT_MEMORY_C(storage->BYTE_buffers[index]);
	}
	for(index = 0; index < BYTE3_buffer_type_capacity; ++index)
	{
		DEINIT_MEMORY_C(storage->BYTE_3plane_buffers[index]);
	}
	for (index = 0; index < cMV_buffer_type_capacity; ++index)
	{
		DEINIT_MEMORY_C(storage->cMV_buffers[index]);
	}
}

BYTE *Buffer_Storage_GetBuffer_BYTE(Buffer_Storage * storage, BYTE_buffer_type type)
{
	if(type < BYTE_buffer_type_capacity)
	{
		return storage->BYTE_buffers[type];
	}
	else if ( (type >= BYTE3_buffer_type_shift) && (type < BYTE3_buffer_type_shift+BYTE3_buffer_type_capacity) )
	{
		return storage->BYTE_3plane_buffers[type - BYTE3_buffer_type_shift];
	}
	else
		return NULL;
}

cMV *Buffer_Storage_GetBuffer_cMV(Buffer_Storage * storage, cMV_buffer_type type)
{
	if(type < cMV_buffer_type_capacity + cMV_buffer_type_shift  &&  type >= cMV_buffer_type_shift)
	{
		return storage->cMV_buffers[type - cMV_buffer_type_shift];
	}
	else
		return NULL;
}

void Buffer_Storage_InternalCopy(Buffer_Storage * storage, BYTE_buffer_type src_type, BYTE_buffer_type dst_type)
{
	if( src_type < cMV_buffer_type_capacity + cMV_buffer_type_shift  &&  src_type >= cMV_buffer_type_shift &&
		dst_type < cMV_buffer_type_capacity + cMV_buffer_type_shift  &&  dst_type >= cMV_buffer_type_shift )
	{
		memcpy( storage->cMV_buffers[dst_type - cMV_buffer_type_shift], storage->cMV_buffers[src_type-cMV_buffer_type_shift], storage->frame_size * sizeof(cMV) );
	}
	else if( src_type >= BYTE_buffer_type_capacity  ||  dst_type >= BYTE_buffer_type_capacity )
		return;
	else
		memcpy( storage->BYTE_buffers[dst_type], storage->BYTE_buffers[src_type], storage->frame_size );
}

void Buffer_Storage_ExportBYTE(Buffer_Storage * storage, BYTE_buffer_type src_type, BYTE *dst)
{
	if( src_type >= BYTE_buffer_type_capacity )
		return;
	else
		memcpy( dst, storage->BYTE_buffers[src_type], storage->frame_size );
}
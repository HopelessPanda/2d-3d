/*
********************************************************************
(c) MSU Video Group 2004-2005, http://compression.ru/video/
File is from "Graphics&Media Lab Common Source Library".

Redistributions of source code must retain the above copyright
notice and license file (see license.txt).

This source code is provided under terms of GMLib license.
********************************************************************
*/

/**
*  \file ImageMemory.h
*  \brief gmImageMemory class for smart memory allocation and external pointer storing
*
*  This file contains implementation of the class that allocates memory and stores 
*  external pointer. Class tracks size of allocated memory and reallocation happens 
*  only if requested memory size greater than already allocated
*
*  $Id: ImageMemory.h,v 1.14 2008/10/22 14:22:33 05s_put Exp $
*  $Log: ImageMemory.h,v $
*  Revision 1.14  2008/10/22 14:22:33  05s_put
*  - made 10 bit 4:2:0 work
*
*  Revision 1.13  2007/08/21 10:42:31  05s_gri
*  Changes added by MM for DemoGUI
*
*  Revision 1.12  2006/05/11 14:11:18  05s_put
*  RGB, YV16 and lsYUVpl added
*
*  Revision 1.3  2006/04/19 20:31:00  05s_put
*  Last version of gmVideoImage was taken
*
*  Revision 1.11  2006/02/07 15:16:19  05m_mah
*  "delete variable" replaced by "delete [] variable"
*
*  Revision 1.10  2005/07/04 08:18:32  05s_put
*  Licenses fixed
*
*  Revision 1.9  2005/07/03 14:53:40  05m_mah
*  doxygen warnings fixed
*
*  Revision 1.8  2005/07/03 13:16:33  05m_mah
*  Permissions header added
*
*  Revision 1.7  2005/04/15 17:14:33  05m_mah
*  const declaration added
*
*  Revision 1.6  2005/04/07 13:40:03  05m_mah
*  Doxygen documentation added
*
*/

#ifndef _IMAGEMEMORY_H_
#define _IMAGEMEMORY_H_

#include <memory.h>

#ifdef USE_AE_MEMORY_MANAGER
//#include "AEMemoryManager.h"

extern "C" void*	Allocate(const size_t);
extern "C" void		Release(void*);

#ifdef malloc
#undef malloc
#endif

#ifdef free
#undef free
#endif

#define malloc(_size)	Allocate(_size)
#define free(_pointer)	Release(_pointer)
#endif


/*!\brief Class for smart memory allocation and external pointer storing
*
*  This class used for smart memory allocation and external pointer storing
*/
class gmImageMemory
{
public:

	gmImageMemory(const gmImageMemory& obj) {
		mf_ClearInternals();
		operator=(obj);
	}
	gmImageMemory& operator=(const gmImageMemory& obj) {
		if (&obj != this) {
			ResetContent();
			m_Allocated = obj.m_Allocated;
			p_Memory = obj.p_Memory;
			if (m_Allocated != 0) {
				m_Memory = new BYTE[m_Allocated];
				memcpy(m_Memory, obj.m_Memory, m_Allocated);
			}
		}
		return *this;
	}

	/**
	**************************************************************************
	* \brief Default constructor 
	*
	* \return None
	*/
	gmImageMemory()
	{
		mf_ClearInternals();
	}

	/**
	**************************************************************************
	* \brief Default destructor 
	*
	* \return None
	*/
	~gmImageMemory()
	{
		ResetContent();
	}

/**
**************************************************************************
* \brief Allocates memory if currently allocated memory less than need
*
* \param bytesNomber     [IN] - number of bytes to allocate
* 
* \return None
*/
	void ImAllocate(long bytesNumber)
	{
		if (bytesNumber > m_Allocated)
		{
			ResetContent();
#ifndef USE_AE_MEMORY_MANAGER
			m_Memory = new BYTE [bytesNumber];
#else
			//alloc_func((size_t)bytesNomber, (void**)&m_Memory);
			m_Memory = (BYTE*)malloc((size_t)bytesNumber);
#endif // USE_AE_MEMORY_MANAGER
			memset(m_Memory, 0, bytesNumber);
			m_Allocated = bytesNumber;
		}
		p_Memory = m_Memory;
	}

    int Allocated() const { return m_Allocated; }

/**
**************************************************************************
* \brief Returns pointer for Storage
*
* \return pointer for Storage
*/
	BYTE* GetMem() { return p_Memory; }

    BYTE* GetMemInternal() { return m_Memory; }

	const BYTE* GetMem() const { return p_Memory; }

/**
**************************************************************************
* \brief Stores pointer instead of allocation
*
* \param pointer     [IN] - pointer to store
* 
* \return None
*/
	inline void StorePointer(BYTE* pointer) 
	{
		p_Memory = pointer;
	}

/**
**************************************************************************
* \brief Deleting memory if allocated, clears pointer
*
* \return None
*/
	void ResetContent()
	{
		if (m_Memory != 0)
		{
#ifndef USE_AE_MEMORY_MANAGER
			delete[] m_Memory;
#else
			//release_func(m_Memory);
			free(m_Memory);
#endif
			m_Memory = 0;
		}

		mf_ClearInternals();	
	}

private:

/**
**************************************************************************
* \brief Clears internal variables
*
* \return None
*/
	void mf_ClearInternals()
	{
		m_Memory = 0;
		m_Allocated = 0;
		p_Memory = 0;
	}

	long  m_Allocated;		//!< Memory curently allocated
	BYTE* m_Memory;			//!< Memory storage
	BYTE* p_Memory;			//!< Pointer storage
};
#endif //#ifndef _IMAGEMEMORY_H_
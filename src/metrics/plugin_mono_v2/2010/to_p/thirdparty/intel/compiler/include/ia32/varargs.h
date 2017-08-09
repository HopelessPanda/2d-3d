/**
*** Copyright (C) 1985-2011 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
**/

/*
 * varargs.h
 *
 * Replacement header for standard varargs.h
 *
 * This replacement is necessary to support the 16 byte
 * aligned parameters.
 *
 * 18 Sept 1997 [kbs]
 */

#ifndef _INC_VARARGS
#define _INC_VARARGS


#ifdef __INTEL_COMPILER
#define __builtin_alignof(t) __ALIGNOF__(t)
#else
#define __builtin_alignof(t) (4)
#endif

#ifdef __INTEL_CHKP_ENABLED
#define KILL_BOUNDS(p) ((va_list)__chkp_kill_bounds(p))
#else
#define KILL_BOUNDS(p) p
#endif

/*
 * The following macro computes the size actually occupied in
 * the parameter block by a parameter of the same type as t.
 * All parameter sizes are really multiples of int size in a
 * parameter block.
 */
#define _PARMSIZE(t) \
    ((sizeof(t) + (sizeof(int)-1)) & ~(sizeof(int) -1))

/*
 * The following macro computes the alignment in the parameter block
 * of a parameter of type t.  Any type that doesn't require 16 byte
 * alignment, only gets 4 byte alignment.
 */
#define _PARMALIGN(t) \
    (__builtin_alignof(t) < 32 ? __builtin_alignof(t) < 16 ? 4 : 16 : 32)

/*
 * The following macro computes the base address of a parameter of type
 * t given the address of the first byte after the previous parameter.
 */
#define _PARMADDR(fb_addr,t) \
    ((char *)((((int)fb_addr) + (_PARMALIGN(t)-1)) & ~(_PARMALIGN(t)-1)))

typedef char * va_list;

#define va_dcl va_list va_alist;

#define va_start(ap) \
    ((ap)  = KILL_BOUNDS((va_list)&va_alist))

#define va_arg(ap,t) \
    (*((t *)((ap = (va_list)(_PARMADDR(ap,t) + _PARMSIZE(t))) - \
                   _PARMSIZE(t))))

#define va_end(ap) \
    ( ap = 0 )

#endif /* _INC_VARARGS */

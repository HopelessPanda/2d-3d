/**
*** Copyright (C) 1985-2007 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
**/

/*
 * xmm_func.h
 *
 * Header for Streaming SIMD Extensions intrinsics (the functionality intrinsics,
 * i.e. x87 "compatability" mode)
 *
 * Should be included only by xmmintrin.h
 *
 * 07 Nov 96 [mpg]
 */

#ifndef _INCLUDED_MM2
#error Header should only be included from xmmintrin.h
#endif

#ifndef _INCLUDED_MM2_FUNC
#define _INCLUDED_MM2_FUNC

#pragma message ("Functional intrinsics are obsolete. Do not define _MM_FUNCTIONALITY")

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * this is the bit and element numbering convention we will use:
 *
 *  127..96  95..64  63..32  31..0
 *        z       y       x      w
 *      [3]     [2]     [1]    [0]
 *
 * (when referring to vectors, we will always put the "fourth" word in
 * the leftmost position)
 * MACROs are made available to reduce confusion :
 * _MM_FP3(x)  _MM_FP2(x)  _MM_FP1(x)  _MM_FP0(x) : x is an m128 type.
 * the macros will return each FP in the m128 type. MM_FP3
 * returns the highest FP, and MM_FP0 the lowest.
 */

#ifndef _MM_NO_INLINE
#define _MM_INLINE_COMMAND             __inline
#else
#define _MM_INLINE_COMMAND
#endif


/* __m128 type declaration */
typedef struct {
  float w,x,y,z;
} ___m128;

#define __m128 ___m128

/* _mm_m64 type declaration: overlay for the real __m64 */
typedef struct {
  union {
	signed char b[8];
	signed short w[4];
	signed int dw[2];
	unsigned char ub[8];
	unsigned short uw[4];
	unsigned int udw[2];
	__m64 m;
  } m64;
} _mm_m64;

typedef struct {
  union {
    	signed int dw[4];
    	__m128 m;
  } m128;
} _mm_m128;


/* some common float and int constants */
#define _MM_ZERO                    (0.0f)
#define _MM_ONE                     (1.0f)
#define _MM_BITONE                  (0xffffffff)
#define _MM_BITZERO                 (0x00000000)

/* MIN and MAX macros for FPs and INTs*/
#define _MM_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define _MM_MAX(a,b)            (((a) > (b)) ? (a) : (b))

/*********************************************************/
/* MACROs to use for accessing struct FPs in m128 types. */
/* MM_FP0 is the lowest FP and MM_FP3 the highest        */
/*********************************************************/
#define _MM_FP0(ffps)                (ffps.w)
#define _MM_FP1(ffps)                (ffps.x)
#define _MM_FP2(ffps)                (ffps.y)
#define _MM_FP3(ffps)                (ffps.z)
/***********************************************************/
/* MACROs to use for accessing m128 struct FPs as ints.  */
/* MM_INT0 is the lowest FP and MM_INT3 the highest        */
/***********************************************************/
#define _MM_INT0(ffps)                (*((int *)(&(ffps.w))))
#define _MM_INT1(ffps)                (*((int *)(&(ffps.x))))
#define _MM_INT2(ffps)                (*((int *)(&(ffps.y))))
#define _MM_INT3(ffps)                (*((int *)(&(ffps.z))))


#ifndef _MM_NO_ABORT
#define _mminternal_abort(str) \
  { fprintf(stderr, "*** Functionality intrinsics error: %s ***\n", str); \
    exit(1); }
#else
#define _mminternal_abort(str) \
  { fprintf(stderr, "*** Functionality intrinsics warning: %s ***\n", str); }
#endif

/*
 * check to make sure addresses are 16-byte aligned
 */
#ifdef NO_ALIGN_CHECK
#ifndef _MM_NO_ALIGN_CHECK
#define _MM_NO_ALIGN_CHECK
#endif
#endif

#ifndef _MM_NO_ALIGN_CHECK
#define _mminternal_assert_16B(addr) \
  if ((unsigned)addr % 16 != 0) { \
    _mminternal_abort("address must be 16-byte aligned"); \
  }
#else
#define _mminternal_assert_16B(addr) ;
#endif


/*
 * return same precision results as via emulator
 */
#ifndef _MM_NO_ACCURACY

_MM_INLINE_COMMAND static float __cdecl _mminternal_approx(float x)
{
  unsigned int *p = (unsigned int*)&x;
  *p = *p & 0xfffff800;
  return x;
}

_MM_INLINE_COMMAND static double __cdecl _mminternal_sqrt(double src)
{
  double ret;
	_asm	{
		fld QWORD PTR src
		fsqrt
		fstp ret;
	}
  return ret;
}

#else
#define _mminternal_approx(x) (x)
#define _mminternal_sqrt(x) ((float)sqrt((double)x))

#endif

/* conversion routines: for cvt ops, we do the MXCSR default, which
   is round to nearest */
_MM_INLINE_COMMAND static int __cdecl _mm_round_nearest(float f) {
  int ret;
	_asm	{
		fld f
		fistp ret
	}
  return ret;
}

/* For cvtt ops, we do round with truncation, so we need to mess with
   the MXCSR */
_MM_INLINE_COMMAND static int __cdecl _mm_round_trunc(float f) {
  int ret;
  int saved_cw;
  int new_cw;
	_asm	{
                push      eax
                fld       f
                fstcw     saved_cw
                mov       eax, saved_cw
                or        eax, 3072
                mov       new_cw, eax
                fldcw     new_cw
                fistp     ret
                fldcw     saved_cw
                pop       eax
	}
  return ret;
}

/****************************************************/
/*       FUNC INTRINSIC FUNCTIONS START HERE        */
/****************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************/
/*  NAME : mm_add_ss                                */
/*  DESCRIPTION : scalar single FP add              */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : (a,a,a,a+b)                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_add_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_FP0(ret) = _MM_FP0(a) + _MM_FP0(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_add_ps                                */
/*  DESCRIPTION : packed single FP add              */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : a + b                           */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_add_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP0(ret) = _MM_FP0(a) + _MM_FP0(b);
  _MM_FP1(ret) = _MM_FP1(a) + _MM_FP1(b);
  _MM_FP2(ret) = _MM_FP2(a) + _MM_FP2(b);
  _MM_FP3(ret) = _MM_FP3(a) + _MM_FP3(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_sub_ss                                */
/*  DESCRIPTION : scalar single FP subtract         */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : (a,a,a,a-b)                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_sub_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_FP0(ret) = _MM_FP0(a) - _MM_FP0(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_sub_ps                                */
/*  DESCRIPTION : packed single FP subtract         */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : a - b                           */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_sub_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP0(ret) = _MM_FP0(a) - _MM_FP0(b);
  _MM_FP1(ret) = _MM_FP1(a) - _MM_FP1(b);
  _MM_FP2(ret) = _MM_FP2(a) - _MM_FP2(b);
  _MM_FP3(ret) = _MM_FP3(a) - _MM_FP3(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_mul_ss                                */
/*  DESCRIPTION : scalar single FP multiply         */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : (a,a,a,a*b)                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_mul_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_FP0(ret) = _MM_FP0(a) * _MM_FP0(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_mul_ps                                */
/*  DESCRIPTION : packed single FP multiply         */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : a * b                           */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_mul_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP0(ret) = _MM_FP0(a) * _MM_FP0(b);
  _MM_FP1(ret) = _MM_FP1(a) * _MM_FP1(b);
  _MM_FP2(ret) = _MM_FP2(a) * _MM_FP2(b);
  _MM_FP3(ret) = _MM_FP3(a) * _MM_FP3(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_div_ss                                */
/*  DESCRIPTION : scalar single FP divide           */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : (a,a,a,a/b)                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_div_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_FP0(ret) = _MM_FP0(a) / _MM_FP0(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_div_ps                                */
/*  DESCRIPTION : packed single FP divide           */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : a / b                           */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_div_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP0(ret) = _MM_FP0(a) / _MM_FP0(b);
  _MM_FP1(ret) = _MM_FP1(a) / _MM_FP1(b);
  _MM_FP2(ret) = _MM_FP2(a) / _MM_FP2(b);
  _MM_FP3(ret) = _MM_FP3(a) / _MM_FP3(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_sqrt_ss                               */
/*  DESCRIPTION : scalar single FP square root      */
/*  IN : m128 a : operand                           */
/*  OUT : none                                      */
/*  RETURN : m128 : (a,a,a,sqrt(a))                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_sqrt_ss(__m128 a)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_FP0(ret) = _mminternal_sqrt(_MM_FP0(a));

  return ret;
}


/****************************************************/
/*  NAME : mm_sqrt_ps                               */
/*  DESCRIPTION : packed single FP square root      */
/*  IN : m128 a : operand                           */
/*  OUT : none                                      */
/*  RETURN : m128 : sqrt(a)                         */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_sqrt_ps(__m128 a)
{
  __m128 ret;

  _MM_FP0(ret) = _mminternal_sqrt(_MM_FP0(a));
  _MM_FP1(ret) = _mminternal_sqrt(_MM_FP1(a));
  _MM_FP2(ret) = _mminternal_sqrt(_MM_FP2(a));
  _MM_FP3(ret) = _mminternal_sqrt(_MM_FP3(a));

  return ret;
}

/****************************************************/
/*  NAME : mm_rcp_ss                                */
/*  DESCRIPTION : scalar single FP reciprocal       */
/*  IN : m128 a : operand                           */
/*  OUT : none                                      */
/*  RETURN : m128 : (a,a,a,(1/a))                   */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_rcp_ss(__m128 a)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_FP0(ret) = _mminternal_approx(_MM_ONE / _MM_FP0(a));

  return ret;
}

/****************************************************/
/*  NAME : mm_rcp_ps                                */
/*  DESCRIPTION : packed single FP reciprocal       */
/*  IN : m128 a : operand                           */
/*  OUT : none                                      */
/*  RETURN : m128 : (1/a)                           */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_rcp_ps(__m128 a)
{
  __m128 ret;

  _MM_FP0(ret) = _mminternal_approx(_MM_ONE / _MM_FP0(a));
  _MM_FP1(ret) = _mminternal_approx(_MM_ONE / _MM_FP1(a));
  _MM_FP2(ret) = _mminternal_approx(_MM_ONE / _MM_FP2(a));
  _MM_FP3(ret) = _mminternal_approx(_MM_ONE / _MM_FP3(a));

  return ret;
}

/****************************************************/
/*  NAME : mm_rsqrt_ss                              */
/*  DESCRIPTION : scalar single FP sqrt reciprocal  */
/*  IN : m128 a : operand                           */
/*  OUT : none                                      */
/*  RETURN : m128 : (a,a,a,(1/sqrt(a)))             */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_rsqrt_ss(__m128 a)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_FP0(ret) = _mminternal_approx(_MM_ONE / _mminternal_sqrt(_MM_FP0(a)));

  return ret;
}

/****************************************************/
/*  NAME : mm_rsqrt_ps                              */
/*  DESCRIPTION : packed single FP sqrt reciprocal  */
/*  IN : m128 a : operand                           */
/*  OUT : none                                      */
/*  RETURN : m128 : round (1/sqrt(a))               */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_rsqrt_ps(__m128 a)
{
  __m128 ret;

  _MM_FP0(ret) = _mminternal_approx(_MM_ONE / _mminternal_sqrt(_MM_FP0(a)));
  _MM_FP1(ret) = _mminternal_approx(_MM_ONE / _mminternal_sqrt(_MM_FP1(a)));
  _MM_FP2(ret) = _mminternal_approx(_MM_ONE / _mminternal_sqrt(_MM_FP2(a)));
  _MM_FP3(ret) = _mminternal_approx(_MM_ONE / _mminternal_sqrt(_MM_FP3(a)));

  return ret;
}

/****************************************************/
/*  NAME : mm_and_ps                                */
/*  DESCRIPTION : bitwise logical AND for single FP */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : a AND b                         */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_and_ps(__m128 a, __m128 b)
{
  __m128 ret;
  
  _MM_INT0(ret) = _MM_INT0(a) & _MM_INT0(b);
  _MM_INT1(ret) = _MM_INT1(a) & _MM_INT1(b);
  _MM_INT2(ret) = _MM_INT2(a) & _MM_INT2(b);
  _MM_INT3(ret) = _MM_INT3(a) & _MM_INT3(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_andnot_ps                             */
/*  DESCRIPTION : bitwise logical AND NOT for singFP*/
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : result of AND NOT               */
/*  NOTES : result is not(a) & b                    */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_andnot_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = (~_MM_INT0(a)) & _MM_INT0(b);
  _MM_INT1(ret) = (~_MM_INT1(a)) & _MM_INT1(b);
  _MM_INT2(ret) = (~_MM_INT2(a)) & _MM_INT2(b);
  _MM_INT3(ret) = (~_MM_INT3(a)) & _MM_INT3(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_or_ps                                 */
/*  DESCRIPTION : bitwise logical OR on single FP   */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : a OR b                          */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_or_ps(__m128 a, __m128 b)
{
  __m128 ret;
  
  _MM_INT0(ret) = _MM_INT0(a) | _MM_INT0(b);
  _MM_INT1(ret) = _MM_INT1(a) | _MM_INT1(b);
  _MM_INT2(ret) = _MM_INT2(a) | _MM_INT2(b);
  _MM_INT3(ret) = _MM_INT3(a) | _MM_INT3(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_xor_ps                                */
/*  DESCRIPTION : bitwise logical XOR on single FP  */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : a XOR b                         */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_xor_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = _MM_INT0(a) ^ _MM_INT0(b);
  _MM_INT1(ret) = _MM_INT1(a) ^ _MM_INT1(b);
  _MM_INT2(ret) = _MM_INT2(a) ^ _MM_INT2(b);
  _MM_INT3(ret) = _MM_INT3(a) ^ _MM_INT3(b);

  return ret;
}

/******************************************************/
/*  NAME : mm_min_ss                                  */
/*  DESCRIPTION : scalar single FP minimum            */
/*  IN : m128 a : operand 1                           */
/*       m128 b : operand 2                           */
/*  OUT : none                                        */
/*  RETURN : m128 : (a,a,a,min(a,b))                  */
/*  NOTES : NaNs will not be supported in the func    */
/*          intrinsics. the outcome of a compare      */
/*          made with NaNs will depend on C semantics */
/******************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_min_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_FP0(ret) = _MM_MIN(_MM_FP0(a), _MM_FP0(b));

  return ret;
}

/******************************************************/
/*  NAME : mm_min_ps                                  */
/*  DESCRIPTION : packed single FP minimum            */
/*  IN : m128 a : operand 1                           */
/*       m128 b : operand 2                           */
/*  OUT : none                                        */
/*  RETURN : m128 : min(a,b)                          */
/*  NOTES : NaNs will not be supported in the func    */
/*          intrinsics. the outcome of a compare      */
/*          made with NaNs will depend on C semantics */
/******************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_min_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP0(ret) = _MM_MIN(_MM_FP0(a), _MM_FP0(b));
  _MM_FP1(ret) = _MM_MIN(_MM_FP1(a), _MM_FP1(b));
  _MM_FP2(ret) = _MM_MIN(_MM_FP2(a), _MM_FP2(b));
  _MM_FP3(ret) = _MM_MIN(_MM_FP3(a), _MM_FP3(b));

  return ret;
}


/******************************************************/
/*  NAME : mm_max_ss                                  */
/*  DESCRIPTION : scalar single FP maximum            */
/*  IN : m128 a : operand 1                           */
/*       m128 b : operand 2                           */
/*  OUT : none                                        */
/*  RETURN : m128 : (a,a,a,max(a,b))                  */
/*  NOTES : NaNs will not be supported in the func    */
/*          intrinsics. the outcome of a compare      */
/*          made with NaNs will depend on C semantics */
/******************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_max_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_FP0(ret) = _MM_MAX(_MM_FP0(a), _MM_FP0(b));

  return ret;
}

/******************************************************/
/*  NAME : mm_max_ps                                  */
/*  DESCRIPTION : packed single FP minimum            */
/*  IN : m128 a : operand 1                           */
/*       m128 b : operand 2                           */
/*  OUT : none                                        */
/*  RETURN : m128 : max(a,b)                          */
/*  NOTES : NaNs will not be supported in the func    */
/*          intrinsics. the outcome of a compare      */
/*          made with NaNs will depend on C semantics */
/******************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_max_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP0(ret) = _MM_MAX(_MM_FP0(a), _MM_FP0(b));
  _MM_FP1(ret) = _MM_MAX(_MM_FP1(a), _MM_FP1(b));
  _MM_FP2(ret) = _MM_MAX(_MM_FP2(a), _MM_FP2(b));
  _MM_FP3(ret) = _MM_MAX(_MM_FP3(a), _MM_FP3(b));

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpeq_ss                              */
/*  DESCRIPTION : scalar single FP equal            */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : low FP masked to all 1's if eq  */
/*                  and all 0's if not. rest FP set */
/*                  to a.                           */
/*                  (a,a,a,a==b)                    */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpeq_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_INT0(ret) = (_MM_FP0(a) == _MM_FP0(b)) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpeq_ps                              */
/*  DESCRIPTION : packed single FP equal            */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : each FP masked to all 1's if eq */
/*                  and all 0's if not.             */
/*                  (a==b,a==b,a==b,a==b)           */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpeq_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = (_MM_FP0(a) == _MM_FP0(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT1(ret) = (_MM_FP1(a) == _MM_FP1(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT2(ret) = (_MM_FP2(a) == _MM_FP2(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT3(ret) = (_MM_FP3(a) == _MM_FP3(b)) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmplt_ss                              */
/*  DESCRIPTION : scalar single FP less than        */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : low FP masked to all 1's if lt  */
/*                  and all 0's if not. rest FP set */
/*                  to a.                           */
/*                  (a,a,a,a<b)                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmplt_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_INT0(ret) = (_MM_FP0(a) < _MM_FP0(b)) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmplt_ps                              */
/*  DESCRIPTION : packed single FP less than        */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : each FP masked to all 1's if lt */
/*                  and all 0's if not.             */
/*                  (a<b,a<b,a<b,a<b)               */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmplt_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = (_MM_FP0(a) < _MM_FP0(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT1(ret) = (_MM_FP1(a) < _MM_FP1(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT2(ret) = (_MM_FP2(a) < _MM_FP2(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT3(ret) = (_MM_FP3(a) < _MM_FP3(b)) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmple_ss                              */
/*  DESCRIPTION : scalar single FP less than equal  */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : low FP masked to all 1's if le  */
/*                  and all 0's if not. rest FP set */
/*                  to a.                           */
/*                  (a,a,a,a<=b)                    */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmple_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_INT0(ret) = (_MM_FP0(a) <= _MM_FP0(b)) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmple_ps                              */
/*  DESCRIPTION : packed single FP less than equal  */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : each FP masked to all 1's if le */
/*                  and all 0's if not.             */
/*                  (a<=b,a<=b,a<=b,a<=b)           */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmple_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = (_MM_FP0(a) <= _MM_FP0(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT1(ret) = (_MM_FP1(a) <= _MM_FP1(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT2(ret) = (_MM_FP2(a) <= _MM_FP2(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT3(ret) = (_MM_FP3(a) <= _MM_FP3(b)) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpgt_ss                              */
/*  DESCRIPTION : scalar single FP greater than     */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : low FP masked to all 1's if gt  */
/*                  and all 0's if not. rest FP set */
/*                  to a.                           */
/*                  (a,a,a,a>b)                     */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpgt_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_INT0(ret) = (_MM_FP0(a) > _MM_FP0(b)) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpgt_ps                              */
/*  DESCRIPTION : packed single FP greater than     */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : each FP masked to all 1's if gt */
/*                  and all 0's if not.             */
/*                  (a>b,a>b,a>b,a>b)               */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpgt_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = (_MM_FP0(a) > _MM_FP0(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT1(ret) = (_MM_FP1(a) > _MM_FP1(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT2(ret) = (_MM_FP2(a) > _MM_FP2(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT3(ret) = (_MM_FP3(a) > _MM_FP3(b)) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpge_ss                              */
/*  DESCRIPTION : scalar single FP greater equal    */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : low FP masked to all 1's if ge  */
/*                  and all 0's if not. rest FP set */
/*                  to a.                           */
/*                  (a,a,a,a>=b)                    */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpge_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_INT0(ret) = (_MM_FP0(a) >= _MM_FP0(b)) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpge_ps                              */
/*  DESCRIPTION : packed single FP greater equal    */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : each FP masked to all 1's if ge */
/*                  and all 0's if not.             */
/*                  (a>=b,a>=b,a>=b,a>=b)           */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpge_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = (_MM_FP0(a) >= _MM_FP0(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT1(ret) = (_MM_FP1(a) >= _MM_FP1(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT2(ret) = (_MM_FP2(a) >= _MM_FP2(b)) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT3(ret) = (_MM_FP3(a) >= _MM_FP3(b)) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpneq_ss                             */
/*  DESCRIPTION : scalar single FP not equal to     */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : low FP masked to all 1's if neq */
/*                  and all 0's if not. rest FP set */
/*                  to a.                           */
/*                  (a,a,a,!(a==b))                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpneq_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_INT0(ret) = (!(_MM_FP0(a) == _MM_FP0(b))) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpneq_ps                             */
/*  DESCRIPTION : packed single FP not equal to     */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : each FP masked to all 1's if neq*/
/*                  and all 0's if not.             */
/*                 (!(a==b),!(a==b),!(a==b),!(a==b))*/
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpneq_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = (!(_MM_FP0(a) == _MM_FP0(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT1(ret) = (!(_MM_FP1(a) == _MM_FP1(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT2(ret) = (!(_MM_FP2(a) == _MM_FP2(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT3(ret) = (!(_MM_FP3(a) == _MM_FP3(b))) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpnlt_ss                             */
/*  DESCRIPTION : scalar single FP not less than    */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : low FP masked to all 1's if nlt */
/*                  and all 0's if not. rest FP set */
/*                  to a.                           */
/*                  (a,a,a,!(a<b))                  */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpnlt_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_INT0(ret) = (!(_MM_FP0(a) < _MM_FP0(b))) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpnlt_ps                             */
/*  DESCRIPTION : packed single FP not less than    */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : each FP masked to all 1's if nlt*/
/*                  and all 0's if not.             */
/*                  (!(a<b),!(a<b),!(a<b),!(a<b))   */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpnlt_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = (!(_MM_FP0(a) < _MM_FP0(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT1(ret) = (!(_MM_FP1(a) < _MM_FP1(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT2(ret) = (!(_MM_FP2(a) < _MM_FP2(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT3(ret) = (!(_MM_FP3(a) < _MM_FP3(b))) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpnle_ss                             */
/*  DESCRIPTION : scalar single FP not less than or */
/*                equal to                          */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : low FP masked to all 1's if nle */
/*                  and all 0's if not. rest FP set */
/*                  to a.                           */
/*                  (a,a,a,!(a<=b))                 */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpnle_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_INT0(ret) = (!(_MM_FP0(a) <= _MM_FP0(b))) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpnle_ps                             */
/*  DESCRIPTION : packed single FP not less than or */
/*                equal to                          */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : each FP masked to all 1's if nle*/
/*                  and all 0's if not.             */
/*                 (!(a<=b),!(a<=b),!(a<=b),!(a<=b))*/
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpnle_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = (!(_MM_FP0(a) <= _MM_FP0(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT1(ret) = (!(_MM_FP1(a) <= _MM_FP1(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT2(ret) = (!(_MM_FP2(a) <= _MM_FP2(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT3(ret) = (!(_MM_FP3(a) <= _MM_FP3(b))) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpngt_ss                             */
/*  DESCRIPTION : scalar single FP not greater than */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : low FP masked to all 1's if ngt */
/*                  and all 0's if not. rest FP set */
/*                  to a.                           */
/*                  (a,a,a,!(a>b))                  */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpngt_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_INT0(ret) = (!(_MM_FP0(a) > _MM_FP0(b))) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpngt_ps                             */
/*  DESCRIPTION : packed single FP not greater than */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : low FP masked to all 1's if ngt */
/*                  and all 0's if not. rest FP set */
/*                  to a.                           */
/*                  (!(a>b),!(a>b),!(a>b),!(a>b))   */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpngt_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = (!(_MM_FP0(a) > _MM_FP0(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT1(ret) = (!(_MM_FP1(a) > _MM_FP1(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT2(ret) = (!(_MM_FP2(a) > _MM_FP2(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT3(ret) = (!(_MM_FP3(a) > _MM_FP3(b))) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/****************************************************/
/*  NAME : mm_cmpnge_ss                             */
/*  DESCRIPTION : scalar single FP not greater than */
/*                or equal to                       */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : each FP masked to all 1's if nge*/
/*                  and all 0's if not.             */
/*                 (a,a,a,!(a>=b))                  */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpnge_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_INT0(ret) = (!(_MM_FP0(a) >= _MM_FP0(b))) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}

/****************************************************/
/*  NAME : mm_cmpnge_ps                             */
/*  DESCRIPTION : packed single FP not greater than */
/*                or equal to                       */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : each FP masked to all 1's if nge*/
/*                  and all 0's if not.             */
/*                 (!(a>=b),!(a>=b),!(a>=b),!(a>=b))*/
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpnge_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_INT0(ret) = (!(_MM_FP0(a) >= _MM_FP0(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT1(ret) = (!(_MM_FP1(a) >= _MM_FP1(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT2(ret) = (!(_MM_FP2(a) >= _MM_FP2(b))) ? _MM_BITONE : _MM_BITZERO;
  _MM_INT3(ret) = (!(_MM_FP3(a) >= _MM_FP3(b))) ? _MM_BITONE : _MM_BITZERO;

  return ret;
}


/*
 * cmp_ord and cmp_unord intrinsics (not supported in faux intrinsics)
 */
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpord_ps(__m128 a, __m128 b)
{
  _mminternal_abort("_mm_cmpord_ps not supported");
  return a;
}
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpord_ss(__m128 a, __m128 b)
{
  _mminternal_abort("_mm_cmpord_ss not supported");
  return a;
}
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpunord_ps(__m128 a, __m128 b)
{
  _mminternal_abort("_mm_cmpunord_ps not supported");
  return a;
}
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cmpunord_ss(__m128 a, __m128 b)
{
  _mminternal_abort("_mm_cmpunord_ss not supported");
  return a;
}


/****************************************************/
/*  NAME : mm_comieq_ss                             */
/*  DESCRIPTION : scalar single FP equal to         */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if equal to                    */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_comieq_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) == _MM_FP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_comilt_ss                             */
/*  DESCRIPTION : scalar single FP less than        */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if less than                   */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_comilt_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) < _MM_FP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_comile_ss                             */
/*  DESCRIPTION : scalar single FP less than or     */
/*                equal to                          */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if less than or equal to       */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_comile_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) <= _MM_FP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_comigt_ss                             */
/*  DESCRIPTION : scalar single FP greater than     */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if greater than                */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_comigt_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) > _MM_FP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_comige_ss                             */
/*  DESCRIPTION : scalar single FP greater than or  */
/*                equal to                          */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if greater than or equal to    */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_comige_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) >= _MM_FP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_comineq_ss                            */
/*  DESCRIPTION : scalar single FP not equal to     */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if not equal to                */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_comineq_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) != _MM_FP0(b)) ? 1 : 0;

  return ret;
}


/****************************************************/
/*  NAME : mm_ucomieq_ss                            */
/*  DESCRIPTION : scalar single FP equal to         */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if equal to                    */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_ucomieq_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) == _MM_FP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_ucomilt_ss                            */
/*  DESCRIPTION : scalar single FP less than        */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if less than                   */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_ucomilt_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) < _MM_FP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_ucomile_ss                            */
/*  DESCRIPTION : scalar single FP less than or     */
/*                equal to                          */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if less than or equal to       */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_ucomile_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) <= _MM_FP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_ucomigt_ss                            */
/*  DESCRIPTION : scalar single FP greater than     */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if greater than                */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_ucomigt_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) > _MM_FP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_ucomige_ss                            */
/*  DESCRIPTION : scalar single FP greater than or  */
/*                equal to                          */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if greater than or equal to    */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_ucomige_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) >= _MM_FP0(b)) ? 1 : 0;

  return ret;
}

/****************************************************/
/*  NAME : mm_ucomineq_ss                           */
/*  DESCRIPTION : scalar single FP not equal to     */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : int : 1 if not equal to                */
/*                 0 if not.                        */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_ucomineq_ss(__m128 a, __m128 b)
{
  int ret;

  ret = (_MM_FP0(a) != _MM_FP0(b)) ? 1 : 0;

  return ret;
}


/****************************************************/
/*  NAME : mm_cvt_ss2si                             */
/*  DESCRIPTION : scalar single FP to signed INT32  */
/*                conversion.                       */
/*  IN : m128 a : number to convert                 */
/*  OUT : none                                      */
/*  RETURN : INT32 : low FP of a rounded            */
/*  NOTES : result is rounded according to MXCSR    */
/*          default, round-to-nearest               */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_cvt_ss2si(__m128 a)
{
  int ret;

  ret = _mm_round_nearest( _MM_FP0(a) );

  return ret;
}

/****************************************************/
/*  NAME : mm_cvt_ps2pi                             */
/*  DESCRIPTION : packed single FP to packed signed */
/*                INT32 conversion.                 */
/*  IN : m128 a : number to convert                 */
/*  OUT : none                                      */
/*  RETURN : __m64 : FP values from a rounded       */
/*  NOTES : result is rounded according to MXCSR    */
/*          default, round-to-nearest               */
/****************************************************/
_MM_INLINE_COMMAND static __m64 __cdecl _mm_cvt_ps2pi(__m128 a)
{
  _mm_m64 ret;

  ret.m64.dw[0] = _mm_round_nearest( _MM_FP0(a) );
  ret.m64.dw[1] = _mm_round_nearest( _MM_FP1(a) );

  return ret.m64.m;
}

/****************************************************/
/*  NAME : mm_cvtt_ss2si                            */
/*  DESCRIPTION : scalar single FP to signed INT32  */
/*                conversion (truncated).           */
/*  IN : m128 a : number to convert                 */
/*  OUT : none                                      */
/*  RETURN : INT32 : low FP of a truncated          */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_cvtt_ss2si(__m128 a)
{
  int ret;

  ret = _mm_round_trunc ( _MM_FP0(a) );

  return ret;
}

/****************************************************/
/*  NAME : mm_cvtt_ps2pi                            */
/*  DESCRIPTION : packed single FP to packed signed */
/*                INT32 conversion (truncated).     */
/*  IN : m128 a : number to convert                 */
/*  OUT : none                                      */
/*  RETURN : INT32 : FP of a truncated              */
/****************************************************/
_MM_INLINE_COMMAND static __m64 __cdecl _mm_cvtt_ps2pi(__m128 a)
{
  _mm_m64 ret;

  ret.m64.dw[0] = _mm_round_trunc ( _MM_FP0(a) );
  ret.m64.dw[1] = _mm_round_trunc ( _MM_FP1(a) );

  return ret.m64.m;
}

/****************************************************/
/*  NAME : mm_cvt_si2ss                             */
/*  DESCRIPTION : scalar signed INT32 to single FP  */
/*                conversion                        */
/*  IN : m128  a : orig contents of dest register   */
/*       INT32 b : number to convert                */
/*  OUT : none                                      */
/*  RETURN : m128 : b is converted into results low */
/*                  FP with the high portion of a   */
/*                  unchanged                       */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cvt_si2ss(__m128 a, int b)
{
  __m128 ret;

  _MM_FP0(ret) = (float) b;
  _MM_INT1(ret) = _MM_INT1(a);
  _MM_INT2(ret) = _MM_INT2(a);
  _MM_INT3(ret) = _MM_INT3(a);

  return ret;
}

/****************************************************/
/*  NAME : mm_cvt_pi2ps                             */
/*  DESCRIPTION : packed signed INT32 to single FP  */
/*                conversion                        */
/*  IN : m128 b: orig contents of dest register     */
/*       m64 a : numbers to convert                 */
/*  OUT : none                                      */
/*  RETURN : m128 : b is converted FP with the      */
/*                  upper two elements of a         */
/*                  unchanged.                      */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_cvt_pi2ps(__m128 a, __m64 b)
{
  __m128 ret;
  _mm_m64 bb;

  bb.m64.m = b;
  _m_empty();
  _MM_FP0(ret) = (float) bb.m64.dw[0];
  _MM_FP1(ret) = (float) bb.m64.dw[1];
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP3(ret) = _MM_FP3(a);

  return ret;
}

/*****************************************************************/
/*  NAME : mm_shuffle_ps                                         */
/*  DESCRIPTION : shuffle single FP                              */
/*  IN : m128 a : SP FP number 1                                 */
/*       m128 b : SP FP number 2                                 */
/*       unsigned int imm8 : type of shuffle to perform          */
/*  OUT : none                                                   */
/*  RETURN : m128 : result of shuffle                            */
/*  NOTES : this function places any of the first parameter's (a)*/
/*  4 FP numbers into any of the result's 2 low FP numbers, and  */
/*  any of the second parameter's (b) 4 FP numbers into any of   */
/*  the result's 2 high FP numbers. which operation to perform is*/
/*  indicated by setting imm8 to the appropriate value. The macro*/
/*  MM_SHUFF(fp3,fp2,fp1,fp0) is written to help with the imm8   */
/*  value. Here is the description for the macro :               */
/*  argument fp3 is a digit[0123] that represents the fp from    */
/*  argument "b" of mm_shuffle_ps that will be placed in fp3 of  */
/*  result. fp2 is the same for fp2 in result. fp1 is a          */
/*  digit[0123] that represents the fp from argument "a" of      */
/*  mm_shuffle_ps that will be places in fp1 of result. fp0 is   */
/*  the same for fp0 of  result                                  */
/*****************************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_shuffle_ps(__m128 a, __m128 b, unsigned int imm8)
{
  __m128 ret;

#if 0
  float a_a[4];
  float b_a[4];

  a_a[0] = _MM_FP0(a); a_a[1] = _MM_FP1(a);
  a_a[2] = _MM_FP2(a); a_a[3] = _MM_FP3(a);
  b_a[0] = _MM_FP0(b); b_a[1] = _MM_FP1(b);
  b_a[2] = _MM_FP2(b); b_a[3] = _MM_FP3(b);

  _MM_FP0(ret) = a_a[ ((imm8 >> 0) & 0x3) ];
  _MM_FP1(ret) = a_a[ ((imm8 >> 2) & 0x3) ];
  _MM_FP2(ret) = b_a[ ((imm8 >> 4) & 0x3) ];
  _MM_FP3(ret) = b_a[ ((imm8 >> 6) & 0x3) ];

#else
  /* slightly faster... */  
  int t;
  
#define a_a0 _MM_FP0(a)
#define a_a1 _MM_FP1(a)
#define a_a2 _MM_FP2(a)
#define a_a3 _MM_FP3(a)
#define b_a0 _MM_FP0(b)
#define b_a1 _MM_FP1(b)
#define b_a2 _MM_FP2(b)
#define b_a3 _MM_FP3(b)

  t =  ((imm8 >> 0) & 0x3) ;
  _MM_FP0(ret) = (t==0 ? a_a0 :
		  (t==1 ? a_a1 :
		   (t==2 ? a_a2 : a_a3)));

  t =  ((imm8 >> 2) & 0x3) ;
  _MM_FP1(ret) = (t==0 ? a_a0 :
		  (t==1 ? a_a1 :
		   (t==2 ? a_a2 : a_a3)));

  t =  ((imm8 >> 4) & 0x3) ;
  _MM_FP2(ret) = (t==0 ? b_a0 :
		  (t==1 ? b_a1 :
		   (t==2 ? b_a2 : b_a3)));

  t =  ((imm8 >> 6) & 0x3) ;
  _MM_FP3(ret) = (t==0 ? b_a0 :
		  (t==1 ? b_a1 :
		   (t==2 ? b_a2 : b_a3)));

#undef a_a0
#undef a_a1
#undef a_a2
#undef a_a3
#undef b_a0
#undef b_a1
#undef b_a2
#undef b_a3

#endif
  
  return ret;
}

/****************************************************/
/*  NAME : mm_unpackhi_ps                           */
/*  DESCRIPTION : unpack high packed single FP data */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : interleaves 2 high FP's of each operand*/
/*           into result like so (3=high 0=low) :   */
/*           (b3, a3, b2, a2) <--result (left=high) */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_unpackhi_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP0(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP2(b);
  _MM_FP2(ret) = _MM_FP3(a);
  _MM_FP3(ret) = _MM_FP3(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_unpacklo_ps                           */
/*  DESCRIPTION : unpack low packed single FP data  */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : interleaves 2 low FP's of each operand */
/*           into result like so (3=high 0=low) :   */
/*           (b1, a1, b0, a0) <--result (left=high) */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_unpacklo_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP0(ret) = _MM_FP0(a);
  _MM_FP1(ret) = _MM_FP0(b);
  _MM_FP2(ret) = _MM_FP1(a);
  _MM_FP3(ret) = _MM_FP1(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_loadh_pi                              */
/*  DESCRIPTION : Move 64 bits of memory to the     */
/*                upper two fields of a _m128 reg.  */
/*  IN : m128 a : 128 bits register destination     */
/*       m64 *b : 64 bit memory location to load    */
/*                from                              */
/*  OUT : none                                      */
/*  RETURN : m128 value with the upper 64 bits      */
/*           replaced with 'b' and the lower 64     */
/*           bits unchanged from 'a'.               */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_loadh_pi(__m128 a, __m64 const *b)
{
  _mm_m128 aa;

  aa.m128.m = a;

  aa.m128.dw[2] = ((int*)b)[0];
  aa.m128.dw[3] = ((int*)b)[1];

  return aa.m128.m;
}

/****************************************************/
/*  NAME : mm_storeh_pi                             */
/*  DESCRIPTION : Move upper 64 bits of a m128 reg  */
/*                to memory                         */
/*  IN : m128 b : 128 bit register source           */
/*  OUT : m64 *a : 64 bit register destination      */
/*  RETURN : none                                   */
/****************************************************/
_MM_INLINE_COMMAND static void __cdecl _mm_storeh_pi(__m64 *a, __m128 b)
{
  _mm_m128 bb;

  bb.m128.m = b;
  ((int*)a)[0] = bb.m128.dw[2];
  ((int*)a)[1] = bb.m128.dw[3];
}

/****************************************************/
/*  NAME : mm_loadl_pi                              */
/*  DESCRIPTION : Move 64 bits of memory to the     */
/*                lower two fields of a _m128 reg.  */
/*  IN : m128 a : 128 bits register destination     */
/*       m64 *b : 64 bit memory location to load    */
/*                from                              */
/*  OUT : none                                      */
/*  RETURN : m128 value with the lower 64 bits      */
/*           replaced with 'b' and the upper 64     */
/*           bits unchanged from 'a'.               */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_loadl_pi(__m128 a, __m64 const *b)
{
  _mm_m128 aa;

  aa.m128.m = a;

  aa.m128.dw[0] = ((int*)b)[0];
  aa.m128.dw[1] = ((int*)b)[1];

  return aa.m128.m;
}

/****************************************************/
/*  NAME : mm_storel_pi                             */
/*  DESCRIPTION : Move lower 64 bits of a m128 reg  */
/*                to memory                         */
/*  IN : m128 b : 128 bit register source           */
/*  OUT : m64 *a : 64 bit register destination      */
/*  RETURN : none                                   */
/****************************************************/
_MM_INLINE_COMMAND static void __cdecl _mm_storel_pi(__m64 *a, __m128 b)
{
  _mm_m128 bb;

  bb.m128.m = b;
  ((int*)a)[0] = bb.m128.dw[0];
  ((int*)a)[1] = bb.m128.dw[1];
}

/****************************************************/
/*  NAME : mm_movehl_ps                             */
/*  DESCRIPTION : Move upper 64 bits of a m128 reg  */
/*                to to the lower 64 bits of another*/
/*  IN : m128 a,b: 128 bit register sources         */
/*  RETURN : 128 bit value whose upper 64 bits are  */
/*           the upper 64 bits of a, and whose lower*/
/*           bits are the upper 64 bits of b        */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_movehl_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP0(ret) = _MM_FP2(b);
  _MM_FP1(ret) = _MM_FP3(b);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP3(ret) = _MM_FP3(a);

  return ret;
}

/****************************************************/
/*  NAME : mm_movelh_ps                             */
/*  DESCRIPTION : Move lower 64 bits of a m128 reg  */
/*                to to the upper 64 bits of another*/
/*  IN : m128 a,b: 128 bit register sources         */
/*  RETURN : 128 bit value whose lower 64 bits are  */
/*           the lower 64 bits of a, and whose upper*/
/*           bits are the lower 64 bits of b        */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_movelh_ps(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP0(ret) = _MM_FP0(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_FP2(ret) = _MM_FP0(b);
  _MM_FP3(ret) = _MM_FP1(b);

  return ret;
}

/****************************************************/
/*  NAME : mm_movemask_ps                           */
/*  DESCRIPTION : move mask to integer              */
/*  IN : m128 a : operand                           */
/*  OUT : none                                      */
/*  RETURN : 4-bit mask formed of the most sig. bits*/
/*           of 4 FPs in a.                         */
/****************************************************/
_MM_INLINE_COMMAND static int __cdecl _mm_movemask_ps(__m128 a)
{
  int ret;

  ret = (((_MM_INT0(a)>>31)&0x1) | ((_MM_INT1(a)>>30)&0x2) |
         ((_MM_INT2(a)>>29)&0x4) | ((_MM_INT3(a)>>28)&0x8));

  return ret;
}


/*
 * Integer extensions
 */
_MM_INLINE_COMMAND static int __cdecl _m_pextrw(__m64 a, int i)
{
  int ret;
  _mm_m64 aa;

  aa.m64.m = a;
  ret = (int)(aa.m64.uw[i & 0x03]);
  _m_empty();
  return ret;
}

_MM_INLINE_COMMAND static __m64 __cdecl _m_pinsrw(__m64 m, int w, int i)
{
  _mm_m64 ret;

  ret.m64.m = m;
  ret.m64.uw[i & 0x03] = w;
  return ret.m64.m;
}

_MM_INLINE_COMMAND static __m64 __cdecl _m_pmaxsw(__m64 a, __m64 b)
{
  _mm_m64 aa, bb, ret;
  int i;

  aa.m64.m = a;
  bb.m64.m = b;
  for (i=0; i<4; i++)
	ret.m64.w[i] = _MM_MAX(aa.m64.w[i], bb.m64.w[i]);
  return ret.m64.m;
}

_MM_INLINE_COMMAND static __m64 __cdecl _m_pmaxub(__m64 a, __m64 b)
{
  _mm_m64 aa, bb, ret;
  int i;

  aa.m64.m = a;
  bb.m64.m = b;
  for (i=0; i<8; i++)
	ret.m64.ub[i] = _MM_MAX(aa.m64.ub[i], bb.m64.ub[i]);
  return ret.m64.m;
}

_MM_INLINE_COMMAND static __m64 __cdecl _m_pminsw(__m64 a, __m64 b)
{
  _mm_m64 aa, bb, ret;
  int i;

  aa.m64.m = a;
  bb.m64.m = b;
  for (i=0; i<4; i++)
	ret.m64.w[i] = _MM_MIN(aa.m64.w[i], bb.m64.w[i]);
  return ret.m64.m;
}

_MM_INLINE_COMMAND static __m64 __cdecl _m_pminub(__m64 a, __m64 b)
{
  _mm_m64 aa, bb, ret;
  int i;

  aa.m64.m = a;
  bb.m64.m = b;
  for (i=0; i<8; i++)
	ret.m64.ub[i] = _MM_MIN(aa.m64.ub[i], bb.m64.ub[i]);
  return ret.m64.m;
}

_MM_INLINE_COMMAND static int __cdecl _m_pmovmskb(__m64 a)
{
  int ret;
  _mm_m64 aa;

  aa.m64.m = a;
  ret =     ((aa.m64.ub[0]>>7)&0x1) +
	    ((aa.m64.ub[1]>>7)&0x1)*2 +
	    ((aa.m64.ub[2]>>7)&0x1)*4 +
	    ((aa.m64.ub[3]>>7)&0x1)*8 +
	    ((aa.m64.ub[4]>>7)&0x1)*16 +
	    ((aa.m64.ub[5]>>7)&0x1)*32 +
	    ((aa.m64.ub[6]>>7)&0x1)*64 +
	    ((aa.m64.ub[7]>>7)&0x1)*128;
  _m_empty();
  return ret;
}

_MM_INLINE_COMMAND static __m64 __cdecl _m_pmulhuw(__m64 a, __m64 b)
{
  _mm_m64 aa, bb, ret;
  unsigned int i;

  aa.m64.m = a;
  bb.m64.m = b;

  i = (unsigned int)aa.m64.uw[0] * (unsigned int)bb.m64.uw[0];
  ret.m64.uw[0] = (unsigned short)(i >> 16);

  i = (unsigned int)aa.m64.uw[1] * (unsigned int)bb.m64.uw[1];
  ret.m64.uw[1] = (unsigned short)(i >> 16);

  i = (unsigned int)aa.m64.uw[2] * (unsigned int)bb.m64.uw[2];
  ret.m64.uw[2] = (unsigned short)(i >> 16);

  i = (unsigned int)aa.m64.uw[3] * (unsigned int)bb.m64.uw[3];
  ret.m64.uw[3] = (unsigned short)(i >> 16);

  return ret.m64.m;
}

_MM_INLINE_COMMAND static __m64 __cdecl _m_pshufw(__m64 a, int i)
{
  _mm_m64 aa, ret;

  aa.m64.m = a;
  ret.m64.w[0] = aa.m64.w[i & 0x03];
  ret.m64.w[1] = aa.m64.w[(i >> 2) & 0x03];
  ret.m64.w[2] = aa.m64.w[(i >> 4) & 0x03];
  ret.m64.w[3] = aa.m64.w[(i >> 6) & 0x03];

  return ret.m64.m;
}

_MM_INLINE_COMMAND static void __cdecl _m_maskmovq(__m64 a, __m64 b, char *c)
{
  int i;
  _mm_m64 msk, aa;


  aa.m64.m  = a;
  msk.m64.m = b;
  for(i=0;i<8;i++) {
    if(msk.m64.ub[i] & 0x80) {
      c[i] = aa.m64.ub[i];
    }
  }
  _m_empty();
}

_MM_INLINE_COMMAND static void __cdecl _mm_stream_pi(__m64 *a, __m64 b)
{
  *a = b;
  _m_empty();
}

_MM_INLINE_COMMAND static __m64 __cdecl _m_pavgb(__m64 a, __m64 b)
{
  _mm_m64 aa, bb, ret;
  int i;

  aa.m64.m = a;
  bb.m64.m = b;

  for (i=0; i<8; i++) {
     unsigned int tmp = aa.m64.ub[i] + bb.m64.ub[i] + 1;
     ret.m64.ub[i] = (unsigned char)(tmp >> 1);
  }

  return ret.m64.m;
}

_MM_INLINE_COMMAND static __m64 __cdecl _m_pavgw(__m64 a, __m64 b)
{
  _mm_m64 aa, bb, ret;
  int i;

  aa.m64.m = a;
  bb.m64.m = b;

  for (i=0; i<4; i++) {
     unsigned int tmp = aa.m64.uw[i] + bb.m64.uw[i] + 1;
     ret.m64.uw[i] = (unsigned short)(tmp >> 1);
  }

  return ret.m64.m;
}

_MM_INLINE_COMMAND static __m64 __cdecl _m_psadbw(__m64 a, __m64 b)
{
  _mm_m64 aa, bb, ret;
  int i;
  unsigned short sum = 0;

  aa.m64.m = a;
  bb.m64.m = b;
  for (i=0; i<8; i++) {
    short t = aa.m64.ub[i] - bb.m64.ub[i];
    t = (t < 0) ? -t : t;
    sum += t;
  }
  ret.m64.uw[0] = sum;
  ret.m64.uw[1] = 0;
  ret.m64.uw[2] = 0;
  ret.m64.uw[3] = 0;
  return ret.m64.m;
}

_MM_INLINE_COMMAND static void __cdecl _mm_stream_ps(float *a, __m128 b)
{
  _mminternal_assert_16B(a);
  *(__m128*)a = b;
}


/****************************************************/
/*  NAME : mm_move_ss                               */
/*  DESCRIPTION : scalar move                       */
/*  IN : m128 a : operand 1                         */
/*       m128 b : operand 2                         */
/*  OUT : none                                      */
/*  RETURN : m128 : (a,a,a,b)                       */
/****************************************************/

/* (note there is no corresponding move_ps intrinsic, since you
   can just say "a=b" and you'll get what you want) */

_MM_INLINE_COMMAND static __m128 __cdecl _mm_move_ss(__m128 a, __m128 b)
{
  __m128 ret;

  _MM_FP3(ret) = _MM_FP3(a);
  _MM_FP2(ret) = _MM_FP2(a);
  _MM_FP1(ret) = _MM_FP1(a);
  _MM_FP0(ret) = _MM_FP0(b);

  return ret;
}


/****************************************************/
/*  NAME : mm_set_ss                                */
/*  DESCRIPTION : scalar single set                 */
/*  IN : float a : value to set to                  */
/*  OUT : none                                      */
/*  RETURN : m128 : (0,0,0,a)                       */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_set_ss(float a)
{
  __m128 ret;

  _MM_FP0(ret) = a;
  _MM_FP1(ret) = _MM_ZERO;
  _MM_FP2(ret) = _MM_ZERO;
  _MM_FP3(ret) = _MM_ZERO;

  return ret;
}

/****************************************************/
/*  NAME : mm_set_ps1                               */
/*  DESCRIPTION : packed single set 1               */
/*  IN : float a : value to set to                  */
/*  OUT : none                                      */
/*  RETURN : m128 : (a,a,a,a)                       */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_set_ps1(float a)
{
  __m128 ret;

  _MM_FP0(ret) = a;
  _MM_FP1(ret) = a;
  _MM_FP2(ret) = a;
  _MM_FP3(ret) = a;

  return ret;
}

/****************************************************/
/*  NAME : mm_set_ps                                */
/*  DESCRIPTION : packed single set                 */
/*  IN : float a : value to set fp0 to              */
/*       float b : value to set fp1 to              */
/*       float c : value to set fp2 to              */
/*       float d : value to set fp3 to              */
/*  OUT : none                                      */
/*  RETURN : m128 : (a,b,c,d)                       */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_set_ps(float a, float b, float c, float d)
{
  __m128 ret;

  _MM_FP0(ret) = d;
  _MM_FP1(ret) = c;
  _MM_FP2(ret) = b;
  _MM_FP3(ret) = a;

  return ret;
}

/****************************************************/
/*  NAME : mm_setr_ps                               */
/*  DESCRIPTION : reversed packed single set        */
/*  IN : float a : value to set fp0 to              */
/*       float b : value to set fp1 to              */
/*       float c : value to set fp2 to              */
/*       float d : value to set fp3 to              */
/*  OUT : none                                      */
/*  RETURN : m128 : (d,c,b,a)                       */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_setr_ps(float a, float b, float c, float d)
{
  __m128 ret;

  _MM_FP0(ret) = a;
  _MM_FP1(ret) = b;
  _MM_FP2(ret) = c;
  _MM_FP3(ret) = d;

  return ret;
}


/****************************************************/
/*  NAME : mm_setzero_ps                            */
/*  DESCRIPTION : set all elements to zero          */
/*  IN : none                                       */
/*  OUT : none                                      */
/*  RETURN : m128 : (0,0,0,0)                       */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_setzero_ps(void)
{
  __m128 ret;

  _MM_FP0(ret) = 0.0;
  _MM_FP1(ret) = 0.0;
  _MM_FP2(ret) = 0.0;
  _MM_FP3(ret) = 0.0;

  return ret;
}

/****************************************************/
/*  NAME : mm_load_ss                               */
/*  DESCRIPTION : scalar single load                */
/*  IN : float *a : array to load from              */
/*  OUT : none                                      */
/*  RETURN : m128 : (0,0,0,a[0])                    */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_load_ss(float const *a)
{
  __m128 ret;

  _MM_FP0(ret) = a[0];
  _MM_FP1(ret) = _MM_ZERO;
  _MM_FP2(ret) = _MM_ZERO;
  _MM_FP3(ret) = _MM_ZERO;

  return ret;
}

/****************************************************/
/*  NAME : mm_load_ps1                              */
/*  DESCRIPTION : packed single load 1              */
/*  IN : float *a : array to load from              */
/*  OUT : none                                      */
/*  RETURN : m128 : (a[0],a[0],a[0],a[0])           */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_load_ps1(float const *a)
{
  __m128 ret;

  _MM_FP0(ret) = a[0];
  _MM_FP1(ret) = a[0];
  _MM_FP2(ret) = a[0];
  _MM_FP3(ret) = a[0];

  return ret;
}

/****************************************************/
/*  NAME : mm_load_ps                               */
/*  DESCRIPTION : packed single load                */
/*  IN : float *a : array to load from              */
/*  OUT : none                                      */
/*  RETURN : m128 : (a[3],a[2],a[1],a[0])           */
/*  NOTE : argument "a" must have 4 floats          */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_load_ps(float const *a)
{
  __m128 ret;

  _mminternal_assert_16B(a);
  _MM_FP0(ret) = a[0];
  _MM_FP1(ret) = a[1];
  _MM_FP2(ret) = a[2];
  _MM_FP3(ret) = a[3];

  return ret;
}

/****************************************************/
/*  NAME : mm_loadu_ps                              */
/*  DESCRIPTION : packed single load                */
/*  IN : float *a : array to load from              */
/*  OUT : none                                      */
/*  RETURN : m128 : (a[3],a[2],a[1],a[0])           */
/*  NOTE : argument "a" must have 4 floats          */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_loadu_ps(float const *a)
{
  __m128 ret;

  _MM_FP0(ret) = a[0];
  _MM_FP1(ret) = a[1];
  _MM_FP2(ret) = a[2];
  _MM_FP3(ret) = a[3];

  return ret;
}

/****************************************************/
/*  NAME : mm_loadr_ps                              */
/*  DESCRIPTION : reversed packed single load       */
/*  IN : float *a : array to load from              */
/*  OUT : none                                      */
/*  RETURN : m128 : (a[0],a[1],a[2],a[3])           */
/*  NOTE : argument "a" must have 4 floats          */
/****************************************************/
_MM_INLINE_COMMAND static __m128 __cdecl _mm_loadr_ps(float const *a)
{
  __m128 ret;

  _mminternal_assert_16B(a);
  _MM_FP0(ret) = a[3];
  _MM_FP1(ret) = a[2];
  _MM_FP2(ret) = a[1];
  _MM_FP3(ret) = a[0];

  return ret;
}

/****************************************************/
/*  NAME : mm_store_ss                              */
/*  DESCRIPTION : scalar single store               */
/*  IN : m128 a : m128 to load from                 */
/*  OUT : float *v : (fp0(a))                       */
/*  RETURN : none                                   */
/*  NOTE : 1 float will be return. it is up to the  */
/*         caller to ensure space is available      */
/****************************************************/
_MM_INLINE_COMMAND static void __cdecl _mm_store_ss(float *v, __m128 a)
{
  v[0] = _MM_FP0(a);
}

/****************************************************/
/*  NAME : mm_store_ps1                             */
/*  DESCRIPTION : packed single store 1             */
/*  IN : m128 a : m128 to load from                 */
/*  OUT : float *v : (fp0(a),fp0(a),fp0(a),fp0(a))  */
/*  RETURN : none                                   */
/*  NOTE : 4 floats will be return. it is up to the */
/*         caller to ensure space is available      */
/****************************************************/
_MM_INLINE_COMMAND static void __cdecl _mm_store_ps1(float *v, __m128 a)
{
  v[0] = _MM_FP0(a);
  v[1] = _MM_FP0(a);
  v[2] = _MM_FP0(a);
  v[3] = _MM_FP0(a);
}

/****************************************************/
/*  NAME : mm_store_ps                              */
/*  DESCRIPTION : packed single store               */
/*  IN : m128 a : m128 to load from                 */
/*  OUT : float *v : (fp0(a),fp1(a),fp2(a),fp3(a))  */
/*  RETURN : none                                   */
/*  NOTE : 4 floats will be return. it is up to the */
/*         caller to ensure space is available      */
/****************************************************/
_MM_INLINE_COMMAND static void __cdecl _mm_store_ps(float *v, __m128 a)
{
  _mminternal_assert_16B(v);
  v[0] = _MM_FP0(a);
  v[1] = _MM_FP1(a);
  v[2] = _MM_FP2(a);
  v[3] = _MM_FP3(a);
}

/****************************************************/
/*  NAME : mm_storeu_ps                              */
/*  DESCRIPTION : packed single store               */
/*  IN : m128 a : m128 to load from                 */
/*  OUT : float *v : (fp0(a),fp1(a),fp2(a),fp3(a))  */
/*  RETURN : none                                   */
/*  NOTE : 4 floats will be return. it is up to the */
/*         caller to ensure space is available      */
/****************************************************/
_MM_INLINE_COMMAND static void __cdecl _mm_storeu_ps(float *v, __m128 a)
{
  v[0] = _MM_FP0(a);
  v[1] = _MM_FP1(a);
  v[2] = _MM_FP2(a);
  v[3] = _MM_FP3(a);
}

/****************************************************/
/*  NAME : mm_storer_ps                             */
/*  DESCRIPTION : reversed packed single store      */
/*  IN : m128 a : m128 to load from                 */
/*  OUT : float *v : (fp3(a),fp2(a),fp1(a),fp0(a))  */
/*  RETURN : none                                   */
/*  NOTE : 4 floats will be return. it is up to the */
/*         caller to ensure space is available      */
/****************************************************/
_MM_INLINE_COMMAND static void __cdecl _mm_storer_ps(float *v, __m128 a)
{
  _mminternal_assert_16B(v);
  v[0] = _MM_FP3(a);
  v[1] = _MM_FP2(a);
  v[2] = _MM_FP1(a);
  v[3] = _MM_FP0(a);
}

/****************************************************/
/*  NAME : mm_prefetch                              */
/*  DESCRIPTION : prefetch                          */
/*  IN : float *a : address to prefetch             */
/*  OUT : none                                      */
/*  RETURN : none                                   */
/****************************************************/
_MM_INLINE_COMMAND static void __cdecl _mm_prefetch(char const *a, int sel)
{
  /* second param not used */
  volatile char pp = *a;
}

_MM_INLINE_COMMAND static void __cdecl _mm_sfence(void)
{
  _mminternal_abort("_mm_sfence not supported");
  return;
}

_MM_INLINE_COMMAND static unsigned int __cdecl _mm_getcsr(void)
{
  _mminternal_abort("_mm_getcsr not supported");
  return 0;
}

_MM_INLINE_COMMAND static void __cdecl _mm_setcsr(unsigned int x)
{
  _mminternal_abort("_mm_setcsr not supported");
  return;
}

#ifdef __cplusplus
}
#endif

#endif	/* _INCLUDED_MM2_FUNC */

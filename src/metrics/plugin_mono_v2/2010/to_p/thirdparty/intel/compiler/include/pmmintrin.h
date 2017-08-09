/**
***
*** Copyright (C) 1985-2011 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part except as expressly provided
*** by the accompanying LICENSE AGREEMENT
***
***
**/
/*
 * pmmintrin.h
 *
 * Principal header file for Intel(R) Pentium(R) 4 processor SSE3 intrinsics
 */

#ifndef _INCLUDED_PMM
#define _INCLUDED_PMM

#include <emmintrin.h>

/*****************************************************/
/*     MACROS FOR USE WITH INTRINSICS                */
/*****************************************************/

/*
 * MACRO functions for setting and reading the DAZ bit in the MXCSR
 */
#define _MM_DENORMALS_ZERO_MASK   0x0040
#define _MM_DENORMALS_ZERO_ON     0x0040
#define _MM_DENORMALS_ZERO_OFF    0x0000

#define _MM_SET_DENORMALS_ZERO_MODE(mode)                                   \
            _mm_setcsr((_mm_getcsr() & ~_MM_DENORMALS_ZERO_MASK) | (mode))
#define _MM_GET_DENORMALS_ZERO_MODE()                                       \
            (_mm_getcsr() & _MM_DENORMALS_ZERO_MASK)


/*****************************************************/
/*     INTRINSICS FUNCTION PROTOTYPES START HERE     */
/*****************************************************/

#if defined __cplusplus
extern "C" { /* Begin "C" */
  /* Intrinsics use C name-mangling. */
#endif /* __cplusplus */

/*
 * New Single precision vector instructions.
 */

extern __m128 __ICL_INTRINCC _mm_addsub_ps(__m128, __m128);
extern __m128 __ICL_INTRINCC _mm_hadd_ps(__m128, __m128);
extern __m128 __ICL_INTRINCC _mm_hsub_ps(__m128, __m128);
extern __m128 __ICL_INTRINCC _mm_movehdup_ps(__m128);
extern __m128 __ICL_INTRINCC _mm_moveldup_ps(__m128);

/*
 * New double precision vector instructions.
 */

extern __m128d __ICL_INTRINCC _mm_addsub_pd(__m128d, __m128d);
extern __m128d __ICL_INTRINCC _mm_hadd_pd(__m128d, __m128d);
extern __m128d __ICL_INTRINCC _mm_hsub_pd(__m128d, __m128d);
extern __m128d __ICL_INTRINCC _mm_loaddup_pd(double const *);
extern __m128d __ICL_INTRINCC _mm_movedup_pd(__m128d);

/*
 * New unaligned integer vector load instruction.
 */
extern __m128i __ICL_INTRINCC _mm_lddqu_si128(__m128i const *);

/*
 * Miscellaneous new instructions.
 */
/*
 * For _mm_monitor p goes in eax, extensions goes in ecx, hints goes in edx.
 */
extern void __ICL_INTRINCC _mm_monitor(void const *, unsigned, unsigned);
/*
 * For _mm_mwait, extensions goes in ecx, hints goes in eax.
 */
extern void __ICL_INTRINCC _mm_mwait(unsigned, unsigned);

#if defined __cplusplus
}; /* End "C" */
#endif /* __cplusplus */
#endif /* _INCLUDED_PMM */

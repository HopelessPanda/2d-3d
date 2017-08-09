/**
*** Copyright (C) 1985-2011 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
***
**/
#ifndef _TMMINTRIN_H
#define _TMMINTRIN_H

#include <pmmintrin.h>  // PSC, and by inclusion X86, instrinsics

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Add horizonally packed [saturated] words, double words,
 * {X,}MM2/m{128,64} (b) to {X,}MM1 (a).
 */
extern __m128i __ICL_INTRINCC _mm_hadd_epi16 (__m128i, __m128i);
extern __m128i __ICL_INTRINCC _mm_hadd_epi32 (__m128i, __m128i);
extern __m128i __ICL_INTRINCC _mm_hadds_epi16 (__m128i, __m128i);

extern __m64 __ICL_INTRINCC _mm_hadd_pi16 (__m64, __m64);
extern __m64 __ICL_INTRINCC _mm_hadd_pi32 (__m64, __m64);
extern __m64 __ICL_INTRINCC _mm_hadds_pi16 (__m64, __m64);

/*
 * Subtract horizonally packed [saturated] words, double words,
 * {X,}MM2/m{128,64} (b) from {X,}MM1 (a).
 */
extern __m128i __ICL_INTRINCC _mm_hsub_epi16 (__m128i, __m128i);
extern __m128i __ICL_INTRINCC _mm_hsub_epi32 (__m128i, __m128i);
extern __m128i __ICL_INTRINCC _mm_hsubs_epi16 (__m128i, __m128i);

extern __m64 __ICL_INTRINCC _mm_hsub_pi16 (__m64, __m64);
extern __m64 __ICL_INTRINCC _mm_hsub_pi32 (__m64, __m64);
extern __m64 __ICL_INTRINCC _mm_hsubs_pi16 (__m64, __m64);

/*
 * Multiply and add packed words,
 * {X,}MM2/m{128,64} (b) to {X,}MM1 (a).
 */
extern __m128i __ICL_INTRINCC _mm_maddubs_epi16 (__m128i, __m128i);
extern __m64   __ICL_INTRINCC _mm_maddubs_pi16 (__m64, __m64);

/*
 * Packed multiply high integers with round and scaling,
 * {X,}MM2/m{128,64} (b) to {X,}MM1 (a).
 */
extern __m128i __ICL_INTRINCC _mm_mulhrs_epi16 (__m128i, __m128i);
extern __m64   __ICL_INTRINCC _mm_mulhrs_pi16 (__m64, __m64);

/*
 * Packed shuffle bytes
 * {X,}MM2/m{128,64} (b) by {X,}MM1 (a).
 */
extern __m128i __ICL_INTRINCC _mm_shuffle_epi8 (__m128i, __m128i);
extern __m64   __ICL_INTRINCC _mm_shuffle_pi8 (__m64, __m64);

/*
 * Packed byte, word, double word sign, {X,}MM2/m{128,64} (b) to
 * {X,}MM1 (a).
 */
extern __m128i __ICL_INTRINCC _mm_sign_epi8 (__m128i, __m128i);
extern __m128i __ICL_INTRINCC _mm_sign_epi16 (__m128i, __m128i);
extern __m128i __ICL_INTRINCC _mm_sign_epi32 (__m128i, __m128i);

extern __m64 __ICL_INTRINCC _mm_sign_pi8 (__m64, __m64);
extern __m64 __ICL_INTRINCC _mm_sign_pi16 (__m64, __m64);
extern __m64 __ICL_INTRINCC _mm_sign_pi32 (__m64, __m64);

/*
 * Packed align and shift right by n*8 bits,
 * {X,}MM2/m{128,64} (b) to {X,}MM1 (a).
 */
extern __m128i __ICL_INTRINCC _mm_alignr_epi8 (__m128i, __m128i, int);
extern __m64   __ICL_INTRINCC _mm_alignr_pi8 (__m64, __m64, int);

/*
 * Packed byte, word, double word absolute value,
 * {X,}MM2/m{128,64} (b) to {X,}MM1 (a).
 */
extern __m128i __ICL_INTRINCC _mm_abs_epi8  (__m128i);
extern __m128i __ICL_INTRINCC _mm_abs_epi16 (__m128i);
extern __m128i __ICL_INTRINCC _mm_abs_epi32 (__m128i);

extern __m64 __ICL_INTRINCC _mm_abs_pi8 (__m64);
extern __m64 __ICL_INTRINCC _mm_abs_pi16 (__m64);
extern __m64 __ICL_INTRINCC _mm_abs_pi32 (__m64);

#ifdef __cplusplus
};
#endif

#endif // #ifndef _TMMINTRIN_H

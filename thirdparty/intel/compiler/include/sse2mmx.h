/**
*** Copyright (C) 1985-2007 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
**/
#ifndef _SSE2MMX
#define _SSE2MMX

/*
 * sse2mmx.h
 *
 * This file contains the definitions for emulated versions of the 128-bit SIMD
 * integer Streaming SIMD Extensions 2 intrinsics.  The emulated code utilizes
 * Streaming SIMD Extensions.
 */

#include <xmmintrin.h>

#ifdef _SSE2_INTEGER_FOR_ITANIUM
#include <ia64intrin.h>
#endif

/*
 * Define __int64 type for __lp64__
 */
#if defined(__LP64__)
#ifndef __int64
#define _SSE2MMX_H_DEFINED__INT64
#define __int64 long
#endif
#endif

/* Define the emulated __m128i type */
typedef __declspec(align(16)) struct {
    __m64 v64[2];
} ___m128i;

#define __m128i ___m128i


#ifdef __cplusplus
extern "C" {
#endif    


/*
 * Define processor specific routine implementations
 */
#ifdef _SSE2_INTEGER_FOR_ITANIUM  /* Itanium(R) processor */


__inline __m64
_mm_add_si64(__m64 a, __m64 b)
{
    return( _m_from_int64((_m_to_int64(a) + _m_to_int64(b))) );
}


__inline __m64
_mm_sub_si64(__m64 a, __m64 b)
{
    return( _m_from_int64((_m_to_int64(a) - _m_to_int64(b))) );
}


__inline void
_mm_clflush(void const *p)
{
	__fc((__int64)p);
}


__inline void 
_mm_mfence(void)
{
	__mf();
}


#else /* Pentium(R)III processor */


__inline __m64
_mm_add_si64(__m64 a, __m64 b)
{
    __m64 x, carry;

    x = _mm_add_pi32(a, b);
    /*
     * Compute carry in bit 31
     *
     * carry = [(a31 | b31) & ~x31] | [a31 & b31]
     */
    carry = _mm_or_si64(_mm_and_si64(a, b),
                        _mm_andnot_si64(x, _mm_or_si64(a, b)));

    /* Create a bit mask of the carry bit in the upper 32 bits */
    carry = _mm_cmpgt_pi32(_mm_setzero_si64(),
                           _mm_slli_si64(carry, 32));

    /* Subtract to conditionally add 1 to the upper 32 bits based on carry */
    x = _mm_sub_pi32(x, carry);

    return x;
}


__inline __m64
_mm_sub_si64 (__m64 a, __m64 b)
{
    __m64 x, borrow;

    x = _mm_sub_pi32(a, b);
    /*
     * Compute borrow in bit 31
     *
     * borrow = [x31 * !(a31 * !b31)] + [b31 * !a31]
     */
    borrow = _mm_or_si64(_mm_andnot_si64(a, b),
                         _mm_andnot_si64(_mm_andnot_si64(b, a), x));

    /* Create a bit mask of the borrow bit in the upper 32 bits */
    borrow = _mm_cmpgt_pi32(_mm_setzero_si64(),
                            _mm_slli_si64(borrow, 32));

    /* Add to conditionally subtract 1 to the upper 32 bits based on borrow. */
    x = _mm_add_pi32(x, borrow);
        
    return x;
}

/********  Non-emulatable Operations for Streaming SIMD Extensions 2  ********/
/********  Flagged as ERROR for Pentium(R)III                         ********/
/*
 * __inline void _mm_stream_si32(int *p, int a)
 * __inline void _mm_clflush(void const *p)
 * __inline void _mm_lfence(void)
 * __inline void _mm_mfence(void)
 */
#endif  /* Pentium(R)III */


/*
 * Define routines that have the same implementation on Pentium(R)III
 * processors and Itanium(R) processors.
 */

/******  Integer Arithmetic Operations for Streaming SIMD Extensions 2  ******/
__inline __m128i
_mm_add_epi8(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_add_pi8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_add_pi8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_add_epi16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_add_pi16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_add_pi16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_add_epi32(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_add_pi32(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_add_pi32(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_add_epi64(__m128i a, __m128i b)
{
    __m128i x;
    x.v64[0] = _mm_add_si64(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_add_si64(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_adds_epi8(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_adds_pi8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_adds_pi8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_adds_epi16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_adds_pi16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_adds_pi16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_adds_epu8(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_adds_pu8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_adds_pu8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_adds_epu16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_adds_pu16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_adds_pu16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_avg_epu8(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_avg_pu8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_avg_pu8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_avg_epu16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_avg_pu16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_avg_pu16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_madd_epi16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_madd_pi16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_madd_pi16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_max_epi16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_max_pi16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_max_pi16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_max_epu8(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_max_pu8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_max_pu8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_min_epi16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_min_pi16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_min_pi16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_min_epu8(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_min_pu8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_min_pu8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_mulhi_epi16(__m128i a, __m128i b)
{
    __m128i x;
    x.v64[0] = _mm_mulhi_pi16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_mulhi_pi16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_mulhi_epu16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_mulhi_pu16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_mulhi_pu16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_mullo_epi16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_mullo_pi16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_mullo_pi16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m64
_mm_mul_su32(__m64 a, __m64 b)
{
    __m64 x;
    unsigned int ai, bi;
    unsigned __int64 r;

    ai = *((unsigned int*)&a);
    bi = *((unsigned int*)&b);
    
    r = (unsigned __int64)ai * (unsigned __int64)bi;
    x = *(__m64*)&r;

    return x;
}


__inline __m128i
_mm_mul_epu32(__m128i a, __m128i b)
{
    __m128i x;
    x.v64[0] = _mm_mul_su32(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_mul_su32(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_sad_epu8(__m128i a, __m128i b)
{
    __m128i x;
    x.v64[0] = _mm_sad_pu8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_sad_pu8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_sub_epi8(__m128i a, __m128i b)
{
    __m128i x;
    x.v64[0] = _mm_sub_pi8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_sub_pi8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_sub_epi16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_sub_pi16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_sub_pi16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_sub_epi32(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_sub_pi32(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_sub_pi32(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_sub_epi64(__m128i a, __m128i b)
{
    __m128i x;
    x.v64[0] = _mm_sub_si64(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_sub_si64(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_subs_epi8(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_subs_pi8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_subs_pi8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_subs_epi16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_subs_pi16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_subs_pi16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_subs_epu8(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_subs_pu8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_subs_pu8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_subs_epu16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_subs_pu16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_subs_pu16(a.v64[1], b.v64[1]);
    return x;
}


/*******  Integer Logical Operations for Streaming SIMD Extensions 2  ********/
__inline __m128i
_mm_and_si128(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_and_si64(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_and_si64(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_andnot_si128(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_andnot_si64(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_andnot_si64(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_or_si128(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_or_si64(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_or_si64(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_xor_si128(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_xor_si64(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_xor_si64(a.v64[1], b.v64[1]);
    return x;
}


/********  Integer Shift Operations for Streaming SIMD Extensions 2  *********/
__inline __m128i
_mm_slli_si128(__m128i a, int imm)
{
    __m128i x;
    if (imm > 7) {
        /* 8 to 15.  Also works correctly when imm is greater than 15 */
        x.v64[0] = _mm_setzero_si64();
        x.v64[1] = _mm_slli_si64(a.v64[0], (imm - 8) * 8);
    }
    else {
        /* 0 to 7 */
        x.v64[0] = _mm_slli_si64(a.v64[0], imm * 8);
        x.v64[1] = _mm_or_si64(_mm_slli_si64(a.v64[1], imm * 8),
                               _mm_srli_si64(a.v64[0], (8 - imm) * 8));
    }
    return x;
}


__inline __m128i
_mm_slli_epi16(__m128i a, int count) 
{
    __m128i x;
    x.v64[0] = _mm_slli_pi16(a.v64[0], count);
    x.v64[1] = _mm_slli_pi16(a.v64[1], count);
    return x;
}


__inline __m128i
_mm_sll_epi16(__m128i a, __m128i count)
{
    __m128i x;
    x.v64[0] = _mm_sll_pi16(a.v64[0], count.v64[0]);
    x.v64[1] = _mm_sll_pi16(a.v64[1], count.v64[0]);
    return x;
}


__inline __m128i
_mm_slli_epi32(__m128i a, int count) 
{
    __m128i x;
    x.v64[0] = _mm_slli_pi32(a.v64[0], count);
    x.v64[1] = _mm_slli_pi32(a.v64[1], count);
    return x;
}


__inline __m128i
_mm_sll_epi32(__m128i a, __m128i count)
{
    __m128i x;
    x.v64[0] = _mm_sll_pi32(a.v64[0], count.v64[0]);
    x.v64[1] = _mm_sll_pi32(a.v64[1], count.v64[0]);
    return x;
}


__inline __m128i
_mm_slli_epi64(__m128i a, int count)
{
    __m128i x;
    x.v64[0] = _mm_slli_si64(a.v64[0], count);
    x.v64[1] = _mm_slli_si64(a.v64[1], count);
    return x;
}


__inline __m128i
_mm_sll_epi64(__m128i a, __m128i count)
{
    __m128i x;
    x.v64[0] = _mm_sll_si64(a.v64[0], count.v64[0]);
    x.v64[1] = _mm_sll_si64(a.v64[1], count.v64[0]);
    return x;
}


__inline __m128i
_mm_srai_epi16(__m128i a, int count) 
{
    __m128i x;
    x.v64[0] = _mm_srai_pi16(a.v64[0], count);
    x.v64[1] = _mm_srai_pi16(a.v64[1], count);
    return x;
}


__inline __m128i
_mm_sra_epi16(__m128i a, __m128i count)
{
    __m128i x;
    x.v64[0] = _mm_sra_pi16(a.v64[0], count.v64[0]);
    x.v64[1] = _mm_sra_pi16(a.v64[1], count.v64[0]);
    return x;
}


__inline __m128i
_mm_srai_epi32(__m128i a, int count) 
{
    __m128i x;
    x.v64[0] = _mm_srai_pi32(a.v64[0], count);
    x.v64[1] = _mm_srai_pi32(a.v64[1], count);
    return x;
}


__inline __m128i
_mm_sra_epi32(__m128i a, __m128i count)
{
    __m128i x;
    x.v64[0] = _mm_sra_pi32(a.v64[0], count.v64[0]);
    x.v64[1] = _mm_sra_pi32(a.v64[1], count.v64[0]);
    return x;
}


__inline __m128i
_mm_srli_si128(__m128i a, int imm)
{
    __m128i x;
    if (imm > 7) {
        /* 8 to 15.  Also works correctly when imm is greater than 15 */
        x.v64[0] = _mm_srli_si64(a.v64[1], (imm - 8) * 8);
        x.v64[1] = _mm_setzero_si64();
    }
    else {
        /* 0 to 7 */
        x.v64[0] = _mm_or_si64(_mm_srli_si64(a.v64[0], imm * 8),
                               _mm_slli_si64(a.v64[1], (8 - imm) * 8));
        x.v64[1] = _mm_srli_si64(a.v64[1], imm * 8);
    }
    return x;
}


__inline __m128i
_mm_srli_epi16(__m128i a, int count) 
{
    __m128i x;
    x.v64[0] = _mm_srli_pi16(a.v64[0], count);
    x.v64[1] = _mm_srli_pi16(a.v64[1], count);
    return x;
}


__inline __m128i
_mm_srl_epi16(__m128i a, __m128i count)
{
    __m128i x;
    x.v64[0] = _mm_srl_pi16(a.v64[0], count.v64[0]);
    x.v64[1] = _mm_srl_pi16(a.v64[1], count.v64[0]);
    return x;
}


__inline __m128i
_mm_srli_epi32(__m128i a, int count) 
{
    __m128i x;
    x.v64[0] = _mm_srli_pi32(a.v64[0], count);
    x.v64[1] = _mm_srli_pi32(a.v64[1], count);
    return x;
}


__inline __m128i
_mm_srl_epi32(__m128i a, __m128i count)
{
    __m128i x;
    x.v64[0] = _mm_srl_pi32(a.v64[0], count.v64[0]);
    x.v64[1] = _mm_srl_pi32(a.v64[1], count.v64[0]);
    return x;
}


__inline __m128i
_mm_srli_epi64(__m128i a, int count)
{
    __m128i x;
    x.v64[0] = _mm_srli_si64(a.v64[0], count);
    x.v64[1] = _mm_srli_si64(a.v64[1], count);
    return x;
}


__inline __m128i
_mm_srl_epi64(__m128i a, __m128i count)
{
    __m128i x;
    x.v64[0] = _mm_srl_si64(a.v64[0], count.v64[0]);
    x.v64[1] = _mm_srl_si64(a.v64[1], count.v64[0]);
    return x;
}


/******  Integer Comparison Operations for Streaming SIMD Extensions 2  ******/
__inline __m128i
_mm_cmpeq_epi8(__m128i a, __m128i b)
{
    __m128i x;
    x.v64[0] = _mm_cmpeq_pi8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_cmpeq_pi8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_cmpeq_epi16(__m128i a, __m128i b)
{
    __m128i x;
    x.v64[0] = _mm_cmpeq_pi16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_cmpeq_pi16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_cmpeq_epi32(__m128i a, __m128i b)
{
    __m128i x;
    x.v64[0] = _mm_cmpeq_pi32(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_cmpeq_pi32(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_cmpgt_epi8(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_cmpgt_pi8(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_cmpgt_pi8(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_cmpgt_epi16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_cmpgt_pi16(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_cmpgt_pi16(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_cmpgt_epi32(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_cmpgt_pi32(a.v64[0], b.v64[0]);
    x.v64[1] = _mm_cmpgt_pi32(a.v64[1], b.v64[1]);
    return x;
}


__inline __m128i
_mm_cmplt_epi8(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_cmpgt_pi8(b.v64[0], a.v64[0]);
    x.v64[1] = _mm_cmpgt_pi8(b.v64[1], a.v64[1]);
    return x;
}


__inline __m128i
_mm_cmplt_epi16(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_cmpgt_pi16(b.v64[0], a.v64[0]);
    x.v64[1] = _mm_cmpgt_pi16(b.v64[1], a.v64[1]);
    return x;
}


__inline __m128i
_mm_cmplt_epi32(__m128i a, __m128i b) 
{
    __m128i x;
    x.v64[0] = _mm_cmpgt_pi32(b.v64[0], a.v64[0]);
    x.v64[1] = _mm_cmpgt_pi32(b.v64[1], a.v64[1]);
    return x;
}


/******  Integer Conversion Operations for Streaming SIMD Extensions 2  ******/
__inline __m128i
_mm_cvtsi32_si128(int a)
{
    __m128i x;
    x.v64[0] = _mm_cvtsi32_si64(a);
    x.v64[1] = _mm_setzero_si64();
    return x;
}

#if defined(__x86_64) || defined(_M_X64)
__inline __m128i
_mm_cvtsi64_si128(__int64 a)
{
    __m128i x;
    x.v64[0] = _m_from_int64(a);
    x.v64[1] = _mm_setzero_si64();
    return x;
}
#endif

__inline int
_mm_cvtsi128_si32(__m128i a)
{
    return (_mm_cvtsi64_si32(a.v64[0]));
}

#if defined(__x86_64) || defined(_M_X64)
__inline __int64
_mm_cvtsi128_si64(__m128i a)
{
    return (_m_to_int64(a.v64[0]));
}
#endif

__inline __m128
_mm_cvtepi32_ps(__m128i a)
{
    __m128 x;
    x = _mm_movelh_ps(_mm_cvtpi32_ps(x, a.v64[0]),
                      _mm_cvtpi32_ps(x, a.v64[1]));
    return x;
}


__inline __m128i
_mm_cvtps_epi32(__m128 a)
{
    __m128i x;
    x.v64[0] = _mm_cvtps_pi32(a);
    x.v64[1] = _mm_cvtps_pi32(_mm_movehl_ps(a, a));
    return x;
}


__inline __m128i
_mm_cvttps_epi32(__m128 a)
{
    __m128i x;
    x.v64[0] = _mm_cvttps_pi32(a);
    x.v64[1] = _mm_cvttps_pi32(_mm_movehl_ps(a, a));
    return x;
}


/*****  Cacheability Support Operations for Streaming SIMD Extensions 2  *****/
__inline void
_mm_stream_si128(__m128i *p, __m128i a)
{
    _mm_stream_pi((__m64*)&p->v64[0], a.v64[0]);
    _mm_stream_pi((__m64*)&p->v64[1], a.v64[1]);
}


/********  Non-emulatable Operations for Streaming SIMD Extensions 2  ********/
/**** __inline void
***** _mm_stream_si32(int *p, int a)
*****
***** __inline void
***** _mm_clflush(void const *p)
*****
***** __inline void
***** _mm_lfence(void)
*****
***** __inline void
***** _mm_mfence(void)
****/


/**  Memory and Initialization Operations for Streaming SIMD Extensions 2  ***/
__inline __m128i
_mm_load_si128(__m128i *p)
{
    __m128i x;
    x.v64[0] = p->v64[0];
    x.v64[1] = p->v64[1];
    return x;
}


__inline __m128i
_mm_loadu_si128(__m128i *p)
{
    __m128i x;
    x.v64[0] = p->v64[0];
    x.v64[1] = p->v64[1];
    return x;
}


__inline __m128i
_mm_loadl_epi64(__m128i const *p)
{
    __m128i x;
    x.v64[0] = p->v64[0];
    x.v64[1] = _mm_setzero_si64();
    return x;
}


__inline __m128i
_mm_set_epi64(__m64 q1, __m64 q0) 
{
    __m128i x;
    x.v64[0] = q0;
    x.v64[1] = q1;
    return x;
}


__inline __m128i
_mm_set_epi32(int i3, int i2, int i1, int i0)
{
    __m128i x;
    x.v64[0] = _mm_set_pi32(i1, i0);
    x.v64[1] = _mm_set_pi32(i3, i2);
    return x;
}


__inline __m128i
_mm_set_epi16(short w7, short w6, short w5, short w4, 
              short w3, short w2, short w1, short w0)
{
    __m128i x;
    x.v64[0] = _mm_set_pi16(w3, w2, w1, w0);
    x.v64[1] = _mm_set_pi16(w7, w6, w5, w4);
    return x;
}


__inline __m128i
_mm_set_epi8(char b15, char b14, char b13, char b12,
             char b11, char b10, char b9,  char b8,
             char b7,  char b6,  char b5,  char b4,
             char b3,  char b2,  char b1,  char b0)
{
    __m128i x;
    x.v64[0] = _mm_set_pi8(b7, b6, b5, b4, b3, b2, b1, b0);
    x.v64[1] = _mm_set_pi8(b15, b14, b13, b12, b11, b10, b9, b8);
    return x;
}


__inline __m128i
_mm_set1_epi64(__m64 q)
{
    __m128i x;
    x.v64[0] = q;
    x.v64[1] = q;
    return x;
}


__inline __m128i
_mm_set1_epi32(int i)
{
    __m128i x;
    x.v64[0] = _mm_set1_pi32(i);
    x.v64[1] = _mm_set1_pi32(i);
    return x;
}


__inline __m128i
_mm_set1_epi16(short s)
{
    __m128i x;
    x.v64[0] = _mm_set1_pi16(s);
    x.v64[1] = _mm_set1_pi16(s);
    return x;
}


__inline __m128i
_mm_set1_epi8(char b)
{
    __m128i x;
    x.v64[0] = _mm_set1_pi8(b);
    x.v64[1] = _mm_set1_pi8(b);
    return x;
}


__inline __m128i
_mm_setr_epi64(__m64 q0, __m64 q1)
{
    __m128i x;
    x.v64[0] = q0;
    x.v64[1] = q1;
    return x;
}


__inline __m128i
_mm_setr_epi32(int i0, int i1, int i2, int i3)
{
    __m128i x;
    x.v64[0] = _mm_set_pi32(i1, i0);
    x.v64[1] = _mm_set_pi32(i3, i2);
    return x;
}


__inline __m128i
_mm_setr_epi16(short w0, short w1, short w2, short w3, 
               short w4, short w5, short w6, short w7)
{
    __m128i x;
    x.v64[0] = _mm_set_pi16(w3, w2, w1, w0);
    x.v64[1] = _mm_set_pi16(w7, w6, w5, w4);
    return x;
}


__inline __m128i
_mm_setr_epi8(char b0,  char b1,  char b2,  char b3,
              char b4,  char b5,  char b6,  char b7,
              char b8,  char b9,  char b10, char b11,
              char b12, char b13, char b14, char b15)
{
    __m128i x;
    x.v64[0] = _mm_set_pi8(b7, b6, b5, b4, b3, b2, b1, b0);
    x.v64[1] = _mm_set_pi8(b15, b14, b13, b12, b11, b10, b9, b8);
    return x;
}


__inline __m128i
_mm_setzero_si128(void) 
{
    __m128i x;
    x.v64[0] = _mm_setzero_si64();
    x.v64[1] = _mm_setzero_si64();
    return x;
}


__inline void
_mm_store_si128(__m128i *p, __m128i a)
{
    p->v64[0] = a.v64[0];
    p->v64[1] = a.v64[1];
}


__inline void
_mm_storeu_si128(__m128i *p, __m128i a)
{
    p->v64[0] = a.v64[0];
    p->v64[1] = a.v64[1];
}


__inline void
_mm_maskmoveu_si128(__m128i d, __m128i n, char *p)
{
    _mm_maskmove_si64(d.v64[0], n.v64[0], p);
    _mm_maskmove_si64(d.v64[1], n.v64[1], p+8);
}


__inline void
_mm_storel_epi64(__m128i *p, __m128i a)
{
    p->v64[0] = a.v64[0];
}


/****  Miscellaneous Integer Operations for Streaming SIMD Extensions 2  *****/
__inline __m128i
_mm_packs_epi16(__m128i a, __m128i b)
{
    /* Desired result: b7|b6|b5|b4|b3|b2|b1|b0|a7|a6|a5|a4|a3|a2|a1|a0 */
    /* a.v64[0] = a3|a2|a1|a0 */
    /* a.v64[1] = a7|a6|a5|a4 */
    /* b.v64[0] = b3|b2|b1|b0 */
    /* b.v64[1] = b7|b6|b5|b4 */
    __m128i x;
    x.v64[0] = _mm_packs_pi16(a.v64[0], a.v64[1]);
    /* x.v64[0] = a7|a6|a5|a4|a3|a2|a1|a0 */
    x.v64[1] = _mm_packs_pi16(b.v64[0], b.v64[1]);
    /* x.v64[1] = b7|b6|b5|b4|b3|b2|b1|b0 */
    return x;
}


__inline __m128i
_mm_packs_epi32(__m128i a, __m128i b) 
{
    /* Desired result: b3|b2|b1|b0|a3|a2|a1|a0 */
    /* a.v64[0] = a1|a0 */
    /* a.v64[1] = a3|a2 */
    /* b.v64[0] = b1|b0 */
    /* b.v64[1] = b3|b2 */
    __m128i x;
    x.v64[0] = _mm_packs_pi32(a.v64[0], a.v64[1]);
    /* x.v64[0] = a3|a2|a1|a0 */
    x.v64[1] = _mm_packs_pi32(b.v64[0], b.v64[1]);
    /* x.v64[1] = b3|b2|b1|b0 */
    return x;
}


__inline __m128i
_mm_packus_epi16(__m128i a, __m128i b) 
{
    /* Desired result: b7|b6|b5|b4|b3|b2|b1|b0|a7|a6|a5|a4|a3|a2|a1|a0 */
    /* a.v64[0] = a3|a2|a1|a0 */
    /* a.v64[1] = a7|a6|a5|a4 */
    /* b.v64[0] = b3|b2|b1|b0 */
    /* b.v64[1] = b7|b6|b5|b4 */
    __m128i x;
    x.v64[0] = _mm_packs_pu16(a.v64[0], a.v64[1]);
    /* x.v64[0] = a7|a6|a5|a4|a3|a2|a1|a0 */
    x.v64[1] = _mm_packs_pu16(b.v64[0], b.v64[1]);
    /* x.v64[1] = b7|b6|b5|b4|b3|b2|b1|b0 */
    return x;
}


__inline int
_mm_extract_epi16(__m128i a, int imm)
{
    return(((unsigned short*)&a)[imm]);
}


__inline __m128i
_mm_insert_epi16(__m128i a, int b, int imm)
{
    ((short*)&a)[imm] = (short)b;
    return a;
}


__inline int
_mm_movemask_epi8(__m128i a)
{
    return (_mm_movemask_pi8(a.v64[1]) << 8 | _mm_movemask_pi8(a.v64[0]));
}


__inline __m128i
_mm_shuffle_epi32(__m128i a, int imm)
{
    __m128i x;
    ((int*)&x)[0] = ((int*)&a)[imm & 0x03];        /* 00 00 00 11 */
    ((int*)&x)[1] = ((int*)&a)[(imm & 0x0C) >> 2]; /* 00 00 11 00 */
    ((int*)&x)[2] = ((int*)&a)[(imm & 0x30) >> 4]; /* 00 11 00 00 */
    ((int*)&x)[3] = ((int*)&a)[(imm & 0xC0) >> 6]; /* 11 00 00 00 */
    return x;
}


__inline __m128i
_mm_shufflehi_epi16(__m128i a, int imm)
{
    __m128i x;
    x.v64[0] = a.v64[0];
    ((short*)&x)[4] = ((short*)&a)[(imm & 0x03) + 4];        /* 00 00 00 11 */
    ((short*)&x)[5] = ((short*)&a)[((imm & 0x0C) >> 2) + 4]; /* 00 00 11 00 */
    ((short*)&x)[6] = ((short*)&a)[((imm & 0x30) >> 4) + 4]; /* 00 11 00 00 */
    ((short*)&x)[7] = ((short*)&a)[((imm & 0xC0) >> 6) + 4]; /* 11 00 00 00 */
    return x;
}


__inline __m128i
_mm_shufflelo_epi16( __m128i a, int imm)
{
    __m128i x;
    x.v64[1] = a.v64[1];
    ((short*)&x)[0] = ((short*)&a)[imm & 0x03];        /* 00 00 00 11 */
    ((short*)&x)[1] = ((short*)&a)[(imm & 0x0C) >> 2]; /* 00 00 11 00 */
    ((short*)&x)[2] = ((short*)&a)[(imm & 0x30) >> 4]; /* 00 11 00 00 */
    ((short*)&x)[3] = ((short*)&a)[(imm & 0xC0) >> 6]; /* 11 00 00 00 */
    return x;
}


__inline __m128i
_mm_unpackhi_epi8(__m128i a, __m128i b) 
{
    /*
     * Desired result:
     *   b15|a15|b14|a14|b13|a13|b12|a12|b11|a11|b10|a10|b9|a9|b8|a8
     */
    /* a.v64[0] = a7|a6|a5|a4|a3|a2|a1|a0       */
    /* a.v64[1] = a15|a14|a13|a12|a11|a10|a9|a8 */
    /* b.v64[0] = b7|b6|b5|b4|b3|b2|b1|b0       */
    /* b.v64[1] = b15|b14|b13|b12|b11|b10|b9|b8 */
    __m128i x;
    x.v64[0] = _mm_unpacklo_pi8(a.v64[1], b.v64[1]);
    /* x.v64[0] = b11|a11|b10|a10|b9|a9|b8|a8 */
    x.v64[1] = _mm_unpackhi_pi8(a.v64[1], b.v64[1]);
    /* x.v64[1] = b15|a15|b14|a14|b13|a13|b12|a12 */
    return x;
}


__inline __m128i
_mm_unpackhi_epi16(__m128i a, __m128i b) 
{
    /* Desired result: b7|a7|b6|a6|b5|a5|b4|a4 */
    /* a.v64[0] = a3|a2|a1|a0 */
    /* a.v64[1] = a7|a6|a5|a4 */
    /* b.v64[0] = b3|b2|b1|b0 */
    /* b.v64[1] = b7|b6|b5|b4 */
    __m128i x;
    x.v64[0] = _mm_unpacklo_pi16(a.v64[1], b.v64[1]);
    /* x.v64[0] = b5|a5|b4|a4 */
    x.v64[1] = _mm_unpackhi_pi16(a.v64[1], b.v64[1]);
    /* x.v64[1] = b7|a7|b6|a6 */
    return x;
}


__inline __m128i
_mm_unpackhi_epi32(__m128i a, __m128i b) 
{
    /* Desired result: b3|a3|b2|a2 */
    /* a.v64[0] = a1|a0 */
    /* a.v64[1] = a3|a2 */
    /* b.v64[0] = b1|b0 */
    /* b.v64[1] = b3|b2 */
    __m128i x;
    x.v64[0] = _mm_unpacklo_pi32(a.v64[1], b.v64[1]);
    /* x.v64[0] = b2|a2 */
    x.v64[1] = _mm_unpackhi_pi32(a.v64[1], b.v64[1]);
    /* x.v64[1] = b3|a3 */
    return x;
}


__inline __m128i
_mm_unpackhi_epi64(__m128i a, __m128i b) 
{
    /* Desired result: b1|a1 */
    /* a.v64[0] = a0 */
    /* a.v64[1] = a1 */
    /* b.v64[0] = b0 */
    /* b.v64[1] = b1 */
    __m128i x;
    x.v64[0] = a.v64[1]; /* x.v64[0] = a1 */
    x.v64[1] = b.v64[1]; /* x.v64[1] = b1 */
    return x;
}


__inline __m128i
_mm_unpacklo_epi8(__m128i a, __m128i b) 
{
    /* Desired result: b7|a7|b6|a6|b5|a5|b4|a4|b3|a3|b2|a2|b1|a1|b0|a0 */
    /* a.v64[0] = a7|a6|a5|a4|a3|a2|a1|a0       */
    /* a.v64[1] = a15|a14|a13|a12|a11|a10|a9|a8 */
    /* b.v64[0] = b7|b6|b5|b4|b3|b2|b1|b0       */
    /* b.v64[1] = b15|b14|b13|b12|b11|b10|b9|b8 */
    __m128i x;
    x.v64[0] = _mm_unpacklo_pi8(a.v64[0], b.v64[0]);
    /* x.v64[0] = b3|a3|b2|a2|b1|a1|b0|a0 */
    x.v64[1] = _mm_unpackhi_pi8(a.v64[0], b.v64[0]);
    /* x.v64[1] = b7|a7|b6|a6|b5|a5|b4|a4 */
    return x;
}


__inline __m128i
_mm_unpacklo_epi16(__m128i a, __m128i b) 
{
    /* Desired result: b3|a3|b2|a2|b1|a1|b0|a0 */
    /* a.v64[0] = a3|a2|a1|a0 */
    /* a.v64[1] = a7|a6|a5|a4 */
    /* b.v64[0] = b3|b2|b1|b0 */
    /* b.v64[1] = b7|b6|b5|b4 */
    __m128i x;
    x.v64[0] = _mm_unpacklo_pi16(a.v64[0], b.v64[0]);
    /* x.v64[0] = b1|a1|b0|a0 */
    x.v64[1] = _mm_unpackhi_pi16(a.v64[0], b.v64[0]);
    /* x.v64[1] = b3|a3|b2|a2 */
    return x;
}


__inline __m128i
_mm_unpacklo_epi32(__m128i a, __m128i b) 
{
    /* Desired result: b1|a1|b0|a0 */
    /* a.v64[0] = a1|a0 */
    /* a.v64[1] = a3|a2 */
    /* b.v64[0] = b1|b0 */
    /* b.v64[1] = b3|b2 */
    __m128i x;
    x.v64[0] = _mm_unpacklo_pi32(a.v64[0], b.v64[0]);
    /* x.v64[0] = b0|a0 */
    x.v64[1] = _mm_unpackhi_pi32(a.v64[0], b.v64[0]);
    /* x.v64[1] = b1|a1 */
    return x;
}


__inline __m128i
_mm_unpacklo_epi64(__m128i a, __m128i b) 
{
    /* Desired result: b0|a0 */
    /* a.v64[0] = a0 */
    /* a.v64[1] = a1 */
    /* b.v64[0] = b0 */
    /* b.v64[1] = b1 */
    __m128i x;
    x.v64[0] = a.v64[0]; /* x.v64[0] = a0 */
    x.v64[1] = b.v64[0]; /* x.v64[1] = b0 */
    return x;
}


__inline __m64
_mm_movepi64_pi64(__m128i a)
{
    return a.v64[0];
}


__inline __m128i
_mm_movpi64_epi64(__m64 a)
{
    __m128i x;
    x.v64[0] = a;
    x.v64[1] = _mm_setzero_si64();
    return x;
}


__inline __m128i
_mm_move_epi64(__m128i a)
{
    __m128i x;
    x.v64[0] = a.v64[0];
    x.v64[1] = _mm_setzero_si64();
    return x;
}


#ifdef __cplusplus
}
#endif

/*
 * Undefine __int64 types for __lp64__
 */
#if defined(__LP64__)
#ifdef _SSE2MMX_H_DEFINED__INT64
#undef __int64
#endif
#endif

#endif /* _SSE2MMX */

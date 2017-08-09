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
 * nmmintrin.h
 *
 * Principal header file for Intel(R) Core(TM) 2 Duo processor
 * SSE4.2 intrinsics.
 */


#ifndef _INCLUDED_NMM
#define _INCLUDED_NMM

#include <smmintrin.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
 * MACRO functions for setting and reading the ULE bit in the MXCSR
 */
#define _MM_ULE_MASK   0x20000
#define _MM_ULE_ON     0x20000
#define _MM_ULE_OFF    0x00000

#define _MM_SET_ULE_MODE(mode)                                   \
            _mm_setcsr((_mm_getcsr() & ~_MM_ULE_MASK) | (mode))
#define _MM_GET_ULE_MODE()                                       \
            (_mm_getcsr() & _MM_ULE_MASK)

/*
 * These defines are used to determine the kind of units to be compared
 */
#define _SIDD_UBYTE_OPS                0x00
#define _SIDD_UWORD_OPS                0x01
#define _SIDD_SBYTE_OPS                0x02
#define _SIDD_SWORD_OPS                0x03


/*
 * These defines are used to determine the comparison operation
 */
#define _SIDD_CMP_EQUAL_ANY            0x00
#define _SIDD_CMP_RANGES               0x04
#define _SIDD_CMP_EQUAL_EACH           0x08
#define _SIDD_CMP_EQUAL_ORDERED        0x0C


/*
 * These defines are used to determine the polarity
 */
#define _SIDD_POSITIVE_POLARITY        0x00
#define _SIDD_NEGATIVE_POLARITY        0x10
#define _SIDD_MASKED_POSITIVE_POLARITY 0x20
#define _SIDD_MASKED_NEGATIVE_POLARITY 0x30


/*
 * These defines are used in _mm_cmpXstri()
 */
#define _SIDD_LEAST_SIGNIFICANT        0x00
#define _SIDD_MOST_SIGNIFICANT         0x40

/*
 * These defines are used _mm_cmpXstrm()
 */
#define _SIDD_BIT_MASK                 0x00
#define _SIDD_UNIT_MASK                0x40


/*
 * Intrinsics for text/string processing.
 */
extern __m128i __ICL_INTRINCC _mm_cmpistrm (__m128i, __m128i, const int);
extern int     __ICL_INTRINCC _mm_cmpistri (__m128i, __m128i, const int);

extern __m128i __ICL_INTRINCC _mm_cmpestrm (__m128i, int, __m128i,
                                            int, const int);
extern int     __ICL_INTRINCC _mm_cmpestri (__m128i, int, __m128i,
                                            int, const int);

/*
 * Intrinsics for text/string processing and reading values of EFlags.
 */
extern int __ICL_INTRINCC _mm_cmpistrz (__m128i, __m128i, const int);
extern int __ICL_INTRINCC _mm_cmpistrc (__m128i, __m128i, const int);
extern int __ICL_INTRINCC _mm_cmpistrs (__m128i, __m128i, const int);
extern int __ICL_INTRINCC _mm_cmpistro (__m128i, __m128i, const int);
extern int __ICL_INTRINCC _mm_cmpistra (__m128i, __m128i, const int);

extern int __ICL_INTRINCC _mm_cmpestrz (__m128i, int, __m128i, int, const int);
extern int __ICL_INTRINCC _mm_cmpestrc (__m128i, int, __m128i, int, const int);
extern int __ICL_INTRINCC _mm_cmpestrs (__m128i, int, __m128i, int, const int);
extern int __ICL_INTRINCC _mm_cmpestro (__m128i, int, __m128i, int, const int);
extern int __ICL_INTRINCC _mm_cmpestra (__m128i, int, __m128i, int, const int);

/*
 * Packed integer 64-bit comparison, zeroing or filling with ones
 * corresponding parts of result
 */
extern __m128i __ICL_INTRINCC _mm_cmpgt_epi64(__m128i, __m128i);

/*
 * Calculate a number of bits set to 1
 */
extern int __ICL_INTRINCC _mm_popcnt_u32(unsigned int);

#if defined(__x86_64) || defined(_M_X64)
extern __int64 __ICL_INTRINCC _mm_popcnt_u64(unsigned __int64);
#endif

/*
 * Accumulate CRC32 (polynomial 0x11EDC6F41) value
 */
extern unsigned int __ICL_INTRINCC _mm_crc32_u8 (unsigned int, unsigned char);
extern unsigned int __ICL_INTRINCC _mm_crc32_u16(unsigned int, unsigned short);
extern unsigned int __ICL_INTRINCC _mm_crc32_u32(unsigned int, unsigned int);

#if defined(__x86_64) || defined(_M_X64)
extern unsigned __int64 __ICL_INTRINCC _mm_crc32_u64(unsigned __int64,
                                                     unsigned __int64);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* _INCLUDED_NMM */

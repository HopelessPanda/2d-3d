// File: bfp754_functionnames.h
// =============================================================================
//
//
// INTEL CONFIDENTIAL
//
// Copyright (c) 2009 Intel Corporation. All Rights Reserved.
//
// The information and source code contained herein is the exclusive property
// of Intel Corporation and may not be disclosed, examined, or reproduced in
// whole or in part without explicit written authorization from the Company.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//
// =============================================================================
//
// Abstract: 
// 
// Macros mapping the internal function names to the external names
// provided in the IEEE 754-2008 (binary part) compliance library
//
// =============================================================================
//
// Edit history:
//
// 1-001 01-MAY-2009 CQL: Initial revision
// 1-002 08-MAY-2009 CQL: Corrected typos in internal and external
//                        function names
//
// =============================================================================

#ifndef _BFP754_FUNCTIONNAMES_H_
#define _BFP754_FUNCTIONNAMES_H_

// Macro- Definitions for mapping the internal names to external names
//
// We have ___(internal macro name) (3 underscores) and __(external
// name) (2 underscores)
//


#define ___binary32_round_integral_nearest_even              \
         __binary32_round_integral_nearest_even
#define ___binary64_round_integral_nearest_even              \
         __binary64_round_integral_nearest_even
#define ___binary32_round_integral_nearest_away              \
         __binary32_round_integral_nearest_away
#define ___binary64_round_integral_nearest_away              \
         __binary64_round_integral_nearest_away
#define ___binary32_round_integral_zero                      \
         __binary32_round_integral_zero
#define ___binary64_round_integral_zero                      \
         __binary64_round_integral_zero
#define ___binary32_round_integral_positive                  \
         __binary32_round_integral_positive
#define ___binary64_round_integral_positive                  \
         __binary64_round_integral_positive
#define ___binary32_round_integral_negative                  \
         __binary32_round_integral_negative
#define ___binary64_round_integral_negative                  \
         __binary64_round_integral_negative
#define ___binary32_round_integral_exact                     \
         __binary32_round_integral_exact
#define ___binary64_round_integral_exact                     \
         __binary64_round_integral_exact
#define ___binary32_nextup                                   \
         __binary32_nextup
#define ___binary64_nextup                                   \
         __binary64_nextup
#define ___binary32_nextdown                                 \
         __binary32_nextdown
#define ___binary64_nextdown                                 \
         __binary64_nextdown
#define ___binary32_rem                                      \
         __binary32_rem
#define ___binary64_rem                                      \
         __binary64_rem
#define ___binary32_minnum                                   \
         __binary32_minnum
#define ___binary64_minnum                                   \
         __binary64_minnum
#define ___binary32_maxnum                                   \
         __binary32_maxnum
#define ___binary64_maxnum                                   \
         __binary64_maxnum
#define ___binary32_minnum_mag                               \
         __binary32_minnum_mag
#define ___binary64_minnum_mag                               \
         __binary64_minnum_mag
#define ___binary32_maxnum_mag                               \
         __binary32_maxnum_mag
#define ___binary64_maxnum_mag                               \
         __binary64_maxnum_mag
#define ___binary32_scalbn                                   \
         __binary32_scalbn
#define ___binary64_scalbn                                   \
         __binary64_scalbn
#define ___binary32_ilogb                                    \
         __binary32_ilogb
#define ___binary64_ilogb                                    \
         __binary64_ilogb
#define ___binary32_add_binary32_binary32                    \
         __binary32_add_binary32_binary32
#define ___binary32_add_binary32_binary64                    \
         __binary32_add_binary32_binary64
#define ___binary32_add_binary64_binary32                    \
         __binary32_add_binary64_binary32
#define ___binary32_add_binary64_binary64                    \
         __binary32_add_binary64_binary64
#define ___binary64_add_binary32_binary32                    \
         __binary64_add_binary32_binary32
#define ___binary64_add_binary32_binary64                    \
         __binary64_add_binary32_binary64
#define ___binary64_add_binary64_binary32                    \
         __binary64_add_binary64_binary32
#define ___binary64_add_binary64_binary64                    \
         __binary64_add_binary64_binary64
#define ___binary32_sub_binary32_binary32                    \
         __binary32_sub_binary32_binary32
#define ___binary32_sub_binary32_binary64                    \
         __binary32_sub_binary32_binary64
#define ___binary32_sub_binary64_binary32                    \
         __binary32_sub_binary64_binary32
#define ___binary32_sub_binary64_binary64                    \
         __binary32_sub_binary64_binary64
#define ___binary64_sub_binary32_binary32                    \
         __binary64_sub_binary32_binary32
#define ___binary64_sub_binary32_binary64                    \
         __binary64_sub_binary32_binary64
#define ___binary64_sub_binary64_binary32                    \
         __binary64_sub_binary64_binary32
#define ___binary64_sub_binary64_binary64                    \
         __binary64_sub_binary64_binary64
#define ___binary32_mul_binary32_binary32                    \
         __binary32_mul_binary32_binary32
#define ___binary32_mul_binary32_binary64                    \
         __binary32_mul_binary32_binary64
#define ___binary32_mul_binary64_binary32                    \
         __binary32_mul_binary64_binary32
#define ___binary32_mul_binary64_binary64                    \
         __binary32_mul_binary64_binary64
#define ___binary64_mul_binary32_binary32                    \
         __binary64_mul_binary32_binary32
#define ___binary64_mul_binary32_binary64                    \
         __binary64_mul_binary32_binary64
#define ___binary64_mul_binary64_binary32                    \
         __binary64_mul_binary64_binary32
#define ___binary64_mul_binary64_binary64                    \
         __binary64_mul_binary64_binary64
#define ___binary32_div_binary32_binary32                    \
         __binary32_div_binary32_binary32
#define ___binary32_div_binary32_binary64                    \
         __binary32_div_binary32_binary64
#define ___binary32_div_binary64_binary32                    \
         __binary32_div_binary64_binary32
#define ___binary32_div_binary64_binary64                    \
         __binary32_div_binary64_binary64
#define ___binary64_div_binary32_binary32                    \
         __binary64_div_binary32_binary32
#define ___binary64_div_binary32_binary64                    \
         __binary64_div_binary32_binary64
#define ___binary64_div_binary64_binary32                    \
         __binary64_div_binary64_binary32
#define ___binary64_div_binary64_binary64                    \
         __binary64_div_binary64_binary64
#define ___binary32_sqrt_binary32                            \
         __binary32_sqrt_binary32
#define ___binary32_sqrt_binary64                            \
         __binary32_sqrt_binary64
#define ___binary64_sqrt_binary32                            \
         __binary64_sqrt_binary32
#define ___binary64_sqrt_binary64                            \
         __binary64_sqrt_binary64
#define ___binary32_fma_binary32_binary32_binary32           \
         __binary32_fma_binary32_binary32_binary32
#define ___binary32_fma_binary32_binary32_binary64           \
         __binary32_fma_binary32_binary32_binary64
#define ___binary32_fma_binary32_binary64_binary32           \
         __binary32_fma_binary32_binary64_binary32
#define ___binary32_fma_binary32_binary64_binary64           \
         __binary32_fma_binary32_binary64_binary64
#define ___binary32_fma_binary64_binary32_binary32           \
         __binary32_fma_binary64_binary32_binary32
#define ___binary32_fma_binary64_binary32_binary64           \
         __binary32_fma_binary64_binary32_binary64
#define ___binary32_fma_binary64_binary64_binary32           \
         __binary32_fma_binary64_binary64_binary32
#define ___binary32_fma_binary64_binary64_binary64           \
         __binary32_fma_binary64_binary64_binary64
#define ___binary64_fma_binary32_binary32_binary32           \
         __binary64_fma_binary32_binary32_binary32
#define ___binary64_fma_binary32_binary32_binary64           \
         __binary64_fma_binary32_binary32_binary64
#define ___binary64_fma_binary32_binary64_binary32           \
         __binary64_fma_binary32_binary64_binary32
#define ___binary64_fma_binary32_binary64_binary64           \
         __binary64_fma_binary32_binary64_binary64
#define ___binary64_fma_binary64_binary32_binary32           \
         __binary64_fma_binary64_binary32_binary32
#define ___binary64_fma_binary64_binary32_binary64           \
         __binary64_fma_binary64_binary32_binary64
#define ___binary64_fma_binary64_binary64_binary32           \
         __binary64_fma_binary64_binary64_binary32
#define ___binary64_fma_binary64_binary64_binary64           \
         __binary64_fma_binary64_binary64_binary64
#define ___binary32_from_int32                               \
         __binary32_from_int32
#define ___binary32_from_uint32                              \
         __binary32_from_uint32
#define ___binary32_from_int64                               \
         __binary32_from_int64
#define ___binary32_from_uint64                              \
         __binary32_from_uint64
#define ___binary64_from_int32                               \
         __binary64_from_int32
#define ___binary64_from_uint32                              \
         __binary64_from_uint32
#define ___binary64_from_int64                               \
         __binary64_from_int64
#define ___binary64_from_uint64                              \
         __binary64_from_uint64
#define ___binary32_to_int32_rnint                           \
         __binary32_to_int32_rnint
#define ___binary32_to_uint32_rnint                          \
         __binary32_to_uint32_rnint
#define ___binary32_to_int64_rnint                           \
         __binary32_to_int64_rnint
#define ___binary32_to_uint64_rnint                          \
         __binary32_to_uint64_rnint
#define ___binary64_to_int32_rnint                           \
         __binary64_to_int32_rnint
#define ___binary64_to_uint32_rnint                          \
         __binary64_to_uint32_rnint
#define ___binary64_to_int64_rnint                           \
         __binary64_to_int64_rnint
#define ___binary64_to_uint64_rnint                          \
         __binary64_to_uint64_rnint
#define ___binary32_to_int32_rninta                          \
         __binary32_to_int32_rninta
#define ___binary32_to_uint32_rninta                         \
         __binary32_to_uint32_rninta
#define ___binary32_to_int64_rninta                          \
         __binary32_to_int64_rninta
#define ___binary32_to_uint64_rninta                         \
         __binary32_to_uint64_rninta
#define ___binary64_to_int32_rninta                          \
         __binary64_to_int32_rninta
#define ___binary64_to_uint32_rninta                         \
         __binary64_to_uint32_rninta
#define ___binary64_to_int64_rninta                          \
         __binary64_to_int64_rninta
#define ___binary64_to_uint64_rninta                         \
         __binary64_to_uint64_rninta
#define ___binary32_to_int32_int                             \
         __binary32_to_int32_int
#define ___binary32_to_uint32_int                            \
         __binary32_to_uint32_int
#define ___binary32_to_int64_int                             \
         __binary32_to_int64_int
#define ___binary32_to_uint64_int                            \
         __binary32_to_uint64_int
#define ___binary64_to_int32_int                             \
         __binary64_to_int32_int
#define ___binary64_to_uint32_int                            \
         __binary64_to_uint32_int
#define ___binary64_to_int64_int                             \
         __binary64_to_int64_int
#define ___binary64_to_uint64_int                            \
         __binary64_to_uint64_int
#define ___binary32_to_int32_ceil                            \
         __binary32_to_int32_ceil
#define ___binary32_to_uint32_ceil                           \
         __binary32_to_uint32_ceil
#define ___binary32_to_int64_ceil                            \
         __binary32_to_int64_ceil
#define ___binary32_to_uint64_ceil                           \
         __binary32_to_uint64_ceil
#define ___binary64_to_int32_ceil                            \
         __binary64_to_int32_ceil
#define ___binary64_to_uint32_ceil                           \
         __binary64_to_uint32_ceil
#define ___binary64_to_int64_ceil                            \
         __binary64_to_int64_ceil
#define ___binary64_to_uint64_ceil                           \
         __binary64_to_uint64_ceil
#define ___binary32_to_int32_floor                           \
         __binary32_to_int32_floor
#define ___binary32_to_uint32_floor                          \
         __binary32_to_uint32_floor
#define ___binary32_to_int64_floor                           \
         __binary32_to_int64_floor
#define ___binary32_to_uint64_floor                          \
         __binary32_to_uint64_floor
#define ___binary64_to_int32_floor                           \
         __binary64_to_int32_floor
#define ___binary64_to_uint32_floor                          \
         __binary64_to_uint32_floor
#define ___binary64_to_int64_floor                           \
         __binary64_to_int64_floor
#define ___binary64_to_uint64_floor                          \
         __binary64_to_uint64_floor
#define ___binary32_to_int32_xrnint                          \
         __binary32_to_int32_xrnint
#define ___binary32_to_uint32_xrnint                         \
         __binary32_to_uint32_xrnint
#define ___binary32_to_int64_xrnint                          \
         __binary32_to_int64_xrnint
#define ___binary32_to_uint64_xrnint                         \
         __binary32_to_uint64_xrnint
#define ___binary64_to_int32_xrnint                          \
         __binary64_to_int32_xrnint
#define ___binary64_to_uint32_xrnint                         \
         __binary64_to_uint32_xrnint
#define ___binary64_to_int64_xrnint                          \
         __binary64_to_int64_xrnint
#define ___binary64_to_uint64_xrnint                         \
         __binary64_to_uint64_xrnint
#define ___binary32_to_int32_xrninta                         \
         __binary32_to_int32_xrninta
#define ___binary32_to_uint32_xrninta                        \
         __binary32_to_uint32_xrninta
#define ___binary32_to_int64_xrninta                         \
         __binary32_to_int64_xrninta
#define ___binary32_to_uint64_xrninta                        \
         __binary32_to_uint64_xrninta
#define ___binary64_to_int32_xrninta                         \
         __binary64_to_int32_xrninta
#define ___binary64_to_uint32_xrninta                        \
         __binary64_to_uint32_xrninta
#define ___binary64_to_int64_xrninta                         \
         __binary64_to_int64_xrninta
#define ___binary64_to_uint64_xrninta                        \
         __binary64_to_uint64_xrninta
#define ___binary32_to_int32_xint                            \
         __binary32_to_int32_xint
#define ___binary32_to_uint32_xint                           \
         __binary32_to_uint32_xint
#define ___binary32_to_int64_xint                            \
         __binary32_to_int64_xint
#define ___binary32_to_uint64_xint                           \
         __binary32_to_uint64_xint
#define ___binary64_to_int32_xint                            \
         __binary64_to_int32_xint
#define ___binary64_to_uint32_xint                           \
         __binary64_to_uint32_xint
#define ___binary64_to_int64_xint                            \
         __binary64_to_int64_xint
#define ___binary64_to_uint64_xint                           \
         __binary64_to_uint64_xint
#define ___binary32_to_int32_xceil                           \
         __binary32_to_int32_xceil
#define ___binary32_to_uint32_xceil                          \
         __binary32_to_uint32_xceil
#define ___binary32_to_int64_xceil                           \
         __binary32_to_int64_xceil
#define ___binary32_to_uint64_xceil                          \
         __binary32_to_uint64_xceil
#define ___binary64_to_int32_xceil                           \
         __binary64_to_int32_xceil
#define ___binary64_to_uint32_xceil                          \
         __binary64_to_uint32_xceil
#define ___binary64_to_int64_xceil                           \
         __binary64_to_int64_xceil
#define ___binary64_to_uint64_xceil                          \
         __binary64_to_uint64_xceil
#define ___binary32_to_int32_xfloor                          \
         __binary32_to_int32_xfloor
#define ___binary32_to_uint32_xfloor                         \
         __binary32_to_uint32_xfloor
#define ___binary32_to_int64_xfloor                          \
         __binary32_to_int64_xfloor
#define ___binary32_to_uint64_xfloor                         \
         __binary32_to_uint64_xfloor
#define ___binary64_to_int32_xfloor                          \
         __binary64_to_int32_xfloor
#define ___binary64_to_uint32_xfloor                         \
         __binary64_to_uint32_xfloor
#define ___binary64_to_int64_xfloor                          \
         __binary64_to_int64_xfloor
#define ___binary64_to_uint64_xfloor                         \
         __binary64_to_uint64_xfloor
#define ___binary32_to_binary64                              \
         __binary32_to_binary64
#define ___binary64_to_binary32                              \
         __binary64_to_binary32
#define ___binary32_from_string                              \
         __binary32_from_string
#define ___binary64_from_string                              \
         __binary64_from_string
#define ___binary32_to_string                                \
         __binary32_to_string
#define ___binary64_to_string                                \
         __binary64_to_string
#define ___binary32_from_hexstring                           \
         __binary32_from_hexstring
#define ___binary64_from_hexstring                           \
         __binary64_from_hexstring
#define ___binary32_to_hexstring                             \
         __binary32_to_hexstring
#define ___binary64_to_hexstring                             \
         __binary64_to_hexstring
#define ___binary32_copy                                     \
         __binary32_copy
#define ___binary64_copy                                     \
         __binary64_copy
#define ___binary32_negate                                   \
         __binary32_negate
#define ___binary64_negate                                   \
         __binary64_negate
#define ___binary32_abs                                      \
         __binary32_abs
#define ___binary64_abs                                      \
         __binary64_abs
#define ___binary32_copysign                                 \
         __binary32_copysign
#define ___binary64_copysign                                 \
         __binary64_copysign
#define ___binary32_quiet_equal_binary32                     \
         __binary32_quiet_equal_binary32
#define ___binary32_quiet_equal_binary64                     \
         __binary32_quiet_equal_binary64
#define ___binary64_quiet_equal_binary32                     \
         __binary64_quiet_equal_binary32
#define ___binary64_quiet_equal_binary64                     \
         __binary64_quiet_equal_binary64
#define ___binary32_quiet_not_equal_binary32                 \
         __binary32_quiet_not_equal_binary32
#define ___binary32_quiet_not_equal_binary64                 \
         __binary32_quiet_not_equal_binary64
#define ___binary64_quiet_not_equal_binary32                 \
         __binary64_quiet_not_equal_binary32
#define ___binary64_quiet_not_equal_binary64                 \
         __binary64_quiet_not_equal_binary64
#define ___binary32_signaling_equal_binary32                 \
         __binary32_signaling_equal_binary32
#define ___binary32_signaling_equal_binary64                 \
         __binary32_signaling_equal_binary64
#define ___binary64_signaling_equal_binary32                 \
         __binary64_signaling_equal_binary32
#define ___binary64_signaling_equal_binary64                 \
         __binary64_signaling_equal_binary64
#define ___binary32_signaling_greater_binary32               \
         __binary32_signaling_greater_binary32
#define ___binary32_signaling_greater_binary64               \
         __binary32_signaling_greater_binary64
#define ___binary64_signaling_greater_binary32               \
         __binary64_signaling_greater_binary32
#define ___binary64_signaling_greater_binary64               \
         __binary64_signaling_greater_binary64
#define ___binary32_signaling_greater_equal_binary32         \
         __binary32_signaling_greater_equal_binary32
#define ___binary32_signaling_greater_equal_binary64         \
         __binary32_signaling_greater_equal_binary64
#define ___binary64_signaling_greater_equal_binary32         \
         __binary64_signaling_greater_equal_binary32
#define ___binary64_signaling_greater_equal_binary64         \
         __binary64_signaling_greater_equal_binary64
#define ___binary32_signaling_less_binary32                  \
         __binary32_signaling_less_binary32
#define ___binary32_signaling_less_binary64                  \
         __binary32_signaling_less_binary64
#define ___binary64_signaling_less_binary32                  \
         __binary64_signaling_less_binary32
#define ___binary64_signaling_less_binary64                  \
         __binary64_signaling_less_binary64
#define ___binary32_signaling_less_equal_binary32            \
         __binary32_signaling_less_equal_binary32
#define ___binary32_signaling_less_equal_binary64            \
         __binary32_signaling_less_equal_binary64
#define ___binary64_signaling_less_equal_binary32            \
         __binary64_signaling_less_equal_binary32
#define ___binary64_signaling_less_equal_binary64            \
         __binary64_signaling_less_equal_binary64
#define ___binary32_signaling_not_equal_binary32             \
         __binary32_signaling_not_equal_binary32
#define ___binary32_signaling_not_equal_binary64             \
         __binary32_signaling_not_equal_binary64
#define ___binary64_signaling_not_equal_binary32             \
         __binary64_signaling_not_equal_binary32
#define ___binary64_signaling_not_equal_binary64             \
         __binary64_signaling_not_equal_binary64
#define ___binary32_signaling_not_greater_binary32           \
         __binary32_signaling_not_greater_binary32
#define ___binary32_signaling_not_greater_binary64           \
         __binary32_signaling_not_greater_binary64
#define ___binary64_signaling_not_greater_binary32           \
         __binary64_signaling_not_greater_binary32
#define ___binary64_signaling_not_greater_binary64           \
         __binary64_signaling_not_greater_binary64
#define ___binary32_signaling_less_unordered_binary32        \
         __binary32_signaling_less_unordered_binary32
#define ___binary32_signaling_less_unordered_binary64        \
         __binary32_signaling_less_unordered_binary64
#define ___binary64_signaling_less_unordered_binary32        \
         __binary64_signaling_less_unordered_binary32
#define ___binary64_signaling_less_unordered_binary64        \
         __binary64_signaling_less_unordered_binary64
#define ___binary32_signaling_not_less_binary32              \
         __binary32_signaling_not_less_binary32
#define ___binary32_signaling_not_less_binary64              \
         __binary32_signaling_not_less_binary64
#define ___binary64_signaling_not_less_binary32              \
         __binary64_signaling_not_less_binary32
#define ___binary64_signaling_not_less_binary64              \
         __binary64_signaling_not_less_binary64
#define ___binary32_signaling_greater_unordered_binary32     \
         __binary32_signaling_greater_unordered_binary32
#define ___binary32_signaling_greater_unordered_binary64     \
         __binary32_signaling_greater_unordered_binary64
#define ___binary64_signaling_greater_unordered_binary32     \
         __binary64_signaling_greater_unordered_binary32
#define ___binary64_signaling_greater_unordered_binary64     \
         __binary64_signaling_greater_unordered_binary64
#define ___binary32_quiet_greater_binary32                   \
         __binary32_quiet_greater_binary32
#define ___binary32_quiet_greater_binary64                   \
         __binary32_quiet_greater_binary64
#define ___binary64_quiet_greater_binary32                   \
         __binary64_quiet_greater_binary32
#define ___binary64_quiet_greater_binary64                   \
         __binary64_quiet_greater_binary64
#define ___binary32_quiet_greater_equal_binary32             \
         __binary32_quiet_greater_equal_binary32
#define ___binary32_quiet_greater_equal_binary64             \
         __binary32_quiet_greater_equal_binary64
#define ___binary64_quiet_greater_equal_binary32             \
         __binary64_quiet_greater_equal_binary32
#define ___binary64_quiet_greater_equal_binary64             \
         __binary64_quiet_greater_equal_binary64
#define ___binary32_quiet_less_binary32                      \
         __binary32_quiet_less_binary32
#define ___binary32_quiet_less_binary64                      \
         __binary32_quiet_less_binary64
#define ___binary64_quiet_less_binary32                      \
         __binary64_quiet_less_binary32
#define ___binary64_quiet_less_binary64                      \
         __binary64_quiet_less_binary64
#define ___binary32_quiet_less_equal_binary32                \
         __binary32_quiet_less_equal_binary32
#define ___binary32_quiet_less_equal_binary64                \
         __binary32_quiet_less_equal_binary64
#define ___binary64_quiet_less_equal_binary32                \
         __binary64_quiet_less_equal_binary32
#define ___binary64_quiet_less_equal_binary64                \
         __binary64_quiet_less_equal_binary64
#define ___binary32_quiet_unordered_binary32                 \
         __binary32_quiet_unordered_binary32
#define ___binary32_quiet_unordered_binary64                 \
         __binary32_quiet_unordered_binary64
#define ___binary64_quiet_unordered_binary32                 \
         __binary64_quiet_unordered_binary32
#define ___binary64_quiet_unordered_binary64                 \
         __binary64_quiet_unordered_binary64
#define ___binary32_quiet_not_greater_binary32               \
         __binary32_quiet_not_greater_binary32
#define ___binary32_quiet_not_greater_binary64               \
         __binary32_quiet_not_greater_binary64
#define ___binary64_quiet_not_greater_binary32               \
         __binary64_quiet_not_greater_binary32
#define ___binary64_quiet_not_greater_binary64               \
         __binary64_quiet_not_greater_binary64
#define ___binary32_quiet_less_unordered_binary32            \
         __binary32_quiet_less_unordered_binary32
#define ___binary32_quiet_less_unordered_binary64            \
         __binary32_quiet_less_unordered_binary64
#define ___binary64_quiet_less_unordered_binary32            \
         __binary64_quiet_less_unordered_binary32
#define ___binary64_quiet_less_unordered_binary64            \
         __binary64_quiet_less_unordered_binary64
#define ___binary32_quiet_not_less_binary32                  \
         __binary32_quiet_not_less_binary32
#define ___binary32_quiet_not_less_binary64                  \
         __binary32_quiet_not_less_binary64
#define ___binary64_quiet_not_less_binary32                  \
         __binary64_quiet_not_less_binary32
#define ___binary64_quiet_not_less_binary64                  \
         __binary64_quiet_not_less_binary64
#define ___binary32_quiet_greater_unordered_binary32         \
         __binary32_quiet_greater_unordered_binary32
#define ___binary32_quiet_greater_unordered_binary64         \
         __binary32_quiet_greater_unordered_binary64
#define ___binary64_quiet_greater_unordered_binary32         \
         __binary64_quiet_greater_unordered_binary32
#define ___binary64_quiet_greater_unordered_binary64         \
         __binary64_quiet_greater_unordered_binary64
#define ___binary32_quiet_ordered_binary32                   \
         __binary32_quiet_ordered_binary32
#define ___binary32_quiet_ordered_binary64                   \
         __binary32_quiet_ordered_binary64
#define ___binary64_quiet_ordered_binary32                   \
         __binary64_quiet_ordered_binary32
#define ___binary64_quiet_ordered_binary64                   \
         __binary64_quiet_ordered_binary64
#define ___binary_is754version1985                           \
         __binary_is754version1985
#define ___binary_is754version2008                           \
         __binary_is754version2008
#define ___binary32_class                                    \
         __binary32_class
#define ___binary64_class                                    \
         __binary64_class
#define ___binary32_isSignMinus                              \
         __binary32_isSignMinus
#define ___binary64_isSignMinus                              \
         __binary64_isSignMinus
#define ___binary32_isNormal                                 \
         __binary32_isNormal
#define ___binary64_isNormal                                 \
         __binary64_isNormal
#define ___binary32_isFinite                                 \
         __binary32_isFinite
#define ___binary64_isFinite                                 \
         __binary64_isFinite
#define ___binary32_isZero                                   \
         __binary32_isZero
#define ___binary64_isZero                                   \
         __binary64_isZero
#define ___binary32_isSubnormal                              \
         __binary32_isSubnormal
#define ___binary64_isSubnormal                              \
         __binary64_isSubnormal
#define ___binary32_isInfinite                               \
         __binary32_isInfinite
#define ___binary64_isInfinite                               \
         __binary64_isInfinite
#define ___binary32_isNaN                                    \
         __binary32_isNaN
#define ___binary64_isNaN                                    \
         __binary64_isNaN
#define ___binary32_isSignaling                              \
         __binary32_isSignaling
#define ___binary64_isSignaling                              \
         __binary64_isSignaling
#define ___binary32_isCanonical                              \
         __binary32_isCanonical
#define ___binary64_isCanonical                              \
         __binary64_isCanonical
#define ___binary32_radix                                    \
         __binary32_radix
#define ___binary64_radix                                    \
         __binary64_radix
#define ___binary32_totalOrder                               \
         __binary32_totalOrder
#define ___binary64_totalOrder                               \
         __binary64_totalOrder
#define ___binary32_totalOrderMag                            \
         __binary32_totalOrderMag
#define ___binary64_totalOrderMag                            \
         __binary64_totalOrderMag
#define ___binary_lowerFlags                                 \
         __binary_lowerFlags
#define ___binary_raiseFlags                                 \
         __binary_raiseFlags
#define ___binary_testFlags                                  \
         __binary_testFlags
#define ___binary_testSavedFlags                             \
         __binary_testSavedFlags
#define ___binary_restoreFlags                               \
         __binary_restoreFlags
#define ___binary_saveFlags                                  \
         __binary_saveFlags
#define ___binary_getBinaryRoundingDirection                 \
         __binary_getBinaryRoundingDirection
#define ___binary_setBinaryRoundingDirection                 \
         __binary_setBinaryRoundingDirection
#define ___binary_saveModes                                  \
         __binary_saveModes
#define ___binary_restoreModes                               \
         __binary_restoreModes
#define ___binary_defaultMode                                \
         __binary_defaultMode


#endif // ifndef _BFP754_FUNCTIONNAMES_H_

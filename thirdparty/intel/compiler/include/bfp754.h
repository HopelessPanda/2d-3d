// File: bfp754.h
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
// Public header file provided for using the IEEE 754-2008 (binary
// part) compliance library
//
// =============================================================================
//
// Edit history:
//
// 1-001 01-MAY-2009 CQL: Initial revision
//
// =============================================================================


// This file gets included by the library user.
// It must not be included internally.
// We have bfp754_internal.h for that reason.
// The files included in this file get also included internally.

// The configuration of both the internal building and the external
// usage of the library is configured in bfp754_conf.h.

#ifndef _BFP754_H_
#define _BFP754_H_

#if defined(__cplusplus)
#define BFP754_EXTERN_C extern "C"
#else
#define BFP754_EXTERN_C extern
#endif

#include "bfp754_types.h"
#include "bfp754_functionnames.h"
#include "bfp754_conf.h"
#include "bfp754_macros.h"

// Declarations of the functions exported by the library
//
// They are all extern here.


BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_round_integral_nearest_even          (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_round_integral_nearest_even          (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_round_integral_nearest_away          (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_round_integral_nearest_away          (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_round_integral_zero                  (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_round_integral_zero                  (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_round_integral_positive              (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_round_integral_positive              (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_round_integral_negative              (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_round_integral_negative              (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_round_integral_exact                 (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_round_integral_exact                 (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_nextup                               (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_nextup                               (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_nextdown                             (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_nextdown                             (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_rem                                  (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_rem                                  (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_minnum                               (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_minnum                               (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_maxnum                               (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_maxnum                               (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_minnum_mag                           (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_minnum_mag                           (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_maxnum_mag                           (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_maxnum_mag                           (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_scalbn                               (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_INT
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_scalbn                               (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_INT
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_INT
            ___binary32_ilogb                                (
                __BFP754_RESULT_POINTER_TYPE_INT
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_INT
            ___binary64_ilogb                                (
                __BFP754_RESULT_POINTER_TYPE_INT
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_add_binary32_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_add_binary32_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_add_binary64_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_add_binary64_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_add_binary32_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_add_binary32_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_add_binary64_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_add_binary64_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_sub_binary32_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_sub_binary32_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_sub_binary64_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_sub_binary64_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_sub_binary32_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_sub_binary32_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_sub_binary64_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_sub_binary64_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_mul_binary32_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_mul_binary32_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_mul_binary64_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_mul_binary64_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_mul_binary32_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_mul_binary32_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_mul_binary64_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_mul_binary64_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_div_binary32_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_div_binary32_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_div_binary64_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_div_binary64_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_div_binary32_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_div_binary32_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_div_binary64_binary32                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_div_binary64_binary64                (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_sqrt_binary32                        (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_sqrt_binary64                        (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_sqrt_binary32                        (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_sqrt_binary64                        (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_fma_binary32_binary32_binary32       (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_fma_binary32_binary32_binary64       (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_fma_binary32_binary64_binary32       (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_fma_binary32_binary64_binary64       (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_fma_binary64_binary32_binary32       (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_fma_binary64_binary32_binary64       (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_fma_binary64_binary64_binary32       (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_fma_binary64_binary64_binary64       (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_fma_binary32_binary32_binary32       (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_fma_binary32_binary32_binary64       (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_fma_binary32_binary64_binary32       (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_fma_binary32_binary64_binary64       (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_fma_binary64_binary32_binary32       (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_fma_binary64_binary32_binary64       (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_fma_binary64_binary64_binary32       (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_fma_binary64_binary64_binary64       (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_from_int32                           (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_SINT32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_from_uint32                          (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_UINT32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_from_int64                           (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_SINT64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_from_uint64                          (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_UINT64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_from_int32                           (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_SINT32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_from_uint32                          (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_UINT32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_from_int64                           (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_SINT64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_from_uint64                          (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_UINT64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary32_to_int32_rnint                       (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary32_to_uint32_rnint                      (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary32_to_int64_rnint                       (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary32_to_uint64_rnint                      (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary64_to_int32_rnint                       (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary64_to_uint32_rnint                      (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary64_to_int64_rnint                       (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary64_to_uint64_rnint                      (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary32_to_int32_rninta                      (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary32_to_uint32_rninta                     (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary32_to_int64_rninta                      (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary32_to_uint64_rninta                     (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary64_to_int32_rninta                      (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary64_to_uint32_rninta                     (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary64_to_int64_rninta                      (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary64_to_uint64_rninta                     (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary32_to_int32_int                         (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary32_to_uint32_int                        (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary32_to_int64_int                         (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary32_to_uint64_int                        (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary64_to_int32_int                         (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary64_to_uint32_int                        (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary64_to_int64_int                         (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary64_to_uint64_int                        (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary32_to_int32_ceil                        (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary32_to_uint32_ceil                       (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary32_to_int64_ceil                        (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary32_to_uint64_ceil                       (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary64_to_int32_ceil                        (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary64_to_uint32_ceil                       (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary64_to_int64_ceil                        (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary64_to_uint64_ceil                       (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary32_to_int32_floor                       (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary32_to_uint32_floor                      (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary32_to_int64_floor                       (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary32_to_uint64_floor                      (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary64_to_int32_floor                       (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary64_to_uint32_floor                      (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary64_to_int64_floor                       (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary64_to_uint64_floor                      (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary32_to_int32_xrnint                      (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary32_to_uint32_xrnint                     (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary32_to_int64_xrnint                      (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary32_to_uint64_xrnint                     (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary64_to_int32_xrnint                      (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary64_to_uint32_xrnint                     (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary64_to_int64_xrnint                      (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary64_to_uint64_xrnint                     (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary32_to_int32_xrninta                     (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary32_to_uint32_xrninta                    (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary32_to_int64_xrninta                     (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary32_to_uint64_xrninta                    (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary64_to_int32_xrninta                     (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary64_to_uint32_xrninta                    (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary64_to_int64_xrninta                     (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary64_to_uint64_xrninta                    (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary32_to_int32_xint                        (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary32_to_uint32_xint                       (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary32_to_int64_xint                        (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary32_to_uint64_xint                       (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary64_to_int32_xint                        (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary64_to_uint32_xint                       (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary64_to_int64_xint                        (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary64_to_uint64_xint                       (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary32_to_int32_xceil                       (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary32_to_uint32_xceil                      (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary32_to_int64_xceil                       (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary32_to_uint64_xceil                      (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary64_to_int32_xceil                       (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary64_to_uint32_xceil                      (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary64_to_int64_xceil                       (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary64_to_uint64_xceil                      (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary32_to_int32_xfloor                      (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary32_to_uint32_xfloor                     (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary32_to_int64_xfloor                      (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary32_to_uint64_xfloor                     (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT32
            ___binary64_to_int32_xfloor                      (
                __BFP754_RESULT_POINTER_TYPE_SINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT32
            ___binary64_to_uint32_xfloor                     (
                __BFP754_RESULT_POINTER_TYPE_UINT32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_SINT64
            ___binary64_to_int64_xfloor                      (
                __BFP754_RESULT_POINTER_TYPE_SINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_UINT64
            ___binary64_to_uint64_xfloor                     (
                __BFP754_RESULT_POINTER_TYPE_UINT64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary32_to_binary64                          (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary64_to_binary32                          (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_from_string                          (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_STRING
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_from_string                          (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_STRING
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_from_hexstring                       (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_STRING
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_from_hexstring                       (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_STRING
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_copy                                 (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_copy                                 (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_negate                               (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_negate                               (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_abs                                  (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_abs                                  (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY32
            ___binary32_copysign                             (
                __BFP754_RESULT_POINTER_TYPE_BINARY32
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BINARY64
            ___binary64_copysign                             (
                __BFP754_RESULT_POINTER_TYPE_BINARY64
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_equal_binary32                 (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_equal_binary64                 (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_equal_binary32                 (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_equal_binary64                 (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_not_equal_binary32             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_not_equal_binary64             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_not_equal_binary32             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_not_equal_binary64             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_equal_binary32             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_equal_binary64             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_equal_binary32             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_equal_binary64             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_greater_binary32           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_greater_binary64           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_greater_binary32           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_greater_binary64           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_greater_equal_binary32     (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_greater_equal_binary64     (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_greater_equal_binary32     (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_greater_equal_binary64     (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_less_binary32              (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_less_binary64              (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_less_binary32              (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_less_binary64              (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_less_equal_binary32        (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_less_equal_binary64        (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_less_equal_binary32        (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_less_equal_binary64        (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_not_equal_binary32         (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_not_equal_binary64         (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_not_equal_binary32         (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_not_equal_binary64         (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_not_greater_binary32       (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_not_greater_binary64       (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_not_greater_binary32       (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_not_greater_binary64       (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_less_unordered_binary32    (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_less_unordered_binary64    (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_less_unordered_binary32    (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_less_unordered_binary64    (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_not_less_binary32          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_not_less_binary64          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_not_less_binary32          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_not_less_binary64          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_greater_unordered_binary32 (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_signaling_greater_unordered_binary64 (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_greater_unordered_binary32 (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_signaling_greater_unordered_binary64 (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_greater_binary32               (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_greater_binary64               (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_greater_binary32               (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_greater_binary64               (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_greater_equal_binary32         (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_greater_equal_binary64         (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_greater_equal_binary32         (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_greater_equal_binary64         (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_less_binary32                  (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_less_binary64                  (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_less_binary32                  (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_less_binary64                  (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_less_equal_binary32            (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_less_equal_binary64            (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_less_equal_binary32            (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_less_equal_binary64            (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_unordered_binary32             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_unordered_binary64             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_unordered_binary32             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_unordered_binary64             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_not_greater_binary32           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_not_greater_binary64           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_not_greater_binary32           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_not_greater_binary64           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_less_unordered_binary32        (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_less_unordered_binary64        (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_less_unordered_binary32        (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_less_unordered_binary64        (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_not_less_binary32              (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_not_less_binary64              (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_not_less_binary32              (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_not_less_binary64              (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_greater_unordered_binary32     (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_greater_unordered_binary64     (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_greater_unordered_binary32     (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_greater_unordered_binary64     (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_ordered_binary32               (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_quiet_ordered_binary64               (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_ordered_binary32               (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_quiet_ordered_binary64               (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary_is754version1985                       (
                __BFP754_RESULT_POINTER_TYPE_BOOL_SINGLE
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary_is754version2008                       (
                __BFP754_RESULT_POINTER_TYPE_BOOL_SINGLE
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_CLASS
            ___binary32_class                                (
                __BFP754_RESULT_POINTER_TYPE_CLASS
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_CLASS
            ___binary64_class                                (
                __BFP754_RESULT_POINTER_TYPE_CLASS
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_isSignMinus                          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_isSignMinus                          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_isNormal                             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_isNormal                             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_isFinite                             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_isFinite                             (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_isZero                               (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_isZero                               (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_isSubnormal                          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_isSubnormal                          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_isInfinite                           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_isInfinite                           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_isNaN                                (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_isNaN                                (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_isSignaling                          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_isSignaling                          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_isCanonical                          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_isCanonical                          (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_RADIX
            ___binary32_radix                                (
                __BFP754_RESULT_POINTER_TYPE_RADIX
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_RADIX
            ___binary64_radix                                (
                __BFP754_RESULT_POINTER_TYPE_RADIX
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_totalOrder                           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_totalOrder                           (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary32_totalOrderMag                        (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY32 ,
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary64_totalOrderMag                        (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_BINARY64 ,
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_VOID
            ___binary_lowerFlags                             (
                __BFP754_RESULT_POINTER_TYPE_VOID
                __BFP754_ARG_TYPE_FLAGS
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_VOID
            ___binary_raiseFlags                             (
                __BFP754_RESULT_POINTER_TYPE_VOID
                __BFP754_ARG_TYPE_FLAGS
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary_testFlags                              (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_FLAGS
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_BOOL
            ___binary_testSavedFlags                         (
                __BFP754_RESULT_POINTER_TYPE_BOOL
                __BFP754_ARG_TYPE_FLAGS ,
                __BFP754_ARG_TYPE_FLAGS
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_VOID
            ___binary_restoreFlags                           (
                __BFP754_RESULT_POINTER_TYPE_VOID
                __BFP754_ARG_TYPE_FLAGS
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_FLAGS
            ___binary_saveFlags                              (
                __BFP754_RESULT_POINTER_TYPE_FLAGS_SINGLE
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_ROUND_MODE
            ___binary_getBinaryRoundingDirection             (
                __BFP754_RESULT_POINTER_TYPE_ROUND_MODE_SINGLE
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_VOID
            ___binary_setBinaryRoundingDirection             (
                __BFP754_RESULT_POINTER_TYPE_VOID
                __BFP754_ARG_TYPE_ROUND_MODE
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_DYN_MODE
            ___binary_saveModes                              (
                __BFP754_RESULT_POINTER_TYPE_DYN_MODE_SINGLE
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_VOID
            ___binary_restoreModes                           (
                __BFP754_RESULT_POINTER_TYPE_VOID
                __BFP754_ARG_TYPE_DYN_MODE
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_VOID
            ___binary_defaultMode                            (
                __BFP754_RESULT_POINTER_TYPE_VOID_SINGLE
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_VOID
            ___binary32_to_string                            (
                __BFP754_RESULT_POINTER_TYPE_STRING
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_VOID
            ___binary64_to_string                            (
                __BFP754_RESULT_POINTER_TYPE_STRING
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_VOID
            ___binary32_to_hexstring                         (
                __BFP754_RESULT_POINTER_TYPE_STRING
                __BFP754_ARG_TYPE_BINARY32
                __BFP754_STATUS_ARGS_TYPE );

BFP754_EXTERN_C __BFP754_RETURN_TYPE_VOID
           ___binary64_to_hexstring                         (
                __BFP754_RESULT_POINTER_TYPE_STRING
                __BFP754_ARG_TYPE_BINARY64
                __BFP754_STATUS_ARGS_TYPE );


#endif // ifndef _BFP754_H_

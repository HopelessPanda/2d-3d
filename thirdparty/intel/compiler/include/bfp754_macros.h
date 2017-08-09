// File: bfp754_macros.h
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
// Macros for supporting both call-by-reference and call-by-value
// functions in the IEEE 754-2008 (binary part) compliance library
//
// =============================================================================
//
// Edit history:
//
// 1-001 01-MAY-2009 CQL: Initial revision
//
// =============================================================================

#ifndef _BFP754_MACROS_H_
#define _BFP754_MACROS_H_

// Macro-Magic for Call-by-reference and Call-by-value declarations

#if BFP754_CALL_BY_REFERENCE

#define __BFP754_RETURN_TYPE_BINARY32        void
#define __BFP754_RETURN_TYPE_BINARY64        void
#define __BFP754_RETURN_TYPE_UINT32          void
#define __BFP754_RETURN_TYPE_SINT32          void
#define __BFP754_RETURN_TYPE_UINT64          void
#define __BFP754_RETURN_TYPE_SINT64          void
#define __BFP754_RETURN_TYPE_INT             void
#define __BFP754_RETURN_TYPE_BOOL            void
#define __BFP754_RETURN_TYPE_CLASS           void
#define __BFP754_RETURN_TYPE_RADIX           void
#define __BFP754_RETURN_TYPE_VOID            void
#define __BFP754_RETURN_TYPE_FLAGS           void
#define __BFP754_RETURN_TYPE_ROUND_MODE      void
#define __BFP754_RETURN_TYPE_DYN_MODE        void

#define __BFP754_RESULT_POINTER_TYPE_BINARY32        __binary32 * ,
#define __BFP754_RESULT_POINTER_TYPE_BINARY64        __binary64 * ,
#define __BFP754_RESULT_POINTER_TYPE_UINT32          __binary_uint32 * ,
#define __BFP754_RESULT_POINTER_TYPE_SINT32          __binary_int32 * ,
#define __BFP754_RESULT_POINTER_TYPE_UINT64          __binary_uint64 * ,
#define __BFP754_RESULT_POINTER_TYPE_SINT64          __binary_int64 * ,
#define __BFP754_RESULT_POINTER_TYPE_INT             int * ,
#define __BFP754_RESULT_POINTER_TYPE_BOOL            int * ,
#define __BFP754_RESULT_POINTER_TYPE_CLASS           __binary_class_t * ,
#define __BFP754_RESULT_POINTER_TYPE_RADIX           __binary_radix_t * ,
#define __BFP754_RESULT_POINTER_TYPE_VOID
#define __BFP754_RESULT_POINTER_TYPE_FLAGS     __binary_exception_flags_t * ,
#define __BFP754_RESULT_POINTER_TYPE_ROUND_MODE   __binary_round_mode_t * ,
#define __BFP754_RESULT_POINTER_TYPE_DYN_MODE     __binary_dynamic_mode_t * ,
#define __BFP754_RESULT_POINTER_TYPE_STRING          char * ,

#define __BFP754_RESULT_POINTER_TYPE_BINARY32_SINGLE        __binary32 *
#define __BFP754_RESULT_POINTER_TYPE_BINARY64_SINGLE        __binary64 *
#define __BFP754_RESULT_POINTER_TYPE_UINT32_SINGLE          __binary_uint32 *
#define __BFP754_RESULT_POINTER_TYPE_SINT32_SINGLE          __binary_int32 *
#define __BFP754_RESULT_POINTER_TYPE_UINT64_SINGLE          __binary_uint64 *
#define __BFP754_RESULT_POINTER_TYPE_SINT64_SINGLE          __binary_int64 *
#define __BFP754_RESULT_POINTER_TYPE_INT_SINGLE             int *
#define __BFP754_RESULT_POINTER_TYPE_BOOL_SINGLE            int *
#define __BFP754_RESULT_POINTER_TYPE_CLASS_SINGLE         __binary_class_t *
#define __BFP754_RESULT_POINTER_TYPE_RADIX_SINGLE         __binary_radix_t *
#define __BFP754_RESULT_POINTER_TYPE_VOID_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_FLAGS_SINGLE                           \
                                                 __binary_exception_flags_t *
#define __BFP754_RESULT_POINTER_TYPE_ROUND_MODE_SINGLE                      \
                                                 __binary_round_mode_t *
#define __BFP754_RESULT_POINTER_TYPE_DYN_MODE_SINGLE                        \
                                                 __binary_dynamic_mode_t *
#define __BFP754_RESULT_POINTER_TYPE_STRING_SINGLE          char *

#define __BFP754_ARG_TYPE_BINARY32        __binary32 *
#define __BFP754_ARG_TYPE_BINARY64        __binary64 *
#define __BFP754_ARG_TYPE_UINT32          __binary_uint32 *
#define __BFP754_ARG_TYPE_SINT32          __binary_int32 *
#define __BFP754_ARG_TYPE_UINT64          __binary_uint64 *
#define __BFP754_ARG_TYPE_SINT64          __binary_int64 *
#define __BFP754_ARG_TYPE_INT             int *
#define __BFP754_ARG_TYPE_BOOL            int *
#define __BFP754_ARG_TYPE_CLASS           __binary_class_t *
#define __BFP754_ARG_TYPE_RADIX           __binary_radix_t *
#define __BFP754_ARG_TYPE_FLAGS           __binary_exception_flags_t *
#define __BFP754_ARG_TYPE_ROUND_MODE      __binary_round_mode_t *
#define __BFP754_ARG_TYPE_DYN_MODE        __binary_dynamic_mode_t *
#define __BFP754_ARG_TYPE_STRING          char *

#else

#define __BFP754_RETURN_TYPE_BINARY32        __binary32
#define __BFP754_RETURN_TYPE_BINARY64        __binary64
#define __BFP754_RETURN_TYPE_UINT32          __binary_uint32
#define __BFP754_RETURN_TYPE_SINT32          __binary_int32
#define __BFP754_RETURN_TYPE_UINT64          __binary_uint64
#define __BFP754_RETURN_TYPE_SINT64          __binary_int64
#define __BFP754_RETURN_TYPE_INT             int
#define __BFP754_RETURN_TYPE_BOOL            int
#define __BFP754_RETURN_TYPE_CLASS           __binary_class_t
#define __BFP754_RETURN_TYPE_RADIX           __binary_radix_t
#define __BFP754_RETURN_TYPE_VOID            void
#define __BFP754_RETURN_TYPE_FLAGS           __binary_exception_flags_t
#define __BFP754_RETURN_TYPE_ROUND_MODE      __binary_round_mode_t
#define __BFP754_RETURN_TYPE_DYN_MODE        __binary_dynamic_mode_t

#define __BFP754_RESULT_POINTER_TYPE_BINARY32
#define __BFP754_RESULT_POINTER_TYPE_BINARY64
#define __BFP754_RESULT_POINTER_TYPE_UINT32
#define __BFP754_RESULT_POINTER_TYPE_SINT32
#define __BFP754_RESULT_POINTER_TYPE_UINT64
#define __BFP754_RESULT_POINTER_TYPE_SINT64
#define __BFP754_RESULT_POINTER_TYPE_INT
#define __BFP754_RESULT_POINTER_TYPE_BOOL
#define __BFP754_RESULT_POINTER_TYPE_CLASS
#define __BFP754_RESULT_POINTER_TYPE_RADIX
#define __BFP754_RESULT_POINTER_TYPE_VOID
#define __BFP754_RESULT_POINTER_TYPE_FLAGS
#define __BFP754_RESULT_POINTER_TYPE_ROUND_MODE
#define __BFP754_RESULT_POINTER_TYPE_DYN_MODE
#define __BFP754_RESULT_POINTER_TYPE_STRING          char * ,

#define __BFP754_RESULT_POINTER_TYPE_BINARY32_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_BINARY64_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_UINT32_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_SINT32_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_UINT64_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_SINT64_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_INT_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_BOOL_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_CLASS_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_RADIX_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_VOID_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_FLAGS_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_ROUND_MODE_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_DYN_MODE_SINGLE
#define __BFP754_RESULT_POINTER_TYPE_STRING_SINGLE          char *


#define __BFP754_ARG_TYPE_BINARY32        __binary32
#define __BFP754_ARG_TYPE_BINARY64        __binary64
#define __BFP754_ARG_TYPE_UINT32          __binary_uint32
#define __BFP754_ARG_TYPE_SINT32          __binary_int32
#define __BFP754_ARG_TYPE_UINT64          __binary_uint64
#define __BFP754_ARG_TYPE_SINT64          __binary_int64
#define __BFP754_ARG_TYPE_INT             int
#define __BFP754_ARG_TYPE_BOOL            int
#define __BFP754_ARG_TYPE_CLASS           __binary_class_t
#define __BFP754_ARG_TYPE_RADIX           __binary_radix_t
#define __BFP754_ARG_TYPE_FLAGS           __binary_exception_flags_t
#define __BFP754_ARG_TYPE_ROUND_MODE      __binary_round_mode_t
#define __BFP754_ARG_TYPE_DYN_MODE        __binary_dynamic_mode_t
#define __BFP754_ARG_TYPE_STRING          char *

#endif


// Explicit or implicit status arguments


#define __BFP754_STATUS_ARGS_TYPE


#endif // ifndef _BFP754_MACROS_H_

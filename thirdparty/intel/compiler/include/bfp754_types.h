// File: bfp754_types.h
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
// Type definitions for the IEEE 754-2008 (binary part) compliance library
//
// =============================================================================
//
// Edit history:
//
// 1-001 01-MAY-2009 CQL: Initial revision
// 1-002 08-MAY-2009 CQL: Removed a unused pragma
//                        Removed denormal from the flags (not supported by 
//                        IEEE 754-2008)
//
// =============================================================================

#ifndef _BFP754_TYPES_H_
#define _BFP754_TYPES_H_

// Set pragma for floating-point state access 

#pragma fenv_access(on)


// Definitions of integer types
//
// Try do use C99 standard headers where supported
//

#if ((defined(__STDC_VERSION__)) && (__STDC_VERSION__ >= 199901L)) || \
     (defined(__GNUC__))

#include <stdint.h>

typedef int8_t             __binary_int8;
typedef uint8_t            __binary_uint8;
typedef int16_t            __binary_int16;
typedef uint16_t           __binary_uint16;
typedef int32_t            __binary_int32;
typedef uint32_t           __binary_uint32;
typedef int64_t            __binary_int64;
typedef uint64_t           __binary_uint64;

#else 

#ifdef WIN32

#    ifdef _MSC_VER
     typedef __int8             __binary_int8;
     typedef unsigned __int8    __binary_uint8;
     typedef __int16            __binary_int16;
     typedef unsigned __int16   __binary_uint16;
     typedef __int32            __binary_int32;
     typedef unsigned __int32   __binary_uint32;
     typedef __int64            __binary_int64;
     typedef unsigned __int64   __binary_uint64;

#    else
     typedef char               __binary_int8;
     typedef unsigned char      __binary_uint8;
     typedef short              __binary_int16;
     typedef unsigned short     __binary_uint16;
     typedef int                __binary_int32;
     typedef unsigned int       __binary_uint32;
     typedef long long          __binary_int64;
     typedef unsigned long long __binary_uint64;

#    endif // Microsoft compiler 

#else 

typedef char               __binary_int8;
typedef unsigned char      __binary_uint8;
typedef short              __binary_int16;
typedef unsigned short     __binary_uint16;
typedef int                __binary_int32;
typedef unsigned int       __binary_uint32;
typedef long long          __binary_int64;
typedef unsigned long long __binary_uint64;

#endif // WIN32 system
 
#endif // C99 system


// Definitions of the binary32 and binary64 floating-point types

#define __binary32 float
#define __binary64 double


// Definitions of exception flags, radices, classes and rounding
// directions
//
// Remark: the optional round-to-nearest-ties-to-away mode is
// currently unsupported

typedef enum __binary_class_types
  {
    signalingNaN,
    quietNaN,
    negativeInfinity,
    negativeNormal,
    negativeSubnormal,
    negativeZero,
    positiveZero,
    positiveSubnormal,
    positiveNormal,
    positiveInfinity
  } __binary_class_t;


typedef enum __binary_radix_types
  {
    binary = 2,
    decimal = 10
  } __binary_radix_t;


typedef int __binary_exception_flags_t;

// Constants for __binary_exception_flags_t
enum {
    __BFP754_INVALID    = 1,
    __BFP754_DIVBYZERO  = 2,
    __BFP754_OVERFLOW   = 4,
    __BFP754_UNDERFLOW  = 8,
    __BFP754_INEXACT    = 16
};


typedef enum {
  __BFP754_ROUND_TO_NEAREST_EVEN,
  __BFP754_ROUND_TOWARD_POSITIVE,
  __BFP754_ROUND_TOWARD_NEGATIVE,
  __BFP754_ROUND_TOWARD_ZERO
} __binary_round_mode_t;


// The only dynamic attribute currently supported is the rounding
// direction. Hence the rounding direction type defines also the complete 
// dynamic attribute state.

#define __binary_dynamic_mode_t __binary_round_mode_t 


#endif // ifndef _BFP754_TYEPS_H_

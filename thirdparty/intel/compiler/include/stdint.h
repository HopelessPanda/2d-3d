/*
**  Copyright (C) 2010-2011 Intel Corporation. All rights reserved.
**
** The information and source code contained herein is the exclusive
** property of Intel Corporation and may not be disclosed, examined
** or reproduced in whole or in part without explicit written authorization
** from the company.
*/



#if _MSC_VER >= 1600       /* VS2010 */

/*
** MSVS 2010 supports stdint.h, so we use MS header.
*/

#ifdef __INTEL_COMPILER
#include_next <stdint.h>
#else

/*
** "../../vc/include/stdint.h" header will be used by default.
** Set __MS_VC_INSTALL_PATH in case of non-standard location of MS headers.
*/

#ifndef __MS_VC_INSTALL_PATH
#define __MS_VC_INSTALL_PATH   ../../vc
#endif

#define __TMP_GLUE(a,b)     a##b
#define __TMP_PASTE2(a,b)   __TMP_GLUE(a,b)
#define __TMP_STR1(x)       #x
#define __TMP_STRING(x)     __TMP_STR1(x)

#include __TMP_STRING(__TMP_PASTE2(__MS_VC_INSTALL_PATH,/include/stdint.h))

#undef __TMP_GLUE
#undef __TMP_PASTE2
#undef __TMP_STR1
#undef __TMP_STRING

#endif  /* ifdef __INTEL_COMPILER */

#else   /* #if _MSC_VER >= 1600 */

/*
** Based on ISO/IEC 9899:TC3 Committee Draft - September 7, 2007, WG14/N1256
**
** 7.18 Integer types <stdint.h>
*/

#ifndef __INTEL_STDINT_H
#define __INTEL_STDINT_H	1

/* Integer types */

/* Exact-width integer types */

typedef signed char             int8_t;
typedef signed short            int16_t;
typedef signed int              int32_t;
typedef signed long long int    int64_t;

typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long int  uint64_t;

/* Minimum-width integer types */

typedef int8_t       int_least8_t;
typedef int16_t      int_least16_t;
typedef int32_t      int_least32_t;
typedef int64_t      int_least64_t;

typedef uint8_t      uint_least8_t;
typedef uint16_t     uint_least16_t;
typedef uint32_t     uint_least32_t;
typedef uint64_t     uint_least64_t;

/* Fastest minimum-width integer types */

typedef int8_t       int_fast8_t;
typedef int16_t      int_fast16_t;
typedef int32_t      int_fast32_t;
typedef int64_t      int_fast64_t;

typedef uint8_t      uint_fast8_t;
typedef uint16_t     uint_fast16_t;
typedef uint32_t     uint_fast32_t;
typedef uint64_t     uint_fast64_t;

/* Integer types capable of holding object pointers */

#ifdef _WIN64
    typedef int64_t   intptr_t;
    typedef uint64_t  uintptr_t;
#else
    typedef int32_t   intptr_t;
    typedef uint32_t  uintptr_t;
#endif

/* Greatest-width integer types */

typedef int64_t       intmax_t;
typedef uint64_t      uintmax_t;


/* Limits of specified-width integer types */

/*
** The ISO C99 standard specifies that in C++ implementations
** these macros should only be defined if explicitly requested.
*/
#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

/* Limits of exact-width integer types */

#define INT8_MAX          (127)
#define INT16_MAX         (32767)
#define INT32_MAX         (2147483647)
#define INT64_MAX         (9223372036854775807LL)

#define INT8_MIN          (-INT8_MAX-1)
#define INT16_MIN         (-INT16_MAX-1)
#define INT32_MIN         (-INT32_MAX-1)
#define INT64_MIN         (-INT64_MAX-1)

#define UINT8_MAX         (255)
#define UINT16_MAX        (65535)
#define UINT32_MAX        (4294967295U)
#define UINT64_MAX        (18446744073709551615ULL)

/* Limits of minimum-width integer types */

#define INT_LEAST8_MIN    INT8_MIN
#define INT_LEAST16_MIN   INT16_MIN
#define INT_LEAST32_MIN   INT32_MIN
#define INT_LEAST64_MIN   INT64_MIN

#define INT_LEAST8_MAX    INT8_MAX
#define INT_LEAST16_MAX   INT16_MAX
#define INT_LEAST32_MAX   INT32_MAX
#define INT_LEAST64_MAX   INT64_MAX

#define UINT_LEAST8_MAX   UINT8_MAX
#define UINT_LEAST16_MAX  UINT16_MAX
#define UINT_LEAST32_MAX  UINT32_MAX
#define UINT_LEAST64_MAX  UINT64_MAX

/* Limits of fastest minimum-width integer types */

#define INT_FAST8_MIN     INT8_MIN
#define INT_FAST16_MIN    INT16_MIN
#define INT_FAST32_MIN    INT32_MIN
#define INT_FAST64_MIN    INT64_MIN

#define INT_FAST8_MAX     INT8_MAX
#define INT_FAST16_MAX    INT16_MAX
#define INT_FAST32_MAX    INT32_MAX
#define INT_FAST64_MAX    INT64_MAX

#define UINT_FAST8_MAX    UINT8_MAX
#define UINT_FAST16_MAX   UINT16_MAX
#define UINT_FAST32_MAX   UINT32_MAX
#define UINT_FAST64_MAX   UINT64_MAX

/* Limits of integer types capable of holding object pointers */

#ifdef _WIN64
    #define INTPTR_MIN    INT64_MIN
    #define INTPTR_MAX    INT64_MAX
    #define UINTPTR_MAX   UINT64_MAX
#else
    #define INTPTR_MIN    INT32_MIN
    #define INTPTR_MAX    INT32_MAX
    #define UINTPTR_MAX   UINT32_MAX
#endif

/* Limits of greatest-width integer types */

#define INTMAX_MIN        INT64_MIN
#define INTMAX_MAX        INT64_MAX
#define UINTMAX_MAX       UINT64_MAX


/* Limits of other integer types */

#ifdef _WIN64
    #define PTRDIFF_MIN   INT64_MIN
    #define PTRDIFF_MAX   INT64_MAX
#else
    #define PTRDIFF_MIN   INT32_MIN
    #define PTRDIFF_MAX   INT32_MAX
#endif

#define SIG_ATOMIC_MIN    INT32_MIN
#define SIG_ATOMIC_MAX    INT32_MAX

#ifndef SIZE_MAX
#ifdef _WIN64
    #define SIZE_MAX      UINT64_MAX
#else
    #define SIZE_MAX      UINT32_MAX
#endif
#endif

/* WCHAR_MIN and WCHAR_MAX constants might also be defined in <wchar.h>. */
#ifndef WCHAR_MIN
    #define WCHAR_MIN     0
#endif

#ifndef WCHAR_MAX
    #define WCHAR_MAX     UINT16_MAX
#endif

#define WINT_MIN          0
#define WINT_MAX          UINT32_MAX

#endif /* if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS) */


/* Macros for integer constants */

/*
** The ISO C99 standard specifies that in C++ implementations
** these macros should only be defined if explicitly requested.
*/
#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

/* Macros for minimum-width integer constants */

#define INT8_C(val)       val
#define INT16_C(val)      val
#define INT32_C(val)      val
#define INT64_C(val)      val ## LL

#define UINT8_C(val)      val ## U
#define UINT16_C(val)     val ## U
#define UINT32_C(val)     val ## U
#define UINT64_C(val)     val ## ULL

/* Macros for greatest-width integer constants */
#define INTMAX_C          INT64_C
#define UINTMAX_C         UINT64_C

#endif /* #if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS) */

#endif /* #ifndef __INTEL_STDINT_H */

#endif /* #if _MSC_VER >= 1600 */

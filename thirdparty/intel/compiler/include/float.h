/**
***  Copyright  (C) 1985-2011 Intel Corporation. All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
**/


#if (defined(_WIN32) || defined(_WIN64))
  /* Include_next should be before guard macros _FLOAT_H__ in order to at last reach system header */
  #ifdef __INTEL_COMPILER
    #include_next <float.h>     /* utilize system header */
  #else
    #if _MSC_VER >= 1400 /* Previous versions of MSVS are not supported. */
      /*
      // Here, the #include <../../vc/include/header.h> is used as the
      // equivalent of #include_next<header.h> working for MS C compiler
      // of MSVS 2005, 2008, 2010 with default installation paths.
      // The equivalent works correctly when Intel compiler header is not
      // located in ../../vc/include subfolder for any searched include path,
      // and MS header is.
      // In case of non standard location of MS headers, say in 
      // C:/PROGRA~1/MSVS/NEW_VC/INCLUDE folder, proper __MS_VC_INSTALL_PATH
      // macro should be defined in command line -D option
      // like -D__MS_VC_INSTALL_PATH=C:/PROGRA~1/MSVS/NEW_VC.
      */
      #ifndef __MS_VC_INSTALL_PATH
      #define __MS_VC_INSTALL_PATH    ../../vc
      #endif

      #define __TMP_GLUE(a,b)         a##b
      #define __TMP_PASTE2(a,b)       __TMP_GLUE(a,b)
      #define __TMP_ANGLE_BRACKETS(x) <x>

      #include __TMP_ANGLE_BRACKETS(__TMP_PASTE2(__MS_VC_INSTALL_PATH,/include/float.h))

      #undef __TMP_GLUE
      #undef __TMP_PASTE2
      #undef __TMP_ANGLE_BRACKETS
    #endif
  #endif
#endif


/* float.h standard header -- IEEE 754 version */
#ifdef __INTEL_COMPILER
#ifndef _FLOAT_H__
#define _FLOAT_H__



#ifdef  __cplusplus
extern "C" {
#endif

#if   (defined(_WIN32) || defined(_WIN64)) && defined(_DLL)   /* Windows DLL */
    extern int __declspec(dllimport) __cdecl fegetround (void) ;
#elif (defined(_WIN32) || defined(_WIN64))   /* Windows static */
    extern int __cdecl fegetround (void) ;
#else
    extern int fegetround (void) ;
#endif

#ifdef  __cplusplus
}
#endif


#if defined(__ECL) || defined(__ECC)            /* Intel(R) Itanium(R) architecture */
    #define __INTEL_FLOAT_H__FE_TONEAREST  0
    #define __INTEL_FLOAT_H__FE_DOWNWARD   1
    #define __INTEL_FLOAT_H__FE_UPWARD     2
    #define __INTEL_FLOAT_H__FE_TOWARDZERO 3
#else
    #define __INTEL_FLOAT_H__FE_TONEAREST  0
    #define __INTEL_FLOAT_H__FE_DOWNWARD   0x400
    #define __INTEL_FLOAT_H__FE_UPWARD     0x800
    #define __INTEL_FLOAT_H__FE_TOWARDZERO 0xc00
#endif

        /* common properties */
#undef  FLT_RADIX
#define FLT_RADIX   2

#undef  FLT_ROUNDS
#define FLT_ROUNDS                                  \
   ((fegetround()==__INTEL_FLOAT_H__FE_TOWARDZERO)?0:      \
   ((fegetround()==__INTEL_FLOAT_H__FE_TONEAREST)?1:       \
   ((fegetround()==__INTEL_FLOAT_H__FE_UPWARD)?2:          \
   ((fegetround()==__INTEL_FLOAT_H__FE_DOWNWARD)?3:-1))))


        /* float properties */
#undef FLT_EPSILON
#undef FLT_MAX
#undef FLT_MIN
#undef FLT_DIG
#undef FLT_MANT_DIG
#undef FLT_MAX_10_EXP
#undef FLT_MAX_EXP
#undef FLT_MIN_10_EXP
#undef FLT_MIN_EXP

#define FLT_EPSILON         1.19209290e-07F
#define FLT_MAX             3.40282347e+38F
#define FLT_MIN             1.17549435e-38F
#define FLT_DIG             6
#define FLT_MANT_DIG        24
#define FLT_MAX_10_EXP      38
#define FLT_MAX_EXP         128
#define FLT_MIN_10_EXP      (-37)
#define FLT_MIN_EXP         (-125)

        /* double properties */
#undef DBL_EPSILON
#undef DBL_MAX
#undef DBL_MIN
#undef DBL_DIG
#undef DBL_MANT_DIG
#undef DBL_MAX_10_EXP
#undef DBL_MAX_EXP
#undef DBL_MIN_10_EXP
#undef DBL_MIN_EXP

#define DBL_EPSILON         2.2204460492503131e-16
#define DBL_MAX             1.7976931348623157e+308
#define DBL_MIN             2.2250738585072014e-308
#define DBL_DIG             15
#define DBL_MANT_DIG        53
#define DBL_MAX_10_EXP      308
#define DBL_MAX_EXP         1024
#define DBL_MIN_10_EXP      (-307)
#define DBL_MIN_EXP         (-1021)

        /* long double properties */
#undef LDBL_EPSILON
#undef LDBL_MAX
#undef LDBL_MIN
#undef LDBL_DIG
#undef LDBL_MANT_DIG
#undef LDBL_MAX_10_EXP
#undef LDBL_MAX_EXP
#undef LDBL_MIN_10_EXP
#undef LDBL_MIN_EXP

#ifndef __IMFLONGDOUBLE
#if defined(__LONG_DOUBLE_SIZE__) /* Compiler-predefined macros. If defined, should be 128|80|64 */
# define __IMFLONGDOUBLE (__LONG_DOUBLE_SIZE__)
#else
# define __IMFLONGDOUBLE 64
#endif
#endif

#if (__IMFLONGDOUBLE == 64) || defined(__LONGDOUBLE_AS_DOUBLE)
    #define LDBL_EPSILON         2.2204460492503131e-16
    #define LDBL_MAX             1.7976931348623157e+308
    #define LDBL_MIN             2.2250738585072014e-308
    #define LDBL_DIG             15
    #define LDBL_MANT_DIG        53
    #define LDBL_MAX_10_EXP      308
    #define LDBL_MAX_EXP         1024
    #define LDBL_MIN_10_EXP      (-307)
    #define LDBL_MIN_EXP         (-1021)
#else
    #define LDBL_EPSILON        1.0842021724855044340075E-19L
    #define LDBL_MAX            1.1897314953572317650213E+4932L
    #define LDBL_MIN            3.3621031431120935062627E-4932L
    #define LDBL_DIG            18
    #define LDBL_MANT_DIG       64
    #define LDBL_MAX_10_EXP     4932
    #define LDBL_MAX_EXP        16384
    #define LDBL_MIN_10_EXP     (-4931)
    #define LDBL_MIN_EXP        (-16381)
#endif

#if defined (__STDC_VERSION__) && __STDC_VERSION__ >=199901L

#undef DECIMAL_DIG
#undef FLT_EVAL_METHOD

#define DECIMAL_DIG 21
#ifdef __FLT_EVAL_METHOD__
#define FLT_EVAL_METHOD __FLT_EVAL_METHOD__
#else
#define FLT_EVAL_METHOD -1
#endif

#endif  /* __STDC_VERSION__ >= C99 */

#ifdef __STDC_WANT_DEC_FP__ 

#ifdef __DEC_EVAL_METHOD__
#define DEC_EVAL_METHOD __DEC_EVAL_METHOD__
#else
#define DEC_EVAL_METHOD -1
#endif

#ifndef DEC32_MANT_DIG
#define DEC32_MANT_DIG __DEC32_MANT_DIG__
#endif
#ifndef DEC64_MANT_DIG
#define DEC64_MANT_DIG __DEC64_MANT_DIG__
#endif             
#ifndef DEC128_MANT_DIG
#define DEC128_MANT_DIG __DEC128_MANT_DIG__           
#endif             
#ifndef DEC32_MIN_EXP           
#define DEC32_MIN_EXP __DEC32_MIN_EXP__
#endif             
#ifndef DEC64_MIN_EXP           
#define DEC64_MIN_EXP __DEC64_MIN_EXP__
#endif             
#ifndef DEC128_MIN_EXP           
#define DEC128_MIN_EXP __DEC128_MIN_EXP__
#endif             
#ifndef DEC32_MAX_EXP           
#define DEC32_MAX_EXP __DEC32_MAX_EXP__
#endif             
#ifndef DEC64_MAX_EXP           
#define DEC64_MAX_EXP __DEC64_MAX_EXP__
#endif             
#ifndef DEC128_MAX_EXP           
#define DEC128_MAX_EXP __DEC128_MAX_EXP__
#endif             
#ifndef DEC32_EPSILON           
#define DEC32_EPSILON __DEC32_EPSILON__
#endif             
#ifndef DEC64_EPSILON           
#define DEC64_EPSILON __DEC64_EPSILON__
#endif             
#ifndef DEC128_EPSILON           
#define DEC128_EPSILON __DEC128_EPSILON__
#endif             
#ifndef DEC32_MIN           
#define DEC32_MIN __DEC32_MIN__
#endif             
#ifndef DEC64_MIN           
#define DEC64_MIN __DEC64_MIN__
#endif             
#ifndef DEC128_MIN           
#define DEC128_MIN __DEC128_MIN__
#endif             
#ifndef DEC32_MAX           
#define DEC32_MAX __DEC32_MAX__
#endif             
#ifndef DEC64_MAX           
#define DEC64_MAX __DEC64_MAX__
#endif             
#ifndef DEC128_MAX           
#define DEC128_MAX __DEC128_MAX__
#endif             
#ifndef DEC32_DEN           
#define DEC32_DEN __DEC32_SUBNORMAL_MIN__
#endif             
#ifndef DEC64_DEN           
#define DEC64_DEN __DEC64_SUBNORMAL_MIN__
#endif             
#ifndef DEC128_DEN           
#define DEC128_DEN __DEC128_SUBNORMAL_MIN__
#endif   

#endif /* __STDC_WANT_DEC_FP__ */

#endif /* #ifndef _FLOAT_H__ */

#endif /* __INTEL_COMPILER */

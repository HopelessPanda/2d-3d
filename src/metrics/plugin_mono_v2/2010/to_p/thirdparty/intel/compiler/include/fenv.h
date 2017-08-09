/* file: fenv.h */

/*
** 
** Copyright  (C) 1985-2010 Intel Corporation. All rights reserved.
**
** The information and source code contained herein is the exclusive property
** of Intel Corporation and may not be disclosed, examined, or reproduced in
** whole or in part without explicit written authorization from the Company.
*/

/*
    fenv.h - C99 floating-point environment control functions
*/

/* Include_next should be before guard macros in order to at last reach system header */
#if defined(__PURE_SYS_C99_HEADERS__) && !(defined(_WIN32) || defined(_WIN64)) && !(defined(__VXWORKS__) && defined(_WRS_KERNEL))
# include_next <fenv.h> /* utilize sys header */
#elif !defined(__PURE_INTEL_C99_HEADERS__) && !(defined(_WIN32) || defined(_WIN64)) && !(defined(__LRB__) && defined(__FreeBSD__)) && !(defined(__VXWORKS__) && defined(_WRS_KERNEL)) && !defined(__NetBSD__)
# include_next <fenv.h> /* utilize and expand sys header */

#ifndef __FENV_H_INCLUDED
#define __FENV_H_INCLUDED

#if defined(__cplusplus)    /* C linkage */
extern "C" {
#endif

#if defined(_DLL) && (defined(_WIN32) || defined(_WIN64))   /* Windows DLL */
# define _FENV_PUBAPI __declspec(dllimport) __cdecl
#elif defined(__linux__) || defined(__APPLE__) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__QNX__) || defined(__VXWORKS__)   /* Linux, MacOS, NetBSD, FreeBSD, QNX or VXWORKS */
# define _FENV_PUBAPI                             /* do not change this line! */
#else                                             /* Windows static */
# define _FENV_PUBAPI __cdecl
#endif

#if defined (__APPLE__) || !defined (__USE_GNU)
extern int _FENV_PUBAPI fegetexcept (void) ;
extern int _FENV_PUBAPI feenableexcept (int __excepts) ;
extern int _FENV_PUBAPI fedisableexcept (int __excepts) ;
#endif

#if defined(__cplusplus)    /* C linkage */
}
#endif

/* Re-define FE_DFL_ENV for Intel(R) Itanium(R) architecture architecture, add FE_DENORMAL support for others */

#if defined(__ECL) || defined(__ECC)            /* Intel(R) Itanium(R) architecture architecture */

/* Default 64-bit Floating Point Status Register   */
#if defined (__linux__)
static const fenv_t __default_fpsr = 0x0009804c0270033fuL;
#else
static const fenv_t __default_fpsr = 0x0009804c0270033fui64;
#endif

#undef  FE_DFL_ENV
#define FE_DFL_ENV &__default_fpsr

#else /* Not Intel(R) Itanium(R) architecture */

#ifdef FE_DFL_ENV
#undef FE_DFL_ENV
#endif
#define FE_DFL_ENV     ((__const fenv_t *) -1)
#if defined (__APPLE__)
#ifdef FE_DFL_DISABLE_SSE_DENORMS_ENV
#undef FE_DFL_DISABLE_SSE_DENORMS_ENV
#endif
#define FE_DFL_DISABLE_SSE_DENORMS_ENV     ((__const fenv_t *) -3)
#endif

#endif /* Intel(R) Itanium(R) architecture */

#ifndef FE_UNNORMAL
#define FE_UNNORMAL     2
#endif
#ifndef FE_DENORMAL
#define FE_DENORMAL     2
#endif

/*
** The macro FE_ALL_EXCEPT is simply the bitwise OR of all floating-point
** exception macros defined by the implementation.
*/
#undef  FE_ALL_EXCEPT
#define FE_ALL_EXCEPT  (FE_INVALID | FE_UNNORMAL | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT)

#endif /* __FENV_H_INCLUDED */

#else /* pure intel header */

#ifndef __FENV_H_INCLUDED
#define __FENV_H_INCLUDED

/* Check usage correctness */

#if !defined(__INTEL_COMPILER)
# error "This <fenv.h> is for use with only the Intel compilers!"
#endif

#if defined(__cplusplus)    /* C linkage */
extern "C" {
#endif

#if defined(__ECL) || defined(__ECC)            /* Intel(R) Itanium(R) architecture */

/* Default 64-bit Floating Point Status Register   */
#if defined(__linux__)
typedef unsigned    long fexcept_t;
#else
typedef unsigned __int64 fexcept_t;
#endif
/*
   fexcept_t	64-bit value representing the floating-point status
		flags; the associated 6 bits are
                Bit 0:  Invalid  Exception (operand based)
                Bit 1:  Unnormal Operand Exception (operand based)
                Bit 2:  Divide by Zero Exception (operand based)
                Bit 3:  Overflow Exception (result based)
                Bit 4:  Underflow Exception (result based)
                Bit 5:  Inexact (Imprecise) Exception (result based)
                Other High-order Bits Should Be 0
*/

/*
   fenv_t       64-bit value representing all status and control bits in
                the fpsr.
*/
#if defined(__linux__)
typedef unsigned    long fenv_t;
#else
typedef unsigned __int64 fenv_t;
#endif

/* Default 64-bit Floating Point Status Register   */
#if defined(__linux__)
static const fenv_t __default_fpsr = 0x0009804c0270033fuL;
#else
static const fenv_t __default_fpsr = 0x0009804c0270033fui64;
#endif
#define FE_DFL_ENV &__default_fpsr

/*
   C99 Rounding Mode Bit-Masks

   These macros specify the IEEE rounding modes that are supported and may
   be manipulated on this architecture. The default rounding mode is
   FE_TONEAREST.
*/
#define FE_TONEAREST  0
#define FE_DOWNWARD   1
#define FE_UPWARD     2
#define FE_TOWARDZERO 3

#else                                           /* other than Intel(R) Itanium(R) architecture */

/*
** The type fexcept_t represents the floating-point status flags collectively,
** including any status the implementation associates with the flags.
*/
typedef unsigned short int fexcept_t;

/*
** The type fenv_t represents the entire floating-point environment.
**
** This structure corresponds to the layout of the block written by the `fstenv'.
*/
#if defined (__APPLE__)
typedef struct {
    unsigned short int cw;
    unsigned short int sw;
    unsigned int       csr;
    char               empty[8];
} fenv_t;
#else
#pragma pack(1)
typedef struct {
    unsigned short int cw;
    unsigned short int empty1;
    unsigned short int sw;
    unsigned short int empty2[11];
#if defined(_M_X64) || defined(__x86_64__)    /* Intel(R) 64 architecture */
    unsigned int csr;
#endif
} fenv_t;
#pragma pack()
#endif
/*
** The macro FE_DFL_ENV represents the default floating-point environment.
*/
#define FE_DFL_ENV     ((__const fenv_t *) -1)
#if defined (__APPLE__)
#define FE_DFL_DISABLE_SSE_DENORMS_ENV     ((__const fenv_t *) -3)
#endif

/*
** C99 definitions for bits representing the rounding direction.
**
** The bit patterns of the FPU control word are used as the values
** for the appropriate macros.
*/
#define FE_TONEAREST  0
#define FE_DOWNWARD   0x400
#define FE_UPWARD     0x800
#define FE_TOWARDZERO 0xc00

#endif                                          /* Platform dependent */


/*
** C99 definitions for bits representing the FP exception.
**
** The bitwise OR of these macros can be used to analyze multiple
** exception bits all at once.
*/
#define FE_INVALID      1
#define FE_UNNORMAL     2
#define FE_DENORMAL     2
#define FE_DIVBYZERO    4
#define FE_OVERFLOW     8
#define FE_UNDERFLOW    16
#define FE_INEXACT      32

/*
** The macro FE_ALL_EXCEPT is simply the bitwise OR of all floating-point
** exception macros defined by the implementation.
*/
#define FE_ALL_EXCEPT  (FE_INVALID | FE_UNNORMAL | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT)
#define FE_NO_EXCEPT   0


#if defined(_DLL) && (defined(_WIN32) || defined(_WIN64))   /* Windows DLL */
# define _FENV_PUBAPI __declspec(dllimport) __cdecl
#elif defined(__linux__) || defined(__APPLE__) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__QNX__) || defined(__VXWORKS__)  /* Linux, MacOS, NetBSD, FreeBSD, QNX or VXWORKS */
# define _FENV_PUBAPI                             /* do not change this line! */
#else                                             /* Windows static */
# define _FENV_PUBAPI __cdecl
#endif

/*
   C99 Floating-point Exception Functions

   These functions allow a user to query or update the status field 0 (s0)
   exception sticky bits. feraiseexcept() will set the inexact flag on
   both overflow and underflow.
*/
extern int  _FENV_PUBAPI feclearexcept (int __excepts) ;
extern int  _FENV_PUBAPI fegetexceptflag (fexcept_t *__flagp, int __excepts) ;
extern int  _FENV_PUBAPI feraiseexcept (int __excepts) ;
extern int  _FENV_PUBAPI fesetexceptflag (const fexcept_t *__flagp, int __excepts) ;
extern int  _FENV_PUBAPI fetestexcept (int __excepts) ;

/*
   C99 Rounding Control Functions

   These functions allow a user to manipulate the rounding control bits.
   Any change in rounding mode affects the rounding control of status
   field 0 (s0) , status field 2 (s2), and status field 3 (s3). The
   software conventions mandate that all control bits in these status
   field remain consistent, that is except for the trap disable bit.
*/
extern int _FENV_PUBAPI fegetround (void) ;
extern int _FENV_PUBAPI fesetround (int __round) ;

/*
   C99 FP Environment Functions

   These functions allow a user to update or read the entire 64-bit
   floating-point status register (fpsr).
*/
extern int  _FENV_PUBAPI fegetenv (fenv_t *__envp) ;
extern int  _FENV_PUBAPI feholdexcept (fenv_t *__envp) ;
extern int  _FENV_PUBAPI fesetenv (const fenv_t *__envp) ;
extern int  _FENV_PUBAPI feupdateenv (const fenv_t *__envp) ;

/*
   Additional Support Functions

   These functions allow a user to manipulate the trap enable or disable bits.

*/
extern int _FENV_PUBAPI fegetexcept (void) ;
extern int _FENV_PUBAPI feenableexcept (int __excepts) ;
extern int _FENV_PUBAPI fedisableexcept (int __excepts) ;

#undef _FENV_PUBAPI
                                  
#ifdef __STDC_WANT_DEC_FP__ 

#define FE_DEC_DOWNWARD           0x00001
#define FE_DEC_TONEAREST          0x00000
#define FE_DEC_TONEARESTFROMZERO  0x00004
#define FE_DEC_TOWARDZERO         0x00003
#define FE_DEC_UPWARD             0x00002

extern int fe_dec_getround(void);

extern int fe_dec_setround(int __round);

extern void fe_dec_feclearexcept (int __excepts);
extern void fe_dec_fegetexceptflag (fexcept_t *__flagp, int __excepts);
extern void fe_dec_feraiseexcept (int __excepts);
extern void fe_dec_fesetexceptflag (const fexcept_t *__flagp, int __excepts);
extern int  fe_dec_fetestexcept (int __excepts);

#endif  /*__STDC_WANT_DEC_FP__*/


#if defined(__cplusplus)    /* end C linkage */
}
#endif

#endif  /* __FENV_H_INCLUDED */
#endif  /* usage of sys headers */

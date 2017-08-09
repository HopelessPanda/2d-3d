/* file: complex.h */

/*
** Copyright  (C) 1985-2010 Intel Corporation. All rights reserved.
**
** The information and source code contained herein is the exclusive property
** of Intel Corporation and may not be disclosed, examined, or reproduced in
** whole or in part without explicit written authorization from the Company.
**
*/

/* Include_next should be before guard macros in order to at last reach system header */
#if defined(__PURE_SYS_C99_HEADERS__) && !(defined(_WIN32) || defined(_WIN64))
# include_next <complex.h> /* utilize sys header */
#else

#if !defined(__PURE_INTEL_C99_HEADERS__) && !(defined(_WIN32) || defined(_WIN64))
# include_next <complex.h> /* utilize and expand sys header */
#endif

#ifndef __COMPLEX_H_INCLUDED
#define __COMPLEX_H_INCLUDED

/* Check usage correctness */
#if !defined(__INTEL_COMPILER)
# error "This Intel <complex.h> is for use with only the Intel compilers!"
#endif

#if !defined (__unix__) && !defined (__APPLE__) && (!defined (__STDC_VERSION__) || (defined (__STDC_VERSION__) && (__STDC_VERSION__ < 199901L)))
# warning "The /Qstd=c99 compilation option is required to enable C99 support for C programs"
#else

#if defined(__LONG_DOUBLE_SIZE__)	/* Compiler-predefined macros. If defined, should be 128|80|64 */
# define __IMFLONGDOUBLE (__LONG_DOUBLE_SIZE__)
#else
# define __IMFLONGDOUBLE 64
#endif

#if defined(__cplusplus)    /* C linkage */
extern "C" {
#endif

#define _LIBIMF_EXT extern

#if defined(_DLL) && (defined(_WIN32) || defined(_WIN64))   /* Windows DLL */
# define _LIBIMF_PUBAPI __declspec(dllimport) __cdecl
# define _LIBIMF_PUBVAR __declspec(dllimport)
#elif defined(__unix__) || defined (__APPLE__) || defined(__QNX__) || defined(__VXWORKS__)        /* Linux, MacOS, QNX or VXWORKS */
# define _LIBIMF_PUBAPI /* do not change this line! */
# define _LIBIMF_PUBVAR
#else                                             /* Windows static */
# define _LIBIMF_PUBAPI __cdecl
# define _LIBIMF_PUBVAR
#endif

/* for Microsoft compatibility */
#if !defined(__linux__) && !defined(__APPLE__) && !defined(__NetBSD__) && !defined(__FreeBSD__) && !defined(__QNX__) && !defined(__VXWORKS__) && !defined(__INTEL_COMPILER)
#define _LIBIMF_LONGLONG __int64
#else
#define _LIBIMF_LONGLONG long long int
#endif
#define _LIBIMF_LONGINT  long int
#define _LIBIMF_INT      int
#define _LIBIMF_DOUBLE   double
#define _LIBIMF_FLOAT    float
#define _LIBIMF_XDOUBLE  long double
#define _LIBIMF_VOID     void
#define _LIBIMF_CCHAR    const char

#define _LIBIMF_DCOMPLEX _LIBIMF_DOUBLE _Complex
#define _LIBIMF_FCOMPLEX _LIBIMF_FLOAT _Complex
#define _LIBIMF_XCOMPLEX _LIBIMF_XDOUBLE _Complex

/*-- Complex functions --*/

#if !defined(__cplusplus)  /* No _Complex or GNU __complex__ types available for C++ */

#if (!defined(__linux__) && !defined(__APPLE__) && !defined(__NetBSD__) && !defined(__FreeBSD__) && !defined(__QNX__) && !defined(__VXWORKS__)) || defined(__PURE_INTEL_C99_HEADERS__)
# define complex		_Complex
# define _Complex_I	(1.0iF)
# undef I
# define I _Complex_I
#endif

/* Complex trigonometric functions */

#if (!defined(__linux__) && !defined(__APPLE__)) || defined(__PURE_INTEL_C99_HEADERS__)

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI ccos( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI ccosf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI csin( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI csinf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI ctan( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI ctanf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI cacos( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI cacosf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI casin( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI casinf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI catan( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI catanf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DOUBLE   _LIBIMF_PUBAPI carg( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FLOAT    _LIBIMF_PUBAPI cargf( _LIBIMF_FCOMPLEX __z );

#endif  /*!__linux__ && !__APPLE__*/

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI cis( _LIBIMF_DOUBLE __x );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI cisf( _LIBIMF_FLOAT __x );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI cisl( _LIBIMF_XDOUBLE __x );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI cisd( _LIBIMF_DOUBLE __x );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI cisdf( _LIBIMF_FLOAT __x );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI cisdl( _LIBIMF_XDOUBLE __x );

/* Complex exponential functions */

#if (!defined(__linux__) && !defined(__APPLE__)) || defined(__PURE_INTEL_C99_HEADERS__)

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI cexp( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI cexpf( _LIBIMF_FCOMPLEX __z );

#endif  /*!__linux__ && !__APPLE__*/

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI cexp2( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI cexp2f( _LIBIMF_FCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI cexp2l( _LIBIMF_XCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI cexp10( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI cexp10f( _LIBIMF_FCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI cexp10l( _LIBIMF_XCOMPLEX __z );

#if (!defined(__linux__) && !defined(__APPLE__)) || defined(__PURE_INTEL_C99_HEADERS__)

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI ccosh( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI ccoshf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI csinh( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI csinhf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI ctanh( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI ctanhf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI cacosh( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI cacoshf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI casinh( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI casinhf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI catanh( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI catanhf( _LIBIMF_FCOMPLEX __z );

#endif  /*!__linux__ && !__APPLE__*/

/* Complex logarithmic functions */

#if (!defined(__linux__) && !defined(__APPLE__)) || defined(__PURE_INTEL_C99_HEADERS__)

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI clog( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI clogf( _LIBIMF_FCOMPLEX __z );

#endif  /*!__linux__ && !__APPLE__*/

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI clog2( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI clog2f( _LIBIMF_FCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI clog2l( _LIBIMF_XCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI clog10( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI clog10f( _LIBIMF_FCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI clog10l( _LIBIMF_XCOMPLEX __z );

/* Complex power/root/abs functions */

#if (!defined(__linux__) && !defined(__APPLE__)) || defined(__PURE_INTEL_C99_HEADERS__)

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI cpow( _LIBIMF_DCOMPLEX __z, _LIBIMF_DCOMPLEX __c );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI cpowf( _LIBIMF_FCOMPLEX __z, _LIBIMF_FCOMPLEX __c );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI csqrt( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI csqrtf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DOUBLE   _LIBIMF_PUBAPI cabs( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FLOAT    _LIBIMF_PUBAPI cabsf( _LIBIMF_FCOMPLEX __z );

#endif  /*!__linux__ && !__APPLE__*/

/* Other complex functions */

#if (!defined(__linux__) && !defined(__APPLE__)) || defined(__PURE_INTEL_C99_HEADERS__)

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI conj( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI conjf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DCOMPLEX _LIBIMF_PUBAPI cproj( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FCOMPLEX _LIBIMF_PUBAPI cprojf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DOUBLE   _LIBIMF_PUBAPI cimag( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FLOAT    _LIBIMF_PUBAPI cimagf( _LIBIMF_FCOMPLEX __z );

_LIBIMF_EXT _LIBIMF_DOUBLE   _LIBIMF_PUBAPI creal( _LIBIMF_DCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_FLOAT    _LIBIMF_PUBAPI crealf( _LIBIMF_FCOMPLEX __z );

#endif  /*!__linux__ && !__APPLE__*/

#if (!defined(__linux__) && !defined(__APPLE__)) || defined(__PURE_INTEL_C99_HEADERS__)
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI ccosl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI csinl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI ctanl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI cacosl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI casinl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI catanl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XDOUBLE  _LIBIMF_PUBAPI cargl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI cexpl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI ccoshl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI csinhl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI ctanhl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI cacoshl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI casinhl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI catanhl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI clogl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI cpowl( _LIBIMF_XCOMPLEX __z, _LIBIMF_XCOMPLEX __c );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI csqrtl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XDOUBLE  _LIBIMF_PUBAPI cabsl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI conjl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XCOMPLEX _LIBIMF_PUBAPI cprojl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XDOUBLE  _LIBIMF_PUBAPI cimagl( _LIBIMF_XCOMPLEX __z );
_LIBIMF_EXT _LIBIMF_XDOUBLE  _LIBIMF_PUBAPI creall( _LIBIMF_XCOMPLEX __z );

#endif  /*!__linux__ */

#if (__IMFLONGDOUBLE == 64) && !defined(__LONGDOUBLE_AS_DOUBLE)
# define cabsl   cabs
# define cacoshl cacosh
# define cacosl  cacos
# define cargl   carg
# define casinhl casinh
# define casinl  casin
# define catanhl catanh
# define catanl  catan
# define ccoshl  ccosh
# define ccosl   ccos
# define cexp10l cexp10
# define cexp2l  cexp2
# define cexpl   cexp
# define cimagl  cimag
# define cisdl   cisd
# define cisl    cis
# define clog10l clog10
# define clog2l  clog2
# define clogl   clog
# define conjl   conj
# define cpowl   cpow
# define cprojl  cproj
# define creall  creal
# define csinhl  csinh
# define csinl   csin
# define csqrtl  csqrt
# define ctanhl  ctanh
# define ctanl   ctan
#endif /*(__IMFLONGDOUBLE == 64)*/

#endif  /*!__cplusplus*/


#undef _LIBIMF_EXT
#undef _LIBIMF_PUBAPI
#undef _LIBIMF_PUBVAR
#undef _LIBIMF_LONGLONG
#undef _LIBIMF_LONGINT
#undef _LIBIMF_INT
#undef _LIBIMF_DOUBLE
#undef _LIBIMF_FLOAT
#undef _LIBIMF_XDOUBLE
#undef _LIBIMF_VOID
#undef _LIBIMF_CCHAR

# undef _LIBIMF_DCOMPLEX
# undef _LIBIMF_FCOMPLEX
# undef _LIBIMF_XCOMPLEX

#if defined(__cplusplus)    /* end C linkage */
}
#endif

#endif /* __STDC_VERSION__ defined and >= 199901 */

#endif  /*__COMPLEX_H_INCLUDED*/
#endif  /* usage of sys headers */

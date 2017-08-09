/**
*** Copyright (C) 1985-2007 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
**/

/*
 * xmmutils.h
 *
 * NEVER use these routines in "performance" and/or "production"
 *   codes; they are only for debugging, development, etc; no
 *   quality assurance here, just some handy hacks
 *
 * Contents:
 *   . macros for quick access to elements of m128's
 *   . the "shuffle hacks" for handling imm8 param requirements
 *   . lots of printing functions
 *   . set/load/store functions for m64's
 */

#ifndef _INCLUDED_MMUTILS
#define _INCLUDED_MMUTILS

#include <stdio.h>
#include "mmintrin.h"
#include "xmmintrin.h"

/************************************************************************
 *
 * prototypes for all functions in this file
 *
 ************************************************************************/

extern __m64 _m_setqi(int, int);
extern __m64 _m_setqui(unsigned int, unsigned int);
extern __m64 _m_setqw(short, short, short, short);
extern __m64 _m_setquw(unsigned short, unsigned short,
		       unsigned short, unsigned short);
extern __m64 _m_setqb(char, char, char, char,
		      char, char, char, char);
extern __m64 _m_setqub(unsigned char, unsigned char,
		       unsigned char, unsigned char,
		       unsigned char, unsigned char,
		       unsigned char, unsigned char);
extern __m64 _m_setqi2(int);
extern __m64 _m_setqui2(unsigned int);
extern __m64 _m_setqw4(short);
extern __m64 _m_setquw4(unsigned short);
extern __m64 _m_setqb8(char);
extern __m64 _m_setqub8(unsigned char);

extern __m128 _mmi_shuffle_hack(__m128, __m128, unsigned int);
extern int _mmi_pextrw_hack(__m64, int);
extern __m64 _mmi_pinsrw_hack(__m64, int, int);
extern __m64 _mmi_pshufw_hack(__m64, int);

extern void _mm_print_mask(__m128);
extern void _mm_print_ps(__m128);
extern void _m_printqi(__m64);
extern void _m_printqw(__m64);
extern void _m_printqb(__m64);
extern void _m_printqui(__m64);
extern void _m_printquw(__m64);
extern void _m_printqub(__m64);

extern void _mm_fprint_mask(FILE*, __m128);
extern void _mm_fprint_ps(FILE*, __m128);
extern void _m_fprintqi(FILE*, __m64);
extern void _m_fprintqw(FILE*, __m64);
extern void _m_fprintqb(FILE*, __m64);
extern void _m_fprintqui(FILE*, __m64);
extern void _m_fprintquw(FILE*, __m64);
extern void _m_fprintqub(FILE*, __m64);


typedef union {
  struct { float z,y,x,w; } f;
  struct { unsigned int z,y,x,w; } ui;
  __m128 m;
} __u128;

typedef union {
  struct { int z,y; } si;
  struct { short z,y,x,w; } sw;
  struct { char z,y,x,w,v,u,t,s; } sb;
  struct { unsigned int z,y; } ui;
  struct { unsigned short z,y,x,w; } uw;
  struct { unsigned char z,y,x,w,v,u,t,s; } ub;
  __m64 m;
} __u64;



/************************************************************************
 *
 * these macros allow access into the individual elements of m128
 *   structures (real or faux)
 *
 ************************************************************************/

#ifndef _MM_FUNCTIONALITY
/* (these are already defined OK in the faux headers) */
#define _MM_FP0(x) (*((float*)&x+0))
#define _MM_FP1(x) (*((float*)&x+1))
#define _MM_FP2(x) (*((float*)&x+2))
#define _MM_FP3(x) (*((float*)&x+3))
#define _MM_INT0(x) (*((int*)&x+0))
#define _MM_INT1(x) (*((int*)&x+1))
#define _MM_INT2(x) (*((int*)&x+2))
#define _MM_INT3(x) (*((int*)&x+3))
#endif


/************************************************************************
 *
 * The Shuffle Hack
 *
 * Several intrinsics require an immediate, but frequently we just want
 *   to just provide a variable or expression.  This hack gets around
 *   the imm8 restriction by providing 2^8 different calls for the
 *   functions.  The implementation works since the Intel C++ Compiler will do the
 *   obvious constant folding for us.
 *
 ************************************************************************/

#define SHUF_CASE(x) \
	case x: return _mm_shuffle_ps(a, b, x);
#define SHUF_CASES(y) \
    SHUF_CASE(y+0); SHUF_CASE(y+1); SHUF_CASE(y+2);	\
    SHUF_CASE(y+3); SHUF_CASE(y+4); SHUF_CASE(y+5);	\
	SHUF_CASE(y+6); SHUF_CASE(y+7); SHUF_CASE(y+8);	\
	SHUF_CASE(y+9); SHUF_CASE(y+0x0a); SHUF_CASE(y+0x0b);		\
    SHUF_CASE(y+0x0c); SHUF_CASE(y+0x0d); SHUF_CASE(y+0x0e);	\
	SHUF_CASE(y+0x0f);
#define SHUF_CASES_256 \
    SHUF_CASES(0x00); SHUF_CASES(0x10); SHUF_CASES(0x20);	\
	SHUF_CASES(0x30);	SHUF_CASES(0x40); SHUF_CASES(0x50);	\
	SHUF_CASES(0x60); SHUF_CASES(0x70); SHUF_CASES(0x80);	\
	SHUF_CASES(0x90); SHUF_CASES(0xa0); SHUF_CASES(0xb0);	\
    SHUF_CASES(0xc0); SHUF_CASES(0xd0); SHUF_CASES(0xe0);	\
	SHUF_CASES(0xf0);

static __m128 _mmi_shuffle_hack(__m128 a, __m128 b, unsigned int imm8)
{
  switch (imm8 & 0x0ff) { SHUF_CASES_256 }
  fprintf(stderr, "error in _mmi_shuffle_hack\n");
  exit(1);
  return a;
}

#undef SHUF_CASE
#undef SHUF_CASES
#undef SHUF_CASES_256


static int _mmi_pextrw_hack(__m64 a, int imm8)
{
  switch (imm8 & 0x03) {
  case 0: return _m_pextrw(a, 0);
  case 1: return _m_pextrw(a, 1);
  case 2: return _m_pextrw(a, 2);
  case 3: return _m_pextrw(a, 3);
  }
  fprintf(stderr, "error in _mmi_pextrw_hack\n");
  exit(1);
  return imm8;
}

static __m64 _mmi_pinsrw_hack(__m64 m, int w, int imm8)
{
  switch (imm8 & 0x03) {
  case 0: return _m_pinsrw(m, w, 0);
  case 1: return _m_pinsrw(m, w, 1);
  case 2: return _m_pinsrw(m, w, 2);
  case 3: return _m_pinsrw(m, w, 3);
  }
  fprintf(stderr, "error in _mmi_pinsrw_hack\n");
  exit(1);
  return m;
}

#define SHUF_CASE(x) \
	case x: return _m_pshufw(m, x);
#define SHUF_CASES(y) \
    SHUF_CASE(y+0); SHUF_CASE(y+1); SHUF_CASE(y+2);	\
    SHUF_CASE(y+3); SHUF_CASE(y+4); SHUF_CASE(y+5);	\
	SHUF_CASE(y+6); SHUF_CASE(y+7); SHUF_CASE(y+8);	\
	SHUF_CASE(y+9); SHUF_CASE(y+0x0a); SHUF_CASE(y+0x0b);		\
    SHUF_CASE(y+0x0c); SHUF_CASE(y+0x0d); SHUF_CASE(y+0x0e);	\
	SHUF_CASE(y+0x0f);
#define SHUF_CASES_256 \
    SHUF_CASES(0x00); SHUF_CASES(0x10); SHUF_CASES(0x20);	\
	SHUF_CASES(0x30);	SHUF_CASES(0x40); SHUF_CASES(0x50);	\
	SHUF_CASES(0x60); SHUF_CASES(0x70); SHUF_CASES(0x80);	\
	SHUF_CASES(0x90); SHUF_CASES(0xa0); SHUF_CASES(0xb0);	\
    SHUF_CASES(0xc0); SHUF_CASES(0xd0); SHUF_CASES(0xe0);	\
	SHUF_CASES(0xf0);

static __m64 _mmi_pshufw_hack(__m64 m, int imm8)
{
  switch (imm8 & 0xff) { SHUF_CASES_256 }
  fprintf(stderr, "error in _mmi_pshufw_hack\n");
  exit(1);
  return m;
}

#undef SHUF_CASE
#undef SHUF_CASES
#undef SHUF_CASES_256


/************************************************************************
 *
 * print functions for m128's and m64's
 *
 ************************************************************************/

static void _mm_fprint_ps(FILE *fp, __m128 a) {
  __u128 u;
  u.m = a;
  fprintf(fp, "[ %.2f, %.2f, %.2f, %.2f ]\n",
	  u.f.w, u.f.x, u.f.y, u.f.z);
}

static void _mm_fprint_mask(FILE *fp, __m128 a) {
  __u128 u;
  u.m = a;
  fprintf(fp, "[ %8x, %8x, %8x, %8x ]\n",
	  u.ui.w, u.ui.x, u.ui.y, u.ui.z);
}

static void _m_fprintqi(FILE *fp, __m64 a) {
  __u64 u;
  u.m = a;
  _m_empty();
  fprintf(fp, "[ %d, %d ]\n", u.si.z, u.si.y);
}

static void _m_fprintqw(FILE *fp, __m64 a) {
  __u64 u;
  u.m = a;
  _m_empty();
  fprintf(fp, "[ %d, %d, %d, %d ]\n",
	  u.sw.z, u.sw.y, u.sw.x, u.sw.w);
}

static void _m_fprintqb(FILE *fp, __m64 a) {
  __u64 u;
  u.m = a;
  _m_empty();
  fprintf(fp, "[ %d, %d, %d, %d, %d, %d, %d, %d ]\n",
	  u.sb.z, u.sb.y, u.sb.x, u.sb.w,
	  u.sb.v, u.sb.u, u.sb.t, u.sb.s);
}

static void _m_fprintqui(FILE *fp, __m64 a) {
  __u64 u;
  u.m = a;
  _m_empty();
  fprintf(fp, "[ %d, %d ]\n",
	  u.ui.z, u.ui.y);
}

static void _m_fprintquw(FILE *fp, __m64 a) {
  __u64 u;
  u.m = a;
  _m_empty();
  fprintf(fp, "[ %d, %d, %d, %d ]\n",
	  u.uw.z, u.uw.y, u.uw.x, u.uw.w);
}

static void _m_fprintqub(FILE *fp, __m64 a) {
  __u64 u;
  u.m = a;
  _m_empty();
  fprintf(fp, "[ %d, %d, %d, %d, %d, %d, %d, %d ]\n",
	  u.ub.z, u.ub.y, u.ub.x, u.ub.w,
	  u.ub.v, u.ub.u, u.ub.t, u.ub.s);
}

static void _mm_print_ps(__m128 a) { _mm_fprint_ps(stdout, a); }
static void _mm_print_mask(__m128 a) { _mm_fprint_mask(stdout, a); }
static void _m_printqi(__m64 a) { _m_fprintqi(stdout, a); }
static void _m_printqw(__m64 a) { _m_fprintqw(stdout, a); }
static void _m_printqb(__m64 a) { _m_fprintqb(stdout, a); }
static void _m_printqui(__m64 a) { _m_fprintqui(stdout, a); }
static void _m_printquw(__m64 a) { _m_fprintquw(stdout, a); }
static void _m_printqub(__m64 a) { _m_fprintqub(stdout, a); }



/************************************************************************
 *
 * set/load/store functions for m64's
 *
 ************************************************************************/

static __m64 _m_setqi(int a, int b)
{
  __u64 ret;
  ret.si.z = a;
  ret.si.y = b;
  return ret.m;
}

static __m64 _m_setqui(unsigned int a, unsigned int b)
{
  __u64 ret;
  ret.ui.z = a;
  ret.ui.y = b;
  return ret.m;
}

static __m64 _m_setqw(short a, short b, short c, short d)
{
  __u64 ret;
  ret.sw.z = a;
  ret.sw.y = b;
  ret.sw.x = c;
  ret.sw.w = d;
  return ret.m;
}

static __m64 _m_setquw(unsigned short a, unsigned short b,
		       unsigned short c, unsigned short d)
{
  __u64 ret;
  ret.uw.z = a;
  ret.uw.y = b;
  ret.uw.x = c;
  ret.uw.w = d;
  return ret.m;
}

static __m64 _m_setqb(char a, char b, char c, char d,
		      char e, char f, char g, char h)
{
  __u64 ret;
  ret.ub.z = a;
  ret.ub.y = b;
  ret.ub.x = c;
  ret.ub.w = d;
  ret.ub.v = e;
  ret.ub.u = f;
  ret.ub.t = g;
  ret.ub.s = h;
  return ret.m;
}

static __m64 _m_setqub(unsigned char a, unsigned char b,
		       unsigned char c, unsigned char d,
		       unsigned char e, unsigned char f,
		       unsigned char g, unsigned char h)
{
  __u64 ret;
  ret.ub.z = a;
  ret.ub.y = b;
  ret.ub.x = c;
  ret.ub.w = d;
  ret.ub.v = e;
  ret.ub.u = f;
  ret.ub.t = g;
  ret.ub.s = h;
  return ret.m;
}

static __m64 _m_setqi2(int a)
{
  __u64 ret;
  ret.si.z = a;
  ret.si.y = a;
  return ret.m;
}

static __m64 _m_setqui2(unsigned int a)
{
  __u64 ret;
  ret.ui.z = a;
  ret.ui.y = a;
  return ret.m;
}

static __m64 _m_setqw4(short a)
{
  __u64 ret;
  ret.sw.z = a;
  ret.sw.y = a;
  ret.sw.x = a;
  ret.sw.w = a;
  return ret.m;
}

static __m64 _m_setquw4(unsigned short a)
{
  __u64 ret;
  ret.uw.z = a;
  ret.uw.y = a;
  ret.uw.x = a;
  ret.uw.w = a;
  return ret.m;
}

static __m64 _m_setqb8(char a)
{
  __u64 ret;
  ret.ub.z = a;
  ret.ub.y = a;
  ret.ub.x = a;
  ret.ub.w = a;
  ret.ub.v = a;
  ret.ub.u = a;
  ret.ub.t = a;
  ret.ub.s = a;
  return ret.m;
}

static __m64 _m_setqub8(unsigned char a)
{
  __u64 ret;
  ret.ub.z = a;
  ret.ub.y = a;
  ret.ub.x = a;
  ret.ub.w = a;
  ret.ub.v = a;
  ret.ub.u = a;
  ret.ub.t = a;
  ret.ub.s = a;
  return ret.m;
}

#endif

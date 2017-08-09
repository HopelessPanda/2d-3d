//      Copyright  (C) 2010-2011 Intel Corporation.
//      All rights reserved.
//
//        INTEL CORPORATION PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license
// agreement or nondisclosure agreement with Intel Corp.
// and may not be copied or disclosed except in accordance
// with the terms of that agreement.

// chkp.h
// Header file for checked pointers

#ifndef _INCLUDED_CHKP
#define _INCLUDED_CHKP

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
#define DLLEXP __declspec(dllexport)
#else
#define DLLEXP
#endif

// This function removes the bounds information from a pointer, giving
// it bounds that allow access to all of memory.  This can be used when a
// pointer is created by code that is not Point Lookout Soft enabled, to
// ensure that the pointer does not inherit bounds from some previous pointer
// that existed at the same memory location.  The return value is a pointer
// that has the new bounds.
void *__chkp_kill_bounds(void *p);

// This function creates bounds for a pointer, replacing any bounds that might
// have been associated with it previously.  The new bounds are:
void * __chkp_make_bounds(void *p, size_t size);


// This function returns the lower bound associated with p.
DLLEXP void * __chkp_lower_bound(void *p);


// This function returns the upper bound associated with p.
DLLEXP void * __chkp_upper_bound(void *p);


// This enum defines the action to take when reporting a bounds check error.
typedef enum
{
    __CHKP_REPORT_NONE,      // Do nothing
    __CHKP_REPORT_BPT,       // Execute a breakpoint interrupt
    __CHKP_REPORT_LOG,       // Log the error and continue
    __CHKP_REPORT_TERM,      // Log the error and exit the program
    __CHKP_REPORT_CALLBACK   // Call a user defined function
} __chkp_report_option_t;

// This function type is the type of a user defined function that is called
// when the __CHKP_REPORT_CALLBACK option is chosen.
typedef void (*__chkp_callback_t)(char *addr, char *lb, char *ub,
                                  char *ref, size_t size);

// This function sets the action to take when a bounds check error occurs.
//     option is the option from the above enum.
//
//     callback is only used when the option is __CHKP_REPORT_CALLBACK and
//     is the address of the function to call.
DLLEXP void __chkp_report_control(__chkp_report_option_t option,
                                  __chkp_callback_t callback);

#ifdef __cplusplus
}
#endif

#endif  /* _INCLUDED_CHKP */

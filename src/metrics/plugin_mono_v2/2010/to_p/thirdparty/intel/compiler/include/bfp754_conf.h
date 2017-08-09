// File: bfp754_conf.h
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
// Macros for compile-time configuration of the IEEE 754-2008 (binary
// part) compliance library
//
// =============================================================================
//
// Edit history:
//
// 1-001 01-MAY-2009 CQL: Initial revision
// 1-002 08-MAY-2009 CQL: Made default values for configuration
//                        options themselves be symbolic constants
//
// =============================================================================

#ifndef _BFP754_CONF_H_
#define _BFP754_CONF_H_

// Definitions for parameter passing

// If BFP754_CALL_BY_REFERENCE is defined as true then numerical
// arguments and results are passed by reference otherwise they are
// passed by value (except that a pointer is always passed to the
// status flags)
// ****************************************************************************
#define BFP754_CALL_BY_REFERENCE_DEFAULT 0

#ifndef BFP754_CALL_BY_REFERENCE
#define BFP754_CALL_BY_REFERENCE BFP754_CALL_BY_REFERENCE_DEFAULT
#endif
// ****************************************************************************

#endif // ifndef _BFP754_CONF_H_

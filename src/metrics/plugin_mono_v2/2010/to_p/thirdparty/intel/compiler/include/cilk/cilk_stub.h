/*  cilk_stub.h                  -*-C++-*-
 *
 *  Copyright (C) 2009-2011 Intel Corporation.  All Rights Reserved.
 *
 *  The source code contained or described herein and all documents related
 *  to the source code ("Material") are owned by Intel Corporation or its
 *  suppliers or licensors.  Title to the Material remains with Intel
 *  Corporation or its suppliers and licensors.  The Material is protected
 *  by worldwide copyright laws and treaty provisions.  No part of the
 *  Material may be used, copied, reproduced, modified, published, uploaded,
 *  posted, transmitted, distributed, or disclosed in any way without
 *  Intel's prior express written permission.
 *
 *  No license under any patent, copyright, trade secret or other
 *  intellectual property right is granted to or conferred upon you by
 *  disclosure or delivery of the Materials, either expressly, by
 *  implication, inducement, estoppel or otherwise.  Any license under such
 *  intellectual property rights must be express and approved by Intel in
 *  writing.
 *
 */

#ifndef INCLUDED_CILK_STUB_DOT_H
#define INCLUDED_CILK_STUB_DOT_H

/* Definitions for creating a serialization from a Cilk program.
 * These definitions are suitable for use by a compiler that is not
 * Cilk-enabled.
 */

/* Pretend we are a non-Cilk compiler */
#undef __cilk
#define CILK_STUB

/* Replace Cilk keywords with serial equivalents */
#define _Cilk_spawn
#define _Cilk_sync
#define _Cilk_for for

#endif /* ! defined(INCLUDED_CILK_STUB_DOT_H) */

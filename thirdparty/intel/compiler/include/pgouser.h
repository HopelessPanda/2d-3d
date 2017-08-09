/**
*** Copyright (C) 1985-2011 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
**/

/**
*** Provided for use in the instrumentation compilation phase of
*** profile-guided optimizations. Interface to the instrumentation
*** run-time.
**/

#if !defined(_PGO_USER_H)
#define _PGO_USER_H

#if defined __cplusplus
extern "C" {
#endif /* __cplusplus */


#if defined _PGO_INSTRUMENT

extern void _PGOPTI_Prof_Dump_All(void);
extern void _PGOPTI_Prof_Dump(void);
extern void _PGOPTI_Prof_Reset(void);
extern void _PGOPTI_Prof_Dump_And_Reset(void);
extern void _PGOPTI_Set_Interval_Prof_Dump(int interval);

#else

static void _PGOPTI_Prof_Dump_All(void) {}
static void _PGOPTI_Prof_Dump(void) {}
static void _PGOPTI_Prof_Reset(void) {}
static void _PGOPTI_Prof_Dump_And_Reset(void) {}
static void _PGOPTI_Set_Interval_Prof_Dump(int interval) {}

#endif /* _PGO_INSTRUMENT */


#if defined __cplusplus
}
#endif /* __cplusplus */

#endif /* _PGO_USER_H */

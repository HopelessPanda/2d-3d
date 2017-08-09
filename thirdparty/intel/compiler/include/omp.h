/**
*** $Revision: 25999 $
*** $Date: 2011-09-09 05:38:31 -0700 (Fri, 09 Sep 2011) $
**/
/* <copyright>

    Copyright 1985-2011 Intel Corporation.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by Intel Corporation or its
    suppliers or licensors.  Title to the Material remains with Intel
    Corporation or its suppliers and licensors.  The Material is protected
    by worldwide copyright laws and treaty provisions.  No part of the
    Material may be used, copied, reproduced, modified, published, uploaded,
    posted, transmitted, distributed, or disclosed in any way without
    Intel's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by Intel in
    writing.

    Portions of this software are protected under the following patents:
        U.S. Patent 5,812,852
        U.S. Patent 6,792,599

</copyright> */

#ifndef __OMP_H
#   define __OMP_H

#   define KMP_VERSION_MAJOR    5
#   define KMP_VERSION_MINOR    0
#   define KMP_VERSION_BUILD    20110823
#   define KMP_BUILD_DATE       "2011-08-23 20:27:33 UTC"

#   ifdef __cplusplus
    extern "C" {
#   endif

#       define omp_set_num_threads          ompc_set_num_threads
#       define omp_set_dynamic              ompc_set_dynamic
#       define omp_set_nested               ompc_set_nested
#       define omp_set_max_active_levels    ompc_set_max_active_levels
#       define omp_set_schedule             ompc_set_schedule
#       define omp_get_ancestor_thread_num  ompc_get_ancestor_thread_num
#       define omp_get_team_size            ompc_get_team_size


#       define kmp_set_stacksize            kmpc_set_stacksize
#       define kmp_set_stacksize_s          kmpc_set_stacksize_s
#       define kmp_set_blocktime            kmpc_set_blocktime
#       define kmp_set_library              kmpc_set_library
#       define kmp_set_parallel_name        kmpc_set_parallel_name
#       define kmp_set_stats                kmpc_set_stats
#       define kmp_set_defaults             kmpc_set_defaults
#       define kmp_set_affinity_mask_proc   kmpc_set_affinity_mask_proc
#       define kmp_unset_affinity_mask_proc kmpc_unset_affinity_mask_proc
#       define kmp_get_affinity_mask_proc   kmpc_get_affinity_mask_proc

#       define kmp_malloc                   kmpc_malloc
#       define kmp_calloc                   kmpc_calloc
#       define kmp_realloc                  kmpc_realloc
#       define kmp_free                     kmpc_free


/* These entry points are for Cluster OMP.
 */
#       define kmp_sharable_malloc          kmpc_sharable_malloc
#       define kmp_aligned_sharable_malloc  kmpc_aligned_sharable_malloc
#       define kmp_sharable_calloc          kmpc_sharable_calloc
#       define kmp_sharable_realloc         kmpc_sharable_realloc
#       define kmp_sharable_free            kmpc_sharable_free

#       define kmp_private_mmap             kmpc_private_mmap
#       define kmp_sharable_mmap            kmpc_sharable_mmap
#       define kmp_private_munmap           kmpc_private_munmap
#       define kmp_sharable_munmap          kmpc_sharable_munmap

#       define kmp_deferred_atomic_add_i4   kmpc_deferred_atomic_add_i4
#       define kmp_deferred_atomic_add_i8   kmpc_deferred_atomic_add_i8
#       define kmp_deferred_atomic_add_r4   kmpc_deferred_atomic_add_r4
#       define kmp_deferred_atomic_add_r8   kmpc_deferred_atomic_add_r8


#   if defined(_WIN32)
#       define __KAI_KMPC_CONVENTION __cdecl
#   else
#       define __KAI_KMPC_CONVENTION
#   endif

    /* schedule kind constants */
    typedef enum omp_sched_t {
	omp_sched_static  = 1,
	omp_sched_dynamic = 2,
	omp_sched_guided  = 3,
	omp_sched_auto    = 4
    } omp_sched_t;

    /* set API functions */
    extern void   __KAI_KMPC_CONVENTION  omp_set_num_threads (int);
    extern void   __KAI_KMPC_CONVENTION  omp_set_dynamic     (int);
    extern void   __KAI_KMPC_CONVENTION  omp_set_nested      (int);
    extern void   __KAI_KMPC_CONVENTION  omp_set_max_active_levels (int);
    extern void   __KAI_KMPC_CONVENTION  omp_set_schedule          (omp_sched_t, int);

    /* query API functions */
    extern int    __KAI_KMPC_CONVENTION  omp_get_num_threads  (void);
    extern int    __KAI_KMPC_CONVENTION  omp_get_dynamic      (void);
    extern int    __KAI_KMPC_CONVENTION  omp_get_nested       (void);
    extern int    __KAI_KMPC_CONVENTION  omp_get_max_threads  (void);
    extern int    __KAI_KMPC_CONVENTION  omp_get_thread_num   (void);
    extern int    __KAI_KMPC_CONVENTION  omp_get_num_procs    (void);
    extern int    __KAI_KMPC_CONVENTION  omp_in_parallel      (void);
    extern int    __KAI_KMPC_CONVENTION  omp_get_active_level        (void);
    extern int    __KAI_KMPC_CONVENTION  omp_get_level               (void);
    extern int    __KAI_KMPC_CONVENTION  omp_get_ancestor_thread_num (int);
    extern int    __KAI_KMPC_CONVENTION  omp_get_team_size           (int);
    extern int    __KAI_KMPC_CONVENTION  omp_get_thread_limit        (void);
    extern int    __KAI_KMPC_CONVENTION  omp_get_max_active_levels   (void);
    extern void   __KAI_KMPC_CONVENTION  omp_get_schedule            (omp_sched_t *, int *);

    /* lock API functions */
    typedef struct omp_lock_t {
        void * _lk;
    } omp_lock_t;

    extern void   __KAI_KMPC_CONVENTION  omp_init_lock    (omp_lock_t *);
    extern void   __KAI_KMPC_CONVENTION  omp_set_lock     (omp_lock_t *);
    extern void   __KAI_KMPC_CONVENTION  omp_unset_lock   (omp_lock_t *);
    extern void   __KAI_KMPC_CONVENTION  omp_destroy_lock (omp_lock_t *);
    extern int    __KAI_KMPC_CONVENTION  omp_test_lock    (omp_lock_t *);

    /* nested lock API functions */
    typedef struct omp_nest_lock_t {
        void * _lk;
    } omp_nest_lock_t;

    extern void   __KAI_KMPC_CONVENTION  omp_init_nest_lock    (omp_nest_lock_t *);
    extern void   __KAI_KMPC_CONVENTION  omp_set_nest_lock     (omp_nest_lock_t *);
    extern void   __KAI_KMPC_CONVENTION  omp_unset_nest_lock   (omp_nest_lock_t *);
    extern void   __KAI_KMPC_CONVENTION  omp_destroy_nest_lock (omp_nest_lock_t *);
    extern int    __KAI_KMPC_CONVENTION  omp_test_nest_lock    (omp_nest_lock_t *);

    /* time API functions */
    extern double __KAI_KMPC_CONVENTION  omp_get_wtime (void);
    extern double __KAI_KMPC_CONVENTION  omp_get_wtick (void);

#   include <stdlib.h>
    /* kmp API functions */
    extern int    __KAI_KMPC_CONVENTION  kmp_get_stacksize          (void);
    extern void   __KAI_KMPC_CONVENTION  kmp_set_stacksize          (int);
    extern size_t __KAI_KMPC_CONVENTION  kmp_get_stacksize_s        (void);
    extern void   __KAI_KMPC_CONVENTION  kmp_set_stacksize_s        (size_t);
    extern int    __KAI_KMPC_CONVENTION  kmp_get_blocktime          (void);
    extern int    __KAI_KMPC_CONVENTION  kmp_get_library            (void);
    extern void   __KAI_KMPC_CONVENTION  kmp_set_blocktime          (int);
    extern void   __KAI_KMPC_CONVENTION  kmp_set_library            (int);
    extern void   __KAI_KMPC_CONVENTION  kmp_set_parallel_name      (char*);
    extern void   __KAI_KMPC_CONVENTION  kmp_set_stats              (int);
    extern void   __KAI_KMPC_CONVENTION  kmp_set_library_serial     (void);
    extern void   __KAI_KMPC_CONVENTION  kmp_set_library_turnaround (void);
    extern void   __KAI_KMPC_CONVENTION  kmp_set_library_throughput (void);
    extern void   __KAI_KMPC_CONVENTION  kmp_set_defaults           (char const *);

    /* affinity API functions */
    typedef void * kmp_affinity_mask_t;

    extern int    __KAI_KMPC_CONVENTION  kmp_set_affinity             (kmp_affinity_mask_t *);
    extern int    __KAI_KMPC_CONVENTION  kmp_get_affinity             (kmp_affinity_mask_t *);
    extern int    __KAI_KMPC_CONVENTION  kmp_get_affinity_max_proc    (void);
    extern void   __KAI_KMPC_CONVENTION  kmp_create_affinity_mask     (kmp_affinity_mask_t *);
    extern void   __KAI_KMPC_CONVENTION  kmp_destroy_affinity_mask    (kmp_affinity_mask_t *);
    extern int    __KAI_KMPC_CONVENTION  kmp_set_affinity_mask_proc   (int, kmp_affinity_mask_t *);
    extern int    __KAI_KMPC_CONVENTION  kmp_unset_affinity_mask_proc (int, kmp_affinity_mask_t *);
    extern int    __KAI_KMPC_CONVENTION  kmp_get_affinity_mask_proc   (int, kmp_affinity_mask_t *);

    extern void * __KAI_KMPC_CONVENTION  kmp_malloc  (size_t);
    extern void * __KAI_KMPC_CONVENTION  kmp_calloc  (size_t, size_t);
    extern void * __KAI_KMPC_CONVENTION  kmp_realloc (void *, size_t);
    extern void   __KAI_KMPC_CONVENTION  kmp_free    (void *);

    /* Cluster OMP API functions */
    extern void * __KAI_KMPC_CONVENTION  kmp_sharable_malloc  (size_t);
    extern void * __KAI_KMPC_CONVENTION  kmp_aligned_sharable_malloc  (size_t);
    extern void * __KAI_KMPC_CONVENTION  kmp_sharable_calloc  (size_t, size_t);
    extern void * __KAI_KMPC_CONVENTION  kmp_sharable_realloc (void *, size_t);
    extern void   __KAI_KMPC_CONVENTION  kmp_sharable_free    (void *);

    extern void   __KAI_KMPC_CONVENTION  kmp_lock_cond_wait           (omp_lock_t *);
    extern void   __KAI_KMPC_CONVENTION  kmp_lock_cond_signal         (omp_lock_t *);
    extern void   __KAI_KMPC_CONVENTION  kmp_lock_cond_broadcast      (omp_lock_t *);
    extern void   __KAI_KMPC_CONVENTION  kmp_nest_lock_cond_wait      (omp_nest_lock_t *);
    extern void   __KAI_KMPC_CONVENTION  kmp_nest_lock_cond_signal    (omp_nest_lock_t *);
    extern void   __KAI_KMPC_CONVENTION  kmp_nest_lock_cond_broadcast (omp_nest_lock_t *);

    extern int    __KAI_KMPC_CONVENTION  kmp_sharable_mmap(char *, size_t *, void **);
    extern int    __KAI_KMPC_CONVENTION  kmp_sharable_munmap(void *);
    extern int    __KAI_KMPC_CONVENTION  kmp_private_mmap(char *, size_t *, void **);
    extern int    __KAI_KMPC_CONVENTION  kmp_private_munmap(void *);

    extern void   __KAI_KMPC_CONVENTION  kmpc_deferred_atomic_add_i4(void *addr, int val);
    extern void   __KAI_KMPC_CONVENTION  kmpc_deferred_atomic_add_i8(void *addr, long long val);
    extern void   __KAI_KMPC_CONVENTION  kmpc_deferred_atomic_add_r4(void *addr, float val);
    extern void   __KAI_KMPC_CONVENTION  kmpc_deferred_atomic_add_r8(void *addr, double val);

    extern int    __KAI_KMPC_CONVENTION  kmp_get_process_num(void);
    extern int    __KAI_KMPC_CONVENTION  kmp_get_num_processes(void);
    extern int    __KAI_KMPC_CONVENTION  kmp_get_process_thread_num(void);

    extern void   __KAI_KMPC_CONVENTION  kmp_set_warnings_on(void);
    extern void   __KAI_KMPC_CONVENTION  kmp_set_warnings_off(void);
    extern int    __KAI_KMPC_CONVENTION  kmp_is_sharable(void*);

#   undef __KAI_KMPC_CONVENTION

    /* Warning:
       The following typedefs are not standard, deprecated and will be removed in a future release.
    */
    typedef int     omp_int_t;
    typedef double  omp_wtime_t;

#   ifdef __cplusplus
    }
#   endif

#endif /* __OMP_H */


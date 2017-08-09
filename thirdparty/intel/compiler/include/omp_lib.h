!***
!*** $Revision: 25999 $
!*** $Date: 2011-09-09 05:38:31 -0700 (Fri, 09 Sep 2011) $
!***
! <copyright>
!
!     Copyright 1985-2011 Intel Corporation.  All Rights Reserved.
!
!     The source code contained or described herein and all documents related
!     to the source code ("Material") are owned by Intel Corporation or its
!     suppliers or licensors.  Title to the Material remains with Intel
!     Corporation or its suppliers and licensors.  The Material is protected
!     by worldwide copyright laws and treaty provisions.  No part of the
!     Material may be used, copied, reproduced, modified, published, uploaded,
!     posted, transmitted, distributed, or disclosed in any way without
!     Intel's prior express written permission.
!
!     No license under any patent, copyright, trade secret or other
!     intellectual property right is granted to or conferred upon you by
!     disclosure or delivery of the Materials, either expressly, by
!     implication, inducement, estoppel or otherwise.  Any license under such
!     intellectual property rights must be express and approved by Intel in
!     writing.
!
!     Portions of this software are protected under the following patents:
!         U.S. Patent 5,812,852
!         U.S. Patent 6,792,599
!
! </copyright>

!***
!*** Some of the directives for the following routine extend past column 72,
!*** so process this file in 132-column mode.
!***

!dec$ fixedformlinesize:132

      integer,      parameter :: kmp_version_major = 5
      integer,      parameter :: kmp_version_minor = 0
      integer,      parameter :: kmp_version_build = 20110823
      character(*)               kmp_build_date
      parameter( kmp_build_date = '2011-08-23 20:27:33 UTC' )
      integer,      parameter :: openmp_version    = 200805

      integer, parameter :: omp_integer_kind       = 4
      integer, parameter :: omp_logical_kind       = 4
      integer, parameter :: omp_real_kind          = 4
      integer, parameter :: omp_lock_kind          = int_ptr_kind()
      integer, parameter :: omp_nest_lock_kind     = int_ptr_kind()
      integer, parameter :: omp_sched_kind         = omp_integer_kind
      integer, parameter :: kmp_pointer_kind       = int_ptr_kind()
      integer, parameter :: kmp_size_t_kind        = int_ptr_kind()
      integer, parameter :: kmp_affinity_mask_kind = int_ptr_kind()

      integer(kind=omp_sched_kind), parameter :: omp_sched_static  = 1
      integer(kind=omp_sched_kind), parameter :: omp_sched_dynamic = 2
      integer(kind=omp_sched_kind), parameter :: omp_sched_guided  = 3
      integer(kind=omp_sched_kind), parameter :: omp_sched_auto    = 4

      interface

!       ***
!       *** omp_* entry points
!       ***

        subroutine omp_set_num_threads(nthreads)
          use omp_lib_kinds
          integer (kind=omp_integer_kind) nthreads
        end subroutine omp_set_num_threads

        subroutine omp_set_dynamic(enable)
          use omp_lib_kinds
          logical (kind=omp_logical_kind) enable
        end subroutine omp_set_dynamic

        subroutine omp_set_nested(enable)
          use omp_lib_kinds
          logical (kind=omp_logical_kind) enable
        end subroutine omp_set_nested

        function omp_get_num_threads()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) omp_get_num_threads
        end function omp_get_num_threads

        function omp_get_max_threads()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) omp_get_max_threads
        end function omp_get_max_threads

        function omp_get_thread_num()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) omp_get_thread_num
        end function omp_get_thread_num

        function omp_get_num_procs()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) omp_get_num_procs
        end function omp_get_num_procs

        function omp_in_parallel()
          use omp_lib_kinds
          logical (kind=omp_logical_kind) omp_in_parallel
        end function omp_in_parallel

        function omp_get_dynamic()
          use omp_lib_kinds
          logical (kind=omp_logical_kind) omp_get_dynamic
        end function omp_get_dynamic

        function omp_get_nested()
          use omp_lib_kinds
          logical (kind=omp_logical_kind) omp_get_nested
        end function omp_get_nested

        function omp_get_thread_limit()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) omp_get_thread_limit
        end function omp_get_thread_limit

        subroutine omp_set_max_active_levels(max_levels)
          use omp_lib_kinds
          integer (kind=omp_integer_kind) max_levels
        end subroutine omp_set_max_active_levels

        function omp_get_max_active_levels()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) omp_get_max_active_levels
        end function omp_get_max_active_levels

        function omp_get_level()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) omp_get_level
        end function omp_get_level

        function omp_get_active_level()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) omp_get_active_level
        end function omp_get_active_level

        function omp_get_ancestor_thread_num(level)
          use omp_lib_kinds
          integer (kind=omp_integer_kind) level
          integer (kind=omp_integer_kind) omp_get_ancestor_thread_num
        end function omp_get_ancestor_thread_num

        function omp_get_team_size(level)
          use omp_lib_kinds
          integer (kind=omp_integer_kind) level
          integer (kind=omp_integer_kind) omp_get_team_size
        end function omp_get_team_size

        subroutine omp_set_schedule(kind, modifier)
          use omp_lib_kinds
          integer (kind=omp_sched_kind) kind
          integer (kind=omp_integer_kind) modifier
        end subroutine omp_set_schedule

        subroutine omp_get_schedule(kind, modifier)
          use omp_lib_kinds
          integer (kind=omp_sched_kind) kind
          integer (kind=omp_integer_kind) modifier
        end subroutine omp_get_schedule

        function omp_get_wtime()
          use omp_lib_kinds
          double precision omp_get_wtime
        end function omp_get_wtime

        function omp_get_wtick ()
          use omp_lib_kinds
          double precision omp_get_wtick
        end function omp_get_wtick

        subroutine omp_init_lock(lockvar)
          use omp_lib_kinds
          integer (kind=omp_lock_kind) lockvar
        end subroutine omp_init_lock

        subroutine omp_destroy_lock(lockvar)
          use omp_lib_kinds
          integer (kind=omp_lock_kind) lockvar
        end subroutine omp_destroy_lock

        subroutine omp_set_lock(lockvar)
          use omp_lib_kinds
          integer (kind=omp_lock_kind) lockvar
        end subroutine omp_set_lock

        subroutine omp_unset_lock(lockvar)
          use omp_lib_kinds
          integer (kind=omp_lock_kind) lockvar
        end subroutine omp_unset_lock

        function omp_test_lock(lockvar)
          use omp_lib_kinds
          logical (kind=omp_logical_kind) omp_test_lock
          integer (kind=omp_lock_kind) lockvar
        end function omp_test_lock

        subroutine omp_init_nest_lock(lockvar)
          use omp_lib_kinds
          integer (kind=omp_nest_lock_kind) lockvar
        end subroutine omp_init_nest_lock

        subroutine omp_destroy_nest_lock(lockvar)
          use omp_lib_kinds
          integer (kind=omp_nest_lock_kind) lockvar
        end subroutine omp_destroy_nest_lock

        subroutine omp_set_nest_lock(lockvar)
          use omp_lib_kinds
          integer (kind=omp_nest_lock_kind) lockvar
        end subroutine omp_set_nest_lock

        subroutine omp_unset_nest_lock(lockvar)
          use omp_lib_kinds
          integer (kind=omp_nest_lock_kind) lockvar
        end subroutine omp_unset_nest_lock

        function omp_test_nest_lock(lockvar)
          use omp_lib_kinds
          integer (kind=omp_integer_kind) omp_test_nest_lock
          integer (kind=omp_nest_lock_kind) lockvar
        end function omp_test_nest_lock

!       ***
!       *** kmp_* entry points
!       ***

        subroutine kmp_set_parallel_name(name)
          use omp_lib_kinds
          character*(*) name
        end subroutine kmp_set_parallel_name

        subroutine kmp_set_stacksize(size)
          use omp_lib_kinds
          integer (kind=omp_integer_kind) size
        end subroutine kmp_set_stacksize

        subroutine kmp_set_stacksize_s(size)
          use omp_lib_kinds
          integer (kind=kmp_size_t_kind) size
        end subroutine kmp_set_stacksize_s

        subroutine kmp_set_blocktime(msec)
          use omp_lib_kinds
          integer (kind=omp_integer_kind) msec
        end subroutine kmp_set_blocktime

        subroutine kmp_set_library_serial()
          use omp_lib_kinds
        end subroutine kmp_set_library_serial

        subroutine kmp_set_library_turnaround()
          use omp_lib_kinds
        end subroutine kmp_set_library_turnaround

        subroutine kmp_set_library_throughput()
          use omp_lib_kinds
        end subroutine kmp_set_library_throughput

        subroutine kmp_set_library(libnum)
          use omp_lib_kinds
          integer (kind=omp_integer_kind) libnum
        end subroutine kmp_set_library

        subroutine kmp_set_stats(enable)
          use omp_lib_kinds
          logical (kind=omp_logical_kind) enable
        end subroutine kmp_set_stats

        subroutine kmp_set_defaults(string)
          character*(*) string
        end subroutine kmp_set_defaults

        function kmp_get_stacksize()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) kmp_get_stacksize
        end function kmp_get_stacksize

        function kmp_get_stacksize_s()
          use omp_lib_kinds
          integer (kind=kmp_size_t_kind) kmp_get_stacksize_s
        end function kmp_get_stacksize_s

        function kmp_get_blocktime()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) kmp_get_blocktime
        end function kmp_get_blocktime

        function kmp_get_library()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) kmp_get_library
        end function kmp_get_library

        function kmp_set_affinity(mask)
          use omp_lib_kinds
          integer kmp_set_affinity
          integer (kind=kmp_affinity_mask_kind) mask
        end function kmp_set_affinity

        function kmp_get_affinity(mask)
          use omp_lib_kinds
          integer kmp_get_affinity
          integer (kind=kmp_affinity_mask_kind) mask
        end function kmp_get_affinity

        function kmp_get_affinity_max_proc()
          use omp_lib_kinds
          integer kmp_get_affinity_max_proc
        end function kmp_get_affinity_max_proc

        subroutine kmp_create_affinity_mask(mask)
          use omp_lib_kinds
          integer (kind=kmp_affinity_mask_kind) mask
        end subroutine kmp_create_affinity_mask

        subroutine kmp_destroy_affinity_mask(mask)
          use omp_lib_kinds
          integer (kind=kmp_affinity_mask_kind) mask
        end subroutine kmp_destroy_affinity_mask

        function kmp_set_affinity_mask_proc(proc, mask)
          use omp_lib_kinds
          integer kmp_set_affinity_mask_proc
          integer proc
          integer (kind=kmp_affinity_mask_kind) mask
        end function kmp_set_affinity_mask_proc

        function kmp_unset_affinity_mask_proc(proc, mask)
          use omp_lib_kinds
          integer kmp_unset_affinity_mask_proc
          integer proc
          integer (kind=kmp_affinity_mask_kind) mask
        end function kmp_unset_affinity_mask_proc

        function kmp_get_affinity_mask_proc(proc, mask)
          use omp_lib_kinds
          integer kmp_get_affinity_mask_proc
          integer proc
          integer (kind=kmp_affinity_mask_kind) mask
        end function kmp_get_affinity_mask_proc

        function kmp_malloc(size)
          use omp_lib_kinds
          integer (kind=kmp_pointer_kind) kmp_malloc
          integer (kind=kmp_size_t_kind) size
        end function kmp_malloc

        function kmp_calloc(nelem, elsize)
          use omp_lib_kinds
          integer (kind=kmp_pointer_kind) kmp_calloc
          integer (kind=kmp_size_t_kind) nelem
          integer (kind=kmp_size_t_kind) elsize
        end function kmp_calloc

        function kmp_realloc(ptr, size)
          use omp_lib_kinds
          integer (kind=kmp_pointer_kind) kmp_realloc
          integer (kind=kmp_pointer_kind) ptr
          integer (kind=kmp_size_t_kind) size
        end function kmp_realloc

        subroutine kmp_free(ptr)
          use omp_lib_kinds
          integer (kind=kmp_pointer_kind) ptr
        end subroutine kmp_free

!       ***
!       *** kmp_* entry points for Cluster OpenMP
!       ***

        function kmp_sharable_malloc(size)
          use omp_lib_kinds
          integer (kind=kmp_pointer_kind) kmp_sharable_malloc
          integer (kind=kmp_size_t_kind) size
        end function kmp_sharable_malloc

        function kmp_aligned_sharable_malloc(size)
          use omp_lib_kinds
          integer (kind=kmp_pointer_kind) kmp_aligned_sharable_malloc
          integer (kind=kmp_size_t_kind) size
        end function kmp_aligned_sharable_malloc

        function kmp_sharable_calloc(nelem, elsize)
          use omp_lib_kinds
          integer (kind=kmp_pointer_kind) kmp_sharable_calloc
          integer (kind=kmp_size_t_kind) nelem
          integer (kind=kmp_size_t_kind) elsize
        end function kmp_sharable_calloc

        function kmp_sharable_realloc(ptr, size)
          use omp_lib_kinds
          integer (kind=kmp_pointer_kind) kmp_sharable_realloc
          integer (kind=kmp_pointer_kind) ptr
          integer (kind=kmp_size_t_kind) size
        end function kmp_sharable_realloc

        subroutine kmp_sharable_free(ptr)
          use omp_lib_kinds
          integer (kind=kmp_pointer_kind) ptr
        end subroutine kmp_sharable_free

        subroutine kmp_lock_cond_wait(lockvar)
          use omp_lib_kinds
          integer (kind=omp_lock_kind) lockvar
        end subroutine kmp_lock_cond_wait

        subroutine kmp_lock_cond_signal(lockvar)
          use omp_lib_kinds
          integer (kind=omp_lock_kind) lockvar
        end subroutine kmp_lock_cond_signal

        subroutine kmp_lock_cond_broadcast(lockvar)
          use omp_lib_kinds
          integer (kind=omp_lock_kind) lockvar
        end subroutine kmp_lock_cond_broadcast

        subroutine kmp_nest_lock_cond_wait(lockvar)
          use omp_lib_kinds
          integer (kind=omp_nest_lock_kind) lockvar
        end subroutine kmp_nest_lock_cond_wait

        subroutine kmp_nest_lock_cond_signal(lockvar)
          use omp_lib_kinds
          integer (kind=omp_nest_lock_kind) lockvar
        end subroutine kmp_nest_lock_cond_signal

        subroutine kmp_nest_lock_cond_broadcast(lockvar)
          use omp_lib_kinds
          integer (kind=omp_nest_lock_kind) lockvar
        end subroutine kmp_nest_lock_cond_broadcast

        function kmp_get_num_processes()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) kmp_get_num_processes
        end function kmp_get_num_processes

        function kmp_get_process_num()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) kmp_get_process_num
        end function kmp_get_process_num

        function kmp_get_process_thread_num()
          use omp_lib_kinds
          integer (kind=omp_integer_kind) kmp_get_process_thread_num
        end function kmp_get_process_thread_num

        subroutine kmp_set_warnings_on()
          use omp_lib_kinds
        end subroutine kmp_set_warnings_on

        subroutine kmp_set_warnings_off()
          use omp_lib_kinds
        end subroutine kmp_set_warnings_off

        function kmp_is_sharable(ptr)
          use omp_lib_kinds
          logical (kind=omp_logical_kind) kmp_is_sharable
          integer (kind=kmp_pointer_kind) ptr
        end function kmp_is_sharable

        subroutine kmp_deferred_atomic_add_i4(addr, val)
          integer(kind=4) addr
          integer(kind=4) val
        end subroutine kmp_deferred_atomic_add_i4

        subroutine kmp_deferred_atomic_add_i8(addr, val)
          integer(kind=8) addr
          integer(kind=8) val
        end subroutine kmp_deferred_atomic_add_i8

        subroutine kmp_deferred_atomic_add_r4(addr, val)
          real(kind=4) addr
          real(kind=4) val
        end subroutine kmp_deferred_atomic_add_r4

        subroutine kmp_deferred_atomic_add_r8(addr, val)
          real(kind=8) addr
          real(kind=8) val
        end subroutine kmp_deferred_atomic_add_r8

      end interface

!dec$ if defined(_WIN32)
!dec$   if defined(_WIN64) .or. defined(_M_IA64) .or. defined(_M_AMD64)

!***
!*** The Fortran entry points must be in uppercase, even if the /Qlowercase
!*** option is specified.  The alias attribute ensures that the specified
!*** string is used as the entry point.
!***
!*** On the Windows IA-32 architecture, the Fortran entry points have an
!*** underscore prepended.  On the Windows Intel(R) 64 and IA-64
!*** architectures, no underscore is prepended.
!***

!dec$ attributes alias:'OMP_SET_NUM_THREADS'::omp_set_num_threads
!dec$ attributes alias:'OMP_SET_DYNAMIC'::omp_set_dynamic
!dec$ attributes alias:'OMP_SET_NESTED'::omp_set_nested
!dec$ attributes alias:'OMP_GET_NUM_THREADS'::omp_get_num_threads
!dec$ attributes alias:'OMP_GET_MAX_THREADS'::omp_get_max_threads
!dec$ attributes alias:'OMP_GET_THREAD_NUM'::omp_get_thread_num
!dec$ attributes alias:'OMP_GET_NUM_PROCS'::omp_get_num_procs
!dec$ attributes alias:'OMP_IN_PARALLEL'::omp_in_parallel
!dec$ attributes alias:'OMP_GET_DYNAMIC'::omp_get_dynamic
!dec$ attributes alias:'OMP_GET_NESTED'::omp_get_nested
!dec$ attributes alias:'OMP_GET_THREAD_LIMIT'::omp_get_thread_limit
!dec$ attributes alias:'OMP_SET_MAX_ACTIVE_LEVELS'::omp_set_max_active_levels
!dec$ attributes alias:'OMP_GET_MAX_ACTIVE_LEVELS'::omp_get_max_active_levels
!dec$ attributes alias:'OMP_GET_LEVEL'::omp_get_level
!dec$ attributes alias:'OMP_GET_ACTIVE_LEVEL'::omp_get_active_level
!dec$ attributes alias:'OMP_GET_ANCESTOR_THREAD_NUM'::omp_get_ancestor_thread_num
!dec$ attributes alias:'OMP_GET_TEAM_SIZE'::omp_get_team_size
!dec$ attributes alias:'OMP_SET_SCHEDULE'::omp_set_schedule
!dec$ attributes alias:'OMP_GET_SCHEDULE'::omp_get_schedule
!dec$ attributes alias:'OMP_GET_WTIME'::omp_get_wtime
!dec$ attributes alias:'OMP_GET_WTICK'::omp_get_wtick

!dec$ attributes alias:'omp_init_lock'::omp_init_lock
!dec$ attributes alias:'omp_destroy_lock'::omp_destroy_lock
!dec$ attributes alias:'omp_set_lock'::omp_set_lock
!dec$ attributes alias:'omp_unset_lock'::omp_unset_lock
!dec$ attributes alias:'omp_test_lock'::omp_test_lock
!dec$ attributes alias:'omp_init_nest_lock'::omp_init_nest_lock
!dec$ attributes alias:'omp_destroy_nest_lock'::omp_destroy_nest_lock
!dec$ attributes alias:'omp_set_nest_lock'::omp_set_nest_lock
!dec$ attributes alias:'omp_unset_nest_lock'::omp_unset_nest_lock
!dec$ attributes alias:'omp_test_nest_lock'::omp_test_nest_lock

!dec$ attributes alias:'KMP_SET_PARALLEL_NAME'::kmp_set_parallel_name
!dec$ attributes alias:'KMP_SET_STACKSIZE'::kmp_set_stacksize
!dec$ attributes alias:'KMP_SET_STACKSIZE_S'::kmp_set_stacksize_s
!dec$ attributes alias:'KMP_SET_BLOCKTIME'::kmp_set_blocktime
!dec$ attributes alias:'KMP_SET_LIBRARY_SERIAL'::kmp_set_library_serial
!dec$ attributes alias:'KMP_SET_LIBRARY_TURNAROUND'::kmp_set_library_turnaround
!dec$ attributes alias:'KMP_SET_LIBRARY_THROUGHPUT'::kmp_set_library_throughput
!dec$ attributes alias:'KMP_SET_LIBRARY'::kmp_set_library
!dec$ attributes alias:'KMP_SET_STATS'::kmp_set_stats
!dec$ attributes alias:'KMP_SET_DEFAULTS'::kmp_set_defaults
!dec$ attributes alias:'KMP_GET_STACKSIZE'::kmp_get_stacksize
!dec$ attributes alias:'KMP_GET_STACKSIZE_S'::kmp_get_stacksize_s
!dec$ attributes alias:'KMP_GET_BLOCKTIME'::kmp_get_blocktime
!dec$ attributes alias:'KMP_GET_LIBRARY'::kmp_get_library
!dec$ attributes alias:'KMP_SET_AFFINITY'::kmp_set_affinity
!dec$ attributes alias:'KMP_GET_AFFINITY'::kmp_get_affinity
!dec$ attributes alias:'KMP_GET_AFFINITY_MAX_PROC'::kmp_get_affinity_max_proc
!dec$ attributes alias:'KMP_CREATE_AFFINITY_MASK'::kmp_create_affinity_mask
!dec$ attributes alias:'KMP_DESTROY_AFFINITY_MASK'::kmp_destroy_affinity_mask
!dec$ attributes alias:'KMP_SET_AFFINITY_MASK_PROC'::kmp_set_affinity_mask_proc
!dec$ attributes alias:'KMP_UNSET_AFFINITY_MASK_PROC'::kmp_unset_affinity_mask_proc
!dec$ attributes alias:'KMP_GET_AFFINITY_MASK_PROC'::kmp_get_affinity_mask_proc
!dec$ attributes alias:'KMP_MALLOC'::kmp_malloc
!dec$ attributes alias:'KMP_CALLOC'::kmp_calloc
!dec$ attributes alias:'KMP_REALLOC'::kmp_realloc
!dec$ attributes alias:'KMP_FREE'::kmp_free

!dec$ attributes alias:'KMP_SHARABLE_MALLOC'::kmp_sharable_malloc
!dec$ attributes alias:'KMP_ALIGNED_SHARABLE_MALLOC'::kmp_aligned_sharable_malloc
!dec$ attributes alias:'KMP_SHARABLE_CALLOC'::kmp_sharable_calloc
!dec$ attributes alias:'KMP_SHARABLE_REALLOC'::kmp_sharable_realloc
!dec$ attributes alias:'KMP_SHARABLE_FREE'::kmp_sharable_free
!dec$ attributes alias:'KMP_LOCK_COND_WAIT'::kmp_lock_cond_wait
!dec$ attributes alias:'KMP_LOCK_COND_SIGNAL'::kmp_lock_cond_signal
!dec$ attributes alias:'KMP_LOCK_COND_BROADCAST'::kmp_lock_cond_broadcast
!dec$ attributes alias:'KMP_NEST_LOCK_COND_WAIT'::kmp_nest_lock_cond_wait
!dec$ attributes alias:'KMP_NEST_LOCK_COND_SIGNAL'::kmp_nest_lock_cond_signal
!dec$ attributes alias:'KMP_NEST_LOCK_COND_BROADCAST'::kmp_nest_lock_cond_broadcast
!dec$ attributes alias:'KMP_GET_NUM_PROCESSES'::kmp_get_num_processes
!dec$ attributes alias:'KMP_GET_PROCESS_NUM'::kmp_get_process_num
!dec$ attributes alias:'KMP_GET_PROCESS_THREAD_NUM'::kmp_get_process_thread_num
!dec$ attributes alias:'KMP_SET_WARNINGS_ON'::kmp_set_warnings_on
!dec$ attributes alias:'KMP_SET_WARNINGS_OFF'::kmp_set_warnings_off
!dec$ attributes alias:'KMP_IS_SHARABLE'::kmp_is_sharable
!dec$ attributes alias:'KMP_DEFERRED_ATOMIC_ADD_I4'::kmp_deferred_atomic_add_i4
!dec$ attributes alias:'KMP_DEFERRED_ATOMIC_ADD_I8'::kmp_deferred_atomic_add_i8
!dec$ attributes alias:'KMP_DEFERRED_ATOMIC_ADD_R4'::kmp_deferred_atomic_add_r4
!dec$ attributes alias:'KMP_DEFERRED_ATOMIC_ADD_R8'::kmp_deferred_atomic_add_r8

!dec$   else

!***
!*** On Windows IA32, the Fortran entry points have an underscore prepended.
!***

!dec$ attributes alias:'_OMP_SET_NUM_THREADS'::omp_set_num_threads
!dec$ attributes alias:'_OMP_SET_DYNAMIC'::omp_set_dynamic
!dec$ attributes alias:'_OMP_SET_NESTED'::omp_set_nested
!dec$ attributes alias:'_OMP_GET_NUM_THREADS'::omp_get_num_threads
!dec$ attributes alias:'_OMP_GET_MAX_THREADS'::omp_get_max_threads
!dec$ attributes alias:'_OMP_GET_THREAD_NUM'::omp_get_thread_num
!dec$ attributes alias:'_OMP_GET_NUM_PROCS'::omp_get_num_procs
!dec$ attributes alias:'_OMP_IN_PARALLEL'::omp_in_parallel
!dec$ attributes alias:'_OMP_GET_DYNAMIC'::omp_get_dynamic
!dec$ attributes alias:'_OMP_GET_NESTED'::omp_get_nested
!dec$ attributes alias:'_OMP_GET_THREAD_LIMIT'::omp_get_thread_limit
!dec$ attributes alias:'_OMP_SET_MAX_ACTIVE_LEVELS'::omp_set_max_active_levels
!dec$ attributes alias:'_OMP_GET_MAX_ACTIVE_LEVELS'::omp_get_max_active_levels
!dec$ attributes alias:'_OMP_GET_LEVEL'::omp_get_level
!dec$ attributes alias:'_OMP_GET_ACTIVE_LEVEL'::omp_get_active_level
!dec$ attributes alias:'_OMP_GET_ANCESTOR_THREAD_NUM'::omp_get_ancestor_thread_num
!dec$ attributes alias:'_OMP_GET_TEAM_SIZE'::omp_get_team_size
!dec$ attributes alias:'_OMP_SET_SCHEDULE'::omp_set_schedule
!dec$ attributes alias:'_OMP_GET_SCHEDULE'::omp_get_schedule
!dec$ attributes alias:'_OMP_GET_WTIME'::omp_get_wtime
!dec$ attributes alias:'_OMP_GET_WTICK'::omp_get_wtick

!dec$ attributes alias:'_omp_init_lock'::omp_init_lock
!dec$ attributes alias:'_omp_destroy_lock'::omp_destroy_lock
!dec$ attributes alias:'_omp_set_lock'::omp_set_lock
!dec$ attributes alias:'_omp_unset_lock'::omp_unset_lock
!dec$ attributes alias:'_omp_test_lock'::omp_test_lock
!dec$ attributes alias:'_omp_init_nest_lock'::omp_init_nest_lock
!dec$ attributes alias:'_omp_destroy_nest_lock'::omp_destroy_nest_lock
!dec$ attributes alias:'_omp_set_nest_lock'::omp_set_nest_lock
!dec$ attributes alias:'_omp_unset_nest_lock'::omp_unset_nest_lock
!dec$ attributes alias:'_omp_test_nest_lock'::omp_test_nest_lock

!dec$ attributes alias:'_KMP_SET_PARALLEL_NAME'::kmp_set_parallel_name
!dec$ attributes alias:'_KMP_SET_STACKSIZE'::kmp_set_stacksize
!dec$ attributes alias:'_KMP_SET_STACKSIZE_S'::kmp_set_stacksize_s
!dec$ attributes alias:'_KMP_SET_BLOCKTIME'::kmp_set_blocktime
!dec$ attributes alias:'_KMP_SET_LIBRARY_SERIAL'::kmp_set_library_serial
!dec$ attributes alias:'_KMP_SET_LIBRARY_TURNAROUND'::kmp_set_library_turnaround
!dec$ attributes alias:'_KMP_SET_LIBRARY_THROUGHPUT'::kmp_set_library_throughput
!dec$ attributes alias:'_KMP_SET_LIBRARY'::kmp_set_library
!dec$ attributes alias:'_KMP_SET_STATS'::kmp_set_stats
!dec$ attributes alias:'_KMP_SET_DEFAULTS'::kmp_set_defaults
!dec$ attributes alias:'_KMP_GET_STACKSIZE'::kmp_get_stacksize
!dec$ attributes alias:'_KMP_GET_STACKSIZE_S'::kmp_get_stacksize_s
!dec$ attributes alias:'_KMP_GET_BLOCKTIME'::kmp_get_blocktime
!dec$ attributes alias:'_KMP_GET_LIBRARY'::kmp_get_library
!dec$ attributes alias:'_KMP_SET_AFFINITY'::kmp_set_affinity
!dec$ attributes alias:'_KMP_GET_AFFINITY'::kmp_get_affinity
!dec$ attributes alias:'_KMP_GET_AFFINITY_MAX_PROC'::kmp_get_affinity_max_proc
!dec$ attributes alias:'_KMP_CREATE_AFFINITY_MASK'::kmp_create_affinity_mask
!dec$ attributes alias:'_KMP_DESTROY_AFFINITY_MASK'::kmp_destroy_affinity_mask
!dec$ attributes alias:'_KMP_SET_AFFINITY_MASK_PROC'::kmp_set_affinity_mask_proc
!dec$ attributes alias:'_KMP_UNSET_AFFINITY_MASK_PROC'::kmp_unset_affinity_mask_proc
!dec$ attributes alias:'_KMP_GET_AFFINITY_MASK_PROC'::kmp_get_affinity_mask_proc
!dec$ attributes alias:'_KMP_MALLOC'::kmp_malloc
!dec$ attributes alias:'_KMP_CALLOC'::kmp_calloc
!dec$ attributes alias:'_KMP_REALLOC'::kmp_realloc
!dec$ attributes alias:'_KMP_FREE'::kmp_free

!dec$ attributes alias:'_KMP_SHARABLE_MALLOC'::kmp_sharable_malloc
!dec$ attributes alias:'_KMP_ALIGNED_SHARABLE_MALLOC'::kmp_aligned_sharable_malloc
!dec$ attributes alias:'_KMP_SHARABLE_CALLOC'::kmp_sharable_calloc
!dec$ attributes alias:'_KMP_SHARABLE_REALLOC'::kmp_sharable_realloc
!dec$ attributes alias:'_KMP_SHARABLE_FREE'::kmp_sharable_free
!dec$ attributes alias:'_KMP_LOCK_COND_WAIT'::kmp_lock_cond_wait
!dec$ attributes alias:'_KMP_LOCK_COND_SIGNAL'::kmp_lock_cond_signal
!dec$ attributes alias:'_KMP_LOCK_COND_BROADCAST'::kmp_lock_cond_broadcast
!dec$ attributes alias:'_KMP_NEST_LOCK_COND_WAIT'::kmp_nest_lock_cond_wait
!dec$ attributes alias:'_KMP_NEST_LOCK_COND_SIGNAL'::kmp_nest_lock_cond_signal
!dec$ attributes alias:'_KMP_NEST_LOCK_COND_BROADCAST'::kmp_nest_lock_cond_broadcast
!dec$ attributes alias:'_KMP_GET_NUM_PROCESSES'::kmp_get_num_processes
!dec$ attributes alias:'_KMP_GET_PROCESS_NUM'::kmp_get_process_num
!dec$ attributes alias:'_KMP_GET_PROCESS_THREAD_NUM'::kmp_get_process_thread_num
!dec$ attributes alias:'_KMP_SET_WARNINGS_ON'::kmp_set_warnings_on
!dec$ attributes alias:'_KMP_SET_WARNINGS_OFF'::kmp_set_warnings_off
!dec$ attributes alias:'_KMP_IS_SHARABLE'::kmp_is_sharable
!dec$ attributes alias:'_KMP_DEFERRED_ATOMIC_ADD_I4'::kmp_deferred_atomic_add_i4
!dec$ attributes alias:'_KMP_DEFERRED_ATOMIC_ADD_I8'::kmp_deferred_atomic_add_i8
!dec$ attributes alias:'_KMP_DEFERRED_ATOMIC_ADD_R4'::kmp_deferred_atomic_add_r4
!dec$ attributes alias:'_KMP_DEFERRED_ATOMIC_ADD_R8'::kmp_deferred_atomic_add_r8

!dec$   endif
!dec$ endif

!dec$ if defined(__linux)

!***
!*** The Linux entry points are in lowercase, with an underscore appended.
!***

!dec$ attributes alias:'omp_set_num_threads_'::omp_set_num_threads
!dec$ attributes alias:'omp_set_dynamic_'::omp_set_dynamic
!dec$ attributes alias:'omp_set_nested_'::omp_set_nested
!dec$ attributes alias:'omp_get_num_threads_'::omp_get_num_threads
!dec$ attributes alias:'omp_get_max_threads_'::omp_get_max_threads
!dec$ attributes alias:'omp_get_thread_num_'::omp_get_thread_num
!dec$ attributes alias:'omp_get_num_procs_'::omp_get_num_procs
!dec$ attributes alias:'omp_in_parallel_'::omp_in_parallel
!dec$ attributes alias:'omp_get_dynamic_'::omp_get_dynamic
!dec$ attributes alias:'omp_get_nested_'::omp_get_nested
!dec$ attributes alias:'omp_get_thread_limit_'::omp_get_thread_limit
!dec$ attributes alias:'omp_set_max_active_levels_'::omp_set_max_active_levels
!dec$ attributes alias:'omp_get_max_active_levels_'::omp_get_max_active_levels
!dec$ attributes alias:'omp_get_level_'::omp_get_level
!dec$ attributes alias:'omp_get_active_level_'::omp_get_active_level
!dec$ attributes alias:'omp_get_ancestor_thread_num_'::omp_get_ancestor_thread_num
!dec$ attributes alias:'omp_get_team_size_'::omp_get_team_size
!dec$ attributes alias:'omp_set_schedule_'::omp_set_schedule
!dec$ attributes alias:'omp_get_schedule_'::omp_get_schedule
!dec$ attributes alias:'omp_get_wtime_'::omp_get_wtime
!dec$ attributes alias:'omp_get_wtick_'::omp_get_wtick

!dec$ attributes alias:'omp_init_lock_'::omp_init_lock
!dec$ attributes alias:'omp_destroy_lock_'::omp_destroy_lock
!dec$ attributes alias:'omp_set_lock_'::omp_set_lock
!dec$ attributes alias:'omp_unset_lock_'::omp_unset_lock
!dec$ attributes alias:'omp_test_lock_'::omp_test_lock
!dec$ attributes alias:'omp_init_nest_lock_'::omp_init_nest_lock
!dec$ attributes alias:'omp_destroy_nest_lock_'::omp_destroy_nest_lock
!dec$ attributes alias:'omp_set_nest_lock_'::omp_set_nest_lock
!dec$ attributes alias:'omp_unset_nest_lock_'::omp_unset_nest_lock
!dec$ attributes alias:'omp_test_nest_lock_'::omp_test_nest_lock

!dec$ attributes alias:'kmp_set_parallel_name_'::kmp_set_parallel_name
!dec$ attributes alias:'kmp_set_stacksize_'::kmp_set_stacksize
!dec$ attributes alias:'kmp_set_stacksize_s_'::kmp_set_stacksize_s
!dec$ attributes alias:'kmp_set_blocktime_'::kmp_set_blocktime
!dec$ attributes alias:'kmp_set_library_serial_'::kmp_set_library_serial
!dec$ attributes alias:'kmp_set_library_turnaround_'::kmp_set_library_turnaround
!dec$ attributes alias:'kmp_set_library_throughput_'::kmp_set_library_throughput
!dec$ attributes alias:'kmp_set_library_'::kmp_set_library
!dec$ attributes alias:'kmp_set_stats_'::kmp_set_stats
!dec$ attributes alias:'kmp_set_defaults_'::kmp_set_defaults
!dec$ attributes alias:'kmp_get_stacksize_'::kmp_get_stacksize
!dec$ attributes alias:'kmp_get_stacksize_s_'::kmp_get_stacksize_s
!dec$ attributes alias:'kmp_get_blocktime_'::kmp_get_blocktime
!dec$ attributes alias:'kmp_get_library_'::kmp_get_library
!dec$ attributes alias:'kmp_set_affinity_'::kmp_set_affinity
!dec$ attributes alias:'kmp_get_affinity_'::kmp_get_affinity
!dec$ attributes alias:'kmp_get_affinity_max_proc_'::kmp_get_affinity_max_proc
!dec$ attributes alias:'kmp_create_affinity_mask_'::kmp_create_affinity_mask
!dec$ attributes alias:'kmp_destroy_affinity_mask_'::kmp_destroy_affinity_mask
!dec$ attributes alias:'kmp_set_affinity_mask_proc_'::kmp_set_affinity_mask_proc
!dec$ attributes alias:'kmp_unset_affinity_mask_proc_'::kmp_unset_affinity_mask_proc
!dec$ attributes alias:'kmp_get_affinity_mask_proc_'::kmp_get_affinity_mask_proc
!dec$ attributes alias:'kmp_malloc_'::kmp_malloc
!dec$ attributes alias:'kmp_calloc_'::kmp_calloc
!dec$ attributes alias:'kmp_realloc_'::kmp_realloc
!dec$ attributes alias:'kmp_free_'::kmp_free

!dec$ attributes alias:'kmp_sharable_malloc_'::kmp_sharable_malloc
!dec$ attributes alias:'kmp_aligned_sharable_malloc_'::kmp_aligned_sharable_malloc
!dec$ attributes alias:'kmp_sharable_calloc_'::kmp_sharable_calloc
!dec$ attributes alias:'kmp_sharable_realloc_'::kmp_sharable_realloc
!dec$ attributes alias:'kmp_sharable_free_'::kmp_sharable_free
!dec$ attributes alias:'kmp_lock_cond_wait_'::kmp_lock_cond_wait
!dec$ attributes alias:'kmp_lock_cond_signal_'::kmp_lock_cond_signal
!dec$ attributes alias:'kmp_lock_cond_broadcast_'::kmp_lock_cond_broadcast
!dec$ attributes alias:'kmp_nest_lock_cond_wait_'::kmp_nest_lock_cond_wait
!dec$ attributes alias:'kmp_nest_lock_cond_signal_'::kmp_nest_lock_cond_signal
!dec$ attributes alias:'kmp_nest_lock_cond_broadcast_'::kmp_nest_lock_cond_broadcast
!dec$ attributes alias:'kmp_get_num_processes_'::kmp_get_num_processes
!dec$ attributes alias:'kmp_get_process_num_'::kmp_get_process_num
!dec$ attributes alias:'kmp_get_process_thread_num_'::kmp_get_process_thread_num
!dec$ attributes alias:'kmp_set_warnings_on_'::kmp_set_warnings_on
!dec$ attributes alias:'kmp_set_warnings_off_'::kmp_set_warnings_off
!dec$ attributes alias:'kmp_is_sharable_'::kmp_is_sharable
!dec$ attributes alias:'kmp_deferred_atomic_add_i4_'::kmp_deferred_atomic_add_i4
!dec$ attributes alias:'kmp_deferred_atomic_add_i8_'::kmp_deferred_atomic_add_i8
!dec$ attributes alias:'kmp_deferred_atomic_add_r4_'::kmp_deferred_atomic_add_r4
!dec$ attributes alias:'kmp_deferred_atomic_add_r8_'::kmp_deferred_atomic_add_r8

!dec$ endif

!dec$ if defined(__APPLE__)

!***
!*** The Mac entry points are in lowercase, with an both an underscore
!*** appended and an underscore prepended.
!***

!dec$ attributes alias:'_omp_set_num_threads_'::omp_set_num_threads
!dec$ attributes alias:'_omp_set_dynamic_'::omp_set_dynamic
!dec$ attributes alias:'_omp_set_nested_'::omp_set_nested
!dec$ attributes alias:'_omp_get_num_threads_'::omp_get_num_threads
!dec$ attributes alias:'_omp_get_max_threads_'::omp_get_max_threads
!dec$ attributes alias:'_omp_get_thread_num_'::omp_get_thread_num
!dec$ attributes alias:'_omp_get_num_procs_'::omp_get_num_procs
!dec$ attributes alias:'_omp_in_parallel_'::omp_in_parallel
!dec$ attributes alias:'_omp_get_dynamic_'::omp_get_dynamic
!dec$ attributes alias:'_omp_get_nested_'::omp_get_nested
!dec$ attributes alias:'_omp_get_thread_limit_'::omp_get_thread_limit
!dec$ attributes alias:'_omp_set_max_active_levels_'::omp_set_max_active_levels
!dec$ attributes alias:'_omp_get_max_active_levels_'::omp_get_max_active_levels
!dec$ attributes alias:'_omp_get_level_'::omp_get_level
!dec$ attributes alias:'_omp_get_active_level_'::omp_get_active_level
!dec$ attributes alias:'_omp_get_ancestor_thread_num_'::omp_get_ancestor_thread_num
!dec$ attributes alias:'_omp_get_team_size_'::omp_get_team_size
!dec$ attributes alias:'_omp_set_schedule_'::omp_set_schedule
!dec$ attributes alias:'_omp_get_schedule_'::omp_get_schedule
!dec$ attributes alias:'_omp_get_wtime_'::omp_get_wtime
!dec$ attributes alias:'_omp_get_wtick_'::omp_get_wtick

!dec$ attributes alias:'_omp_init_lock_'::omp_init_lock
!dec$ attributes alias:'_omp_destroy_lock_'::omp_destroy_lock
!dec$ attributes alias:'_omp_set_lock_'::omp_set_lock
!dec$ attributes alias:'_omp_unset_lock_'::omp_unset_lock
!dec$ attributes alias:'_omp_test_lock_'::omp_test_lock
!dec$ attributes alias:'_omp_init_nest_lock_'::omp_init_nest_lock
!dec$ attributes alias:'_omp_destroy_nest_lock_'::omp_destroy_nest_lock
!dec$ attributes alias:'_omp_set_nest_lock_'::omp_set_nest_lock
!dec$ attributes alias:'_omp_unset_nest_lock_'::omp_unset_nest_lock
!dec$ attributes alias:'_omp_test_nest_lock_'::omp_test_nest_lock

!dec$ attributes alias:'_kmp_set_parallel_name_'::kmp_set_parallel_name
!dec$ attributes alias:'_kmp_set_stacksize_'::kmp_set_stacksize
!dec$ attributes alias:'_kmp_set_stacksize_s_'::kmp_set_stacksize_s
!dec$ attributes alias:'_kmp_set_blocktime_'::kmp_set_blocktime
!dec$ attributes alias:'_kmp_set_library_serial_'::kmp_set_library_serial
!dec$ attributes alias:'_kmp_set_library_turnaround_'::kmp_set_library_turnaround
!dec$ attributes alias:'_kmp_set_library_throughput_'::kmp_set_library_throughput
!dec$ attributes alias:'_kmp_set_library_'::kmp_set_library
!dec$ attributes alias:'_kmp_set_stats_'::kmp_set_stats
!dec$ attributes alias:'_kmp_set_defaults_'::kmp_set_defaults
!dec$ attributes alias:'_kmp_get_stacksize_'::kmp_get_stacksize
!dec$ attributes alias:'_kmp_get_stacksize_s_'::kmp_get_stacksize_s
!dec$ attributes alias:'_kmp_get_blocktime_'::kmp_get_blocktime
!dec$ attributes alias:'_kmp_get_library_'::kmp_get_library
!dec$ attributes alias:'_kmp_set_affinity_'::kmp_set_affinity
!dec$ attributes alias:'_kmp_get_affinity_'::kmp_get_affinity
!dec$ attributes alias:'_kmp_get_affinity_max_proc_'::kmp_get_affinity_max_proc
!dec$ attributes alias:'_kmp_create_affinity_mask_'::kmp_create_affinity_mask
!dec$ attributes alias:'_kmp_destroy_affinity_mask_'::kmp_destroy_affinity_mask
!dec$ attributes alias:'_kmp_set_affinity_mask_proc_'::kmp_set_affinity_mask_proc
!dec$ attributes alias:'_kmp_unset_affinity_mask_proc_'::kmp_unset_affinity_mask_proc
!dec$ attributes alias:'_kmp_get_affinity_mask_proc_'::kmp_get_affinity_mask_proc
!dec$ attributes alias:'_kmp_malloc_'::kmp_malloc
!dec$ attributes alias:'_kmp_calloc_'::kmp_calloc
!dec$ attributes alias:'_kmp_realloc_'::kmp_realloc
!dec$ attributes alias:'_kmp_free_'::kmp_free

!dec$ attributes alias:'_kmp_sharable_malloc_'::kmp_sharable_malloc
!dec$ attributes alias:'_kmp_aligned_sharable_malloc_'::kmp_aligned_sharable_malloc
!dec$ attributes alias:'_kmp_sharable_calloc_'::kmp_sharable_calloc
!dec$ attributes alias:'_kmp_sharable_realloc_'::kmp_sharable_realloc
!dec$ attributes alias:'_kmp_sharable_free_'::kmp_sharable_free
!dec$ attributes alias:'_kmp_lock_cond_wait_'::kmp_lock_cond_wait
!dec$ attributes alias:'_kmp_lock_cond_signal_'::kmp_lock_cond_signal
!dec$ attributes alias:'_kmp_lock_cond_broadcast_'::kmp_lock_cond_broadcast
!dec$ attributes alias:'_kmp_nest_lock_cond_wait_'::kmp_nest_lock_cond_wait
!dec$ attributes alias:'_kmp_nest_lock_cond_signal_'::kmp_nest_lock_cond_signal
!dec$ attributes alias:'_kmp_nest_lock_cond_broadcast_'::kmp_nest_lock_cond_broadcast
!dec$ attributes alias:'_kmp_get_num_processes_'::kmp_get_num_processes
!dec$ attributes alias:'_kmp_get_process_num_'::kmp_get_process_num
!dec$ attributes alias:'_kmp_get_process_thread_num_'::kmp_get_process_thread_num
!dec$ attributes alias:'_kmp_set_warnings_on_'::kmp_set_warnings_on
!dec$ attributes alias:'_kmp_set_warnings_off_'::kmp_set_warnings_off
!dec$ attributes alias:'_kmp_is_sharable_'::kmp_is_sharable
!dec$ attributes alias:'_kmp_deferred_atomic_add_i4_'::kmp_deferred_atomic_add_i4
!dec$ attributes alias:'_kmp_deferred_atomic_add_i8_'::kmp_deferred_atomic_add_i8
!dec$ attributes alias:'_kmp_deferred_atomic_add_r4_'::kmp_deferred_atomic_add_r4
!dec$ attributes alias:'_kmp_deferred_atomic_add_r8_'::kmp_deferred_atomic_add_r8

!dec$ endif



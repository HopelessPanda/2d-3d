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

      module omp_lib_kinds

        use, intrinsic :: iso_c_binding

        integer, parameter :: omp_integer_kind       = c_int
        integer, parameter :: omp_logical_kind       = 4
        integer, parameter :: omp_real_kind          = c_float
        integer, parameter :: kmp_double_kind        = c_double
        integer, parameter :: omp_lock_kind          = c_intptr_t
        integer, parameter :: omp_nest_lock_kind     = c_intptr_t
        integer, parameter :: omp_sched_kind         = omp_integer_kind
        integer, parameter :: kmp_pointer_kind       = c_intptr_t
        integer, parameter :: kmp_size_t_kind        = c_size_t
        integer, parameter :: kmp_affinity_mask_kind = c_intptr_t

      end module omp_lib_kinds

      module omp_lib

        use omp_lib_kinds

        integer,      parameter :: openmp_version    = 200805
        integer,      parameter :: kmp_version_major = 5
        integer,      parameter :: kmp_version_minor = 0
        integer,      parameter :: kmp_version_build = 20110823
        character(*)               kmp_build_date
        parameter( kmp_build_date = '2011-08-23 20:27:33 UTC' )

        integer(kind=omp_sched_kind), parameter :: omp_sched_static  = 1
        integer(kind=omp_sched_kind), parameter :: omp_sched_dynamic = 2
        integer(kind=omp_sched_kind), parameter :: omp_sched_guided  = 3
        integer(kind=omp_sched_kind), parameter :: omp_sched_auto    = 4

        interface

!         ***
!         *** omp_* entry points
!         ***

          subroutine omp_set_num_threads(nthreads) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind), value :: nthreads
          end subroutine omp_set_num_threads

          subroutine omp_set_dynamic(enable) bind(c)
            use omp_lib_kinds
            logical (kind=omp_logical_kind), value :: enable
          end subroutine omp_set_dynamic

          subroutine omp_set_nested(enable) bind(c)
            use omp_lib_kinds
            logical (kind=omp_logical_kind), value :: enable
          end subroutine omp_set_nested

          function omp_get_num_threads() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) omp_get_num_threads
          end function omp_get_num_threads

          function omp_get_max_threads() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) omp_get_max_threads
          end function omp_get_max_threads

          function omp_get_thread_num() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) omp_get_thread_num
          end function omp_get_thread_num

          function omp_get_num_procs() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) omp_get_num_procs
          end function omp_get_num_procs

          function omp_in_parallel() bind(c)
            use omp_lib_kinds
            logical (kind=omp_logical_kind) omp_in_parallel
          end function omp_in_parallel

          function omp_get_dynamic() bind(c)
            use omp_lib_kinds
            logical (kind=omp_logical_kind) omp_get_dynamic
          end function omp_get_dynamic

          function omp_get_nested() bind(c)
            use omp_lib_kinds
            logical (kind=omp_logical_kind) omp_get_nested
          end function omp_get_nested

          function omp_get_thread_limit() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) omp_get_thread_limit
          end function omp_get_thread_limit

          subroutine omp_set_max_active_levels(max_levels) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind), value :: max_levels
          end subroutine omp_set_max_active_levels

          function omp_get_max_active_levels() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) omp_get_max_active_levels
          end function omp_get_max_active_levels

          function omp_get_level() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) :: omp_get_level
          end function omp_get_level

          function omp_get_active_level() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) :: omp_get_active_level
          end function omp_get_active_level

          function omp_get_ancestor_thread_num(level) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) omp_get_ancestor_thread_num
            integer (kind=omp_integer_kind), value :: level
          end function omp_get_ancestor_thread_num

          function omp_get_team_size(level) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) omp_get_team_size
            integer (kind=omp_integer_kind), value :: level
          end function omp_get_team_size

          subroutine omp_set_schedule(kind, modifier) bind(c)
            use omp_lib_kinds
            integer (kind=omp_sched_kind), value :: kind
            integer (kind=omp_integer_kind), value :: modifier
          end subroutine omp_set_schedule

          subroutine omp_get_schedule(kind, modifier) bind(c)
            use omp_lib_kinds
            integer (kind=omp_sched_kind)   :: kind
            integer (kind=omp_integer_kind) :: modifier
          end subroutine omp_get_schedule

          function omp_get_wtime() bind(c)
            use omp_lib_kinds
            real (kind=kmp_double_kind) omp_get_wtime
          end function omp_get_wtime

          function omp_get_wtick() bind(c)
            use omp_lib_kinds
            real (kind=kmp_double_kind) omp_get_wtick
          end function omp_get_wtick

          subroutine omp_init_lock(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_lock_kind) lockvar
          end subroutine omp_init_lock

          subroutine omp_destroy_lock(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_lock_kind) lockvar
          end subroutine omp_destroy_lock

          subroutine omp_set_lock(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_lock_kind) lockvar
          end subroutine omp_set_lock

          subroutine omp_unset_lock(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_lock_kind) lockvar
          end subroutine omp_unset_lock

          function omp_test_lock(lockvar) bind(c)
            use omp_lib_kinds
            logical (kind=omp_logical_kind) omp_test_lock
            integer (kind=omp_lock_kind) lockvar
          end function omp_test_lock

          subroutine omp_init_nest_lock(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_nest_lock_kind) lockvar
          end subroutine omp_init_nest_lock

          subroutine omp_destroy_nest_lock(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_nest_lock_kind) lockvar
          end subroutine omp_destroy_nest_lock

          subroutine omp_set_nest_lock(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_nest_lock_kind) lockvar
          end subroutine omp_set_nest_lock

          subroutine omp_unset_nest_lock(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_nest_lock_kind) lockvar
          end subroutine omp_unset_nest_lock

          function omp_test_nest_lock(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) omp_test_nest_lock
            integer (kind=omp_nest_lock_kind) lockvar
          end function omp_test_nest_lock

!         ***
!         *** kmp_* entry points
!         ***

          subroutine kmp_set_parallel_name(name) bind(c)
            use omp_lib_kinds
            character (kind=c_char) :: name(*)
          end subroutine kmp_set_parallel_name

          subroutine kmp_set_stacksize(size) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind), value :: size
          end subroutine kmp_set_stacksize

          subroutine kmp_set_stacksize_s(size) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_size_t_kind), value :: size
          end subroutine kmp_set_stacksize_s

          subroutine kmp_set_blocktime(msec) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind), value :: msec
          end subroutine kmp_set_blocktime

          subroutine kmp_set_library_serial() bind(c)
            use omp_lib_kinds
          end subroutine kmp_set_library_serial

          subroutine kmp_set_library_turnaround() bind(c)
            use omp_lib_kinds
          end subroutine kmp_set_library_turnaround

          subroutine kmp_set_library_throughput() bind(c)
            use omp_lib_kinds
          end subroutine kmp_set_library_throughput

          subroutine kmp_set_library(libnum) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind), value :: libnum
          end subroutine kmp_set_library

          subroutine kmp_set_stats(enable) bind(c)
            use omp_lib_kinds
            logical (kind=omp_logical_kind), value :: enable
          end subroutine kmp_set_stats

          subroutine kmp_set_defaults(string) bind(c)
            use, intrinsic :: iso_c_binding
            character (kind=c_char) :: string(*)
          end subroutine kmp_set_defaults

          function kmp_get_stacksize() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_get_stacksize
          end function kmp_get_stacksize

          function kmp_get_stacksize_s() bind(c)
            use omp_lib_kinds
            integer (kind=kmp_size_t_kind) kmp_get_stacksize_s
          end function kmp_get_stacksize_s

          function kmp_get_blocktime() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_get_blocktime
          end function kmp_get_blocktime

          function kmp_get_library() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_get_library
          end function kmp_get_library

          function kmp_set_affinity(mask) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_set_affinity
            integer (kind=kmp_affinity_mask_kind) mask
          end function kmp_set_affinity

          function kmp_get_affinity(mask) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_get_affinity
            integer (kind=kmp_affinity_mask_kind) mask
          end function kmp_get_affinity

          function kmp_get_affinity_max_proc() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_get_affinity_max_proc
          end function kmp_get_affinity_max_proc

          subroutine kmp_create_affinity_mask(mask) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_affinity_mask_kind) mask
          end subroutine kmp_create_affinity_mask

          subroutine kmp_destroy_affinity_mask(mask) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_affinity_mask_kind) mask
          end subroutine kmp_destroy_affinity_mask

          function kmp_set_affinity_mask_proc(proc, mask) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_set_affinity_mask_proc
            integer (kind=omp_integer_kind), value :: proc
            integer (kind=kmp_affinity_mask_kind) mask
          end function kmp_set_affinity_mask_proc

          function kmp_unset_affinity_mask_proc(proc, mask) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_unset_affinity_mask_proc
            integer (kind=omp_integer_kind), value :: proc
            integer (kind=kmp_affinity_mask_kind) mask
          end function kmp_unset_affinity_mask_proc

          function kmp_get_affinity_mask_proc(proc, mask) bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_get_affinity_mask_proc
            integer (kind=omp_integer_kind), value :: proc
            integer (kind=kmp_affinity_mask_kind) mask
          end function kmp_get_affinity_mask_proc

          function kmp_malloc(size) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_pointer_kind) kmp_malloc
            integer (kind=kmp_size_t_kind), value :: size
          end function kmp_malloc

          function kmp_calloc(nelem, elsize) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_pointer_kind) kmp_calloc
            integer (kind=kmp_size_t_kind), value :: nelem
            integer (kind=kmp_size_t_kind), value :: elsize
          end function kmp_calloc

          function kmp_realloc(ptr, size) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_pointer_kind) kmp_realloc
            integer (kind=kmp_pointer_kind), value :: ptr
            integer (kind=kmp_size_t_kind), value :: size
          end function kmp_realloc

          subroutine kmp_free(ptr) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_pointer_kind), value :: ptr
          end subroutine kmp_free

!         ***
!         *** kmp_* entry points for Cluster OpenMP
!         ***

          function kmp_sharable_malloc(size) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_pointer_kind) kmp_sharable_malloc
            integer (kind=kmp_size_t_kind), value :: size
          end function kmp_sharable_malloc

          function kmp_aligned_sharable_malloc(size) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_pointer_kind) kmp_aligned_sharable_malloc
            integer (kind=kmp_size_t_kind), value :: size
          end function kmp_aligned_sharable_malloc

          function kmp_sharable_calloc(nelem, elsize) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_pointer_kind) kmp_sharable_calloc
            integer (kind=kmp_size_t_kind), value :: nelem
            integer (kind=kmp_size_t_kind), value :: elsize
          end function kmp_sharable_calloc

          function kmp_sharable_realloc(ptr, size) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_pointer_kind) kmp_sharable_realloc
            integer (kind=kmp_pointer_kind), value :: ptr
            integer (kind=kmp_size_t_kind), value :: size
          end function kmp_sharable_realloc

          subroutine kmp_sharable_free(ptr) bind(c)
            use omp_lib_kinds
            integer (kind=kmp_pointer_kind), value :: ptr
          end subroutine kmp_sharable_free

          subroutine kmp_lock_cond_wait(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_lock_kind) lockvar
          end subroutine kmp_lock_cond_wait

          subroutine kmp_lock_cond_signal(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_lock_kind) lockvar
          end subroutine kmp_lock_cond_signal

          subroutine kmp_lock_cond_broadcast(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_lock_kind) lockvar
          end subroutine kmp_lock_cond_broadcast

          subroutine kmp_nest_lock_cond_wait(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_nest_lock_kind) lockvar
          end subroutine kmp_nest_lock_cond_wait

          subroutine kmp_nest_lock_cond_signal(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_nest_lock_kind) lockvar
          end subroutine kmp_nest_lock_cond_signal

          subroutine kmp_nest_lock_cond_broadcast(lockvar) bind(c)
            use omp_lib_kinds
            integer (kind=omp_nest_lock_kind) lockvar
          end subroutine kmp_nest_lock_cond_broadcast

          function kmp_get_num_processes() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_get_num_processes
          end function kmp_get_num_processes

          function kmp_get_process_num() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_get_process_num
          end function kmp_get_process_num

          function kmp_get_process_thread_num() bind(c)
            use omp_lib_kinds
            integer (kind=omp_integer_kind) kmp_get_process_thread_num
          end function kmp_get_process_thread_num

          subroutine kmp_set_warnings_on() bind(c)
            use omp_lib_kinds
          end subroutine kmp_set_warnings_on

          subroutine kmp_set_warnings_off() bind(c)
            use omp_lib_kinds
          end subroutine kmp_set_warnings_off

          function kmp_is_sharable(ptr) bind(c)
            use omp_lib_kinds
            logical (kind=omp_logical_kind) kmp_is_sharable
            integer (kind=kmp_pointer_kind), value :: ptr
          end function kmp_is_sharable

          subroutine kmp_deferred_atomic_add_i4(var, val) bind(c)
            use, intrinsic :: iso_c_binding
            integer (kind=c_int) var
            integer (kind=c_int), value :: val
          end subroutine kmp_deferred_atomic_add_i4

          subroutine kmp_deferred_atomic_add_i8(var, val) bind(c)
            use, intrinsic :: iso_c_binding
            integer (kind=c_long_long) var
            integer (kind=c_long_long), value :: val
          end subroutine kmp_deferred_atomic_add_i8

          subroutine kmp_deferred_atomic_add_r4(var, val) bind(c)
            use, intrinsic :: iso_c_binding
            real (kind=c_float) var
            real (kind=c_float), value :: val
          end subroutine kmp_deferred_atomic_add_r4

          subroutine kmp_deferred_atomic_add_r8(var, val) bind(c)
            use, intrinsic :: iso_c_binding
            real (kind=c_double) var
            real (kind=c_double), value :: val
          end subroutine kmp_deferred_atomic_add_r8

        end interface

      end module omp_lib

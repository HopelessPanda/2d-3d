/**
*** Copyright (C) 2009-2010 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
**/

#ifndef _INC_STDATOMIC
#define _INC_STDATOMIC

#if !__cplusplus

#include <stddef.h>

#if __STDC_VERSION__ >= 199900
#  include <stdbool.h>
#else

#  ifndef bool
#    if __BOOL_DEFINED || _BOOL
#      define bool _Bool
#    else
#      define bool char
#    endif
#  endif

#  ifndef true
#    define true 1
#  endif
#  ifndef false
#    define false 0
#  endif

#endif

#else

#include <cstddef>

#if __EDG_VERSION__ >= 401 && defined(__STDC_HOSTED__)
#  ifndef __USE_DEFAULTED
#    define __USE_DEFAULTED 1
#  endif
#  ifndef __USE_DELETED
#    define __USE_DELETED 1
#  endif
#endif

#if __USE_DEFAULTED
#  define _DEFAULTED = default;
#else
#  define _DEFAULTED {}
#endif

#if __USE_DELETED
#  define _DELETED(x) x = delete;
#else
#  define _DELETED(x) private: x;
#endif

#if __USE_CONSTEXPR
#  define _CONSTEXPR constexpr
#else
#  define _CONSTEXPR
#endif

namespace std {

#endif

#if defined (_MSC_VER)
#  define _STRONG_INLINE __forceinline
#elif defined(__GNUC__)
#  define _STRONG_INLINE inline __attribute__((always_inline))
#else
#  define _STRONG_INLINE inline
#endif


typedef enum memory_order {
    memory_order_relaxed, memory_order_consume, memory_order_acquire,
    memory_order_release, memory_order_acq_rel, memory_order_seq_cst
} memory_order;

#define ATOMIC_INTEGRAL_LOCK_FREE 2 // always
#define ATOMIC_ADDRESS_LOCK_FREE 2 // always

typedef struct atomic_flag {
    bool _Val;
#if __cplusplus
    bool test_and_set(memory_order = memory_order_seq_cst) volatile;
    bool test_and_set(memory_order = memory_order_seq_cst);
    void clear(memory_order = memory_order_seq_cst) volatile;
    void clear(memory_order = memory_order_seq_cst);
    atomic_flag() _DEFAULTED
    _DELETED( atomic_flag(const atomic_flag&) )
    _DELETED( atomic_flag& operator=(const atomic_flag&) )
    _DELETED( atomic_flag& operator=(const atomic_flag&) volatile )
#endif
} atomic_flag;

#define ATOMIC_FLAG_INIT { false }

_STRONG_INLINE bool
atomic_flag_test_and_set(
    volatile atomic_flag *_A)
{
    return __atomic_flag_test_and_set_explicit(&_A->_Val, memory_order_seq_cst);
}

#if __cplusplus
_STRONG_INLINE bool
atomic_flag_test_and_set(
    atomic_flag *_A)
{
    return __atomic_flag_test_and_set_explicit(&_A->_Val, memory_order_seq_cst);
}
#endif /* __cplusplus */

_STRONG_INLINE bool
atomic_flag_test_and_set_explicit(
    volatile atomic_flag *_A,
    memory_order _O)
{
    return __atomic_flag_test_and_set_explicit(&_A->_Val, _O);
}

#if __cplusplus
_STRONG_INLINE bool
atomic_flag_test_and_set_explicit(
    atomic_flag *_A,
    memory_order _O)
{
    return __atomic_flag_test_and_set_explicit(&_A->_Val, _O);
}
#endif /* __cplusplus */

_STRONG_INLINE void
atomic_flag_clear(
    volatile atomic_flag *_A)
{
    __atomic_flag_clear_explicit(&_A->_Val, memory_order_seq_cst);
}

#if __cplusplus
_STRONG_INLINE void
atomic_flag_clear(
    atomic_flag *_A)
{
    __atomic_flag_clear_explicit(&_A->_Val, memory_order_seq_cst);
}
#endif /* __cplusplus */

_STRONG_INLINE void
atomic_flag_clear_explicit(
    volatile atomic_flag *_A,
    memory_order _O)
{
    __atomic_flag_clear_explicit(&_A->_Val, _O);
}

#if __cplusplus
_STRONG_INLINE void
atomic_flag_clear_explicit(
    atomic_flag *_A,
    memory_order _O)
{
    __atomic_flag_clear_explicit(&_A->_Val, _O);
}
#endif /* __cplusplus */

#if __cplusplus
_STRONG_INLINE bool
atomic_flag::test_and_set(memory_order _O) volatile
{
    return atomic_flag_test_and_set_explicit(this, _O);
}

_STRONG_INLINE bool
atomic_flag::test_and_set(memory_order _O)
{
    return atomic_flag_test_and_set_explicit(this, _O);
}
_STRONG_INLINE void
atomic_flag::clear(memory_order _O) volatile
{
    return atomic_flag_clear_explicit(this, _O);
}

_STRONG_INLINE void
atomic_flag::clear(memory_order _O)
{
    return atomic_flag_clear_explicit(this, _O);
}
#endif

#define atomic_is_lock_free(a) \
    true
#define atomic_store(a, n) \
    __atomic_store_explicit(&(a)->_Val, (n), memory_order_seq_cst)
#define atomic_store_explicit(a, n, o) \
    __atomic_store_explicit(&(a)->_Val, (n), (o))
#define atomic_load(a) \
    __atomic_load_explicit(&(a)->_Val, memory_order_seq_cst)
#define atomic_load_explicit(a, o) \
    __atomic_load_explicit(&(a)->_Val, (o))
#define atomic_exchange(a, n) \
    __atomic_exchange_explicit(&(a)->_Val, (n), memory_order_seq_cst)
#define atomic_exchange_explicit(a, n, o) \
    __atomic_exchange_explicit(&(a)->_Val, (n), (o))
#define atomic_compare_exchange_strong(a, p, n) \
    __atomic_compare_exchange_strong_explicit(&(a)->_Val, (p), (n), \
        memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_weak(a, p, n) \
    __atomic_compare_exchange_weak_explicit(&(a)->_Val, (p), (n), \
        memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_strong_explicit(a, p, n, s, f) \
    __atomic_compare_exchange_strong_explicit(&(a)->_Val, (p), (n), s, f)
#define atomic_compare_exchange_weak_explicit(a, p, n, s, f) \
    __atomic_compare_exchange_weak_explicit(&(a)->_Val, (p), (n), s, f)
#define atomic_fetch_add(a, n) \
    __atomic_fetch_add_explicit(&(a)->_Val, n, memory_order_seq_cst)
#define atomic_fetch_add_explicit(a, n, o) \
    __atomic_fetch_add_explicit(&(a)->_Val, n, (o))
#define atomic_fetch_sub(a, n) \
    __atomic_fetch_sub_explicit(&(a)->_Val, n, memory_order_seq_cst)
#define atomic_fetch_sub_explicit(a, n, o) \
    __atomic_fetch_sub_explicit(&(a)->_Val, n, (o))
#define atomic_fetch_or(a, n) \
    __atomic_fetch_or_explicit(&(a)->_Val, n, memory_order_seq_cst)
#define atomic_fetch_or_explicit(a, n, o) \
    __atomic_fetch_or_explicit(&(a)->_Val, n, (o))
#define atomic_fetch_xor(a, n) \
    __atomic_fetch_xor_explicit(&(a)->_Val, n, memory_order_seq_cst)
#define atomic_fetch_xor_explicit(a, n, o) \
    __atomic_fetch_xor_explicit(&(a)->_Val, n, (o))
#define atomic_fetch_and(a, n) \
    __atomic_fetch_and_explicit(&(a)->_Val, n, memory_order_seq_cst)
#define atomic_fetch_and_explicit(a, n, o) \
    __atomic_fetch_and_explicit(&(a)->_Val, n, (o))

#if __cplusplus
template<class T>
struct atomic
{
    bool is_lock_free() const volatile;
    bool is_lock_free() const;
    void store(T, memory_order = memory_order_seq_cst) volatile;
    void store(T, memory_order = memory_order_seq_cst);
    T load(memory_order = memory_order_seq_cst) const volatile;
    T load(memory_order = memory_order_seq_cst) const;
    operator T() const volatile;
    operator T() const;
    T exchange(T, memory_order = memory_order_seq_cst) volatile;
    T exchange(T, memory_order = memory_order_seq_cst);
    bool compare_exchange_weak(T&, T, memory_order, memory_order) volatile;
    bool compare_exchange_weak(T&, T, memory_order, memory_order);
    bool compare_exchange_strong(T&, T, memory_order, memory_order) volatile;
    bool compare_exchange_strong(T&, T, memory_order, memory_order);
    bool compare_exchange_weak(T&, T, memory_order = memory_order_seq_cst) volatile;
    bool compare_exchange_weak(T&, T, memory_order = memory_order_seq_cst);
    bool compare_exchange_strong(T&, T, memory_order = memory_order_seq_cst) volatile;
    bool compare_exchange_strong(T&, T, memory_order = memory_order_seq_cst);
    atomic() _DEFAULTED
    _CONSTEXPR atomic(T);
    T operator=(T) volatile;
    T operator=(T);
    _DELETED( atomic(const atomic&) )
    _DELETED( atomic& operator=(const atomic&) )
    _DELETED( atomic& operator=(const atomic&) volatile )
};
#endif

#define _ATOMIC_ITYPE atomic_bool
#define _INTEGRAL bool
#include "atomicint.h"

#define _ADDITIVE 1
#define _REALLY_ADDRESS 1

#define _ATOMIC_ITYPE atomic_address
#define _INTEGRAL void*
#include "atomicint.h"

#define _BITWISE 1

#define _ATOMIC_ITYPE atomic_char
#define _INTEGRAL char
#include "atomicint.h"

#define _ATOMIC_ITYPE atomic_schar
#define _INTEGRAL signed char
#include "atomicint.h"

#define _ATOMIC_ITYPE atomic_uchar
#define _INTEGRAL unsigned char
#include "atomicint.h"

#define _ATOMIC_ITYPE atomic_short
#define _INTEGRAL short
#include "atomicint.h"

#define _ATOMIC_ITYPE atomic_ushort
#define _INTEGRAL unsigned short
#include "atomicint.h"

#define _ATOMIC_ITYPE atomic_int
#define _INTEGRAL int
#include "atomicint.h"

#define _ATOMIC_ITYPE atomic_uint
#define _INTEGRAL unsigned int
#include "atomicint.h"

#if _WIN32 && !_NATIVE_WCHAR_T_DEFINED
typedef atomic_ushort atomic_wchar_t;
#else
#define _ATOMIC_ITYPE atomic_wchar_t
#define _INTEGRAL wchar_t
#include "atomicint.h"
#endif /* _WIN32 && !_NATIVE_WCHAR_T_DEFINED */

#define _ATOMIC_ITYPE atomic_long
#define _INTEGRAL long
#include "atomicint.h"

#define _ATOMIC_ITYPE atomic_ulong
#define _INTEGRAL unsigned long
#include "atomicint.h"

#define _ATOMIC_ITYPE atomic_llong
#define _INTEGRAL long long
#include "atomicint.h"

#define _ATOMIC_ITYPE atomic_ullong
#define _INTEGRAL unsigned long long
#include "atomicint.h"

#if __cplusplus
template <class T>
struct atomic<T*> : atomic_address {
    _STRONG_INLINE void store(T* _P, memory_order _O = memory_order_seq_cst) volatile
    {
        atomic_store_explicit(this, (void *)_P, _O);
    }
    _STRONG_INLINE void store(T* _P, memory_order _O = memory_order_seq_cst)
    {
        atomic_store_explicit(this, (void *)_P, _O);
    }
    _STRONG_INLINE T* load(memory_order _O = memory_order_seq_cst) const volatile
    {
        return (T*)atomic_load_explicit(this, _O);
    }
    _STRONG_INLINE T* load(memory_order _O = memory_order_seq_cst) const
    {
        return (T*)atomic_load_explicit(this, _O);
    }
    _STRONG_INLINE operator T*() const volatile
    {
        return (T*)atomic_load_explicit(this, memory_order_seq_cst);
    }
    _STRONG_INLINE operator T*() const
    {
        return (T*)atomic_load_explicit(this, memory_order_seq_cst);
    }
    _STRONG_INLINE T* exchange(T* _P, memory_order _O = memory_order_seq_cst) volatile
    {
        return (T*)atomic_exchange_explicit(this, (void *)_P, _O);
    }
    _STRONG_INLINE T* exchange(T* _P, memory_order _O = memory_order_seq_cst)
    {
        return (T*)atomic_exchange_explicit(this, (void *)_P, _O);
    }
    _STRONG_INLINE bool compare_exchange_weak(T*& _P, T* _N, memory_order _S, memory_order _F) volatile
    {
        return atomic_compare_exchange_weak_explicit(this, (void *)&_P, (void*)_N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_weak(T*& _P, T* _N, memory_order _S, memory_order _F)
    {
        return atomic_compare_exchange_weak_explicit(this, (void *)&_P, (void*)_N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_strong(T*& _P, T* _N, memory_order _S, memory_order _F) volatile
    {
        return atomic_compare_exchange_strong_explicit(this, (void *)&_P, (void*)_N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_strong(T*& _P, T* _N, memory_order _S, memory_order _F)
    {
        return atomic_compare_exchange_strong_explicit(this, (void *)&_P, (void*)_N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_weak(T*& _P, T* _N, memory_order _O = memory_order_seq_cst) volatile
    {
        return atomic_compare_exchange_weak_explicit(this, (void *)&_P, (void *)_N, _O, memory_order_seq_cst);
    }
    _STRONG_INLINE bool compare_exchange_weak(T*& _P, T* _N, memory_order _O = memory_order_seq_cst)
    {
        return atomic_compare_exchange_weak_explicit(this, (void *)&_P, (void *)_N, _O, memory_order_seq_cst);
    }
    _STRONG_INLINE bool compare_exchange_strong(T*& _P, T* _N, memory_order _O = memory_order_seq_cst) volatile
    {
        return atomic_compare_exchange_strong_explicit(this, (void *)&_P, (void *)_N, _O, memory_order_seq_cst);
    }
    _STRONG_INLINE bool compare_exchange_strong(T*& _P, T* _N, memory_order _O = memory_order_seq_cst)
    {
        return atomic_compare_exchange_strong_explicit(this, (void *)&_P, (void *)_N, _O, memory_order_seq_cst);
    }
    _STRONG_INLINE T* fetch_add(ptrdiff_t _N, memory_order _O = memory_order_seq_cst) volatile
    {
        return (T*)atomic_fetch_add_explicit(this, _N, _O);
    }
    _STRONG_INLINE T* fetch_add(ptrdiff_t _N, memory_order _O = memory_order_seq_cst)
    {
        return (T*)atomic_fetch_add_explicit(this, _N, _O);
    }
    _STRONG_INLINE T* fetch_sub(ptrdiff_t _N, memory_order _O = memory_order_seq_cst) volatile
    {
        return (T*)atomic_fetch_sub_explicit(this, _N, _O);
    }
    _STRONG_INLINE T* fetch_sub(ptrdiff_t _N, memory_order _O = memory_order_seq_cst)
    {
        return (T*)atomic_fetch_sub_explicit(this, _N, _O);
    }
    _STRONG_INLINE atomic() _DEFAULTED
    _STRONG_INLINE _CONSTEXPR atomic(T* _P)
    : atomic_address(_P)
    {
    }
    _STRONG_INLINE T* operator+=(ptrdiff_t _N) volatile
    {
        return (T*)__atomic_add_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE T* operator+=(ptrdiff_t _N)
    {
        return (T*)__atomic_add_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE T* operator-=(ptrdiff_t _N) volatile
    {
        return (T*)__atomic_sub_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE T* operator-=(ptrdiff_t _N)
    {
        return (T*)__atomic_sub_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE T* operator=(T* _P) volatile
    {
        atomic_store_explicit(this, (void*)_P, memory_order_seq_cst);
        return _P;
    }
    _STRONG_INLINE T* operator=(T* _P)
    {
        atomic_store_explicit(this, (void*)_P, memory_order_seq_cst);
        return _P;
    }
    _STRONG_INLINE T* operator++(int) volatile
    {
        return (T*)atomic_fetch_add_explicit(this, sizeof(T), memory_order_seq_cst);
    }
    _STRONG_INLINE T* operator++(int)
    {
        return (T*)atomic_fetch_add_explicit(this, sizeof(T), memory_order_seq_cst);
    }
    _STRONG_INLINE T* operator--(int) volatile
    {
        return (T*)atomic_fetch_sub_explicit(this, sizeof(T), memory_order_seq_cst);
    }
    _STRONG_INLINE T* operator--(int)
    {
        return (T*)atomic_fetch_sub_explicit(this, sizeof(T), memory_order_seq_cst);
    }
    _STRONG_INLINE T* operator++() volatile
    {
        return (T*)__atomic_add_fetch_explicit(&_Val, sizeof(T), memory_order_seq_cst);
    }
    _STRONG_INLINE T* operator++()
    {
        return (T*)__atomic_add_fetch_explicit(&_Val, sizeof(T), memory_order_seq_cst);
    }
    _STRONG_INLINE T* operator--() volatile
    {
        return (T*)__atomic_sub_fetch_explicit(&_Val, sizeof(T), memory_order_seq_cst);
    }
    _STRONG_INLINE T* operator--()
    {
        return (T*)__atomic_sub_fetch_explicit(&_Val, sizeof(T), memory_order_seq_cst);
    }
    _DELETED( atomic(const atomic&) )
    _DELETED( atomic& operator=(const atomic&) )
    _DELETED( atomic& operator=(const atomic&) volatile )
};
#endif

#if __cplusplus
}
#endif

#endif

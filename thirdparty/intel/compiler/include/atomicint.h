/**
*** Copyright (C) 2009 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
**/

typedef struct _ATOMIC_ITYPE {
    _INTEGRAL _Val;
#if __cplusplus
    _STRONG_INLINE bool is_lock_free() const volatile
    {
        return true;
    }
    _STRONG_INLINE bool is_lock_free() const
    {
        return true;
    }
    _STRONG_INLINE void store(_INTEGRAL _N,
                              memory_order _O = memory_order_seq_cst) volatile
    {
        atomic_store_explicit(this, _N, _O);
    }
    _STRONG_INLINE void store(_INTEGRAL _N,
                              memory_order _O = memory_order_seq_cst)
    {
        atomic_store_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL load(memory_order _O = memory_order_seq_cst) const volatile
    {
        return atomic_load_explicit(this, _O);
    }
    _STRONG_INLINE _INTEGRAL load(memory_order _O = memory_order_seq_cst) const
    {
        return atomic_load_explicit(this, _O);
    }
    _STRONG_INLINE operator _INTEGRAL() const volatile
    {
        return atomic_load_explicit(this, memory_order_seq_cst);
    }
    _STRONG_INLINE operator _INTEGRAL() const
    {
        return atomic_load_explicit(this, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL exchange(_INTEGRAL _N,
                                      memory_order _O = memory_order_seq_cst) volatile
    {
        return atomic_exchange_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL exchange(_INTEGRAL _N,
                                      memory_order _O = memory_order_seq_cst)
    {
        return atomic_exchange_explicit(this, _N, _O);
    }
    _STRONG_INLINE bool compare_exchange_weak(_INTEGRAL &_P, _INTEGRAL _N,
                                              memory_order _S, memory_order _F) volatile
    {
        return atomic_compare_exchange_weak_explicit(this, &_P, _N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_weak(_INTEGRAL &_P, _INTEGRAL _N,
                                              memory_order _S, memory_order _F)
    {
        return atomic_compare_exchange_weak_explicit(this, &_P, _N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_strong(_INTEGRAL &_P, _INTEGRAL _N,
                                                memory_order _S, memory_order _F) volatile
    {
        return atomic_compare_exchange_strong_explicit(this, &_P, _N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_strong(_INTEGRAL &_P, _INTEGRAL _N,
                                                memory_order _S, memory_order _F)
    {
        return atomic_compare_exchange_strong_explicit(this, &_P, _N, _S, _F);
    }
    _STRONG_INLINE bool compare_exchange_weak(_INTEGRAL &_P, _INTEGRAL _N,
                                              memory_order _O = memory_order_seq_cst) volatile
    {
        return atomic_compare_exchange_weak_explicit(this, &_P, _N, _O, memory_order_seq_cst);
    }
    _STRONG_INLINE bool compare_exchange_weak(_INTEGRAL &_P, _INTEGRAL _N,
                                              memory_order _O = memory_order_seq_cst)
    {
        return atomic_compare_exchange_weak_explicit(this, &_P, _N, _O, memory_order_seq_cst);
    }
    _STRONG_INLINE bool compare_exchange_strong(_INTEGRAL &_P, _INTEGRAL _N,
                                                memory_order _O = memory_order_seq_cst) volatile
    {
        return atomic_compare_exchange_strong_explicit(this, &_P, _N, _O, memory_order_seq_cst);
    }
    _STRONG_INLINE bool compare_exchange_strong(_INTEGRAL &_P, _INTEGRAL _N,
                                                memory_order _O = memory_order_seq_cst)
    {
        return atomic_compare_exchange_strong_explicit(this, &_P, _N, _O, memory_order_seq_cst);
    }
#if _ADDITIVE
#if _REALLY_ADDRESS
#define _ADDEND ptrdiff_t
#define _CAST(x) (char *)(x)
#else
#define _ADDEND _INTEGRAL
#define _CAST(x) (x)
#endif
    _STRONG_INLINE _INTEGRAL fetch_add(_ADDEND _N,
                                       memory_order _O = memory_order_seq_cst) volatile
    {
        return atomic_fetch_add_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_add(_ADDEND _N,
                                       memory_order _O = memory_order_seq_cst)
    {
        return atomic_fetch_add_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_sub(_ADDEND _N,
                                       memory_order _O = memory_order_seq_cst) volatile
    {
        return atomic_fetch_sub_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_sub(_ADDEND _N,
                                       memory_order _O = memory_order_seq_cst)
    {
        return atomic_fetch_sub_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL operator+=(_ADDEND _N) volatile
    {
        return _CAST(__atomic_add_fetch_explicit(&_Val, _N, memory_order_seq_cst));
    }
    _STRONG_INLINE _INTEGRAL operator+=(_ADDEND _N)
    {
        return _CAST(__atomic_add_fetch_explicit(&_Val, _N, memory_order_seq_cst));
    }
    _STRONG_INLINE _INTEGRAL operator-=(_ADDEND _N) volatile
    {
        return _CAST(__atomic_sub_fetch_explicit(&_Val, _N, memory_order_seq_cst));
    }
    _STRONG_INLINE _INTEGRAL operator-=(_ADDEND _N)
    {
        return _CAST(__atomic_sub_fetch_explicit(&_Val, _N, memory_order_seq_cst));
    }
#undef _ADDEND
#undef _CAST
#if !_REALLY_ADDRESS
    _STRONG_INLINE _INTEGRAL operator++(int) volatile
    {
        return atomic_fetch_add_explicit(this, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator++(int)
    {
        return atomic_fetch_add_explicit(this, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator++() volatile
    {
        return __atomic_add_fetch_explicit(&_Val, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator++()
    {
        return __atomic_add_fetch_explicit(&_Val, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator--(int) volatile
    {
        return atomic_fetch_sub_explicit(this, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator--(int)
    {
        return atomic_fetch_sub_explicit(this, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator--() volatile
    {
        return __atomic_sub_fetch_explicit(&_Val, 1, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator--()
    {
        return __atomic_sub_fetch_explicit(&_Val, 1, memory_order_seq_cst);
    }
#endif
#endif
#if _BITWISE
    _STRONG_INLINE _INTEGRAL fetch_and(_INTEGRAL _N,
                                       memory_order _O = memory_order_seq_cst) volatile
    {
        return atomic_fetch_and_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_and(_INTEGRAL _N,
                                       memory_order _O = memory_order_seq_cst)
    {
        return atomic_fetch_and_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_or(_INTEGRAL _N,
                                      memory_order _O = memory_order_seq_cst) volatile
    {
        return atomic_fetch_or_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_or(_INTEGRAL _N,
                                      memory_order _O = memory_order_seq_cst)
    {
        return atomic_fetch_or_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_xor(_INTEGRAL _N,
                                       memory_order _O = memory_order_seq_cst) volatile
    {
        return atomic_fetch_xor_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL fetch_xor(_INTEGRAL _N,
                                       memory_order _O = memory_order_seq_cst)
    {
        return atomic_fetch_xor_explicit(this, _N, _O);
    }
    _STRONG_INLINE _INTEGRAL operator&=(_INTEGRAL _N) volatile
    {
        return __atomic_and_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator&=(_INTEGRAL _N)
    {
        return __atomic_and_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator|=(_INTEGRAL _N) volatile
    {
        return __atomic_or_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator|=(_INTEGRAL _N)
    {
        return __atomic_or_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator^=(_INTEGRAL _N) volatile
    {
        return __atomic_xor_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
    _STRONG_INLINE _INTEGRAL operator^=(_INTEGRAL _N)
    {
        return __atomic_xor_fetch_explicit(&_Val, _N, memory_order_seq_cst);
    }
#endif
    _STRONG_INLINE _ATOMIC_ITYPE() _DEFAULTED
    _CONSTEXPR _STRONG_INLINE _ATOMIC_ITYPE(_INTEGRAL _N)
    : _Val(_N)
    {
    }
    _STRONG_INLINE _INTEGRAL operator=(_INTEGRAL _N) volatile
    {
        atomic_store_explicit(this, _N, memory_order_seq_cst);
        return _N;
    }
    _STRONG_INLINE _INTEGRAL operator=(_INTEGRAL _N)
    {
        atomic_store_explicit(this, _N, memory_order_seq_cst);
        return _N;
    }
    _DELETED( _ATOMIC_ITYPE(const _ATOMIC_ITYPE&) )
    _DELETED( _ATOMIC_ITYPE& operator=(const _ATOMIC_ITYPE&) )
    _DELETED( _ATOMIC_ITYPE& operator=(const _ATOMIC_ITYPE&) volatile )
#endif
} _ATOMIC_ITYPE;

#if __cplusplus
#if !_REALLY_ADDRESS
template <>
struct atomic<_INTEGRAL> : _ATOMIC_ITYPE
{
    _STRONG_INLINE atomic() _DEFAULTED
    _CONSTEXPR _STRONG_INLINE atomic(_INTEGRAL _N)
    : _ATOMIC_ITYPE(_N)
    {
    }
    _STRONG_INLINE _INTEGRAL operator=(_INTEGRAL _N) volatile
    {
        atomic_store_explicit(this, _N, memory_order_seq_cst);
        return _N;
    }
    _STRONG_INLINE _INTEGRAL operator=(_INTEGRAL _N)
    {
        atomic_store_explicit(this, _N, memory_order_seq_cst);
        return _N;
    }
    _STRONG_INLINE operator _INTEGRAL() const volatile
    {
        return atomic_load_explicit(this, memory_order_seq_cst);
    }
    _STRONG_INLINE operator _INTEGRAL() const
    {
        return atomic_load_explicit(this, memory_order_seq_cst);
    }
    _DELETED( atomic(const atomic&) )
    _DELETED( atomic& operator=(const atomic&) )
    _DELETED( atomic& operator=(const atomic&) volatile )
};
#endif
#endif

#undef _ATOMIC_ITYPE
#undef _INTEGRAL
#undef _REALLY_ADDRESS

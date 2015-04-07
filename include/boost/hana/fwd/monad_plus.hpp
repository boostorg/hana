/*!
@file
Forward declares `boost::hana::MonadPlus`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MONAD_PLUS_HPP
#define BOOST_HANA_FWD_MONAD_PLUS_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `MonadPlus` concept represents Monads with a monoidal structure.
    //!
    //! Intuitively, whereas a Monad can be seen as some kind of container
    //! or context, a MonadPlus can be seen as a container or a context that
    //! can be concatenated with other containers or contexts. There must
    //! also be an identity element for this combining operation. For example,
    //! a `Tuple` is a MonadPlus, because tuples can be concatenated and the
    //! empty tuple would act as an identity for concatenation. How is this
    //! different from a Monad which is also a Monoid? The answer is that the
    //! monoidal structure on a MonadPlus must _not_ depend of the contents
    //! of the structure; it must not require the contents to be a Monoid
    //! in order to work.
    //!
    //! While sequences are not the only possible model for MonadPlus, the
    //! method names used here refer to the MonadPlus of sequences under
    //! concatenation. Several useful functions generalizing operations on
    //! sequences are included with this concept, like `append`, `prepend`
    //! and `filter`.
    //!
    //!
    //! Superclasses
    //! ------------
    //! `Functor`, `Applicative` and `Monad`
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `concat` and `empty`
    //!
    //!
    //! Laws
    //! ----
    //! First, a MonadPlus is required to have a monoidal structure. Hence, it
    //! is no surprise that for any MonadPlus `M`, we require `M(T)` to be a
    //! valid monoid. However, we do not enforce that `M(T)` actually models
    //! the Monoid concept provided by Hana. Further, for all objects `a, b, c`
    //! of data type `M(T)`,
    //! @code
    //!     // identity
    //!     concat(empty<M(T)>(), a) == a
    //!     concat(a, empty<M(T)>()) == a
    //!
    //!     // associativity
    //!     concat(a, concat(b, c)) == concat(concat(a, b), c)
    //! @endcode
    //!
    //! Second, a MonadPlus is also required to obey the following laws,
    //! which represent the fact that `empty<M(T)>()` must be some kind of
    //! absorbing element for the `bind` operation. For all objects `a` of
    //! data type `M(T)` and functions @f$ f : T \to M(U) @f$,
    //! @code
    //!     bind(empty<M(T)>(), f)         == empty<M(U)>()
    //!     bind(a, always(empty<M(T)>())) == empty<M(U)>()
    //! @endcode
    struct MonadPlus { };

    //! Combine two monadic structures together.
    //! @relates MonadPlus
    //!
    //! Given two monadic structures, `concat` combines them together and
    //! returns a new monadic structure. The exact definition of `concat`
    //! will depend on the exact model of MonadPlus at hand, but for
    //! sequences it corresponds intuitively to simple concatenation.
    //!
    //! Also note that combination is not required to be commutative.
    //! In other words, there is no requirement that
    //! @code
    //!     concat(xs, ys) == concat(ys, xs)
    //! @endcode
    //! and indeed it does not hold in general.
    //!
    //!
    //! Signature
    //! ---------
    //! Given `M` a MonadPlus, the signature is
    //! @f$ \mathrm{concat} : M(T) \times M(T) \to M(T) @f$.
    //!
    //! @param xs, ys
    //! Two monadic structures to combine together.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp concat
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto concat = [](auto&& xs, auto&& ys) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct concat_impl;

    struct _concat {
        template <typename Xs, typename Ys>
        constexpr decltype(auto) operator()(Xs&& xs, Ys&& ys) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(detail::std::is_same<
                typename datatype<Xs>::type, typename datatype<Ys>::type
            >{},
            "hana::concat(xs, ys) requires xs and ys to have the same data type");

            static_assert(_models<MonadPlus, typename datatype<Xs>::type>{},
            "hana::concat(xs, ys) requires xs and ys to be a MonadPlus");
#endif
            return concat_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Ys&&>(ys)
            );
        }
    };

    constexpr _concat concat{};
#endif

    //! Identity of the monadic combination `concat`.
    //! @relates MonadPlus
    //!
    //! Signature
    //! ---------
    //! Given a MonadPlus `M`, the signature is
    //! @f$ \mathrm{empty}_M : \emptyset \to M(T) @f$.
    //!
    //! @tparam M
    //! The data type of the monadic structure to return. This must be
    //! a model of the MonadPlus concept.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp empty
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto empty = []() -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct empty_impl;

    template <typename M>
    struct _empty {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(_models<MonadPlus, M>{},
        "hana::empty<M>() requires M to be a MonadPlus");
#endif
        constexpr decltype(auto) operator()() const {
            return empty_impl<M>::apply();
        }
    };

    template <typename M>
    constexpr _empty<M> empty{};
#endif

    //! Prepend an element to a monadic structure.
    //! @relates MonadPlus
    //!
    //! Given an element `x` and a monadic structure `xs`, `prepend` returns
    //! a new monadic structure which is the result of lifting `x` into the
    //! monadic structure and then combining that (to the left) with `xs`.
    //! In other words,
    //! @code
    //!     prepend(x, xs) == concat(lift<Xs>(x), xs)
    //! @endcode
    //!
    //! For sequences, this has the intuitive behavior of simply prepending
    //! an element to the beginning of the sequence, hence the name.
    //!
    //! > #### Rationale for not calling this `push_front`
    //! > While `push_front` is the de-facto name used in the standard library,
    //! > it also strongly suggests mutation of the underlying sequence, which
    //! > is not the case here. The author also finds that `push_front`
    //! > suggests too strongly the sole interpretation of putting an
    //! > element to the front of a sequence, whereas `prepend` is slightly
    //! > more nuanced and bears its name better for e.g. `Maybe`.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a MonadPlus `M`, the signature is
    //! @f$ \mathrm{prepend} : T \times M(T) \to M(T) @f$.
    //!
    //! @param x
    //! An element to combine to the left of the monadic structure.
    //!
    //! @param xs
    //! A monadic structure that will be combined to the right of the element.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp prepend
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto prepend = [](auto&& x, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct prepend_impl;

    struct _prepend {
        template <typename X, typename Xs>
        constexpr decltype(auto) operator()(X&& x, Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<MonadPlus, typename datatype<Xs>::type>{},
            "hana::prepend(x, xs) requires xs to be a MonadPlus");
#endif
            return prepend_impl<typename datatype<Xs>::type>::apply(
                static_cast<X&&>(x),
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _prepend prepend{};
#endif

    //! Append an element to a monadic structure.
    //! @relates MonadPlus
    //!
    //! Given an element `x` of data type `X` and a monadic structure
    //! `xs` containing objects of data type `X`, `append` returns a
    //! new monadic structure which is the result of lifting `x` into
    //! the monadic structure and then combining that (to the right)
    //! with `xs`. In other words,
    //! @code
    //!     append(xs, x) == concat(xs, lift<Xs>(x))
    //! @endcode
    //!
    //! For sequences, this has the intuitive behavior of simply appending
    //! an element to the end of the sequence, hence the name.
    //!
    //! > #### Rationale for not calling this `push_back`
    //! > See the rationale for using `prepend` instead of `push_front`.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a MonadPlus `M`, the signature is
    //! @f$ \mathrm{append} : M(T) \times T \to M(T) @f$.
    //!
    //! @param xs
    //! A monadic structure that will be combined to the left of the element.
    //!
    //! @param x
    //! An element to combine to the right of the monadic structure.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp append
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto append = [](auto&& xs, auto&& x) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct append_impl;

    struct _append {
        template <typename Xs, typename X>
        constexpr decltype(auto) operator()(Xs&& xs, X&& x) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<MonadPlus, typename datatype<Xs>::type>{},
            "hana::append(xs, x) requires xs to be a MonadPlus");
#endif
            return append_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<X&&>(x)
            );
        }
    };

    constexpr _append append{};
#endif

    //! Filter a monadic structure using a custom predicate.
    //! @relates MonadPlus
    //!
    //! Given a monadic structure and a predicate, `filter` returns a new
    //! monadic structure containing only those elements that satisfy the
    //! predicate. This is a generalization of the usual `filter` function
    //! for sequences; it works for any MonadPlus. Intuitively, `filter` is
    //! somewhat equivalent to:
    //! @code
    //!     filter(xs, pred) == flatten(transform(xs, [](auto x) {
    //!         return pred(x) ? lift<Xs>(x) : empty<Xs>();
    //!     })
    //! @endcode
    //! In other words, we basically turn a monadic structure containing
    //! `[x1, ..., xn]` into a monadic structure containing
    //! @code
    //!     [
    //!         pred(x1) ? [x1] : [],
    //!         pred(x2) ? [x2] : [],
    //!         ...
    //!         pred(xn) ? [xn] : []
    //!     ]
    //! @endcode
    //! , and we then `flatten` that.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a MonadPlus `M` and a Logical `Bool`, the signature is
    //! @f$ \mathrm{filter} : M(T) \times (T \to \mathrm{Bool}) \to M(T) @f$.
    //!
    //! @param xs
    //! The monadic structure to filter.
    //!
    //! @param pred
    //! A function called as `pred(x)` for each element `x` in the monadic
    //! structure and returning a `Logical` representing whether that element
    //! should be __kept__ in the resulting structure. In the current version
    //! of the library, the predicate has to return a compile-time `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp filter
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/monad_plus/filter.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto filter = [](auto&& xs, auto&& pred) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct filter_impl;

    struct _filter {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<MonadPlus, typename datatype<Xs>::type>{},
            "hana::filter(xs, pred) requires xs to be a MonadPlus");
#endif
            return filter_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
        }
    };

    constexpr _filter filter{};
#endif

    //! Combine a monadic structure with itself `n` times.
    //! @relates MonadPlus
    //!
    //! Given a non-negative number `n` and a monadic structure `xs`,
    //! `cycle` returns a new monadic structure which is the result of
    //! combining `xs` with itself `n` times using the `concat` operation.
    //! In other words,
    //! @code
    //!     cycle(n, xs) == concat(xs, concat(xs, ... concat(xs, xs)))
    //!                                       // ^^^^^ n times total
    //! @endcode
    //!
    //! Also note that since `concat` is required to be associative, we
    //! could also have written
    //! @code
    //!     cycle(n, xs) == concat(concat(... concat(xs, xs), xs), xs)
    //!                               // ^^^^^ n times total
    //! @endcode
    //!
    //! If `n` is zero, then the identity of `concat`, `empty`, is returned.
    //! In the case of sequences, this boils down to returning a sequence
    //! which containes `n` copies of itself; for other models it might
    //! differ.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Constant `C` and a MonadPlus `M`, the signature is
    //! @f$ \mathrm{cycle} : C \times M(T) \to M(T) @f$.
    //!
    //! @param n
    //! A non-negative `Constant` of an unsigned integral type representing
    //! the number of times to combine the monadic structure with itself.
    //! If `n` is zero, `cycle` returns `empty`.
    //!
    //! @param xs
    //! A monadic structure to combine with itself a certain number of times.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp cycle
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto cycle = [](auto&& n, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct cycle_impl;

    struct _cycle {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<MonadPlus, typename datatype<Xs>::type>{},
            "hana::cycle(n, xs) requires xs to be a MonadPlus");
        #endif
            return cycle_impl<typename datatype<Xs>::type>::apply(
                static_cast<N&&>(n),
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _cycle cycle{};
#endif

    //! Remove all the elements of a monadic structure that satisfy some
    //! predicate.
    //! @relates MonadPlus
    //!
    //! Given a monadic structure `xs` and a unary predicate, `remove_if`
    //! returns a new monadic structure equal to `xs` without all its elements
    //! that satisfy the predicate. This is equivalent to `filter` with a
    //! negated predicate, i.e.
    //! @code
    //!     remove_if(xs, predicate) == filter(xs, negated predicated)
    //! @endcode
    //!
    //!
    //! Signature
    //! ---------
    //! Given a MonadPlus `M` and a predicate of type \f$ T \to Bool \f$ for
    //! some compile-time Logical `Bool`, the signature is
    //! \f$
    //!     \mathrm{remove\_if} : M(T) \times (T \to Bool) \to M(T)
    //! \f$
    //!
    //! @param xs
    //! A monadic structure to remove some elements from.
    //!
    //! @param predicate
    //! A unary predicate called as `predicate(x)`, where `x` is an element
    //! of the structure, and returning whether `x` should be removed from
    //! the structure. In the current version of the library, `predicate`
    //! must return a compile-time Logical.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp remove_if
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto remove_if = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct remove_if_impl;

    struct _remove_if {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
                static_assert(_models<MonadPlus, typename datatype<Xs>::type>{},
                "hana::remove_if(xs, predicate) requires xs to be a MonadPlus");
            #endif
            return remove_if_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
        }
    };

    constexpr _remove_if remove_if{};
#endif

    //! Remove all the elements of a monadic structure that are equal to some
    //! value.
    //! @relates MonadPlus
    //!
    //! Given a monadic structure `xs` and a `value`, `remove` returns a new
    //! monadic structure equal to `xs` without all its elements that are
    //! equal to the given `value`. `remove` is equivalent to `remove_if`
    //! with the `equal.to(value)` predicate, i.e.
    //! @code
    //!     remove(xs, value) == remove_if(xs, equal.to(value))
    //! @endcode
    //!
    //!
    //! Signature
    //! ---------
    //! Given a MonadPlus `M` and a value of type `T`, the signature is
    //! \f$
    //!     \mathrm{remove} : M(T) \times T \to M(T)
    //! \f$
    //!
    //! @param xs
    //! A monadic structure to remove some elements from.
    //!
    //! @param value
    //! A value that is compared to every element `x` of the structure.
    //! Elements of the structure that are equal to that value are removed
    //! from the structure. This requires every element to be Comparable
    //! with `value`. Furthermore, in the current version of the library,
    //! comparing `value` with any element of the structure must yield a
    //! compile-time Logical.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp remove
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto remove = [](auto&& xs, auto&& value) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct remove_impl;

    struct _remove {
        template <typename Xs, typename Value>
        constexpr decltype(auto) operator()(Xs&& xs, Value&& value) const {
            #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
                static_assert(_models<MonadPlus, typename datatype<Xs>::type>{},
                "hana::remove(xs, value) requires xs to be a MonadPlus");
            #endif
            return remove_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Value&&>(value)
            );
        }
    };

    constexpr _remove remove{};
#endif

    //! Create a monadic structure by combining a lifted value with itself
    //! `n` times.
    //! @relates MonadPlus
    //!
    //! Given a non-negative number `n`, a value `x` and a monadic data type
    //! `M`, `repeat` creates a new monadic structure which is the result of
    //! combining `x` with itself `n` times inside the monadic structure.
    //! In other words, `repeat` simply `lift`s `x` into the monadic structure,
    //! and then combines that with itself `n` times:
    //! @code
    //!     repeat<M>(n, x) == cycle(n, lift<M>(x))
    //! @endcode
    //!
    //! If `n` is zero, then the identity of the `concat` operation is returned.
    //! In the case of sequences, this corresponds to creating a new sequence
    //! holding `n` copies of `x`.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Constant `C` and MonadPlus `M`, the signature is
    //! @f$ \mathrm{repeat}_M : C \times T \to M(T) @f$.
    //!
    //! @tparam M
    //! The data type of the returned monadic structure. It must be a
    //! model of the MonadPlus concept.
    //!
    //! @param n
    //! A non-negative `Constant` of an unsigned integral type representing
    //! the number of times to combine `lift<M>(x)` with itself. If `n == 0`,
    //! then `repeat` returns `empty<M>()`.
    //!
    //! @param x
    //! The value to lift into a monadic structure and then combine with
    //! itself.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp repeat
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto repeat = [](auto&& n, auto&& x) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct repeat_impl;

    template <typename M>
    struct _repeat {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(_models<MonadPlus, M>{},
        "hana::repeat<M>(n, x) requires M to be a MonadPlus");
#endif
        template <typename N, typename X>
        constexpr decltype(auto) operator()(N&& n, X&& x) const {
            return repeat_impl<M>::apply(
                detail::std::forward<decltype(n)>(n),
                detail::std::forward<decltype(x)>(x)
            );
        }
    };

    template <typename M>
    constexpr _repeat<M> repeat{};
#endif

    //! Inserts a value before each element of a monadic structure.
    //! @relates MonadPlus
    //!
    //! Given a value (called the prefix) `z` and a monadic structure `xs`,
    //! `prefix` returns a new monadic structure which is equivalent to
    //! @code
    //!     prefix(z, xs) == flatten(transform(xs, [](auto x) {
    //!         return concat(lift<M>(z), lift<M>(x));
    //!     }))
    //! @endcode
    //!
    //! For sequences, this simply corresponds to inserting the prefix before
    //! each element of the sequence. For example, given a sequence
    //! `[x1, ..., xn]`, `prefix` will return
    //! @code
    //!     [z, x1, z, x2, ..., z, xn]
    //! @endcode
    //! As explained above, this can be generalized to other MonadPlus models,
    //! with various levels of interest.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a MonadPlus `M`, the signature is
    //! @f$ \mathrm{prefix} : T \times M(T) \to M(T) @f$.
    //!
    //! @param z
    //! A value (the prefix) to insert before each element of a monadic
    //! structure.
    //!
    //! @param xs
    //! A monadic structure.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp prefix
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto prefix = [](auto&& z, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct prefix_impl;

    struct _prefix {
        template <typename Z, typename Xs>
        constexpr decltype(auto) operator()(Z&& z, Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<MonadPlus, typename datatype<Xs>::type>{},
            "hana::prefix(z, xs) requires xs to be a MonadPlus");
#endif
            return prefix_impl<typename datatype<Xs>::type>::apply(
                static_cast<Z&&>(z),
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _prefix prefix{};
#endif

    //! Inserts a value after each element of a monadic structure.
    //! @relates MonadPlus
    //!
    //! Given a value (called the suffix) `z` and a monadic structure `xs`,
    //! `suffix` returns a new monadic structure which is equivalent to
    //! @code
    //!     suffix(z, xs) == flatten(transform(xs, [](auto x) {
    //!         return concat(lift<M>(x), lift<M>(z));
    //!     }))
    //! @endcode
    //!
    //! For sequences, this simply corresponds to inserting the suffix after
    //! each element of the sequence. For example, given a sequence
    //! `[x1, ..., xn]`, `suffix` will return
    //! @code
    //!     [x1, z, x2, z, ..., xn, z]
    //! @endcode
    //! As explained above, this can be generalized to other MonadPlus models,
    //! with various levels of interest.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a MonadPlus `M`, the signature is
    //! @f$ \mathrm{suffix} : T \times M(T) \to M(T) @f$.
    //!
    //! @param z
    //! A value (the suffix) to insert after each element of a monadic
    //! structure.
    //!
    //! @param xs
    //! A monadic structure.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp suffix
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto suffix = [](auto&& z, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct suffix_impl;

    struct _suffix {
        template <typename Z, typename Xs>
        constexpr decltype(auto) operator()(Z&& z, Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<MonadPlus, typename datatype<Xs>::type>{},
            "hana::suffix(z, xs) requires xs to be a MonadPlus");
#endif
            return suffix_impl<typename datatype<Xs>::type>::apply(
                static_cast<Z&&>(z),
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _suffix suffix{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MONAD_PLUS_HPP

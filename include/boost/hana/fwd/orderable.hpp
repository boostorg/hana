/*!
@file
Forward declares `boost::hana::Orderable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ORDERABLE_HPP
#define BOOST_HANA_FWD_ORDERABLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/default.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Orderable` concept represents totally ordered data types.
    //!
    //! Intuitively, `Orderable` objects must define a binary predicate named
    //! `less` returning whether the first argument is to be considered less
    //! than the second argument. The word "total" means that _distinct_
    //! objects must always be ordered; if `a` and `b` are not equal, then
    //! exactly one of `less(a, b)` and `less(b, a)` must be true. This is
    //! a contrast with weaker kinds of orders that would allow some objects
    //! to be incomparable (neither less than nor greater than). Also note
    //! that a non-strict total order may always be obtained from a strict
    //! total order (and vice-versa) by setting
    //! @code
    //!     a <= b  =  !(b < a)
    //!     a <  b  =  !(b <= a)
    //! @endcode
    //! The non-strict version is used in the description of the laws because
    //! it makes them easier to parse for humans, but they could be formulated
    //! equivalently using the strict order.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `less`
    //!
    //! When `less` is defined, the other methods are defined from it using
    //! the same definition as mandated in the laws below.
    //!
    //!
    //! Laws
    //! ----
    //! Rigorously speaking, a [total order][1] `<=` on a set `S` is a binary
    //! predicate @f$ <= \;: S \times S \to bool @f$ such that for all
    //! `a`, `b`, `c` in `S`,
    //! @code
    //!     if  a <= b  and  b <= a  then  a == b // Antisymmetry
    //!     if  a <= b  and  b <= c  then  a <= c // Transitivity
    //!     either  a <= b  or  b <= a            // Totality
    //! @endcode
    //! Additionally, the `less`, `greater` and `greater_equal` methods should
    //! have the following intuitive meanings:
    //! @code
    //!     a <  b  if and only if  !(b <= a)
    //!     a >  b  if and only if    b < a
    //!     a >= b  if and only if  !(a < b)
    //! @endcode
    //!
    //!
    //! Refined concept
    //! ---------------
    //! 1. `Comparable` (free model)\n
    //! Since `Orderable` requires `less_equal` to be a total order, a model
    //! of `Comparable` may always be obtained by setting
    //! @code
    //!     equal(x, y) = less_equal(x, y) && less_equal(y, x)
    //! @endcode
    //! An equivalent implementation of `equal` is provided as
    //! `Orderable::equal_impl`. To use it, simply inherit
    //! `Orderable::equal_impl` in your definition of `equal_impl`:
    //! @code
    //!     template <>
    //!     struct equal_impl<YourType, YourType>
    //!         : Orderable::equal_impl<YourType, YourType>
    //!     { };
    //! @endcode
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `Either`, `IntegralConstant`, `Optional`, `Pair`, `String`, `Tuple`
    //!
    //!
    //! Free model for `LessThanComparable` data types
    //! ----------------------------------------------
    //! Two data types `T` and `U` that model the cross-type version of the
    //! usual [LessThanComparable][2] C++ concept are automatically a model
    //! of `Orderable` by setting
    //! @code
    //!     less(x, y) = (x < y)
    //! @endcode
    //! The cross-type version of the LessThanComparable concept is analogous
    //! to the cross-type version of the EqualityComparable concept presented
    //! in [N3351][3], which is compatible with the usual single type
    //! definition.
    //! However, note that the LessThanComparable concept only requires `<`
    //! to be a [strict weak ordering][4], which is a weaker requirement
    //! than being a total order. Hence, if `less` is used with objects
    //! of a LessThanComparable data type that do not define a total order,
    //! some algorithms may have an unexpected behavior. It is the author's
    //! opinion that defining `operator<` as a non-total order is a bad idea,
    //! but this is debatable and so the design choice of providing a model
    //! for LessThanComparable data types is open to debate. Waiting for
    //! some user input.
    //!
    //!
    //! Order-preserving functions
    //! --------------------------
    //! Let `A` and `B` be two `Orderable` data types. A function
    //! @f$ f : A \to B@f$ is said to be order-preserving (also called
    //! monotone) if it preserves the structure of the `Orderable` concept,
    //! which can be rigorously stated as follows. For all objects `x`, `y`
    //! of data type `A`,
    //! @code
    //!     if  less(x, y)  then  less(f(x), f(y))
    //! @endcode
    //! Another important property is that of being order-reflecting, which
    //! can be stated as
    //! @code
    //!     if  less(f(x), f(y))  then  less(x, y)
    //! @endcode
    //! We say that a function is an order-embedding if it is both
    //! order-preserving and order-reflecting, i.e. if
    //! @code
    //!     less(x, y)  if and only if  less(f(x), f(y))
    //! @endcode
    //!
    //!
    //! Cross-type version of the methods
    //! ---------------------------------
    //! The comparison methods (`less`, `less_equal`, `greater` and
    //! `greater_equal`) are "overloaded" to handle distinct data types
    //! with certain properties. Specifically, they are defined for
    //! _distinct_ data types `A` and `B` such that
    //! 1. `A` and `B` share a common data type `C`, as determined by the
    //!    `common` metafunction
    //! 2. `A`, `B` and `C` are all `Orderable` when taken individually
    //! 3. @f$\mathrm{to<C>} : A \to C@f$ and @f$\mathrm{to<C>} : B \to C@f$
    //!    are both order-embeddings as determined by the `is_embedding`
    //!    metafunction.
    //!
    //! The method definitions for data types satisfying the above
    //! properties are
    //! @code
    //!     less(x, y)          = less(to<C>(x), to<C>(y))
    //!     less_equal(x, y)    = less_equal(to<C>(x), to<C>(y))
    //!     greater_equal(x, y) = greater_equal(to<C>(x), to<C>(y))
    //!     greater(x, y)       = greater(to<C>(x), to<C>(y))
    //! @endcode
    //!
    //!
    //! Partial application of the methods
    //! ----------------------------------
    //! The `less`, `greater`, `less_equal` and `greater_equal` methods can
    //! be called in two different ways. First, they can be called like
    //! normal functions:
    //! @code
    //!     less(x, y)
    //!     greater(x, y)
    //!
    //!     less_equal(x, y)
    //!     greater_equal(x, y)
    //! @endcode
    //!
    //! However, they may also be partially applied to an argument as follows:
    //! @code
    //!     less.than(x)(y)    == less(y, x)
    //!     greater.than(x)(y) == greater(y, x)
    //!
    //!     less_equal.than(x)(y)    == less_equal(y, x)
    //!     greater_equal.than(x)(y) == greater_equal(y, x)
    //! @endcode
    //!
    //! Take good note that the order of the arguments is reversed, so
    //! for example `less.than(x)(y)` is equivalent to `less(y, x)`, not
    //! `less(x, y)`. This is because those variants are meant to be used
    //! with higher order algorithms, where the chosen application order
    //! makes sense:
    //! @snippet example/orderable.cpp less.than
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Total_order
    //! [2]: http://en.cppreference.com/w/cpp/concept/LessThanComparable
    //! [3]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3351.pdf
    //! [4]: http://en.wikipedia.org/wiki/Strict_weak_ordering
    struct Orderable {
        template <typename T, typename U>
        struct equal_impl;
    };

    //! Returns a `Logical` representing whether `x` is less than `y`.
    //! @relates Orderable
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Logical `Bool` and two Orderables `A` and `B` with a common
    //! embedding, the signature is
    //! @f$ \mathrm{less} : A \times B \to Bool @f$.
    //!
    //! @param x, y
    //! Two objects to compare.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/orderable.cpp less
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto less = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct less_impl;

    struct _less_than {
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const;
    };

    template <typename ...AvoidODRViolation>
    struct _less {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            using T = typename datatype<X>::type;
            using U = typename datatype<Y>::type;
            using Less = BOOST_HANA_DISPATCH_IF(decltype(less_impl<T, U>{}),
                _models<Orderable, T>{}() &&
                _models<Orderable, U>{}() &&
                !is_default<less_impl<T, U>>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Orderable, T>{},
            "hana::less(x, y) requires 'x' to be Orderable");

            static_assert(_models<Orderable, U>{},
            "hana::less(x, y) requires 'y' to be Orderable");

            static_assert(!is_default<less_impl<T, U>>{}(),
            "hana::less(x, y) requires 'x' and 'y' to be embeddable "
            "in a common Orderable");
        #endif

            return Less::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
        }

        static constexpr _less_than than{};
    };
    template <typename ...AvoidODRViolation>
    constexpr _less_than _less<AvoidODRViolation...>::than;

    constexpr _less<> less{};
#endif

    //! Returns a `Logical` representing whether `x` is less than or
    //! equal to `y`.
    //! @relates Orderable
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Logical `Bool` and two Orderables `A` and `B` with a common
    //! embedding, the signature is
    //! @f$ \mathrm{less\_equal} : A \times B \to Bool @f$.
    //!
    //! @param x, y
    //! Two objects to compare.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/orderable.cpp less_equal
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto less_equal = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct less_equal_impl;

    struct _less_equal_than {
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const;
    };

    template <typename ...AvoidODRViolation>
    struct _less_equal {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            using T = typename datatype<X>::type;
            using U = typename datatype<Y>::type;
            using LessEqual = BOOST_HANA_DISPATCH_IF(
                decltype(less_equal_impl<T, U>{}),
                _models<Orderable, T>{}() &&
                _models<Orderable, U>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Orderable, T>{},
            "hana::less_equal(x, y) requires 'x' to be Orderable");

            static_assert(_models<Orderable, U>{},
            "hana::less_equal(x, y) requires 'y' to be Orderable");
        #endif

            return LessEqual::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
        }

        static constexpr _less_equal_than than{};
    };
    template <typename ...AvoidODRViolation>
    constexpr _less_equal_than _less_equal<AvoidODRViolation...>::than;

    constexpr _less_equal<> less_equal{};
#endif

    //! Returns a `Logical` representing whether `x` is greater than `y`.
    //! @relates Orderable
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Logical `Bool` and two Orderables `A` and `B` with a common
    //! embedding, the signature is
    //! @f$ \mathrm{greater} : A \times B \to Bool @f$.
    //!
    //! @param x, y
    //! Two objects to compare.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/orderable.cpp greater
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto greater = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct greater_impl;

    struct _greater_than {
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const;
    };

    template <typename ...AvoidODRViolation>
    struct _greater {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            using T = typename datatype<X>::type;
            using U = typename datatype<Y>::type;
            using Greater = BOOST_HANA_DISPATCH_IF(decltype(greater_impl<T, U>{}),
                _models<Orderable, T>{}() &&
                _models<Orderable, U>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Orderable, T>{},
            "hana::greater(x, y) requires 'x' to be Orderable");

            static_assert(_models<Orderable, U>{},
            "hana::greater(x, y) requires 'y' to be Orderable");
        #endif

            return Greater::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
        }

        static constexpr _greater_than than{};
    };
    template <typename ...AvoidODRViolation>
    constexpr _greater_than _greater<AvoidODRViolation...>::than;

    constexpr _greater<> greater{};
#endif

    //! Returns a `Logical` representing whether `x` is greater than or
    //! equal to `y`.
    //! @relates Orderable
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Logical `Bool` and two Orderables `A` and `B` with a common
    //! embedding, the signature is
    //! @f$ \mathrm{greater\_equal} : A \times B \to Bool @f$.
    //!
    //! @param x, y
    //! Two objects to compare.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/orderable.cpp greater_equal
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto greater_equal = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct greater_equal_impl;

    struct _greater_equal_than {
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const;
    };

    template <typename ...AvoidODRViolation>
    struct _greater_equal {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            using T = typename datatype<X>::type;
            using U = typename datatype<Y>::type;
            using GreaterEqual = BOOST_HANA_DISPATCH_IF(
                decltype(greater_equal_impl<T, U>{}),
                _models<Orderable, T>{}() &&
                _models<Orderable, U>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Orderable, T>{},
            "hana::greater_equal(x, y) requires 'x' to be Orderable");

            static_assert(_models<Orderable, U>{},
            "hana::greater_equal(x, y) requires 'y' to be Orderable");
        #endif

            return GreaterEqual::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
        }

        static constexpr _greater_equal_than than{};
    };
    template <typename ...AvoidODRViolation>
    constexpr _greater_equal_than _greater_equal<AvoidODRViolation...>::than;

    constexpr _greater_equal<> greater_equal{};
#endif

    //! Returns the smallest of its arguments according to the `less` ordering.
    //! @relates Orderable
    //!
    //!
    //! @todo
    //! We can't specify the signature right now, because the returned
    //! data type depends on whether `x < y` or not. If we wanted to be
    //! mathematically correct, we should probably ask that `if_(cond, x, y)`
    //! returns a common data type of `x` and `y`, and then the behavior
    //! of `min` would follow naturally. However, I'm unsure whether this
    //! is desirable because that's a big requirement.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/orderable.cpp min
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto min = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct min_impl;

    struct _min {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            using T = typename datatype<X>::type;
            using U = typename datatype<Y>::type;
            using Min = BOOST_HANA_DISPATCH_IF(decltype(min_impl<T, U>{}),
                _models<Orderable, T>{}() &&
                _models<Orderable, U>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Orderable, T>{},
            "hana::min(x, y) requires 'x' to be Orderable");

            static_assert(_models<Orderable, U>{},
            "hana::min(x, y) requires 'y' to be Orderable");
        #endif

            return Min::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
        }
    };

    constexpr _min min{};
#endif

    //! Returns the greatest of its arguments according to the `less` ordering.
    //! @relates Orderable
    //!
    //!
    //! @todo Can't specify the signature here either. See `min` for details.
    //!
    //! Example
    //! -------
    //! @snippet example/orderable.cpp max
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto max = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct max_impl;

    struct _max {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            using T = typename datatype<X>::type;
            using U = typename datatype<Y>::type;
            using Max = BOOST_HANA_DISPATCH_IF(decltype(max_impl<T, U>{}),
                _models<Orderable, T>{}() &&
                _models<Orderable, U>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Orderable, T>{},
            "hana::max(x, y) requires 'x' to be Orderable");

            static_assert(_models<Orderable, U>{},
            "hana::max(x, y) requires 'y' to be Orderable");
        #endif

            return Max::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
        }
    };

    constexpr _max max{};
#endif

    //! Returns a function performing `less` after applying a transformation
    //! to both arguments.
    //! @relates Orderable
    //!
    //! `ordering` creates a total order based on the result of applying a
    //! function to some objects, which is especially useful in conjunction
    //! with algorithms that accept a custom predicate that must represent
    //! a total order.
    //!
    //! Specifically, `ordering` is such that
    //! @code
    //!     ordering(f) == less ^on^ f
    //! @endcode
    //! or, equivalently,
    //! @code
    //!     ordering(f)(x, y) == less(f(x), f(y))
    //! @endcode
    //!
    //! @note
    //! This is not a tag-dispatched method (hence it can't be customized),
    //! but just a convenience function provided with the `Orderable` concept.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Logical `Bool` and an Orderable `B`, the signature is
    //! @f$ \mathrm{ordering} : (A \to B) \to (A \times A \to Bool) @f$.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/orderable.cpp ordering
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto ordering = [](auto&& f) {
        return [perfect-capture](auto&& x, auto&& y) -> decltype(auto) {
            return less(f(forwarded(x)), f(forwarded(y)));
        };
    };
#else
    template <typename F>
    struct _ordering;

    constexpr detail::create<_ordering> ordering{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ORDERABLE_HPP

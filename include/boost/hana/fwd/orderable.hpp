/*!
@file
Forward declares `boost::hana::Orderable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ORDERABLE_HPP
#define BOOST_HANA_FWD_ORDERABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
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
    //! Laws
    //! ----
    //! Rigorously speaking, a [total order][1] `<=` on a set `S` is a binary
    //! predicate `<= : S x S -> bool` such that for all `a, b, c` in `S`,
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
    //! Superclass
    //! ----------
    //! 1. `Comparable` (model provided)\n
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
    //! Minimal complete definition
    //! ---------------------------
    //! 1. `less`\n
    //! When `less` is defined, the other methods are defined from it using
    //! the same definition as mandated in the laws above.
    //!
    //!
    //! Provided models
    //! ---------------
    //! 1. for `LessThanComparable` data types\n
    //! A data type `D` that models the [LessThanComparable][2] C++ concept is
    //! automatically a model of `Orderable` by setting
    //! @code
    //!     less(x, y) = (x < y)
    //! @endcode
    //! However, note that the `LessThanComparable` concept only requires `<`
    //! to be a [strict weak ordering][3], which is a weaker requirement than
    //! being a total order. Hence, if `less` is used with objects of a
    //! `LessThanComparable` data type that do not define a total order,
    //! some algorithms may have an unexpected behavior. It is the author's
    //! opinion that defining `operator<` as a non-total order is a bad idea,
    //! but this is debatable and so the design choice of providing a model
    //! for `LessThanComparable` data types is open to debate. Waiting for
    //! some user input.
    //!
    //!
    //! Operators
    //! ---------
    //! For convenience, the following operators are provided as an
    //! equivalent way of calling the corresponding method:
    //! @code
    //!     <   ->  less
    //!     <=  ->  less_equal
    //!     >   ->  greater
    //!     >=  ->  greater_equal
    //! @endcode
    //!
    //!
    //! Order-preserving functions
    //! --------------------------
    //! Let `A` and `B` be two `Orderable` data types. A function `f : A -> B`
    //! is said to be order-preserving (also called monotone) if it preserves
    //! the structure of the `Orderable` concept, which can be rigorously
    //! stated as follows. For all objects `x, y` of data type `A`,
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
    //! 3. `to<C> : A -> C` and `to<C> : B -> C` are both order-embeddings
    //!    as determined by the `is_embedding` metafunction.
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
    //! @todo
    //! Consider allowing another minimal complete definition using
    //! `less_equal`.
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Total_order
    //! [2]: http://en.cppreference.com/w/cpp/concept/LessThanComparable
    //! [3]: http://en.wikipedia.org/wiki/Strict_weak_ordering
    struct Orderable {
        template <typename T, typename U>
        struct equal_impl;
    };

    //! Returns a `Logical` representing whether `x` is less than `y`.
    //! @relates Orderable
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

    struct _less {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return less_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _less less{};
#endif

    //! Returns a `Logical` representing whether `x` is less than or
    //! equal to `y`.
    //! @relates Orderable
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

    struct _less_equal {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return less_equal_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _less_equal less_equal{};
#endif

    //! Returns a `Logical` representing whether `x` is greater than `y`.
    //! @relates Orderable
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

    struct _greater {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return greater_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _greater greater{};
#endif

    //! Returns a `Logical` representing whether `x` is greater than or
    //! equal to `y`.
    //! @relates Orderable
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

    struct _greater_equal {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return greater_equal_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _greater_equal greater_equal{};
#endif

    //! Returns the smallest of its arguments according to the `less` ordering.
    //! @relates Orderable
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
            return min_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _min min{};
#endif

    //! Returns the greatest of its arguments according to the `less` ordering.
    //! @relates Orderable
    //!
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
            return max_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
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
    struct _ordering {
        F f;
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const& {
            return hana::less(
                f(detail::std::forward<X>(x)),
                f(detail::std::forward<Y>(y))
            );
        }
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) & {
            return hana::less(
                f(detail::std::forward<X>(x)),
                f(detail::std::forward<Y>(y))
            );
        }
    };

    constexpr detail::create<_ordering> ordering{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ORDERABLE_HPP

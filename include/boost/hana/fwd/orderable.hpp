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
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Orderable` type class is used for data types defining a
    //! [strict weak ordering](http://en.wikipedia.org/wiki/Strict_weak_ordering).
    //!
    //! @anchor strict_weak_ordering
    //! ### Laws
    //! `less` must define a strict weak ordering. Formally, let
    //!
    //! @code
    //!     x ~ y if and only if !(x < y) && !(y < x)
    //! @endcode
    //!
    //! be the incomparability relation. Then, for all `a`, `b`, `c` of an
    //! orderable data type,
    //!
    //! @code
    //!     !(a < a)                     // Irreflexivity
    //!     if a < b then !(b < a)       // Asymmetry
    //!     if a < b && b < c then a < c // Transitivity
    //!     if a ~ b && b ~ c then a ~ c // Transitivity of incomparability
    //! @endcode
    struct Orderable {
        BOOST_HANA_BINARY_TYPECLASS(Orderable);
        struct less_mcd;
        template <typename I1, typename I2>
        struct integral_constant_mcd;
        template <typename T, typename U>
        struct default_instance;
    };

    //! Returns a `Logical` representing whether `x` is less than `y`.
    //! @relates Orderable
    //!
    //! ### Example
    //! @snippet example/orderable.cpp less
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto less = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _less {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return Orderable::instance<
                datatype_t<X>, datatype_t<Y>
            >::less_impl(
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
    //! ### Example
    //! @snippet example/orderable.cpp less_equal
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto less_equal = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _less_equal {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return Orderable::instance<
                datatype_t<X>, datatype_t<Y>
            >::less_equal_impl(
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
    //! ### Example
    //! @snippet example/orderable.cpp greater
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto greater = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _greater {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return Orderable::instance<
                datatype_t<X>, datatype_t<Y>
            >::greater_impl(
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
    //! ### Example
    //! @snippet example/orderable.cpp greater_equal
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto greater_equal = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _greater_equal {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return Orderable::instance<
                datatype_t<X>, datatype_t<Y>
            >::greater_equal_impl(
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
    //! ### Example
    //! @snippet example/orderable.cpp min
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto min = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _min {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return Orderable::instance<
                datatype_t<X>, datatype_t<Y>
            >::min_impl(
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
    //! ### Example
    //! @snippet example/orderable.cpp max
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto max = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _max {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return Orderable::instance<
                datatype_t<X>, datatype_t<Y>
            >::max_impl(
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
    //! This is not a method of the `Orderable` type class, but just a
    //! convenience function provided with it. Also note that
    //! @code
    //!     ordering(f) == less ^on^ f
    //! @endcode
    //!
    //! ### Example
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
            return less(
                f(detail::std::forward<X>(x)),
                f(detail::std::forward<Y>(y))
            );
        }
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) & {
            return less(
                f(detail::std::forward<X>(x)),
                f(detail::std::forward<Y>(y))
            );
        }
    };

    constexpr detail::create<_ordering> ordering{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ORDERABLE_HPP

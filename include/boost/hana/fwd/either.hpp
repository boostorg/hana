/*!
@file
Forward declares `boost::hana::Either`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EITHER_HPP
#define BOOST_HANA_FWD_EITHER_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/fwd/monad.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents a value with two possibilities.
    //!
    //! An `Either` contains either a left value or a right value, both of
    //! which may have different data types. An `Either` containing a left
    //! value `a` is represented as `left(a)`, and an `Either` containing a
    //! right value `b` is represented as `right(b)`.
    //!
    //! The `Either` data type can sometimes be used to represent a value
    //! which is either correct or an error; by convention, `left` is used
    //! to hold an error and `right` is used to hold a correct value. A
    //! mnemonic is that _right_ also means _correct_.
    //!
    //! ## Instance of
    //! `Comparable`, `Functor`, `Applicative` and `Monad`.
    struct Either {
        struct hana {
            struct enabled_operators : Comparable, Monad { };
        };
    };

    //! Create an `Either` containing the given left value.
    //! @relates Either
    //!
    //! ### Example
    //! @snippet example/either.cpp left
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto left = [](auto&& x) {
        return unspecified-type;
    };
#else
    template <typename X, typename = operators::enable_adl>
    struct _left {
        X value;
        struct hana { using datatype = Either; };

        template <typename F, typename G>
        constexpr decltype(auto) go(F&& f, G const&) const&
        { return detail::std::forward<F>(f)(value); }

        template <typename F, typename G>
        constexpr decltype(auto) go(F f, G const&) &
        { return detail::std::forward<F>(f)(value); }

        template <typename F, typename G>
        constexpr decltype(auto) go(F&& f, G const&) &&
        { return detail::std::forward<F>(f)(detail::std::move(value)); }
    };

    constexpr detail::create<_left> left{};
#endif

    //! Create an `Either` containing the given right value.
    //! @relates Either
    //!
    //! ### Example
    //! @snippet example/either.cpp right
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto right = [](auto&& x) {
        return unspecified-type;
    };
#else
    template <typename X, typename = operators::enable_adl>
    struct _right {
        X value;
        struct hana { using datatype = Either; };

        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) const&
        { return detail::std::forward<G>(g)(value); }

        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) &
        { return detail::std::forward<G>(g)(value); }

        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) &&
        { return detail::std::forward<G>(g)(detail::std::move(value)); }
    };

    constexpr detail::create<_right> right{};
#endif

    //! Apply one of two functions to the value inside an `Either`.
    //! @relates Either
    //!
    //! Specifically, `either` is such that
    //! @code
    //!     either(f, g, left(x)) == f(x)
    //!     either(f, g, right(x)) == g(x)
    //! @endcode
    //!
    //!
    //! @param f
    //! The function applied to the left value if `e` contains a left value.
    //!
    //! @param g
    //! The function applied to the right value if `e` contains a right value.
    //!
    //! @param e
    //! The `Either` value to analyze.
    //!
    //!
    //! ### Example
    //! @snippet example/either.cpp either
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto either = [](auto&& f, auto&& g, auto&& e) -> decltype(auto) {
        if (e is a left(x))
            return forwarded(f)(forwarded(x));
        else if (e is a right(y))
            return forwarded(g)(forwarded(y));
    };
#else
    struct _either {
        template <typename F, typename G, typename E>
        constexpr decltype(auto) operator()(F&& f, G&& g, E&& e) const {
            return detail::std::forward<E>(e).go(
                detail::std::forward<F>(f),
                detail::std::forward<G>(g)
            );
        }
    };

    constexpr _either either{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EITHER_HPP

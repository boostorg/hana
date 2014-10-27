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
#include <boost/hana/detail/constexpr.hpp>
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

    namespace either_detail {
        template <typename X, typename = operators::enable_adl>
        struct left {
            X value;
            struct hana { using datatype = Either; };

            //! @bug
            //! We can't use perfect forwarding because of this bug:
            //! http://llvm.org/bugs/show_bug.cgi?id=20619
            template <typename F, typename G>
            constexpr decltype(auto) go(F f, G) const&
            { return f(value); }

            template <typename F, typename G>
            constexpr decltype(auto) go(F f, G) &
            { return f(value); }

            template <typename F, typename G>
            constexpr decltype(auto) go(F f, G) &&
            { return f(detail::std::move(value)); }
        };

        template <typename X, typename = operators::enable_adl>
        struct right {
            X value;
            struct hana { using datatype = Either; };

            //! @bug
            //! We can't use perfect forwarding because of this bug:
            //! http://llvm.org/bugs/show_bug.cgi?id=20619
            template <typename F, typename G>
            constexpr decltype(auto) go(F, G g) const&
            { return g(value); }

            template <typename F, typename G>
            constexpr decltype(auto) go(F, G g) &
            { return g(value); }

            template <typename F, typename G>
            constexpr decltype(auto) go(F, G g) &&
            { return g(detail::std::move(value)); }
        };
    }

    //! Create an `Either` containing the given left value.
    //! @relates Either
    //!
    //! ### Example
    //! @snippet example/either.cpp left
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto left = [](auto x) {
        unspecified
    };
#else
    BOOST_HANA_CONSTEXPR_LAMBDA auto left = [](auto x) {
        return either_detail::left<decltype(x)>{detail::std::move(x)};
    };
#endif

    //! Create an `Either` containing the given right value.
    //! @relates Either
    //!
    //! ### Example
    //! @snippet example/either.cpp right
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto right = [](auto x) {
        unspecified
    };
#else
    BOOST_HANA_CONSTEXPR_LAMBDA auto right = [](auto x) {
        return either_detail::right<decltype(x)>{detail::std::move(x)};
    };
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
        unspecified
    };
#else
    BOOST_HANA_CONSTEXPR_LAMBDA auto either = [](auto&& f, auto&& g, auto&& e) -> decltype(auto) {
        return detail::std::forward<decltype(e)>(e).go(
            detail::std::forward<decltype(f)>(f),
            detail::std::forward<decltype(g)>(g)
        );
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EITHER_HPP

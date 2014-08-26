/*!
@file
Forward declares `boost::hana::Either`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EITHER_EITHER_HPP
#define BOOST_HANA_EITHER_EITHER_HPP

#include <boost/hana/comparable/operators.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/monad/operators.hpp>


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
    struct Either { struct hana_enabled_operators : Comparable, Monad { }; };

    namespace either_detail {
        template <typename X, typename = operators::enable_adl>
        struct left {
            X value;
            using hana_datatype = Either;

            template <typename F, typename G>
            constexpr auto go(F f, G g) const { return f(value); }
        };

        template <typename X, typename = operators::enable_adl>
        struct right {
            X value;
            using hana_datatype = Either;

            template <typename F, typename G>
            constexpr auto go(F f, G g) const { return g(value); }
        };
    }

    //! Create an `Either` containing the given left value.
    //! @relates Either
    //!
    //! ### Example
    //! @snippet example/either.cpp left
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    BOOST_HANA_CONSTEXPR_LAMBDA auto left = [](auto x) {
        unspecified
    };
#else
    BOOST_HANA_CONSTEXPR_LAMBDA auto left = [](auto x) {
        return either_detail::left<decltype(x)>{x};
    };
#endif

    //! Create an `Either` containing the given right value.
    //! @relates Either
    //!
    //! ### Example
    //! @snippet example/either.cpp right
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    BOOST_HANA_CONSTEXPR_LAMBDA auto right = [](auto x) {
        unspecified
    };
#else
    BOOST_HANA_CONSTEXPR_LAMBDA auto right = [](auto x) {
        return either_detail::right<decltype(x)>{x};
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
    BOOST_HANA_CONSTEXPR_LAMBDA auto either = [](auto f, auto g, auto e) {
#ifdef BOOST_HANA_DOXYGEN_INVOKED
        unspecified
#else
        return e.go(f, g);
#endif
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_EITHER_HPP

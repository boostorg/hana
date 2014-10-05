/*!
@file
Defines the instance of `boost::hana::Comparable` for `boost::hana::Maybe`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_COMPARABLE_HPP
#define BOOST_HANA_MAYBE_COMPARABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/maybe/maybe.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for `Maybe`s.
    //!
    //! Two `Maybe`s are equal if and only if they are both empty or they
    //! both contain a value and those values are equal.
    //!
    //! ### Example
    //! @snippet example/maybe/comparable.cpp main
    template <>
    struct Comparable::instance<Maybe, Maybe> : Comparable::equal_mcd {
        template <typename T, typename U>
        static constexpr decltype(auto)
        equal_impl(maybe_detail::just<T> const& t, maybe_detail::just<U> const& u)
        { return equal(t.val, u.val); }

        template <bool tv, typename T, bool uv, typename U>
        static constexpr auto
        equal_impl(maybe_detail::maybe<tv, T> const&,
                   maybe_detail::maybe<uv, U> const&)
        { return bool_<tv == uv>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_COMPARABLE_HPP

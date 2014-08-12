/*!
@file
Defines `boost::hana::detail::variadic::foldr`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_FOLDR_HPP
#define BOOST_HANA_DETAIL_VARIADIC_FOLDR_HPP

#include <boost/hana/detail/variadic/foldr1.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/type/type.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    template <typename ...Xs, typename F, typename S>
    constexpr auto foldr_impl(F f, S s, ...) {
        return foldr1(f, type<Xs>..., s);
    }

    template <typename ...Xs, typename F, typename S>
    constexpr auto foldr_impl(F f, S, Type*) {
        return foldr1<Xs..., typename S::type>(f);
    }

    template <typename ...Xs, typename F, typename S>
    constexpr auto foldr(F f, S s) {
        return foldr_impl<Xs...>(f, s, (datatype_t<S>*)0);
    }

    template <typename F, typename S, typename ...Xs>
    constexpr auto foldr(F f, S s, Xs ...xs) {
        return foldr1(f, xs..., s);
    }
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_FOLDR_HPP

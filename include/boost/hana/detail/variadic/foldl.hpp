/*!
@file
Defines `boost::hana::detail::variadic::foldl`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_FOLDL_HPP
#define BOOST_HANA_DETAIL_VARIADIC_FOLDL_HPP

#include <boost/hana/detail/variadic/foldl1.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/type_fwd.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    template <typename ...Xs, typename F, typename S>
    constexpr auto foldl_impl(F f, S s, ...) {
        return foldl1(f, s, type<Xs>...);
    }

    template <typename ...Xs, typename F, typename S>
    constexpr auto foldl_impl(F f, S, Type*) {
        return foldl1<typename S::type, Xs...>(f);
    }

    template <typename ...Xs, typename F, typename S>
    constexpr auto foldl(F f, S s) {
        return foldl_impl<Xs...>(f, s, (datatype_t<S>*)0);
    }

    template <typename F, typename S, typename ...Xs>
    constexpr auto foldl(F f, S s, Xs ...xs) {
        return foldl1(f, s, xs...);
    }
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_FOLDL_HPP

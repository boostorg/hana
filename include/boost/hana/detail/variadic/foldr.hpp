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
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/type/type.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    template <typename ...Xs, typename F, typename S>
    constexpr decltype(auto) foldr_impl(F&& f, S&& s, ...) {
        return foldr1(
            detail::std::forward<F>(f),
            type<Xs>...,
            detail::std::forward<S>(s)
        );
    }

    template <typename ...Xs, typename F, typename S>
    constexpr decltype(auto) foldr_impl(F&& f, S, Type*) {
        return foldr1<Xs..., typename S::type>(
            detail::std::forward<F>(f)
        );
    }

    template <typename ...Xs, typename F, typename S>
    constexpr decltype(auto) foldr(F&& f, S&& s) {
        return foldr_impl<Xs...>(
            detail::std::forward<F>(f),
            detail::std::forward<S>(s),
            (datatype_t<S>*)nullptr
        );
    }

    template <typename F, typename S, typename ...Xs>
    constexpr decltype(auto) foldr(F&& f, S&& s, Xs&& ...xs) {
        return foldr1(
            detail::std::forward<F>(f),
            detail::std::forward<Xs>(xs)...,
            detail::std::forward<S>(s)
        );
    }
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_FOLDR_HPP

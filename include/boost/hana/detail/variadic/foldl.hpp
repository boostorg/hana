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
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/fwd/type.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    template <typename ...Xs, typename F, typename S>
    constexpr decltype(auto) foldl_impl(F&& f, S&& s, ...) {
        return foldl1(
            detail::std::forward<F>(f),
            detail::std::forward<S>(s),
            type<Xs>...
        );
    }

    template <typename ...Xs, typename F, typename S>
    constexpr decltype(auto) foldl_impl(F&& f, S, Type*) {
        return foldl1_t<typename S::type, Xs...>(
            detail::std::forward<F>(f)
        );
    }

    template <typename ...Xs, typename F, typename S>
    constexpr decltype(auto) foldl_t(F&& f, S&& s) {
        return foldl_impl<Xs...>(
            detail::std::forward<F>(f),
            detail::std::forward<S>(s),
            (datatype_t<S>*)0
        );
    }

    struct _foldl {
        template <typename F, typename S, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, S&& s, Xs&& ...xs) const { 
            return foldl1(
                detail::std::forward<F>(f),
                detail::std::forward<S>(s),
                detail::std::forward<Xs>(xs)...
            );
        }
    };

    constexpr _foldl foldl{};
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_FOLDL_HPP

/*!
@file
Defines `boost::hana::detail::variadic::split_at`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_SPLIT_AT_HPP
#define BOOST_HANA_DETAIL_VARIADIC_SPLIT_AT_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/functional/partial.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    namespace split_at_detail {
        template <detail::std::size_t n>
        struct split_at_rec;
    }

    template <detail::std::size_t n>
    constexpr split_at_detail::split_at_rec<n> split_at{};

    namespace split_at_detail {
        template <detail::std::size_t n>
        struct split_at_rec {
            template <typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename X7, typename X8, typename ...Xs>
            constexpr decltype(auto) operator()(X1&& x1, X2&& x2, X3&& x3, X4&& x4, X5&& x5, X6&& x6, X7&& x7, X8&& x8, Xs&& ...xs) const {
                return [=](auto&& f) -> decltype(auto) {
                    return split_at<n - 8>(xs...)(partial(f, x1, x2, x3, x4, x5, x6, x7, x8));
                };
            }
        };
    }

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto split_at<0> = [](auto&& ...xs) {
        return [=](auto&& f) -> decltype(auto) {
            return detail::std::forward<decltype(f)>(f)()(xs...);
        };
    };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto split_at<1> = [](auto&& x1, auto&& ...xs) {
        return [=](auto&& f) -> decltype(auto) {
            return detail::std::forward<decltype(f)>(f)(x1)(xs...);
        };
    };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto split_at<2> = [](auto&& x1, auto&& x2, auto&& ...xs) {
        return [=](auto&& f) -> decltype(auto) {
            return detail::std::forward<decltype(f)>(f)(x1, x2)(xs...);
        };
    };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto split_at<3> = [](auto&& x1, auto&& x2, auto&& x3, auto&& ...xs) {
        return [=](auto&& f) -> decltype(auto) {
            return detail::std::forward<decltype(f)>(f)(x1, x2, x3)(xs...);
        };
    };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto split_at<4> = [](auto&& x1, auto&& x2, auto&& x3, auto&& x4, auto&& ...xs) {
        return [=](auto&& f) -> decltype(auto) {
            return detail::std::forward<decltype(f)>(f)(x1, x2, x3, x4)(xs...);
        };
    };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto split_at<5> = [](auto&& x1, auto&& x2, auto&& x3, auto&& x4, auto&& x5, auto&& ...xs) {
        return [=](auto&& f) -> decltype(auto) {
            return detail::std::forward<decltype(f)>(f)(x1, x2, x3, x4, x5)(xs...);
        };
    };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto split_at<6> = [](auto&& x1, auto&& x2, auto&& x3, auto&& x4, auto&& x5, auto&& x6, auto&& ...xs) {
        return [=](auto&& f) -> decltype(auto) {
            return detail::std::forward<decltype(f)>(f)(x1, x2, x3, x4, x5, x6)(xs...);
        };
    };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto split_at<7> = [](auto&& x1, auto&& x2, auto&& x3, auto&& x4, auto&& x5, auto&& x6, auto&& x7, auto&& ...xs) {
        return [=](auto&& f) -> decltype(auto) {
            return detail::std::forward<decltype(f)>(f)(x1, x2, x3, x4, x5, x6, x7)(xs...);
        };
    };
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_SPLIT_AT_HPP

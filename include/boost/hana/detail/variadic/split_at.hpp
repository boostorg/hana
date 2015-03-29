/*!
@file
Defines `boost::hana::detail::variadic::split_at`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_SPLIT_AT_HPP
#define BOOST_HANA_DETAIL_VARIADIC_SPLIT_AT_HPP

#include <boost/hana/detail/reverse_partial.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/functional/partial.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    template <detail::std::size_t n>
    struct _split_at {
        template <typename F, typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename X7, typename X8, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4, X5&& x5, X6&& x6, X7&& x7, X8&& x8, Xs&& ...xs) const {
            return _split_at<n - 8>{}(
                hana::partial(detail::std::forward<F>(f),
                    detail::std::forward<X1>(x1),
                    detail::std::forward<X2>(x2),
                    detail::std::forward<X3>(x3),
                    detail::std::forward<X4>(x4),
                    detail::std::forward<X5>(x5),
                    detail::std::forward<X6>(x6),
                    detail::std::forward<X7>(x7),
                    detail::std::forward<X8>(x8)
                ),
                detail::std::forward<Xs>(xs)...
            );
        }
    };

    template <>
    struct _split_at<0> {
        template <typename F, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, Xs&& ...xs) const {
            return detail::std::forward<F>(f)()(detail::std::forward<Xs>(xs)...);
        }
    };

    template <>
    struct _split_at<1> {
        template <typename F, typename X1, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, X1&& x1, Xs&& ...xs) const {
            return detail::std::forward<F>(f)(
                detail::std::forward<X1>(x1)
            )(detail::std::forward<Xs>(xs)...);
        }
    };

    template <>
    struct _split_at<2> {
        template <typename F, typename X1, typename X2, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, X1&& x1, X2&& x2, Xs&& ...xs) const {
            return detail::std::forward<F>(f)(
                detail::std::forward<X1>(x1),
                detail::std::forward<X2>(x2)
            )(detail::std::forward<Xs>(xs)...);
        }
    };

    template <>
    struct _split_at<3> {
        template <typename F, typename X1, typename X2, typename X3, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, X1&& x1, X2&& x2, X3&& x3, Xs&& ...xs) const {
            return detail::std::forward<F>(f)(
                detail::std::forward<X1>(x1),
                detail::std::forward<X2>(x2),
                detail::std::forward<X3>(x3)
            )(detail::std::forward<Xs>(xs)...);
        }
    };

    template <>
    struct _split_at<4> {
        template <typename F, typename X1, typename X2, typename X3, typename X4, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4, Xs&& ...xs) const {
            return detail::std::forward<F>(f)(
                detail::std::forward<X1>(x1),
                detail::std::forward<X2>(x2),
                detail::std::forward<X3>(x3),
                detail::std::forward<X4>(x4)
            )(detail::std::forward<Xs>(xs)...);
        }
    };

    template <>
    struct _split_at<5> {
        template <typename F, typename X1, typename X2, typename X3, typename X4, typename X5, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4, X5&& x5, Xs&& ...xs) const {
            return detail::std::forward<F>(f)(
                detail::std::forward<X1>(x1),
                detail::std::forward<X2>(x2),
                detail::std::forward<X3>(x3),
                detail::std::forward<X4>(x4),
                detail::std::forward<X5>(x5)
            )(detail::std::forward<Xs>(xs)...);
        }
    };

    template <>
    struct _split_at<6> {
        template <typename F, typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4, X5&& x5, X6&& x6, Xs&& ...xs) const {
            return detail::std::forward<F>(f)(
                detail::std::forward<X1>(x1),
                detail::std::forward<X2>(x2),
                detail::std::forward<X3>(x3),
                detail::std::forward<X4>(x4),
                detail::std::forward<X5>(x5),
                detail::std::forward<X6>(x6)
            )(detail::std::forward<Xs>(xs)...);
        }
    };

    template <>
    struct _split_at<7> {
        template <typename F, typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename X7, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4, X5&& x5, X6&& x6, X7&& x7, Xs&& ...xs) const {
            return detail::std::forward<F>(f)(
                detail::std::forward<X1>(x1),
                detail::std::forward<X2>(x2),
                detail::std::forward<X3>(x3),
                detail::std::forward<X4>(x4),
                detail::std::forward<X5>(x5),
                detail::std::forward<X6>(x6),
                detail::std::forward<X7>(x7)
            )(detail::std::forward<Xs>(xs)...);
        }
    };

    template <detail::std::size_t n>
    struct _make_split_at {
        template <typename ...Xs>
        constexpr decltype(auto) operator()(Xs&& ...xs) const {
            return detail::reverse_partial(_split_at<n>{}, detail::std::forward<Xs>(xs)...);
        }
    };

    template <detail::std::size_t n>
    constexpr _make_split_at<n> split_at{};
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_SPLIT_AT_HPP

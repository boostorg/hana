/*!
@file
Defines `boost::hana::detail::variadic::take`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_TAKE_HPP
#define BOOST_HANA_DETAIL_VARIADIC_TAKE_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/detail/variadic/split_at.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    namespace take_detail {
        template <detail::std::size_t n>
        struct take_impl {
            template <typename ...Xs>
            constexpr decltype(auto) operator()(Xs&& ...xs) const {
                return split_at<n>(detail::std::forward<Xs>(xs)...)(
                    [](auto&& ...xs) {
                        return [=](auto&& ...ys) {
                            return [=](auto&& f) -> decltype(auto) {
                                return detail::std::forward<decltype(f)>(f)(xs...);
                            };
                        };
                    }
                );
            }
        };
    }

    template <detail::std::size_t n>
    constexpr take_detail::take_impl<n> take{};
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_TAKE_HPP

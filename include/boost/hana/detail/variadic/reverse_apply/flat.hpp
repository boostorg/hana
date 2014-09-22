/*!
@file
Defines `boost::hana::detail::variadic::reverse_apply_flat`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_REVERSE_APPLY_FLAT_HPP
#define BOOST_HANA_DETAIL_VARIADIC_REVERSE_APPLY_FLAT_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/variadic/at.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    template <int ...i, typename F, typename ...X>
    constexpr decltype(auto)
    reverse_apply_flat_helper(detail::std::integer_sequence<int, i...>, F&& f, X&& ...x)
    {
        return detail::std::forward<F>(f)(
            detail::variadic::at<sizeof...(x) - i - 1>(
                detail::std::forward<X>(x)...
            )...
        );
    }

    template <typename F, typename ...X>
    constexpr decltype(auto) reverse_apply_flat(F&& f, X&& ...x) {
        return reverse_apply_flat_helper(
            detail::std::make_integer_sequence<int, sizeof...(x)>{},
            detail::std::forward<F>(f),
            detail::std::forward<X>(x)...
        );
    }
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_REVERSE_APPLY_FLAT_HPP

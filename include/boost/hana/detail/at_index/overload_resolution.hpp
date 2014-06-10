/*!
@file
Defines `boost::hana::detail::at_index::overload_resolution`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_AT_INDEX_OVERLOAD_RESOLUTION_HPP
#define BOOST_HANA_DETAIL_AT_INDEX_OVERLOAD_RESOLUTION_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/integer_sequence.hpp>

#include <cstddef>


namespace boost { namespace hana { namespace detail { namespace at_index {
    struct eat { template <typename ...T> eat(T&&...) { } };

    template <std::size_t ...ignore>
    constexpr auto overload_impl(index_sequence<ignore...>) {
        return [](decltype(ignore, eat{})..., auto nth, auto ...rest)
        { return nth; };
    }

    template <std::size_t n>
    BOOST_HANA_CONSTEXPR_LAMBDA auto overload_resolution = [](auto ...xs) {
        return overload_impl(make_index_sequence<n>{})(xs...);
    };
}}}} // end namespace boost::hana::detail::at_index

#endif // !BOOST_HANA_DETAIL_AT_INDEX_OVERLOAD_RESOLUTION_HPP

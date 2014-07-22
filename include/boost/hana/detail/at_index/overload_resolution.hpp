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
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>


namespace boost { namespace hana { namespace detail { namespace at_index {
    template <std::size_t ...ignore>
    constexpr auto overload_impl(std::index_sequence<ignore...>) {
        return [](decltype(ignore, (void*)0)..., auto nth, ...)
        { return *nth; };
    }

    template <std::size_t n>
    BOOST_HANA_CONSTEXPR_LAMBDA auto overload_resolution = [](auto ...xs) {
        return overload_impl(std::make_index_sequence<n>{})(&xs...);
    };
}}}} // end namespace boost::hana::detail::at_index

#endif // !BOOST_HANA_DETAIL_AT_INDEX_OVERLOAD_RESOLUTION_HPP

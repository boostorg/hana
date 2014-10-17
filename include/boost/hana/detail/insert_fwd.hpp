/*!
@file
Forward declares `boost::hana::insert`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_INSERT_FWD_HPP
#define BOOST_HANA_DETAIL_INSERT_FWD_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    template <typename T>
    struct insert_impl;

    BOOST_HANA_CONSTEXPR_LAMBDA auto insert = [](auto&& set, auto&& x) -> decltype(auto) {
        return insert_impl<
            datatype_t<decltype(set)>
        >::apply(
            detail::std::forward<decltype(set)>(set),
            detail::std::forward<decltype(x)>(x)
        );
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_INSERT_FWD_HPP

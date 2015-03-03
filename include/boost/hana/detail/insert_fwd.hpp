/*!
@file
Forward declares `boost::hana::insert`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_INSERT_FWD_HPP
#define BOOST_HANA_DETAIL_INSERT_FWD_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    template <typename T>
    struct insert_impl;

    struct _insert {
        template <typename Set, typename X>
        constexpr decltype(auto) operator()(Set&& set, X&& x) const {
            return insert_impl<
                datatype_t<Set>
            >::apply(
                detail::std::forward<Set>(set),
                detail::std::forward<X>(x)
            );
        }
    };

    constexpr _insert insert{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_INSERT_FWD_HPP

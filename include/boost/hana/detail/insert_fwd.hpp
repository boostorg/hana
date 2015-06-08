/*!
@file
Forward declares `boost::hana::insert`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_INSERT_FWD_HPP
#define BOOST_HANA_DETAIL_INSERT_FWD_HPP

#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/when.hpp>


namespace boost { namespace hana {
    template <typename T, typename = void>
    struct insert_impl;

    template <typename T, typename>
    struct insert_impl : insert_impl<T, when<true>> { };

    struct _insert {
        template <typename Xs, typename ...Args>
        constexpr decltype(auto) operator()(Xs&& xs, Args&& ...args) const {
            return insert_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Args&&>(args)...
            );
        }
    };

    constexpr _insert insert{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_INSERT_FWD_HPP

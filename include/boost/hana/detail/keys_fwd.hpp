/*!
@file
Forward declares `boost::hana::keys`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_KEYS_FWD_HPP
#define BOOST_HANA_DETAIL_KEYS_FWD_HPP

#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/when.hpp>


namespace boost { namespace hana {
    template <typename T, typename = void>
    struct keys_impl;

    template <typename T, typename>
    struct keys_impl : keys_impl<T, when<true>> { };

    struct _keys {
        template <typename Map>
        constexpr decltype(auto) operator()(Map&& map) const {
            return keys_impl<typename datatype<Map>::type>::apply(
                static_cast<Map&&>(map)
            );
        }
    };

    constexpr _keys keys{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_KEYS_FWD_HPP

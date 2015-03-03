/*!
@file
Forward declares `boost::hana::ext::std::Pair`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EXT_STD_PAIR_HPP
#define BOOST_HANA_FWD_EXT_STD_PAIR_HPP

#include <boost/hana/core/datatype.hpp>

#include <utility>


namespace boost { namespace hana {
    namespace ext { namespace std { struct Pair; }}

    template <typename First, typename Second>
    struct datatype< ::std::pair<First, Second>> {
        using type = ext::std::Pair;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EXT_STD_PAIR_HPP

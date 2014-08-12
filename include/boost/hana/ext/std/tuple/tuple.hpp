/*!
@file
Forward declares `boost::hana::StdTuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_TUPLE_TUPLE_HPP
#define BOOST_HANA_EXT_STD_TUPLE_TUPLE_HPP

#include <boost/hana/core/datatype.hpp>

#include <tuple>


namespace boost { namespace hana {
    struct StdTuple { };

    template <typename ...Xs>
    struct datatype<std::tuple<Xs...>> {
        using type = StdTuple;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_TUPLE_TUPLE_HPP

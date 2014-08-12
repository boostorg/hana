/*!
@file
Forward declares `boost::hana::BoostTuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_TUPLE_TUPLE_HPP
#define BOOST_HANA_EXT_BOOST_TUPLE_TUPLE_HPP

#include <boost/hana/core/datatype.hpp>

#include <boost/tuple/tuple.hpp>


namespace boost { namespace hana {
    struct BoostTuple;

    template <typename ...Xs>
    struct datatype<boost::tuple<Xs...>> {
        using type = BoostTuple;
    };

    template <typename H, typename T>
    struct datatype<boost::tuples::cons<H, T>> {
        using type = BoostTuple;
    };

    template <>
    struct datatype<boost::tuples::null_type> {
        using type = BoostTuple;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_TUPLE_TUPLE_HPP

/*!
@file
Forward declares `boost::hana::ext::boost::Tuple`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EXT_BOOST_TUPLE_HPP
#define BOOST_HANA_FWD_EXT_BOOST_TUPLE_HPP

#include <boost/hana/core/datatype.hpp>

#include <boost/tuple/tuple.hpp>


namespace boost { namespace hana {
    namespace ext { namespace boost { struct Tuple; }}

    template <typename ...Xs>
    struct datatype< ::boost::tuple<Xs...>> {
        using type = ext::boost::Tuple;
    };

    template <typename H, typename T>
    struct datatype< ::boost::tuples::cons<H, T>> {
        using type = ext::boost::Tuple;
    };

    template <>
    struct datatype< ::boost::tuples::null_type> {
        using type = ext::boost::Tuple;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EXT_BOOST_TUPLE_HPP

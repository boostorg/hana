/*!
@file
Forward declares `boost::hana::ext::std::IntegerSequence`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EXT_STD_INTEGER_SEQUENCE_HPP
#define BOOST_HANA_FWD_EXT_STD_INTEGER_SEQUENCE_HPP

#include <boost/hana/core/datatype.hpp>

#include <utility>


namespace boost { namespace hana {
    namespace ext { namespace std { struct IntegerSequence; }}

    template <typename T, T ...v>
    struct datatype< ::std::integer_sequence<T, v...>> {
        using type = ext::std::IntegerSequence;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EXT_STD_INTEGER_SEQUENCE_HPP

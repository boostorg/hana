/*!
@file
Forward declares `boost::hana::ext::std::Array`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EXT_STD_ARRAY_HPP
#define BOOST_HANA_FWD_EXT_STD_ARRAY_HPP

#include <boost/hana/core/datatype.hpp>

#include <array>
#include <cstddef>


namespace boost { namespace hana {
    namespace ext { namespace std { struct Array; }}

    template <typename T, std::size_t N>
    struct datatype< ::std::array<T, N>> {
        using type = ext::std::Array;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EXT_STD_ARRAY_HPP

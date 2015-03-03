/*!
@file
Forward declares `boost::hana::ext::std::Ratio`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EXT_STD_RATIO_HPP
#define BOOST_HANA_FWD_EXT_STD_RATIO_HPP

#include <boost/hana/core/datatype.hpp>

#include <cstdint>
#include <ratio>


namespace boost { namespace hana {
    namespace ext { namespace std { struct Ratio; }}

    template < ::std::intmax_t num, ::std::intmax_t den>
    struct datatype< ::std::ratio<num, den>> {
        using type = ext::std::Ratio;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EXT_STD_RATIO_HPP

/*!
@file
Defines `boost::hana::detail::at_index::best`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_AT_INDEX_BEST_HPP
#define BOOST_HANA_DETAIL_AT_INDEX_BEST_HPP

#include <boost/hana/detail/at_index/overload_resolution.hpp>


namespace boost { namespace hana { namespace detail { namespace at_index {
    BOOST_HANA_CONSTEXPR_LAMBDA auto best = overload_resolution;
}}}}

#endif // !BOOST_HANA_DETAIL_AT_INDEX_BEST_HPP

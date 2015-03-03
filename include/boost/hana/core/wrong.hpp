/*!
@file
Defines `boost::hana::wrong`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_WRONG_HPP
#define BOOST_HANA_CORE_WRONG_HPP

#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana {
    template <typename ...>
    struct wrong : detail::std::false_type { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_WRONG_HPP

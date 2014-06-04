/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


struct x { };

int main() {
    BOOST_HANA_STATIC_ASSERT(decltype_(1) == type<int>);
    BOOST_HANA_STATIC_ASSERT(decltype_('1') == type<char>);
    BOOST_HANA_STATIC_ASSERT(decltype_(x{}) == type<x>);
}

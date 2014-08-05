/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/instance.hpp>

#include <boost/hana/detail/assert.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTANT_ASSERT(init(list(0)) == list());
    BOOST_HANA_CONSTEXPR_ASSERT(init(list(0, '1')) == list(0));
    BOOST_HANA_CONSTEXPR_ASSERT(init(list(0, '1', 2.2)) == list(0, '1'));
    BOOST_HANA_CONSTEXPR_ASSERT(init(list(0, '1', 2.2, nullptr)) == list(0, '1', 2.2));
}

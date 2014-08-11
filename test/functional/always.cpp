/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/assert.hpp>
using namespace boost::hana;


struct nonpod { virtual ~nonpod() { } };

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(always('a')() == 'a');
    BOOST_HANA_CONSTEXPR_ASSERT(always('a')(1) == 'a');
    BOOST_HANA_CONSTEXPR_ASSERT(always('a')(1, '2') == 'a');
    BOOST_HANA_CONSTEXPR_ASSERT(always('a')(1, '2', 3.3) == 'a');
    BOOST_HANA_CONSTEXPR_ASSERT(always('a')(1, '2', 3.3, nullptr) == 'a');
    BOOST_HANA_RUNTIME_ASSERT(always('a')(1, '2', 3.3, nullptr, nonpod{}) == 'a');
}

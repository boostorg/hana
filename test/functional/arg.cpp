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
    BOOST_HANA_CONSTEXPR_ASSERT(arg<1>(1) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(arg<1>(1, '2') == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(arg<1>(1, '2', 3.3) == 1);
    BOOST_HANA_RUNTIME_ASSERT(arg<1>(1, '2', 3.3, nonpod{}) == 1);

    BOOST_HANA_CONSTEXPR_ASSERT(arg<2>(1, '2') == '2');
    BOOST_HANA_CONSTEXPR_ASSERT(arg<2>(1, '2', 3.3) == '2');
    BOOST_HANA_CONSTEXPR_ASSERT(arg<2>(1, '2', 3.3, 4.4f) == '2');
    BOOST_HANA_RUNTIME_ASSERT(arg<2>(1, '2', 3.3, 4.4f, nonpod{}) == '2');

    BOOST_HANA_CONSTEXPR_ASSERT(arg<3>(1, '2', 3.3) == 3.3);
    BOOST_HANA_CONSTEXPR_ASSERT(arg<3>(1, '2', 3.3, 4.4f) == 3.3);
    BOOST_HANA_RUNTIME_ASSERT(arg<3>(1, '2', 3.3, 4.4f, nonpod{}) == 3.3);

    BOOST_HANA_CONSTEXPR_ASSERT(arg<4>(1, '2', 3.3, 4.4f) == 4.4f);
    BOOST_HANA_RUNTIME_ASSERT(arg<4>(1, '2', 3.3, 4.4f, nonpod{}) == 4.4f);

    nonpod n = arg<5>(1, '2', 3.3, 4.4f, nonpod{});
    (void)n;

    // Check with non-pods before the index we're targetting.
    BOOST_HANA_RUNTIME_ASSERT(arg<2>(nonpod{}, '2') == '2');
    BOOST_HANA_RUNTIME_ASSERT(arg<3>(nonpod{}, '2', 3.3) == 3.3);
    BOOST_HANA_RUNTIME_ASSERT(arg<4>(nonpod{}, '2', 3.3, 4.4f) == 4.4f);
}

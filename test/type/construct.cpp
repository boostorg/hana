/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/assert.hpp>

#include <initializer_list>
#include <utility>
#include <vector>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(type<int>() == int{});
    BOOST_HANA_CONSTEXPR_ASSERT(type<int>(2) == int{2});
    BOOST_HANA_CONSTEXPR_ASSERT(type<float>(2.42f) == float{2.42});
    BOOST_HANA_CONSTEXPR_ASSERT(type<double>(2.42) == double{2.42});
    BOOST_HANA_CONSTEXPR_ASSERT(type<double>(2.2f) == double{2.2f});
    BOOST_HANA_CONSTEXPR_ASSERT(type<float>(2.2) == float{2.2});

    BOOST_HANA_RUNTIME_ASSERT(type<std::vector<int>>(std::initializer_list<int>{1, 2, 3}) == std::vector<int>{1, 2, 3});
    BOOST_HANA_RUNTIME_ASSERT(type<std::pair<int, char>>(1, '2') == std::make_pair(1, '2'));
}

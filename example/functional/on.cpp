/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>

#include <utility>
using namespace boost::hana;


int main() {
    //! [main1]
    BOOST_HANA_CONSTEXPR_LAMBDA auto first = [](auto pair) {
        return pair.first;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto plus = [](auto x, auto y) {
        return x + y;
    };

    std::pair<int, int> x{1, 2}, y{10, 20};

    BOOST_HANA_STATIC_ASSERT(on(plus, first)(x, y) == 1 + 10);
    //! [main1]


    //! [main2]
    BOOST_HANA_CONSTEXPR_LAMBDA auto increment = [](auto x) {
        return x + 1;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto multiply = [](auto x, auto y) {
        return x * y;
    };

    BOOST_HANA_STATIC_ASSERT(on(multiply, increment)(2, 4) == 3 * 5);
    //! [main2]
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/pair.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto plus = [](auto x, auto y) {
        return x + y;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto x = pair(1, 2);
    BOOST_HANA_CONSTEXPR_LAMBDA auto y = pair(10, 20);
    BOOST_HANA_CONSTEXPR_CHECK(on(plus, first)(x, y) == 1 + 10);
    //! [main]
}

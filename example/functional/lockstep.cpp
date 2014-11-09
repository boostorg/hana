/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto plus = [](auto x, auto y) {
        return x + y;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto to_int = [](char c) {
        return static_cast<int>(c) - 48;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) {
        return x + 1;
    };

    BOOST_HANA_CONSTEXPR_CHECK(lockstep(plus)(to_int, inc)('3', 4) == 3 + 5);
    //! [main]
}

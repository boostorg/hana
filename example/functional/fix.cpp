/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto factorial = fix([](auto fact, auto n) {
        if (n == 0) return 1;
        else        return n * fact(n - 1);
    });

    BOOST_HANA_CONSTEXPR_CHECK(factorial(5) == 120);
    //! [main]
}

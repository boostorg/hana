/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/lazy.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = lazy([](auto x) {
        return 1 / x;
    });
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = lazy([](auto x) {
        return x + 1;
    });

    BOOST_HANA_CONSTEXPR_ASSERT(eval(if_(false_, f(0), g(0))) == 0 + 1);
    //! [main]
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto increment = [](auto x) {
        return x + 1;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto double_ = [](auto x) {
        return x * 2;
    };

    BOOST_HANA_STATIC_ASSERT(compose(increment, double_)(3) == (3 * 2) + 1);
    //! [main]
}

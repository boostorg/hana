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
    BOOST_HANA_CONSTEXPR_LAMBDA auto divide = [](auto x, auto y) {
        return x / y;
    };

    BOOST_HANA_STATIC_ASSERT(fbind(divide, arg<2>, arg<1>)(5, 10) == 10 / 5);
    //! [main]
}

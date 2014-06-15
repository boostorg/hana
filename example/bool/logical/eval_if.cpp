/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/logical.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto safe_divide = [](auto x, auto y) {
        return eval_if(y == 0,
            [=](auto) { return 0; },
            [=](auto id) { return id(x) / y; }
        );
    };

    BOOST_HANA_STATIC_ASSERT(safe_divide(6, 3) == 2);
    BOOST_HANA_STATIC_ASSERT(safe_divide(6, 0) == 0);
    //! [main]
}

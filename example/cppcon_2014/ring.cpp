/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/foreign.hpp>

#include "matrix/comparable.hpp"
#include "matrix/ring.hpp"
using namespace boost::hana;
using namespace cppcon;


int main() {
    // mult
    {
        auto a = matrix(
            row(1, 2, 3),
            row(4, 5, 6)
        );

        auto b = matrix(
            row(1, 2),
            row(3, 4),
            row(5, 6)
        );

        BOOST_HANA_CONSTEXPR_CHECK(a * b == matrix(
            row(1*1 + 2*3 + 5*3, 1*2 + 2*4 + 3*6),
            row(4*1 + 3*5 + 5*6, 4*2 + 5*4 + 6*6)
        ));
    }
}

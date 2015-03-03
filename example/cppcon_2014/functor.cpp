/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/functional/placeholder.hpp>
#include <boost/hana/integral_constant.hpp>

#include "matrix/comparable.hpp"
#include "matrix/functor.hpp"
using namespace boost::hana;
using namespace cppcon;


int main() {
    // transform
    {
        auto m = matrix(
            row(1,       int_<2>, 3),
            row(int_<4>, 5,       6),
            row(7,       8,       int_<9>)
        );

        BOOST_HANA_CONSTEXPR_CHECK(transform(m, _ + int_<1>) ==
            matrix(
                row(2,       int_<3>, 4),
                row(int_<5>, 6,       7),
                row(8,       9,       int_<10>)
            )
        );
    }
}

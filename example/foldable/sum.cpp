/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/range.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(
        sum(range(int_<1>, int_<6>)) == int_<1 + 2 + 3 + 4 + 5>
    );

    BOOST_HANA_CONSTEXPR_ASSERT(
        sum(list(1, int_<3>, long_<-5>, 9)) == 1 + 3 - 5 + 9
    );
    //! [main]
}

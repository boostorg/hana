/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/range.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(
        product(range(int_<1>, int_<6>)) == int_<1 * 2 * 3 * 4 * 5>
    );
    //! [main]
}

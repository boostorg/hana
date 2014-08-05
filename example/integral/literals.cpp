/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    using namespace literals;

    BOOST_HANA_CONSTANT_ASSERT(1234_c == llong<1234>);
    BOOST_HANA_CONSTANT_ASSERT(-1234_c == llong<-1234>);
    BOOST_HANA_CONSTANT_ASSERT(sum(list(1_c, 2_c, 3_c, 4_c)) == 10_c);
    //! [main]
}

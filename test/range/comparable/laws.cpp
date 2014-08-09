/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/comparable/detail/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTANT_ASSERT(Comparable::laws::check(
        list(
            range(int_<0>, int_<0>),
            range(int_<1>, int_<1>),

            range(int_<0>, int_<1>),
            range(long_<0>, long_<1>),
            range(long_<0>, int_<1>),

            range(int_<3>, int_<6>)
        )
    ));
}

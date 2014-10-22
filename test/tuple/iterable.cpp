/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>

#include <test/injection.hpp>
#include <test/iso/list/iterable.hpp>
using namespace boost::hana;


int main() {
    // operators
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            tuple(test::x<0>, test::x<1>)[int_<0>],
            test::x<0>
        ));
    }

    List_iterable<Tuple>();
}

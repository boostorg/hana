/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(int_<1> + int_<3> == int_<4>);

    // Mixed-type operations are supported:
    BOOST_HANA_CONSTANT_ASSERT(size_t<3> * ushort<5> == size_t<15>);
    BOOST_HANA_CONSTANT_ASSERT(size_t<15> == int_<15>);
    //! [main]
}

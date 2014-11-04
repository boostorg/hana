/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    //! [comparable]
    BOOST_HANA_CONSTANT_ASSERT(equal(int_<1>, int_<1>));
    BOOST_HANA_CONSTANT_ASSERT(not_equal(int_<1>, int_<2>));

    BOOST_HANA_CONSTANT_ASSERT(equal(int_<1>, long_<1>));
    BOOST_HANA_CONSTANT_ASSERT(not_equal(int_<1>, long_<2>));
    //! [comparable]

    //! [literals]
    using namespace literals; // contains the _c suffix

    BOOST_HANA_CONSTANT_ASSERT(1234_c == llong<1234>);
    BOOST_HANA_CONSTANT_ASSERT(-1234_c == llong<-1234>);
    BOOST_HANA_CONSTANT_ASSERT(1_c + (3_c * 4_c) == llong<1 + (3 * 4)>);
    //! [literals]

    //! [operators]
    BOOST_HANA_CONSTANT_ASSERT(int_<1> + int_<3> == int_<4>);

    // Mixed-type operations are supported:
    BOOST_HANA_CONSTANT_ASSERT(size_t<3> * ushort<5> == size_t<15>);
    BOOST_HANA_CONSTANT_ASSERT(size_t<15> == int_<15>);
    //! [operators]

    //! [orderable]
    BOOST_HANA_CONSTANT_ASSERT(less(int_<-3>, int_<3>));
    BOOST_HANA_CONSTANT_ASSERT(max(int_<-3>, long_<2>) == long_<2>);
    BOOST_HANA_CONSTANT_ASSERT(min(int_<-3>, long_<2>) == int_<-3>);
    //! [orderable]
}

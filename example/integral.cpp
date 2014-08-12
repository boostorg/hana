/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [comparable]
    BOOST_HANA_CONSTANT_ASSERT(equal(int_<1>, int_<1>));
    BOOST_HANA_CONSTANT_ASSERT(not_equal(int_<1>, int_<2>));

    BOOST_HANA_CONSTANT_ASSERT(equal(int_<1>, long_<1>));
    BOOST_HANA_CONSTANT_ASSERT(not_equal(int_<1>, long_<2>));

    // Incomparable integrals are considered unequal.
    struct Person {
        int age;
        // ...
    };
    BOOST_HANA_CONSTANT_ASSERT(not_equal(
        integral<int, 1>,
        integral<int Person::*, &Person::age>
    ));
    //! [comparable]

    //! [literals]
    using namespace literals; // contains the _c suffix

    BOOST_HANA_CONSTANT_ASSERT(1234_c == llong<1234>);
    BOOST_HANA_CONSTANT_ASSERT(-1234_c == llong<-1234>);
    BOOST_HANA_CONSTANT_ASSERT(sum(list(1_c, 2_c, 3_c, 4_c)) == 10_c);
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

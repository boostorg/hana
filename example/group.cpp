/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    //! [minus]
    BOOST_HANA_CONSTANT_CHECK(minus(int_<3>, int_<5>) == int_<-2>);
    BOOST_HANA_CONSTEXPR_CHECK(minus(1, 2) == -1);
    //! [minus]

    //! [negate]
    BOOST_HANA_CONSTANT_CHECK(negate(int_<3>) == int_<-3>);
    BOOST_HANA_CONSTEXPR_CHECK(negate(2) == -2);
    //! [negate]
}

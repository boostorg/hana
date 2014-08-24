/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    //! [zero]
    BOOST_HANA_CONSTANT_ASSERT(zero<Integral> == int_<0>);
    BOOST_HANA_CONSTEXPR_ASSERT(zero<long> == 0l);
    //! [zero]

    //! [plus]
    BOOST_HANA_CONSTANT_ASSERT(plus(int_<3>, int_<5>) == int_<8>);
    BOOST_HANA_CONSTEXPR_ASSERT(plus(1, 2) == 3);
    //! [plus]
}

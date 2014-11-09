/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    {
        //! [integral_constant]
        BOOST_HANA_CONSTANT_CHECK(
            integral_constant<Integral, int, 2>
            ==
            int_<2>
        );

        BOOST_HANA_CONSTANT_CHECK(equal(
            integral_constant<ext::std::IntegralConstant, bool, true>,
            std::true_type{}
        ));
        //! [integral_constant]
    }
    {
        //! [enumerable]
        BOOST_HANA_CONSTANT_CHECK(
            succ(integral_constant<Integral, int, 2>)
            ==
            integral_constant<Integral, int, 3>
        );

        BOOST_HANA_CONSTANT_CHECK(
            pred(integral_constant<Integral, int, 2>)
            ==
            integral_constant<Integral, int, 1>
        );
        //! [enumerable]
    }
}

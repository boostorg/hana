/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign/comparable.hpp>
#include <boost/hana/foreign/enumerable.hpp>
#include <boost/hana/integral/comparable.hpp>
#include <boost/hana/integral/enumerable.hpp>
using namespace boost::hana;


int main() {
    {
        //! [succ]
        BOOST_HANA_CONSTANT_ASSERT(succ(int_<4>) == int_<5>);
        BOOST_HANA_CONSTEXPR_ASSERT(succ(10) == 11);
        //! [succ]
    }

    {
        //! [pred]
        BOOST_HANA_CONSTANT_ASSERT(pred(int_<4>) == int_<3>);
        BOOST_HANA_CONSTEXPR_ASSERT(pred(10) == 9);
        //! [pred]
    }
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(length(list()) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(length(list(1, '2', 3.0)) == int_<3>);

    BOOST_HANA_CONSTANT_ASSERT(length(nothing) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(length(just('x')) == int_<1>);
    //! [main]
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(
        slice(tuple(1, '2', 3.3, type<float>), int_<1>, int_<3>)
        ==
        tuple('2', 3.3)
    );
    //! [main]
}

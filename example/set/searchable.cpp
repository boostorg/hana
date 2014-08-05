/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/set.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto xs = set(int_<0>, int_<1>, int_<2>);
    BOOST_HANA_CONSTANT_ASSERT(lookup(xs, int_<0>) == just(int_<0>));
    BOOST_HANA_CONSTANT_ASSERT(lookup(xs, int_<3>) == nothing);
    //! [main]
}

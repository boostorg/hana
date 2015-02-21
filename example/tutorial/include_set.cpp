/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

//! [main]
#include <boost/hana/set.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto xs = set(1, 2.2, 'x');
    BOOST_HANA_CONSTEXPR_CHECK(elem(xs, 'x'));
}
//! [main]

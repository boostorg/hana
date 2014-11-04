/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(trait_<std::is_integral>(2));
    BOOST_HANA_CONSTANT_ASSERT(trait_<std::is_integral>(2ll));
    BOOST_HANA_CONSTANT_ASSERT(not_(trait_<std::is_integral>(2.2)));
    //! [main]
}

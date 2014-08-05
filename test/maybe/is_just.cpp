/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/assert.hpp>
using namespace boost::hana;


struct anything { };

int main() {
    BOOST_HANA_CONSTANT_ASSERT( is_just(just(anything{})));
    BOOST_HANA_CONSTANT_ASSERT(!is_just(nothing));
}

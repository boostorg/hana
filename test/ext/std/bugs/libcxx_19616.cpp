/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/foreign.hpp>

#include <tuple>
using namespace boost::hana;


int main() {
    // Make sure we workaround the bug at:
    // http://llvm.org/bugs/show_bug.cgi?id=19616
    BOOST_HANA_CONSTEXPR_ASSERT(equal(
        lift<ext::std::Tuple>(std::make_tuple(1)),
        std::make_tuple(std::make_tuple(1))
    ));
}

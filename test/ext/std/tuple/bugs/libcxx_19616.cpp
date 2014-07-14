/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include <tuple>
using namespace boost::hana;


int main() {
    // Make sure we workaround the bug at:
    // http://llvm.org/bugs/show_bug.cgi?id=19616
    BOOST_HANA_STATIC_ASSERT(
        lift<StdTuple>(std::make_tuple(1)) ==
        std::make_tuple(std::make_tuple(1))
    );

    // The following shows the actual bug:
#if 0
    static_assert(
        std::tuple_cat(
            std::make_tuple(std::make_tuple(1)),
            std::make_tuple()
        )
        ==
        std::make_tuple(std::make_tuple(1))
    , "");
#endif
}

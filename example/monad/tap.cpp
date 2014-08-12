/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <cassert>
#include <sstream>
using namespace boost::hana;


int main() {
    //! [main]
    std::stringstream before, after;

    auto xs = tuple(1, 2, 3)
        | tap<Tuple>([&](auto x) { before << x << ' '; })
        | [](auto x) { return tuple(x, -x); }
        | tap<Tuple>([&](auto x) { after << x << ' '; });

    assert(before.str() == "1 2 3 ");
    assert(after.str() == "1 -1 2 -2 3 -3 ");
    assert(xs == tuple(1, -1, 2, -2, 3, -3));
    //! [main]
}

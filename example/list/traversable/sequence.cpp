/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <cassert>
#include <string>
using namespace boost::hana;
using namespace std::string_literals;


int main() {
    //! [main]
    assert(
        sequence<Tuple>(
            tuple(tuple("a1"s, "a2"s), tuple("b1"s), tuple("c1", "c2", "c3"))
        )
        ==
        tuple(
            tuple("a1"s, "b1"s, "c1"s),
            tuple("a1"s, "b1"s, "c2"s),
            tuple("a1"s, "b1"s, "c3"s),

            tuple("a2"s, "b1"s, "c1"s),
            tuple("a2"s, "b1"s, "c2"s),
            tuple("a2"s, "b1"s, "c3"s)
        )
    );
    //! [main]
}

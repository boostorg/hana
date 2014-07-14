/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <cassert>
#include <string>
using namespace boost::hana;
using namespace std::string_literals;


int main() {
    //! [main]
    assert(
        sequence<List>(
            list(list("a1"s, "a2"s), list("b1"s), list("c1", "c2", "c3"))
        )
        ==
        list(
            list("a1"s, "b1"s, "c1"s),
            list("a1"s, "b1"s, "c2"s),
            list("a1"s, "b1"s, "c3"s),

            list("a2"s, "b1"s, "c1"s),
            list("a2"s, "b1"s, "c2"s),
            list("a2"s, "b1"s, "c3"s)
        )
    );
    //! [main]
}

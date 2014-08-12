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
    std::stringstream ss;
    for_each(list(0, '1', "234", 5.5), [&](auto x) {
        ss << x << ' ';
    });

    assert(ss.str() == "0 1 234 5.5 ");
    //! [main]
}

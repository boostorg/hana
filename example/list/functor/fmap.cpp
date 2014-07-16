/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/instance.hpp>

#include <cassert>
#include <sstream>
#include <string>
using namespace boost::hana;


int main() {
    //! [main]
    auto to_string = [](auto x) {
        return (std::ostringstream{} << x).str();
    };

    assert(
        fmap(to_string, list(1, '2', "345", std::string{"67"})) ==
        list("1", "2", "345", "67")
    );
    //! [main]
}

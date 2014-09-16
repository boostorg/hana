/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <sstream>
#include <string>
using namespace boost::hana;


int main() {
    //! [foldl]
    auto to_string = [](auto x) {
        return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
    };

    auto show = [=](auto x, auto y) {
        return "(" + to_string(x) + " + " + to_string(y) + ")";
    };

    BOOST_HANA_RUNTIME_ASSERT(foldl(tuple(2, "3", '4'), "1", show) == "(((1 + 2) + 3) + 4)");
    //! [foldl]
}

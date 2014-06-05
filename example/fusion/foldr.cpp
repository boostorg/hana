/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <cassert>
#include <sstream>
#include <string>
using namespace boost::hana;


auto make_string = [](auto x, std::string str) {
    return str + (std::ostringstream{} << x).str();
};

int main() {
    assert(foldr(make_string, "4", list(1, "2", '3')) == "4321");
}

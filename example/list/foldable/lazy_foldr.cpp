/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/range.hpp>

#include <cassert>
#include <sstream>
using namespace boost::hana;
using namespace literals;


int main() {
    //! [main]
    auto to_string = [](auto x) { return (std::ostringstream{} << x).str(); };

    auto show = [=](auto x, auto y) {
        auto y_ = if_(x() < 4_c, y, always("..."))();
        return "(" + to_string(x()) + " + " + to_string(y_) + ")";
    };

    auto state = [] { assert(false); }; // this will never be evaluated
    assert(lazy_foldr(show, state, range(1_c, 999999_c)) == "(1 + (2 + (3 + (4 + ...))))");
    //! [main]
}

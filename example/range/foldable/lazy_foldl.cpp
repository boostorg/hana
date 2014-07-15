/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/range.hpp>

#include <cassert>
#include <sstream>
using namespace boost::hana;
using namespace literals;


int main() {
    //! [main]
    auto to_string = [](auto x) { return (std::ostringstream{} << x).str(); };

    auto down_to = [=](auto n) {
        return [=](auto x, auto y) {
            auto x_ = if_(y() == n, always("..."), x)();
            return "(" + to_string(x_) + " + " + to_string(y()) + ")";
        };
    };

    assert(
        lazy_foldl(down_to(4_c), 1_c, range(2_c, 8_c)) == "((((... + 4) + 5) + 6) + 7)"
    );
    //! [main]
}

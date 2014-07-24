/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <array>
#include <cassert>
#include <iostream>
using namespace boost::hana;


int main() {
    // Make sure the monadic chain is evaluated in the right order.
    std::array<bool, 3> executed = {{false, false, false}};
    int dummy = 0;

    std::cerr << "creating the monadic chain...\n";
    auto chain = lazy(dummy)
        | [&](int dummy) {
            std::cerr << "executing the first computation...\n";
            executed[0] = true;
            assert((executed == std::array<bool, 3>{{true, false, false}}));
            return lazy(dummy);
        }
        | [&](int dummy) {
            std::cerr << "executing the second computation...\n";
            executed[1] = true;
            assert((executed == std::array<bool, 3>{{true, true, false}}));
            return lazy(dummy);
        }
        | [&](int dummy) {
            std::cerr << "executing the third computation...\n";
            executed[2] = true;
            assert((executed == std::array<bool, 3>{{true, true, true}}));
            return lazy(dummy);
        };

    assert((executed == std::array<bool, 3>{{false, false, false}}));

    std::cerr << "evaluating the chain...\n";
    eval(chain);
}

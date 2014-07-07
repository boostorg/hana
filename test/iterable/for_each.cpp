/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable.hpp>

#include "minimal_iterable.hpp"
#include <cassert>
#include <vector>
using namespace boost::hana;


auto test = [](auto ...xs) {
    std::vector<int> seen{};
    for_each(iterable(xs...), [&](int x) {
        seen.push_back(x);
    });
    assert(seen == std::vector<int>{xs...});
};

int main() {
    test();
    test(0);
    test(0, 1);
    test(0, 1, 2);
    test(0, 1, 2, 3);
    test(0, 1, 2, 3, 4);
}

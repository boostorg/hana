/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/iterable.hpp>

#include <vector>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto iterable = detail::minimal::iterable<mcd>;

    auto check = [=](auto ...xs) {
        std::vector<int> seen{};
        for_each(iterable(xs...), [&](int x) {
            seen.push_back(x);
        });
        BOOST_HANA_RUNTIME_ASSERT(seen == std::vector<int>{xs...});
    };
    check();
    check(0);
    check(0, 1);
    check(0, 1, 2);
    check(0, 1, 2, 3);
    check(0, 1, 2, 3, 4);
}

int main() {
    test<Iterable::mcd>();
}

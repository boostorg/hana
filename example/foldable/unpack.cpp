/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>
#include <boost/hana/tuple.hpp>

#include <cassert>
using namespace boost::hana;


int main() {
    //! [main]
    auto cheap_tie = [](auto& ...vars) {
        return partial(flip(unpack), [&vars...](auto ...values) {
            // Using an initializer list sequences the assignments.
            int dummy[] = {((vars = values), 0)...};
            (void)dummy;
        });
    };
    int a = 0;
    char b = '\0';
    double c = 0;

    cheap_tie(a, b, c)(tuple(1, '2', 3.3));
    assert(a == 1 && b == '2' && c == 3.3);
    //! [main]
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>
#include <boost/hana/list.hpp>

#include <cassert>
using namespace boost::hana;


int main() {
    //! [main]
    auto cheap_tie = [](auto& ...vars) {
        return partial(unpack, [&vars...](auto ...values) {
            int dummy[] = {((vars = values), 0)...};
            (void)dummy;
        });
    };
    int a = 0;
    char b = '\0';
    double c = 0;
    cheap_tie(a, b, c)(list(1, '2', 3.3));
    assert(a == 1);
    assert(b == '2');
    assert(c == 3.3);
    //! [main]
}

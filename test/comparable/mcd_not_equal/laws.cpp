/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list.hpp>

#include "minimal.hpp"
using namespace boost::hana;


int main() {
    auto xs = list(comparable<0>, comparable<1>, comparable<2>);
    auto check = curry<3>([](auto ...xs) {
        auto r = Comparable::laws::check(xs...);
        BOOST_HANA_STATIC_ASSERT(r);
        return r;
    });
    ap(ap(fmap(check, xs), xs), xs);
}

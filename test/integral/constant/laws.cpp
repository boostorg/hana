/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/constant/detail/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <typename T>
void test() {
    BOOST_HANA_CONSTANT_ASSERT(Constant::laws::check(
        list(
            integral<T, 0>,
            integral<T, 1>,
            integral<T, 2>,
            integral<T, 3>
        )
    ));
}

int main() {
    test<int>();
    test<long>();
}

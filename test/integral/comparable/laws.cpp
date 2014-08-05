/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <typename T, typename U>
void test() {
    BOOST_HANA_CONSTEXPR_ASSERT(
        Comparable::laws::check(
            list(
                integral<T, 0>, integral<U, 0>,
                integral<T, 1>, integral<U, 1>,
                T{0}, T{1}, U{0}, U{1}
            )
        )
    );
}

int main() {
    test<int, int>();
    test<int, long long>();
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/constant.hpp>

#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


struct UDT {
    struct Type { };
    Type member;
};

template <typename T, typename U>
void test() {
    BOOST_HANA_STATIC_ASSERT(
        Comparable::laws::check(
            list(
                constant<T, 0>,
                constant<T, 1>,

                constant<U, 1>,
                constant<U, 2>,

                constant<decltype(&UDT::member), &UDT::member>
            )
        )
    );
}

int main() {
    test<int, int>();
    test<int, unsigned long long>();
}

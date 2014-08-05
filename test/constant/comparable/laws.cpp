/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/constant/mcd.hpp>

#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/constant.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


struct UDT {
    struct Type { };
    Type member;
};

template <typename mcd, typename T, typename U>
void test() {
    BOOST_HANA_CONSTANT_ASSERT(
        Comparable::laws::check(
            list(
                detail::minimal::constant<T, 0, mcd>,
                detail::minimal::constant<T, 1, mcd>,

                detail::minimal::constant<U, 1, mcd>,
                detail::minimal::constant<U, 2, mcd>,

                detail::minimal::constant<decltype(&UDT::member), &UDT::member, mcd>
            )
        )
    );
}

int main() {
    test<Constant::mcd, int, int>();
    test<Constant::mcd, int, unsigned long long>();
}

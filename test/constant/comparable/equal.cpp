/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/constant/mcd.hpp>

#include <boost/hana/detail/minimal/constant.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


struct UDT {
    struct Type1 { };
    struct Type2 { };
    Type1 member1;
    Type2 member2;
};

template <typename mcd, typename T, typename U>
void test() {
    BOOST_HANA_STATIC_ASSERT( equal(detail::minimal::constant<T, 0, mcd>, detail::minimal::constant<U, 0, mcd>));
    BOOST_HANA_STATIC_ASSERT(!equal(detail::minimal::constant<T, 0, mcd>, detail::minimal::constant<U, 1, mcd>));

    // Incomparable constants must appear as unequal.
    BOOST_HANA_STATIC_ASSERT(!equal(
        detail::minimal::constant<T, 0, mcd>,
        detail::minimal::constant<decltype(&UDT::member1), &UDT::member1, mcd>
    ));
    BOOST_HANA_STATIC_ASSERT(!equal(
        detail::minimal::constant<decltype(&UDT::member1), &UDT::member1, mcd>,
        detail::minimal::constant<decltype(&UDT::member2), &UDT::member2, mcd>
    ));
}

int main() {
    test<Constant::mcd, int, int>();
    test<Constant::mcd, int, long>();
}

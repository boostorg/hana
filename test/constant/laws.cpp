/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/constant/mcd.hpp>

#include <boost/hana/constant/laws.hpp>
#include <boost/hana/detail/minimal/constant.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <typename mcd, typename T>
void test() {
    BOOST_HANA_STATIC_ASSERT(Constant::laws::check(
        list(
            detail::minimal::constant<T, 0, mcd>,
            detail::minimal::constant<T, 1, mcd>,
            detail::minimal::constant<T, 2, mcd>,
            detail::minimal::constant<T, 3, mcd>
        )
    ));
}

int main() {
    test<Constant::mcd, int>();
    test<Constant::mcd, long>();
}

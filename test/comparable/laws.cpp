/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/comparable/not_equal_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto comparable = detail::minimal::comparable<mcd>;
    BOOST_HANA_CONSTEXPR_ASSERT(
        Comparable::laws::check(
            list(comparable(0), comparable(1), comparable(2))
        )
    );
}

int main() {
    test<Comparable::equal_mcd>();
    test<Comparable::not_equal_mcd>();
}

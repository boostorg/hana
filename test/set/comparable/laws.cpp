/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set.hpp>

#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = int_<i>;

int main() {
    BOOST_HANA_STATIC_ASSERT(Comparable::laws::check(
        list(
            set(),
            set(x<0>),
            set(x<0>, x<1>),
            set(x<1>, x<0>),
            set(x<0>, x<1>, x<2>)
        )
    ));
}

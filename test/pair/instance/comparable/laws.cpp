/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/pair/instance.hpp>

#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    BOOST_HANA_STATIC_ASSERT(Comparable::laws::check(
        list(
            pair(x<0>, x<1>),
            pair(x<1>, x<0>),
            pair(x<0>, x<0>)
        )
    ));
}

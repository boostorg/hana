/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type_list.hpp>

#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <int i>
struct x;

template <int ...i>
constexpr auto tlist = type_list<x<i>...>;

int main() {
    BOOST_HANA_STATIC_ASSERT(Comparable::laws::check(
        list(
            tlist<>,
            tlist<0>,
            tlist<0, 1>,
            tlist<0, 1, 2>
        )
    ));
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/list.hpp>

#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/list/instance.hpp>

#include <boost/mpl/list.hpp>
using namespace boost::hana;


template <int i> struct x;

template <int ...i>
constexpr boost::mpl::list<x<i>...> mpl_list{};

int main() {
    BOOST_HANA_CONSTANT_ASSERT(
        Comparable::laws::check(
            list(
                mpl_list<>, mpl_list<0>, mpl_list<0, 1>,
                mpl_list<0, 1, 2>, mpl_list<1, 0, 2>
            )
        )
    );
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/vector.hpp>

#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list/instance.hpp>

#include <boost/mpl/vector.hpp>
using namespace boost::hana;


template <int i> struct x;

template <int ...i>
constexpr boost::mpl::vector<x<i>...> mpl_vector{};

int main() {
    BOOST_HANA_STATIC_ASSERT(
        Comparable::laws::check(
            list(
                mpl_vector<>, mpl_vector<0>, mpl_vector<0, 1>,
                mpl_vector<0, 1, 2>, mpl_vector<1, 0, 2>
            )
        )
    );
}

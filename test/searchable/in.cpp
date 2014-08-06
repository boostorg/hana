/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/searchable.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    constexpr auto searchable = detail::minimal::searchable<>;

    BOOST_HANA_CONSTEXPR_ASSERT(x<0> ^in^ searchable(x<0>));
    BOOST_HANA_CONSTEXPR_ASSERT(!(x<1> ^in^ searchable(x<0>)));
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/searchable/find_mcd.hpp>

#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/searchable.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
    return [=](auto y) { return equal(x, y); };
};

template <typename mcd>
void test() {
    constexpr auto searchable = detail::minimal::searchable<mcd>;

    BOOST_HANA_STATIC_ASSERT( any(is(x<0>), searchable(x<0>)));
    BOOST_HANA_STATIC_ASSERT(!any(is(x<1>), searchable(x<0>)));
};

int main() {
    test<Searchable::find_mcd>();
}

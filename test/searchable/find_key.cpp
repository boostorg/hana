/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/searchable/find_mcd.hpp>

#include <boost/hana/detail/minimal/searchable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = int_<i>;

template <typename mcd>
void test() {
    constexpr auto searchable = detail::minimal::searchable<mcd>;

    BOOST_HANA_STATIC_ASSERT(find_key(x<0>, searchable(x<0>)) == just(x<0>));
    BOOST_HANA_STATIC_ASSERT(find_key(x<1>, searchable(x<0>)) == nothing);
};

int main() {
    test<Searchable::find_mcd>();
}

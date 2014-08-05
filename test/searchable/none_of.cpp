/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/searchable/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/searchable.hpp>
using namespace boost::hana;


template <bool b>
constexpr auto x = b;

template <typename mcd>
void test() {
    constexpr auto searchable = detail::minimal::searchable<mcd>;

    BOOST_HANA_CONSTEXPR_ASSERT(!none_of(searchable(x<true>)));
    BOOST_HANA_CONSTEXPR_ASSERT( none_of(searchable(x<false>)));
};

int main() {
    test<Searchable::mcd>();
}

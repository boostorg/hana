/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/orderable/less_mcd.hpp>

#include <boost/hana/detail/minimal/orderable.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto orderable = detail::minimal::orderable<mcd>;

    BOOST_HANA_STATIC_ASSERT(min(orderable(0), orderable(0)) == orderable(0));
    BOOST_HANA_STATIC_ASSERT(min(orderable(1), orderable(0)) == orderable(0));
    BOOST_HANA_STATIC_ASSERT(min(orderable(0), orderable(1)) == orderable(0));
}

int main() {
    test<Orderable::less_mcd>();
}

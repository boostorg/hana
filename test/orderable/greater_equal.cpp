/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/orderable/less_mcd.hpp>

#include <boost/hana/detail/minimal/orderable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/logical.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto orderable = detail::minimal::orderable<mcd>;

    BOOST_HANA_STATIC_ASSERT(greater_equal(orderable(1), orderable(0)));
    BOOST_HANA_STATIC_ASSERT(greater_equal(orderable(0), orderable(0)));
    BOOST_HANA_STATIC_ASSERT(not_(greater_equal(orderable(0), orderable(1))));

    // operators
    using operators::operator>=;
    BOOST_HANA_STATIC_ASSERT(orderable(1) >= orderable(0));
    BOOST_HANA_STATIC_ASSERT(orderable(0) >= orderable(0));
    BOOST_HANA_STATIC_ASSERT(not_(orderable(0) >= orderable(1)));
}

int main() {
    test<Orderable::less_mcd>();
}

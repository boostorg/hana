/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_GROUP_HPP
#define BOOST_HANA_TEST_TEST_LAWS_GROUP_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/group/group.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monoid/monoid.hpp>


template <typename G, typename ...Group>
constexpr auto Group_laws(Group ...x) {
    using namespace boost::hana;
    return and_(
        and_(
            equal(plus(x, negate(x)), zero<G>),
            equal(plus(negate(x), x), zero<G>)
        )...
    );
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_GROUP_HPP

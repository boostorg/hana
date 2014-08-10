/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/tuple.hpp>

#include <boost/hana/detail/assert.hpp>

#include <boost/tuple/tuple.hpp>
using namespace boost::hana;


int main() {
    // head
    {
        BOOST_HANA_CONSTEXPR_ASSERT(head(boost::make_tuple(1)) == 1);
        BOOST_HANA_CONSTEXPR_ASSERT(head(boost::make_tuple(1, '2')) == 1);
        BOOST_HANA_CONSTEXPR_ASSERT(head(boost::make_tuple(1, '2', 3.3)) == 1);
    }

    // is_empty
    {
        BOOST_HANA_CONSTANT_ASSERT(is_empty(boost::make_tuple()));
        BOOST_HANA_CONSTANT_ASSERT(is_empty(boost::tuples::null_type{}));

        BOOST_HANA_CONSTANT_ASSERT(!is_empty(boost::make_tuple(1)));
        BOOST_HANA_CONSTANT_ASSERT(!is_empty(boost::make_tuple(1, '2')));
        BOOST_HANA_CONSTANT_ASSERT(!is_empty(boost::make_tuple(1, '2', 3.3)));
    }

    // tail
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(tail(boost::make_tuple(1)), boost::make_tuple()));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(tail(boost::make_tuple(1, '2')), boost::make_tuple('2')));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(tail(boost::make_tuple(1, '2', 3.3)), boost::make_tuple('2', 3.3)));
    }
}

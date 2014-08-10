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
    // cons
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(1, boost::tuples::null_type{}), boost::make_tuple(1)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(1, cons('2', boost::tuples::null_type{})), boost::make_tuple(1, '2')));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(1, cons('2', boost::make_tuple(3.3))), boost::make_tuple(1, '2', 3.3)));
    }

    // nil
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(nil<BoostTuple>, boost::make_tuple()));
        BOOST_HANA_CONSTANT_ASSERT(equal(nil<BoostTuple>, boost::tuples::null_type{}));
    }
}

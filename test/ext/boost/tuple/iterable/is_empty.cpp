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
    BOOST_HANA_CONSTANT_ASSERT(is_empty(boost::make_tuple()));
    BOOST_HANA_CONSTANT_ASSERT(is_empty(boost::tuples::null_type{}));

    BOOST_HANA_CONSTANT_ASSERT(!is_empty(boost::make_tuple(1)));
    BOOST_HANA_CONSTANT_ASSERT(!is_empty(boost::make_tuple(1, '2')));
    BOOST_HANA_CONSTANT_ASSERT(!is_empty(boost::make_tuple(1, '2', 3.3)));
}

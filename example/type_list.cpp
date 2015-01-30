/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>
using namespace boost::hana;


int main() {

{

//! [type_list]
BOOST_HANA_CONSTANT_CHECK(
    to<Tuple>(type_list<void, int, float>)
    ==
    tuple(type<void>, type<int>, type<float>)
);

BOOST_HANA_CONSTANT_CHECK(head(type_list<void, int, float>) == type<void>);
//! [type_list]

}

}

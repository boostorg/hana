/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {

{

//! [comparing]
constexpr auto grouped = group_by(comparing(length), tuple(
    tuple(1, 2, 3),
    tuple('x', 'y', 'z'),
    range_c<long, 0, 1>,
    tuple_t<char, int>,
    range_c<int, 0, 2>,
    tuple(123.4, nullptr)
));

BOOST_HANA_CONSTEXPR_CHECK(grouped == tuple(
    tuple(
        tuple(1, 2, 3),
        tuple('x', 'y', 'z')
    ),
    tuple(
        range_c<long, 0, 1>
    ),
    tuple(
        tuple_t<char, int>,
        range_c<int, 0, 2>,
        tuple(123.4, nullptr)
    )
));
//! [comparing]

}{

//! [equal]
BOOST_HANA_CONSTEXPR_CHECK(equal(tuple(1, 2), tuple(1, 2)));
BOOST_HANA_CONSTEXPR_CHECK(!equal('x', 'y'));
BOOST_HANA_CONSTANT_CHECK(!equal(tuple(1, 2), 'y'));
//! [equal]

}{

//! [not_equal]
BOOST_HANA_CONSTEXPR_CHECK(not_equal(tuple(1, 2), tuple(3)));
BOOST_HANA_CONSTEXPR_CHECK(not_equal('x', 'y'));
BOOST_HANA_CONSTANT_CHECK(not_equal(tuple(1, 2), 'y'));
//! [not_equal]

}

}

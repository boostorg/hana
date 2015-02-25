/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {

{

//! [comparing]
BOOST_HANA_CONSTEXPR_LAMBDA auto grouped = group_by(comparing(length), make<Tuple>(
    make<Tuple>(1, 2, 3),
    make<Tuple>('x', 'y', 'z'),
    range_c<long, 0, 1>,
    tuple_t<char, int>,
    range_c<int, 0, 2>,
    make<Tuple>(123.4, nullptr)
));

BOOST_HANA_CONSTEXPR_CHECK(grouped == make<Tuple>(
    make<Tuple>(
        make<Tuple>(1, 2, 3),
        make<Tuple>('x', 'y', 'z')
    ),
    make<Tuple>(
        range_c<long, 0, 1>
    ),
    make<Tuple>(
        tuple_t<char, int>,
        range_c<int, 0, 2>,
        make<Tuple>(123.4, nullptr)
    )
));
//! [comparing]

}{

//! [equal]
BOOST_HANA_CONSTEXPR_CHECK(equal(make<Tuple>(1, 2), make<Tuple>(1, 2)));
BOOST_HANA_CONSTEXPR_CHECK(!equal('x', 'y'));
BOOST_HANA_CONSTANT_CHECK(!equal(make<Tuple>(1, 2), 'y'));

BOOST_HANA_CONSTEXPR_CHECK(any(make<Tuple>(1, 2, 3), equal.to(2)));
//! [equal]

}{

//! [not_equal]
BOOST_HANA_CONSTEXPR_CHECK(not_equal(make<Tuple>(1, 2), make<Tuple>(3)));
BOOST_HANA_CONSTEXPR_CHECK(not_equal('x', 'y'));
BOOST_HANA_CONSTANT_CHECK(not_equal(make<Tuple>(1, 2), 'y'));

BOOST_HANA_CONSTEXPR_CHECK(all(make<Tuple>(1, 2, 3), not_equal.to(5)));
//! [not_equal]

}

}

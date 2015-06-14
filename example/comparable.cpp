/*
@copyright Louis Dionne 2015
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
constexpr auto grouped = group.by(comparing(length), make_tuple(
    make_tuple(1, 2, 3),
    make_tuple('x', 'y', 'z'),
    range_c<long, 0, 1>,
    tuple_t<char, int>,
    range_c<int, 0, 2>,
    make_tuple(123.4, nullptr)
));

static_assert(grouped == make_tuple(
    make_tuple(
        make_tuple(1, 2, 3),
        make_tuple('x', 'y', 'z')
    ),
    make_tuple(
        range_c<long, 0, 1>
    ),
    make_tuple(
        tuple_t<char, int>,
        range_c<int, 0, 2>,
        make_tuple(123.4, nullptr)
    )
), "");
//! [comparing]

}{

//! [equal]
static_assert(equal(make<Tuple>(1, 2), make<Tuple>(1, 2)), "");
static_assert(!equal('x', 'y'), "");
BOOST_HANA_CONSTANT_CHECK(!equal(make<Tuple>(1, 2), 'y'));

static_assert(any_of(make<Tuple>(1, 2, 3), equal.to(2)), "");
//! [equal]

}{

//! [not_equal]
static_assert(not_equal(make<Tuple>(1, 2), make<Tuple>(3)), "");
static_assert(not_equal('x', 'y'), "");
BOOST_HANA_CONSTANT_CHECK(not_equal(make<Tuple>(1, 2), 'y'));

static_assert(all_of(make<Tuple>(1, 2, 3), not_equal.to(5)), "");
//! [not_equal]

}

}

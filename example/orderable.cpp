/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {

{

//! [less.than]
BOOST_HANA_CONSTEXPR_CHECK(all_of(tuple_c<int, 1, 2, 3, 4>, less.than(5)));

BOOST_HANA_CONSTANT_CHECK(all_of(tuple_c<int, 1, 2, 3, 4>, less_equal.than(int_<4>)));
//! [less.than]

}{

//! [greater]
BOOST_HANA_CONSTEXPR_CHECK(greater(4, 1));
BOOST_HANA_CONSTANT_CHECK(!greater(int_<1>, int_<3>));
//! [greater]

}{

//! [greater_equal]
BOOST_HANA_CONSTEXPR_CHECK(greater_equal(4, 1));
BOOST_HANA_CONSTEXPR_CHECK(greater_equal(1, 1));
BOOST_HANA_CONSTANT_CHECK(!greater_equal(int_<1>, int_<2>));
//! [greater_equal]

}{

//! [less]
BOOST_HANA_CONSTEXPR_CHECK(less(1, 4));
BOOST_HANA_CONSTANT_CHECK(!less(int_<3>, int_<2>));
//! [less]

}{

//! [less_equal]
BOOST_HANA_CONSTEXPR_CHECK(less_equal(1, 4));
BOOST_HANA_CONSTEXPR_CHECK(less_equal(1, 1));
BOOST_HANA_CONSTANT_CHECK(!less_equal(int_<3>, int_<2>));
//! [less_equal]

}{

//! [max]
BOOST_HANA_CONSTEXPR_CHECK(max(1, 4) == 4);
BOOST_HANA_CONSTANT_CHECK(max(int_<7>, int_<5>) == int_<7>);
//! [max]

}{

//! [min]
BOOST_HANA_CONSTEXPR_CHECK(min(1, 4) == 1);
BOOST_HANA_CONSTANT_CHECK(min(int_<7>, int_<5>) == int_<5>);
//! [min]

}{

//! [ordering]
BOOST_HANA_CONSTEXPR_LAMBDA auto sorted = sort.by(ordering(sizeof_), tuple_t<
    char[3], char[1], char[2], char[15]
>);
BOOST_HANA_CONSTANT_CHECK(sorted == tuple_t<
    char[1], char[2], char[3], char[15]
>);
//! [ordering]

}

}

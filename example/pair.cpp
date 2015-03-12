/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/pair.hpp>
using namespace boost::hana;


int main() {

{

//! [make<Pair>]
BOOST_HANA_CONSTEXPR_CHECK(first(make<Pair>(1, 'x')) == 1);
BOOST_HANA_CONSTEXPR_CHECK(second(make<Pair>(1, 'x')) == 'x');
//! [make<Pair>]

}{

//! [make_pair]
BOOST_HANA_CONSTEXPR_CHECK(make_pair(1, 'x') == make<Pair>(1, 'x'));
//! [make_pair]

}{

//! [comparable]
BOOST_HANA_CONSTEXPR_CHECK(make<Pair>(1, 'x') == make<Pair>(1, 'x'));
BOOST_HANA_CONSTEXPR_CHECK(make<Pair>(2, 'x') != make<Pair>(1, 'x'));
BOOST_HANA_CONSTEXPR_CHECK(make<Pair>(1, 'y') != make<Pair>(1, 'x'));
//! [comparable]

}{

//! [orderable]
BOOST_HANA_CONSTEXPR_CHECK(make<Pair>(1, 'x') < make<Pair>(1, 'y'));
BOOST_HANA_CONSTEXPR_CHECK(make<Pair>(1, 'x') < make<Pair>(10, 'x'));
BOOST_HANA_CONSTEXPR_CHECK(make<Pair>(1, 'y') < make<Pair>(10, 'x'));
//! [orderable]

}{

//! [foldable]
BOOST_HANA_CONSTEXPR_CHECK(foldl(make<Pair>(1, 3), 0, plus) == 4);
BOOST_HANA_CONSTEXPR_CHECK(foldr(make<Pair>(1, 3), 0, minus) == -2);
//! [foldable]

}{

//! [product]
BOOST_HANA_CONSTEXPR_CHECK(first(make<Pair>(1, 'x')) == 1);
BOOST_HANA_CONSTEXPR_CHECK(second(make<Pair>(1, 'x')) == 'x');
//! [product]

}

}

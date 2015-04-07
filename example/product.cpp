/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/pair.hpp>

#include <string>
using namespace boost::hana;
using namespace std::literals;


int main() {

{

//! [comparable]
BOOST_HANA_RUNTIME_CHECK(make<Pair>(1, "234"s) == make<Pair>(1ll, "234"s));
BOOST_HANA_CONSTEXPR_CHECK(make<Pair>('x', 2) != make<Pair>('y', 2));
//! [comparable]

}{

//! [first]
BOOST_HANA_CONSTEXPR_CHECK(first(make<Pair>(1, 'x')) == 1);
//! [first]

}{

//! [second]
BOOST_HANA_CONSTEXPR_CHECK(second(make<Pair>(1, 'x')) == 'x');
//! [second]

}{

//! [make]
BOOST_HANA_CONSTEXPR_CHECK(first(make<Pair>(1, 'x')) == 1);
BOOST_HANA_CONSTEXPR_CHECK(second(make<Pair>(1, 'x')) == 'x');
//! [make]

}

}

/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <sstream>
#include <type_traits>
using namespace boost::hana;


int main() {

//////////////////////////////////////////////////////////////////////////////
// Models
//////////////////////////////////////////////////////////////////////////////
{

//! [Searchable]
BOOST_HANA_CONSTEXPR_CHECK(
    find_if(make<Tuple>(1.0, 2, '3'), trait<std::is_integral>) == just(2)
);
BOOST_HANA_CONSTANT_CHECK(
    find_if(make<Tuple>(1.0, 2, '3'), trait<std::is_class>) == nothing
);

BOOST_HANA_CONSTANT_CHECK(
    find(make<Tuple>(int_<1>, char_<'c'>, type<void>), type<void>) == just(type<void>)
);
BOOST_HANA_CONSTANT_CHECK(
    find(make<Tuple>(int_<1>, char_<'c'>, type<void>), type<int>) == nothing
);
//! [Searchable]

}

//////////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////////
{

//! [at]
BOOST_HANA_CONSTEXPR_CHECK(at(int_<0>, make<Tuple>(0, '1', 2.0)) == 0);
BOOST_HANA_CONSTEXPR_CHECK(at(int_<1>, make<Tuple>(0, '1', 2.0)) == '1');
BOOST_HANA_CONSTEXPR_CHECK(at(int_<2>, make<Tuple>(0, '1', 2.0)) == 2.0);
//! [at]

}{

//! [at_c]
BOOST_HANA_CONSTEXPR_CHECK(at_c<0>(make<Tuple>(0, '1', 2.0)) == 0);
BOOST_HANA_CONSTEXPR_CHECK(at_c<1>(make<Tuple>(0, '1', 2.0)) == '1');
BOOST_HANA_CONSTEXPR_CHECK(at_c<2>(make<Tuple>(0, '1', 2.0)) == 2.0);
//! [at_c]

}{

//! [drop]
constexpr auto xs = make<Tuple>(0, '1', 2.0);

// drop (equivalent to drop.at_most)
BOOST_HANA_CONSTEXPR_CHECK(drop(int_<0>, xs) == xs);
BOOST_HANA_CONSTEXPR_CHECK(drop(int_<1>, xs) == make<Tuple>('1', 2.0));
BOOST_HANA_CONSTEXPR_CHECK(drop(int_<2>, xs) == make<Tuple>(2.0));
BOOST_HANA_CONSTANT_CHECK(drop(int_<3>, xs) == make<Tuple>());
BOOST_HANA_CONSTANT_CHECK(drop(int_<4>, xs) == make<Tuple>());
BOOST_HANA_CONSTANT_CHECK(drop.at_most(int_<4>, xs) == make<Tuple>());

// drop.exactly
static_assert(drop.exactly(int_<2>, xs) == make<Tuple>(2.0), "");
BOOST_HANA_CONSTANT_CHECK(drop.exactly(int_<3>, xs) == make<Tuple>());
//! [drop]

}{

//! [drop_c]
BOOST_HANA_CONSTEXPR_CHECK(drop_c<2>(make<Tuple>(0, '1', 2.0)) == make<Tuple>(2.0));
//! [drop_c]

}{

//! [drop_until]
using namespace literals;
BOOST_HANA_CONSTEXPR_LAMBDA auto positive = [](auto x) {
    return x > int_<0>;
};

BOOST_HANA_CONSTANT_CHECK(
    drop_until(range(int_<-3>, int_<6>), positive) == range(int_<1>, int_<6>)
);

BOOST_HANA_CONSTANT_CHECK(
    drop_until(make<Tuple>(1_c, -2_c, 4_c, 5_c), positive) == make<Tuple>(1_c, -2_c, 4_c, 5_c)
);
//! [drop_until]

}{

//! [drop_while]
using namespace literals;
BOOST_HANA_CONSTEXPR_LAMBDA auto negative = [](auto x) {
    return x < int_<0>;
};

BOOST_HANA_CONSTANT_CHECK(
    drop_while(range(int_<-3>, int_<6>), negative) == range(int_<0>, int_<6>)
);

BOOST_HANA_CONSTANT_CHECK(
    drop_while(make<Tuple>(1_c, -2_c, 4_c, 5_c), negative) == make<Tuple>(1_c, -2_c, 4_c, 5_c)
);
//! [drop_while]

}{

//! [head]
BOOST_HANA_CONSTEXPR_CHECK(head(make<Tuple>(1, '2', 3.3, nullptr)) == 1);
//! [head]

}{

//! [is_empty]
BOOST_HANA_CONSTANT_CHECK(!is_empty(make<Tuple>(1, '2')));
BOOST_HANA_CONSTANT_CHECK( is_empty(make<Tuple>()));
//! [is_empty]

}{

//! [last]
BOOST_HANA_CONSTEXPR_CHECK(last(make<Tuple>(1, '2', 3.3, nullptr)) == nullptr);
//! [last]

}{

//! [tail]
BOOST_HANA_CONSTEXPR_CHECK(
    tail(make<Tuple>(1, '2', 3.3, nullptr)) == make<Tuple>('2', 3.3, nullptr)
);
//! [tail]

}

}

/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
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
    find_if(make_tuple(1.0, 2, '3'), trait<std::is_integral>) == just(2)
);
BOOST_HANA_CONSTANT_CHECK(
    find_if(make_tuple(1.0, 2, '3'), trait<std::is_class>) == nothing
);

BOOST_HANA_CONSTANT_CHECK(
    find(make_tuple(int_<1>, char_<'c'>, type<void>), type<void>) == just(type<void>)
);
BOOST_HANA_CONSTANT_CHECK(
    find(make_tuple(int_<1>, char_<'c'>, type<void>), type<int>) == nothing
);
//! [Searchable]

}

//////////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////////
{

//! [at]
BOOST_HANA_CONSTEXPR_CHECK(at(make_tuple(0, '1', 2.0), int_<0>) == 0);
BOOST_HANA_CONSTEXPR_CHECK(at(make_tuple(0, '1', 2.0), int_<1>) == '1');
BOOST_HANA_CONSTEXPR_CHECK(at(make_tuple(0, '1', 2.0), int_<2>) == 2.0);
//! [at]

}{

//! [at_c]
BOOST_HANA_CONSTEXPR_CHECK(at_c<0>(make_tuple(0, '1', 2.0)) == 0);
BOOST_HANA_CONSTEXPR_CHECK(at_c<1>(make_tuple(0, '1', 2.0)) == '1');
BOOST_HANA_CONSTEXPR_CHECK(at_c<2>(make_tuple(0, '1', 2.0)) == 2.0);
//! [at_c]

}{

//! [back]
static_assert(back(make_tuple(1, '2', 3.3)) == 3.3, "");
static_assert(back(make_tuple(1, '2', 3.3, nullptr)) == nullptr, "");
//! [back]

}{

//! [drop_front]
constexpr auto xs = make_tuple(0, '1', 2.0);

static_assert(drop_front(xs, size_t<0>) == xs, "");
static_assert(drop_front(xs, size_t<1>) == make_tuple('1', 2.0), "");
static_assert(drop_front(xs, size_t<2>) == make_tuple(2.0), "");
BOOST_HANA_CONSTANT_CHECK(drop_front(xs, size_t<3>) == make_tuple());
BOOST_HANA_CONSTANT_CHECK(drop_front(xs, size_t<4>) == make_tuple());

static_assert(drop_front(xs) == make_tuple('1', 2.0), "");
//! [drop_front]

}{

//! [drop_front_exactly]
constexpr auto xs = make_tuple(0, '1', 2.0);

static_assert(drop_front_exactly(xs, size_t<1>) == make_tuple('1', 2.0), "");
static_assert(drop_front_exactly(xs, size_t<2>) == make_tuple(2.0), "");
BOOST_HANA_CONSTANT_CHECK(drop_front_exactly(xs, size_t<3>) == make_tuple());

static_assert(drop_front_exactly(xs) == make_tuple('1', 2.0), "");
//! [drop_front_exactly]

}{

//! [drop_until]
using namespace boost::hana::literals;

BOOST_HANA_CONSTEXPR_LAMBDA auto positive = [](auto x) {
    return x > int_<0>;
};

BOOST_HANA_CONSTANT_CHECK(
    drop_until(range(int_<-3>, int_<6>), positive) == range(int_<1>, int_<6>)
);

BOOST_HANA_CONSTANT_CHECK(
    drop_until(make_tuple(1_c, -2_c, 4_c, 5_c), positive) == make_tuple(1_c, -2_c, 4_c, 5_c)
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
    drop_while(make_tuple(1_c, -2_c, 4_c, 5_c), negative) == make_tuple(1_c, -2_c, 4_c, 5_c)
);
//! [drop_while]

}{

//! [front]
static_assert(front(make_tuple(1, '2', 3.3, nullptr)) == 1, "");
//! [front]

}{

//! [is_empty]
BOOST_HANA_CONSTANT_CHECK(!is_empty(make_tuple(1, '2')));
BOOST_HANA_CONSTANT_CHECK( is_empty(make_tuple()));
//! [is_empty]

}{

//! [tail]
BOOST_HANA_CONSTEXPR_CHECK(
    tail(make_tuple(1, '2', 3.3, nullptr)) == make_tuple('2', 3.3, nullptr)
);
//! [tail]

}

}

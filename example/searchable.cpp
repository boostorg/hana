/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {

{

//! [all]
using namespace literals;

BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % 2_c != 0_c;
};

BOOST_HANA_CONSTEXPR_CHECK(all(make<Tuple>(1, 3), odd));
BOOST_HANA_CONSTANT_CHECK(!all(make<Tuple>(3_c, 4_c), odd));

BOOST_HANA_CONSTANT_CHECK(
    !all(make<Tuple>(type<void>, type<char&>), trait<std::is_void>)
);
BOOST_HANA_CONSTANT_CHECK(
    all(make<Tuple>(type<int>, type<char>), trait<std::is_integral>)
);
//! [all]

}{

//! [all_of]
BOOST_HANA_CONSTEXPR_CHECK(all_of(make<Tuple>(true_, true, true_)));
BOOST_HANA_CONSTANT_CHECK(!all_of(make<Tuple>(true, false_, true_)));
//! [all_of]

}{

//! [any]
using namespace literals;

BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % 2_c != 0_c;
};

BOOST_HANA_CONSTEXPR_CHECK(any(make<Tuple>(1, 2), odd));
BOOST_HANA_CONSTANT_CHECK(!any(make<Tuple>(2_c, 4_c), odd));

BOOST_HANA_CONSTANT_CHECK(
    any(make<Tuple>(type<void>, type<char&>), trait<std::is_void>)
);
BOOST_HANA_CONSTANT_CHECK(
    !any(make<Tuple>(type<void>, type<char&>), trait<std::is_integral>)
);
//! [any]

}{

//! [any_of]
BOOST_HANA_CONSTANT_CHECK(any_of(make<Tuple>(false, false_, true_)));
BOOST_HANA_CONSTEXPR_CHECK(any_of(make<Tuple>(false, false_, true)));
BOOST_HANA_CONSTEXPR_CHECK(!any_of(make<Tuple>(false, false_, false_)));
//! [any_of]

}{

//! [elem]
BOOST_HANA_CONSTANT_CHECK(elem(make<Tuple>(2, int_<2>, int_<3>, 'x'), int_<3>));
BOOST_HANA_CONSTANT_CHECK(elem(set(1, '2', type<int>, "foobar"), type<int>));
//! [elem]

}{

//! [find]
BOOST_HANA_CONSTEXPR_CHECK(find(make<Tuple>(1.0, 2, '3'), trait_<std::is_integral>) == just(2));
BOOST_HANA_CONSTANT_CHECK(find(make<Tuple>(1.0, 2, '3'), trait_<std::is_class>) == nothing);

constexpr auto types = tuple_t<char, int, unsigned, long, unsigned long>;
BOOST_HANA_CONSTANT_CHECK(find(types, _ == type<unsigned>) == just(type<unsigned>));
BOOST_HANA_CONSTANT_CHECK(find(types, _ == type<void>) == nothing);
//! [find]

}{

//! [in]
BOOST_HANA_CONSTEXPR_LAMBDA auto xs = make<Tuple>(
    int_<1>, type<int>, int_<2>, type<float>, int_<3>, type<void>, type<char>
);
BOOST_HANA_CONSTANT_CHECK(
    filter(xs, in ^ make<Tuple>(int_<3>, type<int>, type<void>))
    ==
    make<Tuple>(type<int>, int_<3>, type<void>)
);
//! [in]

}{

//! [lookup]
BOOST_HANA_CONSTANT_CHECK(lookup(make<Tuple>(int_<1>, type<int>, '3'), type<int>) == just(type<int>));
BOOST_HANA_CONSTANT_CHECK(lookup(make<Tuple>(int_<1>, type<int>, '3'), type<void>) == nothing);

BOOST_HANA_CONSTEXPR_LAMBDA auto m = map(
    pair(1, 'x'),
    pair(type<float>, 3.3),
    pair(type<char>, type<int>)
);
BOOST_HANA_CONSTEXPR_CHECK(lookup(m, type<float>) == just(3.3));
//! [lookup]

}{

//! [none]
using namespace literals;

BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % 2_c != 0_c;
};

BOOST_HANA_CONSTANT_CHECK(none(make<Tuple>(2_c, 4_c), odd));
BOOST_HANA_CONSTEXPR_CHECK(!none(make<Tuple>(1, 2), odd));

BOOST_HANA_CONSTANT_CHECK(
    !none(make<Tuple>(type<void>, type<char&>), trait<std::is_void>)
);
BOOST_HANA_CONSTANT_CHECK(
    none(make<Tuple>(type<void>, type<char&>), trait<std::is_integral>)
);
//! [none]

}{

//! [none_of]
BOOST_HANA_CONSTEXPR_CHECK(none_of(make<Tuple>(false, false_, false_)));
BOOST_HANA_CONSTEXPR_CHECK(!none_of(make<Tuple>(false, false_, true)));
BOOST_HANA_CONSTANT_CHECK(!none_of(make<Tuple>(false, false_, true_)));
//! [none_of]

}{

//! [subset]
BOOST_HANA_CONSTEXPR_CHECK(subset(make<Tuple>(1, '2', 3.3), make<Tuple>(3.3, 1, '2', nullptr)));
//! [subset]

}

}

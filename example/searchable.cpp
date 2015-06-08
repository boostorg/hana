/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <string>
#include <type_traits>
using namespace boost::hana;


int main() {

{

//! [all_of]
using namespace literals;

BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % 2_c != 0_c;
};

BOOST_HANA_CONSTEXPR_CHECK(all_of(make<Tuple>(1, 3), odd));
BOOST_HANA_CONSTANT_CHECK(!all_of(make<Tuple>(3_c, 4_c), odd));

BOOST_HANA_CONSTANT_CHECK(
    !all_of(make<Tuple>(type<void>, type<char&>), trait<std::is_void>)
);
BOOST_HANA_CONSTANT_CHECK(
    all_of(make<Tuple>(type<int>, type<char>), trait<std::is_integral>)
);
//! [all_of]

}{

//! [all]
BOOST_HANA_CONSTEXPR_CHECK(all(make<Tuple>(true_, true, true_)));
BOOST_HANA_CONSTANT_CHECK(!all(make<Tuple>(true, false_, true_)));
//! [all]

}{

//! [any_of]
using namespace literals;

BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % 2_c != 0_c;
};

BOOST_HANA_CONSTEXPR_CHECK(any_of(make<Tuple>(1, 2), odd));
BOOST_HANA_CONSTANT_CHECK(!any_of(make<Tuple>(2_c, 4_c), odd));

BOOST_HANA_CONSTANT_CHECK(
    any_of(make<Tuple>(type<void>, type<char&>), trait<std::is_void>)
);
BOOST_HANA_CONSTANT_CHECK(
    !any_of(make<Tuple>(type<void>, type<char&>), trait<std::is_integral>)
);
//! [any_of]

}{

//! [any]
BOOST_HANA_CONSTANT_CHECK(any(make_tuple(false, false_, true_)));
BOOST_HANA_CONSTEXPR_CHECK(any(make_tuple(false, false_, true)));
BOOST_HANA_CONSTEXPR_CHECK(!any(make_tuple(false, false_, false_)));
//! [any]

}{

//! [contains]
BOOST_HANA_CONSTANT_CHECK(contains(make_tuple(2, int_<2>, int_<3>, 'x'), int_<3>));
BOOST_HANA_CONSTANT_CHECK(contains(make_set(1, '2', type<int>, "foobar"), type<int>));
//! [contains]

}{

//! [contains.infix]
BOOST_HANA_CONSTANT_CHECK(
    make_tuple(2, int_<2>, int_<3>, 'x') ^contains^ int_<2>
);
//! [contains.infix]

}{

//! [in]
BOOST_HANA_CONSTANT_CHECK(int_<2> ^in^ make_tuple(2, int_<2>, int_<3>, 'x'));
//! [in]

}{

//! [find_if]
BOOST_HANA_CONSTEXPR_CHECK(
    find_if(make_tuple(1.0, 2, '3'), trait<std::is_integral>) == just(2)
);
BOOST_HANA_CONSTANT_CHECK(
    find_if(make_tuple(1.0, 2, '3'), trait<std::is_class>) == nothing
);

constexpr auto types = tuple_t<char, int, unsigned, long, unsigned long>;
BOOST_HANA_CONSTANT_CHECK(
    find_if(types, _ == type<unsigned>) == just(type<unsigned>)
);
BOOST_HANA_CONSTANT_CHECK(
    find_if(types, _ == type<void>) == nothing
);
//! [find_if]

}{

//! [find]
BOOST_HANA_CONSTANT_CHECK(
    find(make<Tuple>(int_<1>, type<int>, '3'), type<int>) == just(type<int>)
);
BOOST_HANA_CONSTANT_CHECK(
    find(make<Tuple>(int_<1>, type<int>, '3'), type<void>) == nothing
);

BOOST_HANA_CONSTEXPR_LAMBDA auto m = make<Map>(
    make<Pair>(1, 'x'),
    make<Pair>(type<float>, 3.3),
    make<Pair>(type<char>, type<int>)
);
BOOST_HANA_CONSTEXPR_CHECK(find(m, type<float>) == just(3.3));
//! [find]

}{

//! [at_key]
auto m = make_map(
    make_pair(type<int>, std::string{"int"}),
    make_pair(int_<3>, std::string{"3"})
);

BOOST_HANA_RUNTIME_CHECK(at_key(m, type<int>) == "int");

// operator-form
BOOST_HANA_RUNTIME_CHECK(m[type<int>] == "int");
BOOST_HANA_RUNTIME_CHECK(m[int_<3>] == "3");
//! [at_key]

}{

//! [none_of]
using namespace literals;

BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % 2_c != 0_c;
};

BOOST_HANA_CONSTANT_CHECK(none_of(make<Tuple>(2_c, 4_c), odd));
BOOST_HANA_CONSTEXPR_CHECK(!none_of(make<Tuple>(1, 2), odd));

BOOST_HANA_CONSTANT_CHECK(
    !none_of(make<Tuple>(type<void>, type<char&>), trait<std::is_void>)
);
BOOST_HANA_CONSTANT_CHECK(
    none_of(make<Tuple>(type<void>, type<char&>), trait<std::is_integral>)
);
//! [none_of]

}{

//! [none]
BOOST_HANA_CONSTEXPR_CHECK(none(make<Tuple>(false, false_, false_)));
BOOST_HANA_CONSTEXPR_CHECK(!none(make<Tuple>(false, false_, true)));
BOOST_HANA_CONSTANT_CHECK(!none(make<Tuple>(false, false_, true_)));
//! [none]

}{

//! [is_subset]
BOOST_HANA_CONSTEXPR_CHECK(
    is_subset(make_tuple(1, '2', 3.3), make_tuple(3.3, 1, '2', nullptr))
);
//! [is_subset]

}{

//! [is_subset.infix]
BOOST_HANA_CONSTEXPR_CHECK(
    make_tuple(1, '2', 3.3) ^is_subset^ make_tuple(3.3, 1, '2', nullptr)
);
//! [is_subset.infix]

}

}

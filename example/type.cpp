/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


namespace ns1 {
//! [comparable]
struct T;
struct U;
BOOST_HANA_CONSTANT_CHECK(type<T> == type<T>);
BOOST_HANA_CONSTANT_CHECK(type<T> != type<U>);
//! [comparable]
}

namespace ns2 {
//! [largest]
template <typename ...Types>
constexpr auto largest(Types ...types) {
    return maximum.by(ordering(sizeof_), make<Tuple>(types...));
}

template <int size>
struct storage { char s[size]; };

static_assert(
    largest(type<storage<1>>, type<storage<2>>, type<storage<3>>) == type<storage<3>>
, "");
//! [largest]
}

namespace ns22 {
//! [largest2]
// Note: tuple_t<T...> is equivalent to make<Tuple>(type<T>...)
template <typename ...T>
struct largest
    : decltype(maximum.by(ordering(sizeof_), tuple_t<T...>))
{ };

template <int size>
struct storage { char s[size]; };

static_assert(std::is_same<
    largest<storage<1>, storage<2>, storage<3>>::type,
    storage<3>
>{}, "");
//! [largest2]
}

namespace ns3 {
//! [decltype_]
struct X { };
BOOST_HANA_CONSTANT_CHECK(decltype_(X{}) == type<X>);
BOOST_HANA_CONSTANT_CHECK(decltype_(type<X>) == type<X>);

BOOST_HANA_CONSTANT_CHECK(decltype_(1) == type<int>);

static int const& i = 1;
BOOST_HANA_CONSTANT_CHECK(decltype_(i) == type<int const>);
//! [decltype_]
}

namespace ns4 {
//! [sizeof_]
struct X { };
static_assert(sizeof_(type<X>) == sizeof(X), "");

static_assert(sizeof_(1) == sizeof(1), "");
static_assert(sizeof_(type<int>) == sizeof(int), "");
//! [sizeof_]
}

namespace ns44 {
//! [alignof_]
struct X { };
static_assert(alignof_(type<X>) == alignof(X), "");

static_assert(alignof_(1) == alignof(decltype(1)), "");
static_assert(alignof_(type<int>) == alignof(int), "");
//! [alignof_]
}

namespace ns5 {
//! [template]
template <typename ...> struct f;
struct x;
struct y;

BOOST_HANA_CONSTANT_CHECK(template_<f>() == type<f<>>);
BOOST_HANA_CONSTANT_CHECK(template_<f>(type<x>) == type<f<x>>);
BOOST_HANA_CONSTANT_CHECK(template_<f>(type<x>, type<y>) == type<f<x, y>>);

// calling `template_` on non-Types
BOOST_HANA_CONSTANT_CHECK(template_<f>(1) == type<f<int>>);

static_assert(std::is_same<
    decltype(template_<f>)::apply<x, y>::type,
    f<x, y>
>::value, "");
//! [template]
}

namespace ns6 {
//! [metafunction]
template <typename ...> struct f { struct type; };
struct x;
struct y;

BOOST_HANA_CONSTANT_CHECK(metafunction<f>() == type<f<>::type>);
BOOST_HANA_CONSTANT_CHECK(metafunction<f>(type<x>) == type<f<x>::type>);
BOOST_HANA_CONSTANT_CHECK(metafunction<f>(type<x>, type<y>) == type<f<x, y>::type>);

// calling `metafunction` on non-Types
BOOST_HANA_CONSTANT_CHECK(metafunction<f>(1) == type<f<int>::type>);

static_assert(std::is_same<
    decltype(metafunction<f>)::apply<x, y>::type,
    f<x, y>::type
>::value, "");
//! [metafunction]
}

namespace ns7 {
//! [liftable_trait]
BOOST_HANA_CONSTANT_CHECK(trait<std::is_integral>(type<int>));
BOOST_HANA_CONSTANT_CHECK(not_(trait<std::is_integral>(type<float>)));
//! [liftable_trait]
}

namespace ns8 {
//! [non_liftable_trait]
BOOST_HANA_CONSTEXPR_LAMBDA auto extent = [](auto t, auto n) {
    return std::extent<typename decltype(t)::type, value(n)>{};
};
BOOST_HANA_CONSTANT_CHECK(extent(type<char>, int_<1>) == size_t<0>);
BOOST_HANA_CONSTANT_CHECK(extent(type<char[1][2]>, int_<1>) == size_t<2>);
//! [non_liftable_trait]
}

namespace ns9 {
//! [make<Type>]
struct X { };
BOOST_HANA_CONSTANT_CHECK(make<Type>(X{}) == decltype_(X{}));
BOOST_HANA_CONSTANT_CHECK(make<Type>(type<X>) == decltype_(type<X>));
//! [make<Type>]
}

int main() { }

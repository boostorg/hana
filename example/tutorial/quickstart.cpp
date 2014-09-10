/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>

#include <boost/hana/detail/assert.hpp>

//! [includes]
#include <boost/hana.hpp>
using namespace boost::hana;
//! [includes]


int main() {

//! [xs]
auto xs = tuple(1, '2', std::string{"345"});
//! [xs]

//! [value_operations]
assert(last(xs) == "345");

assert(tail(xs) == tuple('2', std::string{"345"}));

BOOST_HANA_CONSTANT_ASSERT(!is_empty(xs));

for_each(xs, [](auto x) {
    std::cout << x;
});
//! [value_operations]


//! [std_tuple_parallel]
auto ys = std::make_tuple(1, '2', std::string{"345"});

static_assert(std::tuple_size<decltype(ys)>::value != 0, "");
//! [std_tuple_parallel]


//! [useless]
using wow_that_is_so_useless = decltype(xs);
//! [useless]
(void)sizeof(wow_that_is_so_useless); // remove unused variable warning


//! [ts]
auto ts = tuple(type<int*>, type<void>, type<char const>);
//! [ts]

//! [type_operations]
BOOST_HANA_CONSTANT_ASSERT(last(ts) == type<char const>);

BOOST_HANA_CONSTANT_ASSERT(tail(ts) == tuple(type<void>, type<char const>));
//! [type_operations]

//! [type_out]
static_assert(std::is_same<
    decltype(type<char const>)::type,
    char const
>::value, "");
//! [type_out]

//! [type_foreach]
for_each(ts, [](auto t) {
    using T = typename decltype(t)::type;
    std::cout << typeid(T).name();
});
//! [type_foreach]

//! [type_transformation_def]
auto add_pointer = [](auto t) {
    using T = typename decltype(t)::type;
    return type<typename std::add_pointer<T>::type>;
};
//! [type_transformation_def]

//! [type_transformation_check]
static_assert(std::is_same<
    decltype(add_pointer(type<char const>))::type,
    char const*
>::value, "");
//! [type_transformation_check]

//! [metafunction]
static_assert(std::is_same<
    decltype(metafunction<std::add_pointer>(type<char const>))::type,
    char const*
>::value, "");
//! [metafunction]
}

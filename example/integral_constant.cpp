/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
#include <vector>
using namespace boost::hana;


int main() {

{

//! [operators]
BOOST_HANA_CONSTANT_CHECK(int_<1> + int_<3> == int_<4>);

// Mixed-type operations are supported, but only when it involves a
// promotion, and not a conversion that could be lossy.
BOOST_HANA_CONSTANT_CHECK(size_t<3> * ushort<5> == size_t<15>);
BOOST_HANA_CONSTANT_CHECK(llong<15> == int_<15>);
//! [operators]

}

//////////////////////////////////////////////////////////////////////////////
// Note: Those examples are also act as our unit tests for the `times`
//       member function, so don't delete them.
//////////////////////////////////////////////////////////////////////////////
{

//! [times_loop_unrolling]
std::string s;
for (char c = 'x'; c <= 'z'; ++c)
    int_<5>.times([&] { s += c; });

BOOST_HANA_RUNTIME_CHECK(s == "xxxxxyyyyyzzzzz");
//! [times_loop_unrolling]

}{

//! [as_static_member]
std::string s;
for (char c = 'x'; c <= 'z'; ++c)
    decltype(int_<5>)::times([&] { s += c; });

BOOST_HANA_RUNTIME_CHECK(s == "xxxxxyyyyyzzzzz");
//! [as_static_member]

}{

//! [times_higher_order]
std::string s;
BOOST_HANA_CONSTEXPR_LAMBDA auto functions = make<Tuple>(
    [&] { s += "x"; },
    [&] { s += "y"; },
    [&] { s += "z"; }
);
for_each(functions, int_<5>.times);
BOOST_HANA_RUNTIME_CHECK(s == "xxxxxyyyyyzzzzz");
//! [times_higher_order]

}{

//! [times_with_index_runtime]
std::vector<int> v;
int_<5>.times.with_index([&](auto index) { v.push_back(index); });

BOOST_HANA_RUNTIME_CHECK(v == std::vector<int>{0, 1, 2, 3, 4});
//! [times_with_index_runtime]

//! [times_with_index_compile_time]
constexpr auto xs = make<Tuple>(0, 1, 2);
int_<3>.times.with_index([xs](auto index) {
    static_assert(xs[index] == index, "");
});
//! [times_with_index_compile_time]

}{

//! [literals]
using namespace literals; // contains the _c suffix

BOOST_HANA_CONSTANT_CHECK(1234_c == llong<1234>);
BOOST_HANA_CONSTANT_CHECK(-1234_c == llong<-1234>);
BOOST_HANA_CONSTANT_CHECK(1_c + (3_c * 4_c) == llong<1 + (3 * 4)>);
//! [literals]

}{

//! [integral_constant]
BOOST_HANA_CONSTANT_CHECK(integral_constant<int, 2> == int_<2>);
static_assert(decltype(integral_constant<int, 2>)::value == 2, "");
//! [integral_constant]

}

}

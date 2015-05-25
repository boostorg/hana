/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>

#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
using namespace boost::hana;


int main() {

//! [setup]
struct Person { std::string name; };
struct Car    { std::string name; };
struct City   { std::string name; };

auto stuff = make_tuple(Person{"Louis"}, Car{"Toyota"}, City{"Quebec"});
//   ^^^^^ not constexpr
static_assert(length(stuff) == 3u, ""); // <--- used in a constant expression
//! [setup]

//! [is_empty]
static_assert(!is_empty(stuff), "");
//! [is_empty]

{

//! [all_of_runtime]
bool result = all_of(stuff, [](auto x) {
    return x.name == "Louis";
});
BOOST_HANA_RUNTIME_CHECK(!result);
//! [all_of_runtime]

}{

//! [all_of_compile_time]
auto result = all_of(stuff, [](auto x) {
    return std::is_same<
        std::remove_reference_t<decltype(x.name)>,
        std::string
    >{};
});
static_assert(result, "");
//! [all_of_compile_time]

}{

//! [all_of_compile_time_integral_constant]
_integral_constant<bool, true> result = all_of(stuff, [](auto x) {
    return std::is_same<
        std::remove_reference_t<decltype(x.name)>,
        std::string
    >{};
});
static_assert(result, "");
//! [all_of_compile_time_integral_constant]

}{

//! [std_tuple_size]
std::tuple<int, char, std::string> xs{1, '2', std::string{"345"}};
static_assert(std::tuple_size<decltype(xs)>::value == 3u, "");
//! [std_tuple_size]

}

//! [filter]
auto result = filter(stuff, [](auto x) {
    return std::is_same<decltype(x), Car>{};
});
//! [filter]

}

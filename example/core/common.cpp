/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/common.hpp>

#include <type_traits>
using namespace boost::hana;


//! [common]
struct Person { };
struct Employee : Person { };

static_assert(std::is_same<common<int, float>::type, float>{}, "");
static_assert(std::is_same<common<Person, Employee>::type, Person>{}, "");
//! [common]

//! [common_t]
static_assert(std::is_same<common_t<int, float>, common<int, float>::type>{}, "");
//! [common_t]

//! [has_common]
static_assert(has_common<int, float>{}, "");
static_assert(!has_common<void, float>{}, "");
//! [has_common]

int main() { }

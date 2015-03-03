/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/datatype.hpp>

#include <type_traits>
using namespace boost::hana;


namespace example1 {
//! [datatype]
static_assert(std::is_same<datatype<int>::type, int>{}, "");
static_assert(std::is_same<datatype<int&>::type, int>{}, "");
static_assert(std::is_same<datatype<int const&>::type, int>{}, "");

struct Datatype;
struct Person { struct hana { using datatype = Datatype; }; };
static_assert(std::is_same<datatype<Person>::type, Datatype>{}, "");
static_assert(std::is_same<datatype<Person volatile&&>::type, Datatype>{}, "");
//! [datatype]
}

namespace example2 {
//! [datatype_t]
struct Datatype;
struct Person { struct hana { using datatype = Datatype; }; };
static_assert(std::is_same<datatype_t<Person>, Datatype>{}, "");
//! [datatype_t]
}

int main() { }

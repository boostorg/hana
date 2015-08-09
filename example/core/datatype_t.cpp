/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/datatype.hpp>

#include <type_traits>
namespace hana = boost::hana;


struct Datatype;
struct Person { struct hana { using datatype = Datatype; }; };
static_assert(std::is_same<hana::datatype_t<Person>, Datatype>{}, "");

int main() { }

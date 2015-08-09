/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/convert.hpp>
namespace hana = boost::hana;


struct Person { };
struct Employee : Person { };

static_assert(hana::is_convertible<Employee, Person>{}, "");
static_assert(!hana::is_convertible<Person, Employee>{}, "");

static_assert(hana::is_convertible<int, float>{}, "");

int main() { }

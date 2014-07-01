/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


struct T;
static_assert(std::is_same<decltype(type<T>)::type, T>{}, "");

int main() { }

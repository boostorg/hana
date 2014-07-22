/*
This file is used to as a dirty worksheet for prototyping and checking stuff
that does not need to be documented. Always leave this empty.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */


#include <type_traits>
using z = std::add_pointer_t<char const>;
static_assert(std::is_same<z, char const*>{}, "");

int main() { }

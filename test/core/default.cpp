/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/default.hpp>
using namespace boost::hana;


template <typename T>
struct method_impl : default_ { };

template <>
struct method_impl<int> { };

static_assert(is_default<method_impl<void>>{}, "");
static_assert(!is_default<method_impl<int>>{}, "");

int main() { }

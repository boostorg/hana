/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/detail/is_valid.hpp>

#include <boost/hana/assert.hpp>
using namespace boost::hana;


constexpr struct { } invalid{};

auto add = [](auto x, auto y) -> decltype(x + y) { };
BOOST_HANA_CONSTANT_ASSERT(detail::is_valid(add)(1, 2));
BOOST_HANA_CONSTANT_ASSERT(!detail::is_valid(add)(1, invalid));
BOOST_HANA_CONSTANT_ASSERT(!detail::is_valid(add)(invalid, 1));

auto f = []() -> void { };
BOOST_HANA_CONSTANT_ASSERT(detail::is_valid(f)());
BOOST_HANA_CONSTANT_ASSERT(!detail::is_valid(f)(1));

int main() { (void)add; (void)f; }

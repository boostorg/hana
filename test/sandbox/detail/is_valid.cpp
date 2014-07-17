/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/detail/is_valid.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


constexpr struct { } invalid{};

int main() {
    auto plus = [](auto x, auto y) -> decltype(x + y) { };
    BOOST_HANA_STATIC_ASSERT(detail::is_valid(plus)(1, 2));
    BOOST_HANA_STATIC_ASSERT(!detail::is_valid(plus)(1, invalid));
    BOOST_HANA_STATIC_ASSERT(!detail::is_valid(plus)(invalid, 1));

    BOOST_HANA_STATIC_ASSERT(detail::is_valid([]() -> void {})());
    BOOST_HANA_STATIC_ASSERT(!detail::is_valid([]() -> void {})(1));
}

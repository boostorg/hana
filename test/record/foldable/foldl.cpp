/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include "../udt.hpp"
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto s, auto x) {
    return std::make_tuple(s, x);
};

int main() {
    constexpr auto s = 3.3;
    BOOST_HANA_STATIC_ASSERT(foldl(f, s, udt{0, '0'}) == f(f(s, 0), '0'));
}

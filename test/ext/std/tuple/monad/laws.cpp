/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/monad/laws.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return std::make_tuple(x + int_<1>);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) {
    return std::make_tuple(x * int_<3>);
};

int main() {
    BOOST_HANA_CONSTANT_ASSERT(Monad::laws::check(std::make_tuple(), int_<1>, f, g));
    BOOST_HANA_CONSTANT_ASSERT(Monad::laws::check(std::make_tuple(int_<1>), int_<1>, f, g));
    BOOST_HANA_CONSTANT_ASSERT(Monad::laws::check(std::make_tuple(int_<1>, int_<2>), int_<1>, f, g));
    BOOST_HANA_CONSTEXPR_ASSERT(Monad::laws::check(std::make_tuple(1, 2, 3, 4), int_<1>, f, g));
}

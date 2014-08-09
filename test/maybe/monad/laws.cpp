/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/monad/detail/laws.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return just(x + int_<1>);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) {
    return just(x * int_<3>);
};

int main() {
    BOOST_HANA_CONSTANT_ASSERT(Monad::laws::check(nothing, int_<1>, f, g));
    BOOST_HANA_CONSTANT_ASSERT(Monad::laws::check(just(int_<1>), int_<1>, f, g));
    BOOST_HANA_CONSTEXPR_ASSERT(Monad::laws::check(just(1), int_<1>, f, g));
}

/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) {
    return x + int_<1>;
};

int main() {
    BOOST_HANA_STATIC_ASSERT(fmap(inc, nothing) == nothing);
    BOOST_HANA_STATIC_ASSERT(fmap(inc, just(1)) == just(2));
    BOOST_HANA_STATIC_ASSERT(fmap(inc, just(int_<1>)) == just(int_<2>));
}

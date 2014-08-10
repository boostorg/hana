/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/comparable/detail/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


constexpr auto x = detail::number<>(0);
constexpr auto y = detail::number<>(1);

int main() {
    // equal
    {
        BOOST_HANA_CONSTANT_ASSERT( equal(nothing, nothing));
        BOOST_HANA_CONSTANT_ASSERT(!equal(nothing, just(x)));
        BOOST_HANA_CONSTANT_ASSERT(!equal(just(x), nothing));
        BOOST_HANA_CONSTEXPR_ASSERT( equal(just(x), just(x)));
        BOOST_HANA_CONSTEXPR_ASSERT(!equal(just(x), just(y)));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Comparable::laws::check(
            list(nothing, just(x), just(y))
        ));
    }
}

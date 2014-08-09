/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/monoid/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/detail/number/monoid.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/monoid/detail/laws.hpp>
using namespace boost::hana;


template <typename Mcd>
void test() {
    constexpr auto n = detail::number<Mcd>;
    using N = detail::Number<Mcd>;

    // zero
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(zero<N>, n(0)));
    }

    // plus
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(plus(n(2), n(3)), n(2 + 3)));
    }

    // operators
    {
        constexpr auto n = detail::number<Mcd, operators<Monoid>>;
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            n(2) + n(3),
            plus(n(2), n(3))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Monoid::laws::check(
            list(n(0), n(1), n(2), n(3))
        ));
    }
}

int main() {
    test<Monoid::mcd>();
}

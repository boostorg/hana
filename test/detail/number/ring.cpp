/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ring/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/detail/number/ring.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/ring/detail/laws.hpp>
using namespace boost::hana;


template <typename Mcd>
void test() {
    constexpr auto n = detail::number<Mcd>;
    using N = detail::Number<Mcd>;

    // one
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(one<N>, n(1)));
    }

    // mult
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(mult(n(2), n(3)), n(2 * 3)));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Ring::laws::check(
            list(n(0), n(1), n(2), n(3))
        ));
    }

    // operators
    {
        constexpr auto n = detail::number<Mcd, operators<Ring>>;
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            n(2) * n(3),
            mult(n(2), n(3))
        ));
    }
}

int main() {
    test<Ring::mcd>();
}

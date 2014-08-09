/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/group/minus_mcd.hpp>
#include <boost/hana/group/negate_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/detail/number/group.hpp>
#include <boost/hana/group/detail/laws.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <typename Mcd>
void test() {
    constexpr auto n = detail::number<Mcd>;

    // minus
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(minus(n(2), n(3)), n(2 - 3)));
    }

    // negate
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(negate(n(2)), n(-2)));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Group::laws::check(
            list(n(0), n(1), n(2), n(3))
        ));
    }

    // operators
    {
        constexpr auto n = detail::number<Mcd, operators<Group>>;
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            n(2) - n(3),
            minus(n(2), n(3))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            -n(2), negate(n(2))
        ));
    }
}

int main() {
    test<detail::quote<Group::negate_mcd>>();
    test<detail::quote<Group::minus_mcd>>();
}

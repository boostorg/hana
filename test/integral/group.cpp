/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/group/detail/laws.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


int main() {
    // negate
    {
        BOOST_HANA_CONSTANT_ASSERT(negate(int_<1>) == int_<-1>);
    }

    // minus
    {
        BOOST_HANA_CONSTANT_ASSERT(minus(int_<1>, int_<3>) == int_<-2>);
        BOOST_HANA_CONSTEXPR_ASSERT(minus(int_<1>, 3) == -2);
        BOOST_HANA_CONSTEXPR_ASSERT(minus(1, int_<3>) == -2);
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Group::laws::check(list(
            int_<1>, short_<2>, long_<3>, ullong<4>, 5, 6ull
        )));
    }
}

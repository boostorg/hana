/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/tuple.hpp>

#include <test/auto/base.hpp>
#include <test/auto/group.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <> auto objects<int> = make<Tuple>(0,1,2,3,4,5);
    template <> auto objects<long> = make<Tuple>(0l,1l,2l,3l,4l,5l);
}}}


int main() {
    test::laws<Group, int>();
    test::laws<Group, long>();

    using T = int;
    constexpr T x = 6, y = 4;

    // minus
    {
        BOOST_HANA_CONSTEXPR_CHECK(minus(x, y) == x - y);
    }

    // negate
    {
        BOOST_HANA_CONSTEXPR_CHECK(negate(x) == -x);
    }
}

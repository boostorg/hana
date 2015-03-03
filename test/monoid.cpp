/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/tuple.hpp>

#include <test/auto/base.hpp>
#include <test/auto/monoid.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <> auto objects<int> = make<Tuple>(0,1,2,3,4,5);
    template <> auto objects<unsigned int> = make<Tuple>(0u,1u,2u,3u,4u,5u);
    template <> auto objects<long> = make<Tuple>(0l,1l,2l,3l,4l,5l);
    template <> auto objects<unsigned long> = make<Tuple>(0ul,1ul,2ul,3ul,4ul,5ul);
}}}


int main() {
    test::laws<Monoid, int>();
    test::laws<Monoid, unsigned int>();

    test::laws<Monoid, long>();
    test::laws<Monoid, unsigned long>();

    using T = int;
    constexpr T x = 6, y = 4;

    // zero
    {
        BOOST_HANA_CONSTEXPR_CHECK(zero<T>() == static_cast<T>(0));
    }

    // plus
    {
        BOOST_HANA_CONSTEXPR_CHECK(plus(x, y) == x + y);
    }
}

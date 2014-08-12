/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
        return [=](auto y) { return equal(x, y); };
    };
    using test::x;

    // any
    {
        BOOST_HANA_CONSTANT_ASSERT(not_(any(set(), is(x<1>))));

        BOOST_HANA_CONSTANT_ASSERT(any(set(x<1>), is(x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(not_(any(set(x<1>), is(x<2>))));

        BOOST_HANA_CONSTANT_ASSERT(any(set(x<1>, x<2>), is(x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(any(set(x<1>, x<2>), is(x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(not_(any(set(x<1>, x<2>), is(x<3>))));
    }

    // find
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(set(), is(x<1>)),
            nothing
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(set(x<1>), is(x<1>)),
            just(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(set(x<1>), is(x<2>)),
            nothing
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(set(x<1>, x<2>), is(x<1>)),
            just(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(set(x<1>, x<2>), is(x<2>)),
            just(x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(set(x<1>, x<2>), is(x<3>)),
            nothing
        ));
    }
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe/comparable.hpp>
#include <boost/hana/maybe/searchable.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
        return [=](auto y) { return equal(x, y); };
    };
    using test::x;

    // find
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(just(x<0>), is(x<0>)),
            just(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(just(x<0>), is(x<1>)),
            nothing
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(nothing, is(x<0>)),
            nothing
        ));
    }

    // any
    {
        BOOST_HANA_CONSTANT_ASSERT(any(just(x<0>), is(x<0>)));
        BOOST_HANA_CONSTANT_ASSERT(not_(any(just(x<0>), is(x<1>))));
        BOOST_HANA_CONSTANT_ASSERT(not_(any(nothing, is(x<0>))));
    }
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/laws/comparable.hpp>
#include <test/injection.hpp>
#include <test/minimal_list.hpp>
using namespace boost::hana;


template <typename mcd>
void tests() {
    auto list = test::list<mcd>;
    using test::x;

    // equal
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            list(),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            list(x<0>),
            list()
        )));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            list(),
            list(x<0>)
        )));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            list(x<0>),
            list(x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            list(x<0>, x<1>),
            list(x<0>)
        )));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            list(x<0>, x<1>),
            list(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            list(x<0>, x<1>, x<2>, x<3>),
            list(x<0>, x<1>, x<2>, x<4>)
        )));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable_laws(
            list(),
            list(x<0>),
            list(x<0>, x<1>),
            list(x<0>, x<1>, x<2>)
        ));
    }
}

int main() {
    tests<List::mcd<void>>();
}

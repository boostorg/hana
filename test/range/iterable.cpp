/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range/comparable.hpp>
#include <boost/hana/range/iterable.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>

#include <test/laws/iterable.hpp>
using namespace boost::hana;


int main() {
    // is_empty
    {
        BOOST_HANA_CONSTANT_ASSERT(is_empty(range(int_<0>, int_<0>)));
        BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(range(int_<0>, int_<1>))));
        BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(range(int_<0>, int_<2>))));
    }

    // head
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(head(range(int_<0>, int_<1>)), int_<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(head(range(int_<0>, int_<2>)), int_<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(head(range(int_<2>, int_<5>)), int_<2>));
        BOOST_HANA_CONSTANT_ASSERT(equal(head(range(int_<5>, int_<6>)), int_<5>));
    }

    // tail
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            tail(range(int_<0>, int_<1>)),
            range(int_<1>, int_<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            tail(range(int_<0>, int_<2>)),
            range(int_<1>, int_<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            tail(range(int_<0>, int_<3>)),
            range(int_<1>, int_<3>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            tail(range(int_<4>, int_<7>)),
            range(int_<5>, int_<7>)
        ));
    }

    // at
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(at(int_<0>, range(int_<0>, int_<1>)), int_<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(at(int_<0>, range(int_<0>, int_<2>)), int_<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(at(int_<1>, range(int_<0>, int_<2>)), int_<1>));
        BOOST_HANA_CONSTANT_ASSERT(equal(at(int_<46>, range(int_<4>, int_<90>)), int_<50>));
    }

    // last
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(last(range(int_<0>, int_<1>)), int_<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(last(range(int_<0>, int_<2>)), int_<1>));
        BOOST_HANA_CONSTANT_ASSERT(equal(last(range(int_<0>, int_<3>)), int_<2>));
        BOOST_HANA_CONSTANT_ASSERT(equal(last(range(int_<3>, int_<6>)), int_<5>));
    }

    // drop
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<0>, range(int_<0>, int_<0>)),
            range(int_<0>, int_<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<1>, range(int_<0>, int_<0>)),
            range(int_<0>, int_<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<2>, range(int_<0>, int_<0>)),
            range(int_<0>, int_<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<0>, range(int_<0>, int_<1>)),
            range(int_<0>, int_<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<1>, range(int_<0>, int_<1>)),
            range(int_<1>, int_<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<2>, range(int_<0>, int_<1>)),
            range(int_<1>, int_<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<0>, range(int_<0>, int_<2>)),
            range(int_<0>, int_<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<1>, range(int_<0>, int_<2>)),
            range(int_<1>, int_<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<2>, range(int_<0>, int_<2>)),
            range(int_<2>, int_<2>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<0>, range(int_<0>, int_<3>)),
            range(int_<0>, int_<3>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<1>, range(int_<0>, int_<3>)),
            range(int_<1>, int_<3>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<2>, range(int_<0>, int_<3>)),
            range(int_<2>, int_<3>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<3>, range(int_<0>, int_<3>)),
            range(int_<3>, int_<3>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(int_<10>, range(int_<20>, int_<50>)),
            range(int_<30>, int_<50>)
        ));
    }

    // operators
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            range(int_<0>, int_<10>)[int_<3>],
            int_<3>
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Iterable_laws(
            range(int_<0>, int_<0>),
            range(int_<0>, int_<1>),
            range(int_<3>, int_<10>),
            range(int_<20>, int_<20>),
            range(int_<4>, int_<8>)
        ));
    }
}

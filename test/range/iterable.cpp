/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/iterable.hpp>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto ranges = make<Tuple>(
          range(int_<0>, int_<0>)
        , range(int_<0>, int_<1>)
        , range(int_<0>, int_<2>)
        , range(int_<1>, int_<1>)
        , range(int_<1>, int_<2>)
        , range(int_<1>, int_<3>)
        , range(int_<50>, int_<60>)

        , range(int_<50>, long_<60>)
        , range(long_<50>, int_<60>)
    );

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        // is_empty
        {
            BOOST_HANA_CONSTANT_CHECK(is_empty(range(int_<0>, int_<0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(range(int_<0>, int_<1>))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(range(int_<0>, int_<2>))));
        }

        // head
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(range(int_<0>, int_<1>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(range(int_<0>, int_<2>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(range(int_<2>, int_<5>)), int_<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(range(int_<5>, int_<6>)), int_<5>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                head(range(int_<5>, long_<6>)), long_<5>
            ));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(range(int_<0>, int_<1>)),
                range(int_<1>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(range(int_<0>, int_<2>)),
                range(int_<1>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(range(int_<0>, int_<3>)),
                range(int_<1>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(range(int_<4>, int_<7>)),
                range(int_<5>, int_<7>)
            ));
        }

        // at
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(range(int_<0>, int_<1>), int_<0>),
                int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(range(int_<0>, int_<2>), int_<0>),
                int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(range(int_<0>, int_<2>), int_<1>),
                int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(range(int_<4>, int_<90>), int_<46>),
                int_<50>
            ));
        }

        // last
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<0>, int_<1>)),
                int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<0>, int_<2>)),
                int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<0>, int_<3>)),
                int_<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<3>, int_<6>)),
                int_<5>
            ));
        }

        // drop.at_most
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<0>), int_<0>),
                range(int_<0>, int_<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<0>), int_<1>),
                range(int_<0>, int_<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<0>), int_<2>),
                range(int_<0>, int_<0>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<1>), int_<0>),
                range(int_<0>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<1>), int_<1>),
                range(int_<1>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<1>), int_<2>),
                range(int_<1>, int_<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<2>), int_<0>),
                range(int_<0>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<2>), int_<1>),
                range(int_<1>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<2>), int_<2>),
                range(int_<2>, int_<2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<3>), int_<0>),
                range(int_<0>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<3>), int_<1>),
                range(int_<1>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<3>), int_<2>),
                range(int_<2>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<0>, int_<3>), int_<3>),
                range(int_<3>, int_<3>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.at_most(range(int_<20>, int_<50>), int_<10>),
                range(int_<30>, int_<50>)
            ));
        }

        // drop.exactly
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.exactly(range(int_<0>, int_<0>), int_<0>),
                range(int_<0>, int_<0>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.exactly(range(int_<0>, int_<1>), int_<0>),
                range(int_<0>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.exactly(range(int_<0>, int_<1>), int_<1>),
                range(int_<1>, int_<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.exactly(range(int_<0>, int_<2>), int_<0>),
                range(int_<0>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.exactly(range(int_<0>, int_<2>), int_<1>),
                range(int_<1>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.exactly(range(int_<0>, int_<2>), int_<2>),
                range(int_<2>, int_<2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.exactly(range(int_<0>, int_<3>), int_<0>),
                range(int_<0>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.exactly(range(int_<0>, int_<3>), int_<1>),
                range(int_<1>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.exactly(range(int_<0>, int_<3>), int_<2>),
                range(int_<2>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.exactly(range(int_<0>, int_<3>), int_<3>),
                range(int_<3>, int_<3>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop.exactly(range(int_<20>, int_<50>), int_<10>),
                range(int_<30>, int_<50>)
            ));
        }

        // operators
        static_assert(has_operator<Range, decltype(at)>, "");

        // laws
        test::TestIterable<Range>{ranges};
    }
}

/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/ratio.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/comparable.hpp>
#include <laws/group.hpp>
#include <laws/integral_domain.hpp>
#include <laws/monoid.hpp>
#include <laws/orderable.hpp>
#include <laws/ring.hpp>
#include <test/cnumeric.hpp>

#include <ratio>
using namespace boost::hana;


int main() {
    auto ratios = make<tuple_tag>(
          std::ratio<0>{}
        , std::ratio<1, 3>{}
        , std::ratio<1, 2>{}
        , std::ratio<2, 6>{}
        , std::ratio<3, 1>{}
        , std::ratio<7, 8>{}
        , std::ratio<3, 5>{}
        , std::ratio<2, 1>{}
    );
    (void)ratios;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Conversions
    //////////////////////////////////////////////////////////////////////////
    {
        // Constant -> Ratio
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<ext::std::ratio_tag>(test::cnumeric<int, 0>),
                std::ratio<0>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<ext::std::ratio_tag>(test::cnumeric<int, 1>),
                std::ratio<1>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<ext::std::ratio_tag>(test::cnumeric<int, 3>),
                std::ratio<3>{}
            ));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                std::ratio<0>{},
                std::ratio<0>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                std::ratio<3, 5>{},
                std::ratio<6, 10>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                std::ratio<4, 5>{},
                std::ratio<6, 10>{}
            )));
        }

        // laws
        test::TestComparable<ext::std::ratio_tag>{ratios};
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        // less
        {
            BOOST_HANA_CONSTANT_CHECK(less(
                std::ratio<1>{},
                std::ratio<3>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(less(
                std::ratio<4, 10>{},
                std::ratio<3, 5>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(not_(less(
                std::ratio<3, 5>{},
                std::ratio<4, 10>{}
            )));
        }

        // laws
        auto ratios = make<tuple_tag>(
              std::ratio<0>{}
            , std::ratio<1, 3>{}
            , std::ratio<1, 2>{}
            , std::ratio<2, 6>{}
            , std::ratio<7, 8>{}
            , std::ratio<3, 5>{}
        );

        test::TestOrderable<ext::std::ratio_tag>{ratios};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Monoid
    //////////////////////////////////////////////////////////////////////////
    {
        // plus
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                plus(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
                std::ratio<3*10 + 5*4, 4*10>{}
            ));
        }

        // zero
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                zero<ext::std::ratio_tag>(),
                std::ratio<0, 1>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                zero<ext::std::ratio_tag>(),
                std::ratio<0, 2>{}
            ));
        }

        // laws
        test::TestMonoid<ext::std::ratio_tag>{ratios};
    }

    //////////////////////////////////////////////////////////////////////////
    // Group
    //////////////////////////////////////////////////////////////////////////
    {
        // minus
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                minus(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
                std::ratio<3*10 - 5*4, 4*10>{}
            ));
        }

        // laws
        test::TestGroup<ext::std::ratio_tag>{ratios};
    }

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Ring
    //////////////////////////////////////////////////////////////////////////
    {
        // mult
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                mult(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
                std::ratio<3*5, 4*10>{}
            ));
        }

        // one
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                one<ext::std::ratio_tag>(),
                std::ratio<1, 1>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                one<ext::std::ratio_tag>(),
                std::ratio<2, 2>{}
            ));
        }

        // laws
        test::TestRing<ext::std::ratio_tag>{ratios};
    }

#elif BOOST_HANA_TEST_PART == 5
    //////////////////////////////////////////////////////////////////////////
    // IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    {
        // div
        {
            using boost::hana::div; // hide ::div
            BOOST_HANA_CONSTANT_CHECK(equal(
                div(std::ratio<6>{}, std::ratio<4>{}),
                std::ratio<6, 4>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                div(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
                std::ratio<3*10, 4*5>{}
            ));
        }

        // mod
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                mod(std::ratio<6>{}, std::ratio<4>{}),
                std::ratio<0>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                mod(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
                std::ratio<0>{}
            ));
        }

        // laws
        test::TestIntegralDomain<ext::std::ratio_tag>{ratios};
    }
#endif
}

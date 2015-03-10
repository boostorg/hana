/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/ratio.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/group.hpp>
#include <laws/integral_domain.hpp>
#include <laws/monoid.hpp>
#include <laws/ring.hpp>

#include <ratio>
using namespace boost::hana;


int main() {
    auto ratios = make<Tuple>(
          std::ratio<0>{}
        , std::ratio<1, 3>{}
        , std::ratio<1, 2>{}
        , std::ratio<2, 6>{}
        , std::ratio<3, 1>{}
        , std::ratio<7, 8>{}
        , std::ratio<3, 5>{}
        , std::ratio<2, 1>{}
    );

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
                zero<ext::std::Ratio>(),
                std::ratio<0, 1>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                zero<ext::std::Ratio>(),
                std::ratio<0, 2>{}
            ));
        }

        // laws
        test::TestMonoid<ext::std::Ratio>{ratios};
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
        test::TestGroup<ext::std::Ratio>{ratios};
    }

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
                one<ext::std::Ratio>(),
                std::ratio<1, 1>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                one<ext::std::Ratio>(),
                std::ratio<2, 2>{}
            ));
        }

        // laws
        test::TestRing<ext::std::Ratio>{ratios};
    }

    //////////////////////////////////////////////////////////////////////////
    // IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    {
        // quot
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                quot(std::ratio<6>{}, std::ratio<4>{}),
                std::ratio<6, 4>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                quot(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
                std::ratio<3*10, 4*5>{}
            ));
        }

        // rem
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                rem(std::ratio<6>{}, std::ratio<4>{}),
                std::ratio<0>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                rem(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
                std::ratio<0>{}
            ));
        }

        // laws
        test::TestIntegralDomain<ext::std::Ratio>{ratios};
    }
}

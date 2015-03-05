/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integral_constant.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/enumerable.hpp>
#include <laws/group.hpp>
#include <laws/integral_domain.hpp>
#include <laws/monoid.hpp>
#include <laws/ring.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    auto ints = make<Tuple>(
        std::integral_constant<int, -10>{},
        std::integral_constant<int, -2>{},
        std::integral_constant<int, 0>{},
        std::integral_constant<int, 1>{},
        std::integral_constant<int, 3>{}
    );

    //////////////////////////////////////////////////////////////////////////
    // Enumerable, Monoid, Group, Ring, IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    {
        // laws
        test::TestEnumerable<ext::std::IntegralConstant<int>>{ints};
        test::TestMonoid<ext::std::IntegralConstant<int>>{ints};
        test::TestGroup<ext::std::IntegralConstant<int>>{ints};
        test::TestRing<ext::std::IntegralConstant<int>>{ints};
        test::TestIntegralDomain<ext::std::IntegralConstant<int>>{ints};
    }
}

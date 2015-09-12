/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/canonical_constant.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/comparable.hpp>
#include <laws/constant.hpp>
#include <laws/group.hpp>
#include <laws/integral_domain.hpp>
#include <laws/logical.hpp>
#include <laws/monoid.hpp>
#include <laws/orderable.hpp>
#include <laws/ring.hpp>
using namespace boost::hana;


template <typename T, T v>
struct canonical {
    static constexpr T value = v;
    using hana_tag = detail::CanonicalConstant<T>;
};

int main() {
    auto ints = make<tuple_tag>(
        canonical<int, -10>{}, canonical<int, -2>{}, canonical<int, 0>{},
        canonical<int, 1>{}, canonical<int, 3>{}, canonical<int, 4>{}
    );
    (void)ints;

    auto bools = make<tuple_tag>(canonical<bool, true>{}, canonical<bool, false>{});
    (void)bools;

    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
#if BOOST_HANA_TEST_PART == 1
    test::TestConstant<detail::CanonicalConstant<int>>{ints, tuple_t<int, long, long long>};
    test::TestConstant<detail::CanonicalConstant<bool>>{bools, tuple_t<bool>};
#endif

    //////////////////////////////////////////////////////////////////////////
    // Monoid, Group, Ring, IntegralDomain
    //////////////////////////////////////////////////////////////////////////
#if BOOST_HANA_TEST_PART == 2
    test::TestMonoid<detail::CanonicalConstant<int>>{ints};
    test::TestGroup<detail::CanonicalConstant<int>>{ints};
#endif

#if BOOST_HANA_TEST_PART == 3
    test::TestRing<detail::CanonicalConstant<int>>{ints};
#endif

#if BOOST_HANA_TEST_PART == 4
    test::TestIntegralDomain<detail::CanonicalConstant<int>>{ints};
#endif

    //////////////////////////////////////////////////////////////////////////
    // Logical
    //////////////////////////////////////////////////////////////////////////
#if BOOST_HANA_TEST_PART == 5
    {
        auto ints = make<tuple_tag>(
            canonical<int, -2>{}, canonical<int, 0>{},
            canonical<int, 1>{}, canonical<int, 3>{}
        );
        test::TestLogical<detail::CanonicalConstant<int>>{ints};
        test::TestLogical<detail::CanonicalConstant<bool>>{bools};
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    // Comparable and Orderable
    //////////////////////////////////////////////////////////////////////////
#if BOOST_HANA_TEST_PART == 6
    test::TestComparable<detail::CanonicalConstant<int>>{ints};
    test::TestOrderable<detail::CanonicalConstant<int>>{ints};
#endif
}

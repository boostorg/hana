/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/integral_c.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/enumerable.hpp>
#include <laws/group.hpp>
#include <laws/integral_domain.hpp>
#include <laws/monoid.hpp>
#include <laws/ring.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/integral_c.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


int main() {
    auto int_constants = make<Tuple>(
        mpl::int_<-10>{}, mpl::int_<-2>{}, mpl::integral_c<int, 0>{},
        mpl::integral_c<int, 1>{}, mpl::integral_c<int, 3>{}
    );

    //////////////////////////////////////////////////////////////////////////
    // Enumerable, Monoid, Group, Ring, IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    {
        // laws
        test::TestEnumerable<ext::boost::mpl::IntegralC<int>>{int_constants};
        test::TestMonoid<ext::boost::mpl::IntegralC<int>>{int_constants};
        test::TestGroup<ext::boost::mpl::IntegralC<int>>{int_constants};
        test::TestRing<ext::boost::mpl::IntegralC<int>>{int_constants};
        test::TestIntegralDomain<ext::boost::mpl::IntegralC<int>>{int_constants};
    }
}

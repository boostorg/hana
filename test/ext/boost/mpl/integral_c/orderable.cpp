/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/integral_c.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/comparable.hpp>
#include <laws/orderable.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/integral_c.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto ints = make<Tuple>(
        mpl::int_<-10>{}, mpl::int_<-2>{}, mpl::integral_c<int, 0>{},
        mpl::integral_c<int, 1>{}, mpl::integral_c<int, 3>{}
    );

    //////////////////////////////////////////////////////////////////////////
    // Comparable and Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        // laws
        test::TestComparable<ext::boost::mpl::IntegralC<int>>{ints};
        test::TestOrderable<ext::boost::mpl::IntegralC<int>>{ints};
    }
}

/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/integral_c.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/constant.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/integral_c.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


int main() {
    auto constants = make<Tuple>(
        mpl::int_<-10>{}, mpl::int_<-2>{}, mpl::integral_c<int, 0>{},
        mpl::integral_c<int, 1>{}, mpl::integral_c<int, 3>{}
    );

    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    {
        // value
        static_assert(value(mpl::integral_c<int, 0>{}) == 0, "");
        static_assert(value(mpl::integral_c<int, 1>{}) == 1, "");
        static_assert(value(mpl::integral_c<int, 3>{}) == 3, "");

        // laws
        test::TestConstant<ext::boost::mpl::IntegralC<int>>{constants};
    }
}

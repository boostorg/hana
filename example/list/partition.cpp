/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_integral_constant.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [fusion]
    BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
        return x % int_<2> != int_<0>;
    };

    BOOST_HANA_STATIC_ASSERT(
        partition(odd, list_c<int, 1, 2, 3, 4, 5, 6, 7>)
        ==
        pair(
            list_c<int, 1, 3, 5, 7>,
            list_c<int, 2, 4, 6>
        )
    );
    //! [fusion]

    //! [mpl]
    BOOST_HANA_STATIC_ASSERT(
        partition(trait<std::is_floating_point>, list_t<void, int, float, char, double>)
        ==
        pair(
            list_t<float, double>,
            list_t<void, int, char>
        )
    );
    //! [mpl]
}

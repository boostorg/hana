/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    //! [liftable]
    BOOST_HANA_STATIC_ASSERT(trait<std::is_integral>(type<int>));
    BOOST_HANA_STATIC_ASSERT(!trait<std::is_integral>(type<float>));
    //! [liftable]

    //! [nonliftable]
    BOOST_HANA_CONSTEXPR_LAMBDA auto extent = [](auto t, auto n) {
        return std::extent<typename decltype(t)::type, n()>{};
    };
    BOOST_HANA_STATIC_ASSERT(extent(type<char>, int_<1>) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(extent(type<char[1][2]>, int_<1>) == int_<2>);
    //! [nonliftable]
}

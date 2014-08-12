/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto xs = list(0, '1', 2.0);
    BOOST_HANA_CONSTEXPR_ASSERT(drop(int_<0>, xs) == xs);
    BOOST_HANA_CONSTEXPR_ASSERT(drop(int_<1>, xs) == list('1', 2.0));
    BOOST_HANA_CONSTEXPR_ASSERT(drop(int_<2>, xs) == list(2.0));
    BOOST_HANA_CONSTANT_ASSERT(drop(int_<3>, xs) == list());
    BOOST_HANA_CONSTANT_ASSERT(drop(int_<4>, xs) == list());
    //! [main]
}

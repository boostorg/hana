/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(elem(int_<3>, list(int_<1>, int_<2>, int_<3>, int_<4>)));
    BOOST_HANA_STATIC_ASSERT(!elem(int_<0>, list(int_<1>, int_<2>, int_<3>, int_<4>)));
    //! [main]
}

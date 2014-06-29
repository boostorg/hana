/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(less(int_<-3>, int_<3>));
    BOOST_HANA_STATIC_ASSERT(max(int_<-3>, long_<2>) == long_<2>);
    BOOST_HANA_STATIC_ASSERT(min(int_<-3>, long_<2>) == int_<-3>);
    //! [main]
}

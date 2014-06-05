/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(int_<1> + int_<3> == int_<4>);
    BOOST_HANA_STATIC_ASSERT(decltype_(int_<1> + int_<3>) == decltype_(int_<4>));

    // Mixed-type operations are supported:
    BOOST_HANA_STATIC_ASSERT(decltype_(size_t<3> * ushort<5>) == decltype_(size_t<15>));
    BOOST_HANA_STATIC_ASSERT(decltype_(size_t<15>) != decltype_(int_<15>));
    BOOST_HANA_STATIC_ASSERT(size_t<3> * ushort<5> == int_<15>);
    //! [main]
}

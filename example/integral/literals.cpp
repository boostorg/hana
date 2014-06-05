/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;
using namespace literals;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(1234_c == ullong<1234>);
    BOOST_HANA_STATIC_ASSERT(decltype_(1234_c) == decltype_(ullong<1234>));

    BOOST_HANA_STATIC_ASSERT(sum(list(1_c, 2_c, 3_c, 4_c)) == 10_c);
    //! [main]
}

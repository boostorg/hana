/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(
        adjust(_ < int_<0>, -_, list(-3, int_<0>, 1, int_<-5>)) == list(3, int_<0>, 1, int_<5>)
    );
    //! [main]
}

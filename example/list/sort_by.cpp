/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;
using namespace literals;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(
        sort_by(_>_, list(1_c, -2_c, 3_c, 0_c)) == list(3_c, 1_c, 0_c, -2_c)
    );
    //! [main]
}

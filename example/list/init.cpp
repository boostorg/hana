/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;
using namespace literals;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(init(list(1)) == list());
    BOOST_HANA_STATIC_ASSERT(init(list(1, '2', "3", 4_c)) == list(1, '2', "3"));
    //! [main]
}

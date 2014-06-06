/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;
using namespace literals;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(concat() == list());
    BOOST_HANA_STATIC_ASSERT(concat(list(1, "abc")) == list(1, "abc"));

    BOOST_HANA_STATIC_ASSERT(
        concat(list(1, 'a'), list(2_c, "bb"))
        ==
        list(1, 'a', 2_c, "bb")
    );

    BOOST_HANA_STATIC_ASSERT(
        concat(list_t<int, char>, list_t<>, list_t<void, struct foo>)
        ==
        list_t<int, char, void, struct foo>
    );
    //! [main]
}

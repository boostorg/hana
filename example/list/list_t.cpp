/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(list_t<void, int, float> == list(type<void>, type<int>, type<float>));
    BOOST_HANA_STATIC_ASSERT(head(list_t<void, int, float>) == type<void>);
    //! [main]
}

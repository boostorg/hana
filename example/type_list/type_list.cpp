/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(to<List>(type_list<void, int, float>) == list(type<void>, type<int>, type<float>));
    BOOST_HANA_CONSTANT_ASSERT(head(type_list<void, int, float>) == type<void>);
    //! [main]
}

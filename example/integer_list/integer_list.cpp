/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integer_list.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(
        to<Tuple>(integer_list<int, 0, 1, 2>)
        ==
        tuple(int_<0>, int_<1>, int_<2>)
    );
    BOOST_HANA_CONSTANT_ASSERT(head(integer_list<int, 0, 1, 2>) == int_<0>);
    //! [main]
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto sorted = sort_by(ordering(sizeof_), list(
        type<char[3]>,
        type<char[1]>,
        type<char[2]>,
        type<char[15]>
    ));
    BOOST_HANA_STATIC_ASSERT(sorted == list(
        type<char[1]>,
        type<char[2]>,
        type<char[3]>,
        type<char[15]>
    ));
    //! [main]
}

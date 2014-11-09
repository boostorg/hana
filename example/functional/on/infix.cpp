/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto sorted = sort_by(less ^on^ first, tuple(
        pair(int_<3>, 'x'),
        pair(int_<1>, type<void>),
        pair(int_<2>, 9876)
    ));

    BOOST_HANA_CONSTEXPR_CHECK(sorted == tuple(
        pair(int_<1>, type<void>),
        pair(int_<2>, 9876),
        pair(int_<3>, 'x')
    ));
    //! [main]
}

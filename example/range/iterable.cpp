/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/range.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto r = range(int_<0>, int_<1000>);
    BOOST_HANA_STATIC_ASSERT(head(r) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(last(r) == int_<999>);
    BOOST_HANA_STATIC_ASSERT(!is_empty(r));
    BOOST_HANA_STATIC_ASSERT(is_empty(range(int_<3>, int_<3>)));
    //! [main]
}

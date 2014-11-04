/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/range.hpp>
using namespace boost::hana;


int main() {
    //! [range_c]
    BOOST_HANA_CONSTANT_ASSERT(head(range_c<int, 0, 5>) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(last(range_c<unsigned long, 0, 5>) == ulong<4>);
    BOOST_HANA_CONSTANT_ASSERT(tail(range_c<int, 0, 5>) == range(int_<1>, int_<5>));
    //! [range_c]

    //! [iterable]
    BOOST_HANA_CONSTEXPR_LAMBDA auto r = range(int_<0>, int_<1000>);
    BOOST_HANA_CONSTANT_ASSERT(head(r) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(last(r) == int_<999>);
    BOOST_HANA_CONSTANT_ASSERT(!is_empty(r));
    BOOST_HANA_CONSTANT_ASSERT(is_empty(range(int_<3>, int_<3>)));
    //! [iterable]

    //! [range]
    using namespace literals;

    BOOST_HANA_CONSTANT_ASSERT(head(range(0_c, 5_c)) == 0_c);
    BOOST_HANA_CONSTANT_ASSERT(last(range(0_c, 5_c)) == 4_c);
    BOOST_HANA_CONSTANT_ASSERT(tail(range(0_c, 5_c)) == range(1_c, 5_c));
    //! [range]
}

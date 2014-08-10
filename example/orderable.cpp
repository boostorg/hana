/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [greater]
    BOOST_HANA_CONSTEXPR_ASSERT(greater(4, 1));
    BOOST_HANA_CONSTANT_ASSERT(!greater(int_<1>, int_<3>));
    //! [greater]

    //! [greater_equal]
    BOOST_HANA_CONSTEXPR_ASSERT(greater_equal(4, 1));
    BOOST_HANA_CONSTEXPR_ASSERT(greater_equal(1, 1));
    BOOST_HANA_CONSTANT_ASSERT(!greater_equal(int_<1>, int_<2>));
    //! [greater_equal]

    //! [less]
    BOOST_HANA_CONSTEXPR_ASSERT(less(1, 4));
    BOOST_HANA_CONSTANT_ASSERT(!less(int_<3>, int_<2>));
    //! [less]

    //! [less_equal]
    BOOST_HANA_CONSTEXPR_ASSERT(less_equal(1, 4));
    BOOST_HANA_CONSTEXPR_ASSERT(less_equal(1, 1));
    BOOST_HANA_CONSTANT_ASSERT(!less_equal(int_<3>, int_<2>));
    //! [less_equal]

    //! [max]
    BOOST_HANA_CONSTEXPR_ASSERT(max(1, 4) == 4);
    BOOST_HANA_CONSTANT_ASSERT(max(int_<7>, int_<5>) == int_<7>);
    //! [max]

    //! [min]
    BOOST_HANA_CONSTEXPR_ASSERT(min(1, 4) == 1);
    BOOST_HANA_CONSTANT_ASSERT(min(int_<7>, int_<5>) == int_<5>);
    //! [min]

    //! [ordering]
    BOOST_HANA_CONSTEXPR_LAMBDA auto sorted = sort_by(ordering(sizeof_), list(
        type<char[3]>,
        type<char[1]>,
        type<char[2]>,
        type<char[15]>
    ));
    BOOST_HANA_CONSTANT_ASSERT(sorted == list(
        type<char[1]>,
        type<char[2]>,
        type<char[3]>,
        type<char[15]>
    ));
    //! [ordering]
}

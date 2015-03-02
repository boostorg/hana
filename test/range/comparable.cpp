/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/comparable.hpp>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto ranges = make<Tuple>(
          range(int_<0>, int_<0>)
        , range(int_<0>, int_<1>)
        , range(int_<0>, int_<2>)
        , range(int_<1>, int_<1>)
        , range(int_<1>, int_<2>)
        , range(int_<1>, int_<3>)
        , range(int_<50>, int_<60>)

        , range(int_<50>, long_<60>)
        , range(long_<50>, int_<60>)
    );

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                range(int_<0>, int_<0>),
                range(int_<0>, int_<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                range(int_<0>, int_<0>),
                range(int_<0>, int_<1>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                range(int_<0>, int_<1>),
                range(int_<0>, int_<0>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                range(int_<0>, int_<1>),
                range(int_<0>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                range(int_<0>, int_<2>),
                range(int_<0>, int_<1>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                range(int_<0>, int_<2>),
                range(int_<0>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                range(int_<0>, int_<0>),
                range(int_<2>, int_<2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                range(int_<2>, int_<4>),
                range(int_<2>, int_<4>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                range(int_<-4>, int_<-3>),
                range(int_<-4>, int_<-3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                range(int_<-4>, int_<2>),
                range(int_<-4>, int_<2>)
            ));
        }

        // operators
        static_assert(has_operator<Range, decltype(equal)>{}, "");
        static_assert(has_operator<Range, decltype(not_equal)>{}, "");

        // laws
        test::TestComparable<Range>{ranges};
    }
}

/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/searchable.hpp>
#include <test/cnumeric.hpp>
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
        , range(int_<50>, long_<60>)
        , range(long_<50>, int_<60>)
    );

    auto integers = tuple_c<int, 0, 1, 900>;

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // find
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(range(int_<0>, int_<0>), test::cnumeric<int, 0>),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(range(int_<0>, int_<1>), test::cnumeric<int, 0>),
                just(int_<0>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(range(int_<0>, int_<10>), test::cnumeric<int, 3>),
                just(int_<3>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(range(int_<0>, int_<10>), test::cnumeric<int, 9>),
                just(int_<9>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(range(int_<-10>, int_<10>), test::cnumeric<int, -10>),
                just(int_<-10>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(range(int_<-10>, int_<10>), test::cnumeric<int, -5>),
                just(int_<-5>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(range(int_<-10>, int_<0>), test::cnumeric<int, 3>),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(range(int_<0>, int_<10>), test::cnumeric<int, 15>),
                nothing
            ));
        }

        // elem
        {
            BOOST_HANA_CONSTANT_CHECK(
                not_(elem(range(int_<0>, int_<0>), test::cnumeric<int, 0>))
            );

            BOOST_HANA_CONSTANT_CHECK(
                elem(range(int_<0>, int_<1>), test::cnumeric<int, 0>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                elem(range(int_<0>, int_<10>), test::cnumeric<int, 3>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                elem(range(int_<0>, int_<10>), test::cnumeric<int, 9>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                elem(range(int_<-10>, int_<10>), test::cnumeric<int, -10>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                elem(range(int_<-10>, int_<10>), test::cnumeric<int, -5>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                not_(elem(range(int_<-10>, int_<0>), test::cnumeric<int, 3>))
            );

            BOOST_HANA_CONSTANT_CHECK(
                not_(elem(range(int_<0>, int_<10>), test::cnumeric<int, 15>))
            );
        }

        // laws
        test::TestSearchable<Range>{ranges, integers};
    }
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/comparable.hpp>
#include <test/auto/foldable.hpp>
#include <test/auto/iterable.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto objects<Range> = tuple(
          range(int_<0>, int_<0>)
        , range(int_<0>, int_<1>)
        , range(int_<0>, int_<2>)
        , range(int_<1>, int_<1>)
        , range(int_<1>, int_<2>)
        , range(int_<1>, int_<3>)
        , range(int_<50>, int_<60>)
        , range(long_<0>, long_<1>)
        , range(long_<0>, int_<1>)
    );

    template <>
    auto instances<Range> = tuple(
        type<Iterable>,
        type<Foldable>,
        type<Comparable>
    );
}}}


int main() {
    test::check_datatype<Range>();

    // range_c
    {
        using T = int;
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 0, 0>,
            range(integral<T, 0>, integral<T, 0>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 0, 1>,
            range(integral<T, 0>, integral<T, 1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 0, 2>,
            range(integral<T, 0>, integral<T, 2>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 1, 1>,
            range(integral<T, 1>, integral<T, 1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 1, 2>,
            range(integral<T, 1>, integral<T, 2>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 1, 3>,
            range(integral<T, 1>, integral<T, 3>)
        ));
    }

    // Comparable
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
    }

    // Foldable
    {
        auto f = test::injection([]{});

        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(range(int_<0>, int_<0>), f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(range(int_<0>, int_<1>), f),
                f(int_<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(range(int_<0>, int_<2>), f),
                f(int_<0>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(range(int_<0>, int_<3>), f),
                f(int_<0>, int_<1>, int_<2>)
            ));
        }

        // length
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<0>, int_<0>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<0>, int_<1>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<0>, int_<2>)), int_<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<4>, int_<4>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<4>, int_<10>)), int_<6>
            ));
        }

        // minimum
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                minimum(range(int_<3>, int_<4>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                minimum(range(int_<3>, int_<5>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                minimum(range(int_<-1>, int_<5>)), int_<-1>
            ));
        }

        // maximum
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                maximum(range(int_<3>, int_<4>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                maximum(range(int_<3>, int_<5>)), int_<4>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                maximum(range(int_<-1>, int_<6>)), int_<5>
            ));
        }

        // sum
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<-3>, int_<-3>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<-3>, int_<-2>)), int_<-3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<-3>, int_<-1>)), int_<-3 + -2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<-3>, int_<0>)), int_<-3 + -2 + -1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<-3>, int_<1>)), int_<-3 + -2 + -1 + 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<-3>, int_<2>)), int_<-3 + -2 + -1 + 0 + 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<-3>, int_<3>)), int_<-3 + -2 + -1 + 0 + 1 + 2>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<0>, int_<0>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<0>, int_<1>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<0>, int_<2>)), int_<0 + 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<0>, int_<3>)), int_<0 + 1 + 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<0>, int_<4>)), int_<0 + 1 + 2 + 3>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<3>, int_<3>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<3>, int_<4>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<3>, int_<5>)), int_<3 + 4>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<3>, int_<6>)), int_<3 + 4 + 5>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum(range(int_<3>, int_<7>)), int_<3 + 4 + 5 + 6>
            ));
        }

        // product
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<-3>, int_<-3>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<-3>, int_<-2>)), int_<-3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<-3>, int_<-1>)), int_<-3 * -2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<-3>, int_<0>)), int_<-3 * -2 * -1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<-3>, int_<1>)), int_<-3 * -2 * -1 * 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<-3>, int_<2>)), int_<-3 * -2 * -1 * 0 * 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<-3>, int_<3>)), int_<-3 * -2 * -1 * 0 * 1 * 2>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<1>, int_<1>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<1>, int_<2>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<1>, int_<3>)), int_<1 * 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<1>, int_<4>)), int_<1 * 2 * 3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<1>, int_<5>)), int_<1 * 2 * 3 * 4>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<3>, int_<3>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<3>, int_<4>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<3>, int_<5>)), int_<3 * 4>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<3>, int_<6>)), int_<3 * 4 * 5>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product(range(int_<3>, int_<7>)), int_<3 * 4 * 5 * 6>
            ));
        }
    }

    // Iterable
    {
        // is_empty
        {
            BOOST_HANA_CONSTANT_CHECK(is_empty(range(int_<0>, int_<0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(range(int_<0>, int_<1>))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(range(int_<0>, int_<2>))));
        }

        // head
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(range(int_<0>, int_<1>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(range(int_<0>, int_<2>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(range(int_<2>, int_<5>)), int_<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(range(int_<5>, int_<6>)), int_<5>
            ));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(range(int_<0>, int_<1>)),
                range(int_<1>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(range(int_<0>, int_<2>)),
                range(int_<1>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(range(int_<0>, int_<3>)),
                range(int_<1>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(range(int_<4>, int_<7>)),
                range(int_<5>, int_<7>)
            ));
        }

        // at
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<0>, range(int_<0>, int_<1>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<0>, range(int_<0>, int_<2>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<1>, range(int_<0>, int_<2>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<46>, range(int_<4>, int_<90>)), int_<50>
            ));
        }

        // last
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<0>, int_<1>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<0>, int_<2>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<0>, int_<3>)), int_<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<3>, int_<6>)), int_<5>
            ));
        }

        // drop
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<0>, range(int_<0>, int_<0>)),
                range(int_<0>, int_<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<1>, range(int_<0>, int_<0>)),
                range(int_<0>, int_<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<2>, range(int_<0>, int_<0>)),
                range(int_<0>, int_<0>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<0>, range(int_<0>, int_<1>)),
                range(int_<0>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<1>, range(int_<0>, int_<1>)),
                range(int_<1>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<2>, range(int_<0>, int_<1>)),
                range(int_<1>, int_<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<0>, range(int_<0>, int_<2>)),
                range(int_<0>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<1>, range(int_<0>, int_<2>)),
                range(int_<1>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<2>, range(int_<0>, int_<2>)),
                range(int_<2>, int_<2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<0>, range(int_<0>, int_<3>)),
                range(int_<0>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<1>, range(int_<0>, int_<3>)),
                range(int_<1>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<2>, range(int_<0>, int_<3>)),
                range(int_<2>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<3>, range(int_<0>, int_<3>)),
                range(int_<3>, int_<3>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(int_<10>, range(int_<20>, int_<50>)),
                range(int_<30>, int_<50>)
            ));
        }

        // operators
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                range(int_<0>, int_<10>)[int_<3>],
                int_<3>
            ));
        }
    }
}

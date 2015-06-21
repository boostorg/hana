/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/iterable.hpp>
#include <laws/searchable.hpp>
#include <test/cnumeric.hpp>

#include <type_traits>
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

    auto integers = tuple_c<int, 0, 1, 900>; (void)integers;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // range_c
    //////////////////////////////////////////////////////////////////////////
    {
        using T = int;
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 0, 0>,
            range(integral_constant<T, 0>, integral_constant<T, 0>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 0, 1>,
            range(integral_constant<T, 0>, integral_constant<T, 1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 0, 2>,
            range(integral_constant<T, 0>, integral_constant<T, 2>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 1, 1>,
            range(integral_constant<T, 1>, integral_constant<T, 1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 1, 2>,
            range(integral_constant<T, 1>, integral_constant<T, 2>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 1, 3>,
            range(integral_constant<T, 1>, integral_constant<T, 3>)
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // make<Range>
    //////////////////////////////////////////////////////////////////////////
    {
        // make sure make<Range> works with arbitrary Constants
        BOOST_HANA_CONSTANT_CHECK(equal(
            make<Range>(test::_constant<1>{}, test::_constant<4>{}),
            range(integral_constant<int, 1>, integral_constant<int, 4>)
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(range(int_<0>, int_<0>) == range(int_<0>, int_<0>));
            BOOST_HANA_CONSTANT_CHECK(range(int_<0>, int_<4>) != range(int_<0>, int_<0>));

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

        // laws
        test::TestComparable<Range>{ranges};
    }
#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};

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

            // Previously, we would only unpack with `std::size_t`s. Make
            // sure this does not happen.
            unpack(range(int_<0>, int_<1>), [](auto x) {
                using T = datatype_t<decltype(x)>;
                static_assert(std::is_same<typename T::value_type, int>{}, "");
            });
        }

        // length
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<0>, int_<0>)), size_t<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<0>, int_<1>)), size_t<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<0>, int_<2>)), size_t<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<4>, int_<4>)), size_t<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<4>, int_<10>)), size_t<6>
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
                sum<>(range(int_<-3>, int_<-3>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<-2>)), int_<-3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<-1>)), int_<-3 + -2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<0>)), int_<-3 + -2 + -1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<1>)), int_<-3 + -2 + -1 + 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<2>)), int_<-3 + -2 + -1 + 0 + 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<3>)), int_<-3 + -2 + -1 + 0 + 1 + 2>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<0>, int_<0>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<0>, int_<1>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<0>, int_<2>)), int_<0 + 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<0>, int_<3>)), int_<0 + 1 + 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<0>, int_<4>)), int_<0 + 1 + 2 + 3>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<3>, int_<3>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<3>, int_<4>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<3>, int_<5>)), int_<3 + 4>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<3>, int_<6>)), int_<3 + 4 + 5>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<3>, int_<7>)), int_<3 + 4 + 5 + 6>
            ));
        }

        // product
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<-3>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<-2>)), int_<-3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<-1>)), int_<-3 * -2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<0>)), int_<-3 * -2 * -1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<1>)), int_<-3 * -2 * -1 * 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<2>)), int_<-3 * -2 * -1 * 0 * 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<3>)), int_<-3 * -2 * -1 * 0 * 1 * 2>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<1>, int_<1>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<1>, int_<2>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<1>, int_<3>)), int_<1 * 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<1>, int_<4>)), int_<1 * 2 * 3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<1>, int_<5>)), int_<1 * 2 * 3 * 4>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<3>, int_<3>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<3>, int_<4>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<3>, int_<5>)), int_<3 * 4>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<3>, int_<6>)), int_<3 * 4 * 5>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<3>, int_<7>)), int_<3 * 4 * 5 * 6>
            ));
        }

        // laws
        test::TestFoldable<Range>{ranges};
    }
#elif BOOST_HANA_TEST_PART == 3

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
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

            BOOST_HANA_CONSTANT_CHECK(equal(
                head(range(int_<5>, long_<6>)), long_<5>
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
            BOOST_HANA_CONSTANT_CHECK(
                range(int_<0>, int_<10>)[int_<2>] == int_<2>
            );

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(range(int_<0>, int_<1>), int_<0>),
                int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(range(int_<0>, int_<2>), int_<0>),
                int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(range(int_<0>, int_<2>), int_<1>),
                int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(range(int_<4>, int_<90>), int_<46>),
                int_<50>
            ));
        }

        // last
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<0>, int_<1>)),
                int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<0>, int_<2>)),
                int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<0>, int_<3>)),
                int_<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(range(int_<3>, int_<6>)),
                int_<5>
            ));
        }

        // drop
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<0>), int_<0>),
                range(int_<0>, int_<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<0>), int_<1>),
                range(int_<0>, int_<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<0>), int_<2>),
                range(int_<0>, int_<0>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<1>), int_<0>),
                range(int_<0>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<1>), int_<1>),
                range(int_<1>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<1>), int_<2>),
                range(int_<1>, int_<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<2>), int_<0>),
                range(int_<0>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<2>), int_<1>),
                range(int_<1>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<2>), int_<2>),
                range(int_<2>, int_<2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<3>), int_<0>),
                range(int_<0>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<3>), int_<1>),
                range(int_<1>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<3>), int_<2>),
                range(int_<2>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<0>, int_<3>), int_<3>),
                range(int_<3>, int_<3>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(range(int_<20>, int_<50>), int_<10>),
                range(int_<30>, int_<50>)
            ));
        }

        // drop_exactly
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_exactly(range(int_<0>, int_<0>), int_<0>),
                range(int_<0>, int_<0>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_exactly(range(int_<0>, int_<1>), int_<0>),
                range(int_<0>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_exactly(range(int_<0>, int_<1>), int_<1>),
                range(int_<1>, int_<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_exactly(range(int_<0>, int_<2>), int_<0>),
                range(int_<0>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_exactly(range(int_<0>, int_<2>), int_<1>),
                range(int_<1>, int_<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_exactly(range(int_<0>, int_<2>), int_<2>),
                range(int_<2>, int_<2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_exactly(range(int_<0>, int_<3>), int_<0>),
                range(int_<0>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_exactly(range(int_<0>, int_<3>), int_<1>),
                range(int_<1>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_exactly(range(int_<0>, int_<3>), int_<2>),
                range(int_<2>, int_<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_exactly(range(int_<0>, int_<3>), int_<3>),
                range(int_<3>, int_<3>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_exactly(range(int_<20>, int_<50>), int_<10>),
                range(int_<30>, int_<50>)
            ));
        }

        // laws
        test::TestIterable<Range>{ranges};
    }
#elif BOOST_HANA_TEST_PART == 4
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

        // contains
        {
            BOOST_HANA_CONSTANT_CHECK(
                not_(contains(range(int_<0>, int_<0>), test::cnumeric<int, 0>))
            );

            BOOST_HANA_CONSTANT_CHECK(
                contains(range(int_<0>, int_<1>), test::cnumeric<int, 0>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                contains(range(int_<0>, int_<10>), test::cnumeric<int, 3>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                contains(range(int_<0>, int_<10>), test::cnumeric<int, 9>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                contains(range(int_<-10>, int_<10>), test::cnumeric<int, -10>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                contains(range(int_<-10>, int_<10>), test::cnumeric<int, -5>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                not_(contains(range(int_<-10>, int_<0>), test::cnumeric<int, 3>))
            );

            BOOST_HANA_CONSTANT_CHECK(
                not_(contains(range(int_<0>, int_<10>), test::cnumeric<int, 15>))
            );
        }

        // laws
        test::TestSearchable<Range>{ranges, integers};
    }
#endif
}

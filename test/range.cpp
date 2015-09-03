/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/range.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/tag_of.hpp>
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
namespace hana = boost::hana;
namespace test = hana::test;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto ranges = hana::make_tuple(
          hana::make_range(hana::int_c<0>, hana::int_c<0>)
        , hana::make_range(hana::int_c<0>, hana::int_c<1>)
        , hana::make_range(hana::int_c<0>, hana::int_c<2>)
        , hana::make_range(hana::int_c<1>, hana::int_c<1>)
        , hana::make_range(hana::int_c<1>, hana::int_c<2>)
        , hana::make_range(hana::int_c<1>, hana::int_c<3>)
        , hana::make_range(hana::int_c<50>, hana::int_c<60>)

        , hana::make_range(hana::int_c<50>, hana::long_c<60>)
        , hana::make_range(hana::long_c<50>, hana::int_c<60>)
    );

    auto integers = hana::tuple_c<int, 0, 1, 900>; (void)integers;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // range_c
    //////////////////////////////////////////////////////////////////////////
    {
        using T = int;
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::range_c<T, 0, 0>,
            hana::make_range(hana::integral_c<T, 0>, hana::integral_c<T, 0>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::range_c<T, 0, 1>,
            hana::make_range(hana::integral_c<T, 0>, hana::integral_c<T, 1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::range_c<T, 0, 2>,
            hana::make_range(hana::integral_c<T, 0>, hana::integral_c<T, 2>)
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::range_c<T, 1, 1>,
            hana::make_range(hana::integral_c<T, 1>, hana::integral_c<T, 1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::range_c<T, 1, 2>,
            hana::make_range(hana::integral_c<T, 1>, hana::integral_c<T, 2>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::range_c<T, 1, 3>,
            hana::make_range(hana::integral_c<T, 1>, hana::integral_c<T, 3>)
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // make<range_tag>
    //////////////////////////////////////////////////////////////////////////
    {
        // make sure make<range_tag> works with arbitrary Constants
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::make<hana::range_tag>(test::_constant<1>{}, test::_constant<4>{}),
            hana::make_range(hana::integral_c<int, 1>, hana::integral_c<int, 4>)
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(
                hana::make_range(hana::int_c<0>, hana::int_c<0>) ==
                hana::make_range(hana::int_c<0>, hana::int_c<0>)
            );
            BOOST_HANA_CONSTANT_CHECK(
                hana::make_range(hana::int_c<0>, hana::int_c<4>) !=
                hana::make_range(hana::int_c<0>, hana::int_c<0>)
            );

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::make_range(hana::int_c<0>, hana::int_c<0>),
                hana::make_range(hana::int_c<0>, hana::int_c<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
                hana::make_range(hana::int_c<0>, hana::int_c<0>),
                hana::make_range(hana::int_c<0>, hana::int_c<1>)
            )));
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
                hana::make_range(hana::int_c<0>, hana::int_c<1>),
                hana::make_range(hana::int_c<0>, hana::int_c<0>)
            )));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::make_range(hana::int_c<0>, hana::int_c<1>),
                hana::make_range(hana::int_c<0>, hana::int_c<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
                hana::make_range(hana::int_c<0>, hana::int_c<2>),
                hana::make_range(hana::int_c<0>, hana::int_c<1>)
            )));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::make_range(hana::int_c<0>, hana::int_c<2>),
                hana::make_range(hana::int_c<0>, hana::int_c<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::make_range(hana::int_c<0>, hana::int_c<0>),
                hana::make_range(hana::int_c<2>, hana::int_c<2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::make_range(hana::int_c<2>, hana::int_c<4>),
                hana::make_range(hana::int_c<2>, hana::int_c<4>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::make_range(hana::int_c<-4>, hana::int_c<-3>),
                hana::make_range(hana::int_c<-4>, hana::int_c<-3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::make_range(hana::int_c<-4>, hana::int_c<2>),
                hana::make_range(hana::int_c<-4>, hana::int_c<2>)
            ));
        }

        // laws
        test::TestComparable<hana::range_tag>{ranges};
    }
#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};

        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::unpack(hana::make_range(hana::int_c<0>, hana::int_c<0>), f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::unpack(hana::make_range(hana::int_c<0>, hana::int_c<1>), f),
                f(hana::int_c<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::unpack(hana::make_range(hana::int_c<0>, hana::int_c<2>), f),
                f(hana::int_c<0>, hana::int_c<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::unpack(hana::make_range(hana::int_c<0>, hana::int_c<3>), f),
                f(hana::int_c<0>, hana::int_c<1>, hana::int_c<2>)
            ));

            // Previously, we would only unpack with `std::size_t`s. Make
            // sure this does not happen.
            hana::unpack(hana::make_range(hana::int_c<0>, hana::int_c<1>), [](auto x) {
                using T = hana::tag_of_t<decltype(x)>;
                static_assert(std::is_same<typename T::value_type, int>{}, "");
            });
        }

        // length
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::length(hana::make_range(hana::int_c<0>, hana::int_c<0>)), hana::size_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::length(hana::make_range(hana::int_c<0>, hana::int_c<1>)), hana::size_c<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::length(hana::make_range(hana::int_c<0>, hana::int_c<2>)), hana::size_c<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::length(hana::make_range(hana::int_c<4>, hana::int_c<4>)), hana::size_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::length(hana::make_range(hana::int_c<4>, hana::int_c<10>)), hana::size_c<6>
            ));
        }

        // minimum
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::minimum(hana::make_range(hana::int_c<3>, hana::int_c<4>)), hana::int_c<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::minimum(hana::make_range(hana::int_c<3>, hana::int_c<5>)), hana::int_c<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::minimum(hana::make_range(hana::int_c<-1>, hana::int_c<5>)), hana::int_c<-1>
            ));
        }

        // maximum
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::maximum(hana::make_range(hana::int_c<3>, hana::int_c<4>)), hana::int_c<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::maximum(hana::make_range(hana::int_c<3>, hana::int_c<5>)), hana::int_c<4>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::maximum(hana::make_range(hana::int_c<-1>, hana::int_c<6>)), hana::int_c<5>
            ));
        }

        // sum
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<-3>, hana::int_c<-3>)), hana::int_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<-3>, hana::int_c<-2>)), hana::int_c<-3>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<-3>, hana::int_c<-1>)), hana::int_c<-3 + -2>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<-3>, hana::int_c<0>)), hana::int_c<-3 + -2 + -1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<-3>, hana::int_c<1>)), hana::int_c<-3 + -2 + -1 + 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<-3>, hana::int_c<2>)), hana::int_c<-3 + -2 + -1 + 0 + 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<-3>, hana::int_c<3>)), hana::int_c<-3 + -2 + -1 + 0 + 1 + 2>
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<0>, hana::int_c<0>)), hana::int_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<0>, hana::int_c<1>)), hana::int_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<0>, hana::int_c<2>)), hana::int_c<0 + 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<0>, hana::int_c<3>)), hana::int_c<0 + 1 + 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<0>, hana::int_c<4>)), hana::int_c<0 + 1 + 2 + 3>
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<3>, hana::int_c<3>)), hana::int_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<3>, hana::int_c<4>)), hana::int_c<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<3>, hana::int_c<5>)), hana::int_c<3 + 4>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<3>, hana::int_c<6>)), hana::int_c<3 + 4 + 5>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::sum<>(hana::make_range(hana::int_c<3>, hana::int_c<7>)), hana::int_c<3 + 4 + 5 + 6>
            ));
        }

        // product
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<-3>, hana::int_c<-3>)), hana::int_c<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<-3>, hana::int_c<-2>)), hana::int_c<-3>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<-3>, hana::int_c<-1>)), hana::int_c<-3 * -2>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<-3>, hana::int_c<0>)), hana::int_c<-3 * -2 * -1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<-3>, hana::int_c<1>)), hana::int_c<-3 * -2 * -1 * 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<-3>, hana::int_c<2>)), hana::int_c<-3 * -2 * -1 * 0 * 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<-3>, hana::int_c<3>)), hana::int_c<-3 * -2 * -1 * 0 * 1 * 2>
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<1>, hana::int_c<1>)), hana::int_c<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<1>, hana::int_c<2>)), hana::int_c<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<1>, hana::int_c<3>)), hana::int_c<1 * 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<1>, hana::int_c<4>)), hana::int_c<1 * 2 * 3>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<1>, hana::int_c<5>)), hana::int_c<1 * 2 * 3 * 4>
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<3>, hana::int_c<3>)), hana::int_c<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<3>, hana::int_c<4>)), hana::int_c<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<3>, hana::int_c<5>)), hana::int_c<3 * 4>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<3>, hana::int_c<6>)), hana::int_c<3 * 4 * 5>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::product<>(hana::make_range(hana::int_c<3>, hana::int_c<7>)), hana::int_c<3 * 4 * 5 * 6>
            ));
        }

        // laws
        test::TestFoldable<hana::range_tag>{ranges};
    }
#elif BOOST_HANA_TEST_PART == 3

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        // is_empty
        {
            BOOST_HANA_CONSTANT_CHECK(hana::is_empty(hana::make_range(hana::int_c<0>, hana::int_c<0>)));
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(hana::make_range(hana::int_c<0>, hana::int_c<1>))));
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(hana::make_range(hana::int_c<0>, hana::int_c<2>))));
        }

        // front
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::front(hana::make_range(hana::int_c<0>, hana::int_c<1>)), hana::int_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::front(hana::make_range(hana::int_c<0>, hana::int_c<2>)), hana::int_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::front(hana::make_range(hana::int_c<2>, hana::int_c<5>)), hana::int_c<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::front(hana::make_range(hana::int_c<5>, hana::int_c<6>)), hana::int_c<5>
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::front(hana::make_range(hana::int_c<5>, hana::long_c<6>)), hana::long_c<5>
            ));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::tail(hana::make_range(hana::int_c<0>, hana::int_c<1>)),
                hana::make_range(hana::int_c<1>, hana::int_c<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::tail(hana::make_range(hana::int_c<0>, hana::int_c<2>)),
                hana::make_range(hana::int_c<1>, hana::int_c<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::tail(hana::make_range(hana::int_c<0>, hana::int_c<3>)),
                hana::make_range(hana::int_c<1>, hana::int_c<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::tail(hana::make_range(hana::int_c<4>, hana::int_c<7>)),
                hana::make_range(hana::int_c<5>, hana::int_c<7>)
            ));
        }

        // at
        {
            BOOST_HANA_CONSTANT_CHECK(
                hana::make_range(hana::int_c<0>, hana::int_c<10>)[hana::int_c<2>] == hana::int_c<2>
            );

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::at(hana::make_range(hana::int_c<0>, hana::int_c<1>), hana::int_c<0>),
                hana::int_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::at(hana::make_range(hana::int_c<0>, hana::int_c<2>), hana::int_c<0>),
                hana::int_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::at(hana::make_range(hana::int_c<0>, hana::int_c<2>), hana::int_c<1>),
                hana::int_c<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::at(hana::make_range(hana::int_c<4>, hana::int_c<90>), hana::int_c<46>),
                hana::int_c<50>
            ));
        }

        // back
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::back(hana::make_range(hana::int_c<0>, hana::int_c<1>)),
                hana::int_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::back(hana::make_range(hana::int_c<0>, hana::int_c<2>)),
                hana::int_c<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::back(hana::make_range(hana::int_c<0>, hana::int_c<3>)),
                hana::int_c<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::back(hana::make_range(hana::int_c<3>, hana::int_c<6>)),
                hana::int_c<5>
            ));
        }

        // drop_front
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<0>), hana::int_c<0>),
                hana::make_range(hana::int_c<0>, hana::int_c<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<0>), hana::int_c<1>),
                hana::make_range(hana::int_c<0>, hana::int_c<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<0>), hana::int_c<2>),
                hana::make_range(hana::int_c<0>, hana::int_c<0>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<1>), hana::int_c<0>),
                hana::make_range(hana::int_c<0>, hana::int_c<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<1>), hana::int_c<1>),
                hana::make_range(hana::int_c<1>, hana::int_c<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<1>), hana::int_c<2>),
                hana::make_range(hana::int_c<1>, hana::int_c<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<2>), hana::int_c<0>),
                hana::make_range(hana::int_c<0>, hana::int_c<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<2>), hana::int_c<1>),
                hana::make_range(hana::int_c<1>, hana::int_c<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<2>), hana::int_c<2>),
                hana::make_range(hana::int_c<2>, hana::int_c<2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<3>), hana::int_c<0>),
                hana::make_range(hana::int_c<0>, hana::int_c<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<3>), hana::int_c<1>),
                hana::make_range(hana::int_c<1>, hana::int_c<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<3>), hana::int_c<2>),
                hana::make_range(hana::int_c<2>, hana::int_c<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<0>, hana::int_c<3>), hana::int_c<3>),
                hana::make_range(hana::int_c<3>, hana::int_c<3>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front(hana::make_range(hana::int_c<20>, hana::int_c<50>), hana::int_c<10>),
                hana::make_range(hana::int_c<30>, hana::int_c<50>)
            ));
        }

        // drop_front_exactly
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front_exactly(hana::make_range(hana::int_c<0>, hana::int_c<0>), hana::int_c<0>),
                hana::make_range(hana::int_c<0>, hana::int_c<0>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front_exactly(hana::make_range(hana::int_c<0>, hana::int_c<1>), hana::int_c<0>),
                hana::make_range(hana::int_c<0>, hana::int_c<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front_exactly(hana::make_range(hana::int_c<0>, hana::int_c<1>), hana::int_c<1>),
                hana::make_range(hana::int_c<1>, hana::int_c<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front_exactly(hana::make_range(hana::int_c<0>, hana::int_c<2>), hana::int_c<0>),
                hana::make_range(hana::int_c<0>, hana::int_c<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front_exactly(hana::make_range(hana::int_c<0>, hana::int_c<2>), hana::int_c<1>),
                hana::make_range(hana::int_c<1>, hana::int_c<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front_exactly(hana::make_range(hana::int_c<0>, hana::int_c<2>), hana::int_c<2>),
                hana::make_range(hana::int_c<2>, hana::int_c<2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front_exactly(hana::make_range(hana::int_c<0>, hana::int_c<3>), hana::int_c<0>),
                hana::make_range(hana::int_c<0>, hana::int_c<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front_exactly(hana::make_range(hana::int_c<0>, hana::int_c<3>), hana::int_c<1>),
                hana::make_range(hana::int_c<1>, hana::int_c<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front_exactly(hana::make_range(hana::int_c<0>, hana::int_c<3>), hana::int_c<2>),
                hana::make_range(hana::int_c<2>, hana::int_c<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front_exactly(hana::make_range(hana::int_c<0>, hana::int_c<3>), hana::int_c<3>),
                hana::make_range(hana::int_c<3>, hana::int_c<3>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::drop_front_exactly(hana::make_range(hana::int_c<20>, hana::int_c<50>), hana::int_c<10>),
                hana::make_range(hana::int_c<30>, hana::int_c<50>)
            ));
        }

        // laws
        test::TestIterable<hana::range_tag>{ranges};
    }
#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // find
        {
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::find(hana::make_range(hana::int_c<0>, hana::int_c<0>), test::cnumeric<int, 0>),
                hana::nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::find(hana::make_range(hana::int_c<0>, hana::int_c<1>), test::cnumeric<int, 0>),
                hana::just(hana::int_c<0>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::find(hana::make_range(hana::int_c<0>, hana::int_c<10>), test::cnumeric<int, 3>),
                hana::just(hana::int_c<3>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::find(hana::make_range(hana::int_c<0>, hana::int_c<10>), test::cnumeric<int, 9>),
                hana::just(hana::int_c<9>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::find(hana::make_range(hana::int_c<-10>, hana::int_c<10>), test::cnumeric<int, -10>),
                hana::just(hana::int_c<-10>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::find(hana::make_range(hana::int_c<-10>, hana::int_c<10>), test::cnumeric<int, -5>),
                hana::just(hana::int_c<-5>)
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::find(hana::make_range(hana::int_c<-10>, hana::int_c<0>), test::cnumeric<int, 3>),
                hana::nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::find(hana::make_range(hana::int_c<0>, hana::int_c<10>), test::cnumeric<int, 15>),
                hana::nothing
            ));
        }

        // contains
        {
            BOOST_HANA_CONSTANT_CHECK(
                hana::not_(hana::contains(hana::make_range(hana::int_c<0>, hana::int_c<0>), test::cnumeric<int, 0>))
            );

            BOOST_HANA_CONSTANT_CHECK(
                hana::contains(hana::make_range(hana::int_c<0>, hana::int_c<1>), test::cnumeric<int, 0>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                hana::contains(hana::make_range(hana::int_c<0>, hana::int_c<10>), test::cnumeric<int, 3>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                hana::contains(hana::make_range(hana::int_c<0>, hana::int_c<10>), test::cnumeric<int, 9>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                hana::contains(hana::make_range(hana::int_c<-10>, hana::int_c<10>), test::cnumeric<int, -10>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                hana::contains(hana::make_range(hana::int_c<-10>, hana::int_c<10>), test::cnumeric<int, -5>)
            );

            BOOST_HANA_CONSTANT_CHECK(
                hana::not_(hana::contains(hana::make_range(hana::int_c<-10>, hana::int_c<0>), test::cnumeric<int, 3>))
            );

            BOOST_HANA_CONSTANT_CHECK(
                hana::not_(hana::contains(hana::make_range(hana::int_c<0>, hana::int_c<10>), test::cnumeric<int, 15>))
            );
        }

        // laws
        test::TestSearchable<hana::range_tag>{ranges, integers};
    }
#endif
}

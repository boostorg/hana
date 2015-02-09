/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/builtin.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>
#include <test/numeric.hpp>

// instances
#include <test/auto/comparable.hpp>
#include <test/auto/foldable.hpp>
#include <test/auto/orderable.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <int i>
    auto instances<builtin<i>> = tuple(
        type<Comparable>,
        type<Orderable>
    );

    template <int i>
    auto objects<builtin<i>> = tuple(
        builtin<i>{0},
        builtin<i>{1},
        builtin<i>{2},
        builtin<i>{3},
        builtin<i>{4},
        builtin<i>{5}
    );
}}}


int main() {
    test::check_datatype<test::builtin<1>>();
    using integer = test::builtin<1>;
    using integer2 = test::builtin<2>;

    // Comparable
    {
        // equal
        {
            // same type
            BOOST_HANA_CONSTEXPR_CHECK(equal(integer{0}, integer{0}));
            BOOST_HANA_CONSTEXPR_CHECK(not_(equal(integer{0}, integer{1})));
            BOOST_HANA_CONSTANT_CHECK(are<Comparable>(integer{0}, integer{0}));

            // mixed types
            BOOST_HANA_CONSTEXPR_CHECK(equal(integer{0}, integer2{0}));
            BOOST_HANA_CONSTEXPR_CHECK(not_(equal(integer{0}, integer2{1})));
            BOOST_HANA_CONSTANT_CHECK(are<Comparable>(integer{0}, integer2{0}));
        }
    }

    // Orderable
    {
        // less
        {
            // same type
            BOOST_HANA_CONSTEXPR_CHECK(less(integer{0}, integer{1}));
            BOOST_HANA_CONSTEXPR_CHECK(not_(less(integer{0}, integer{0})));
            BOOST_HANA_CONSTEXPR_CHECK(not_(less(integer{1}, integer{0})));
            BOOST_HANA_CONSTANT_CHECK(are<Orderable, integer, integer>);

            // mixed types
            BOOST_HANA_CONSTEXPR_CHECK(less(integer{0}, integer2{1}));
            BOOST_HANA_CONSTEXPR_CHECK(not_(less(integer{0}, integer2{0})));
            BOOST_HANA_CONSTEXPR_CHECK(not_(less(integer{1}, integer2{0})));
            BOOST_HANA_CONSTANT_CHECK(are<Orderable, integer, integer2>);
        }
    }

    // Foldable
    {
        int a[] = {1};
        int b[] = {1, 2};
        int c[] = {1, 2, 3};
        int d[] = {1, 2, 3, 4};

        // unpack
        {
            BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});


            BOOST_HANA_CONSTEXPR_CHECK(equal(
                unpack(a, f),
                f(1)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                unpack(b, f),
                f(1, 2)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                unpack(c, f),
                f(1, 2, 3)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                unpack(d, f),
                f(1, 2, 3, 4)
            ));
        }
    }
}

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
#include <test/auto/enumerable.hpp>
#include <test/auto/foldable.hpp>
#include <test/auto/group.hpp>
#include <test/auto/integral_domain.hpp>
#include <test/auto/logical.hpp>
#include <test/auto/monoid.hpp>
#include <test/auto/orderable.hpp>
#include <test/auto/ring.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <int i>
    auto instances<builtin<i>> = tuple(
        type<Comparable>,
        type<Orderable>,
        type<Enumerable>,
        type<Ring>,
        type<Monoid>,
        type<Group>,
        type<IntegralDomain>,
        type<Logical>
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

    // Enumerable
    {
        // succ
        {
            BOOST_HANA_CONSTEXPR_ASSERT(succ(integer{0}) == integer{1});
            BOOST_HANA_CONSTEXPR_ASSERT(succ(integer{1}) == integer{2});
            BOOST_HANA_CONSTEXPR_ASSERT(succ(integer{2}) == integer{3});
        }

        // pred
        {
            BOOST_HANA_CONSTEXPR_ASSERT(pred(integer{3}) == integer{2});
            BOOST_HANA_CONSTEXPR_ASSERT(pred(integer{2}) == integer{1});
            BOOST_HANA_CONSTEXPR_ASSERT(pred(integer{1}) == integer{0});
        }

        // make sure pred and succ works with fundamental types
        {
            BOOST_HANA_CONSTEXPR_ASSERT(pred(3) == 2);
            BOOST_HANA_CONSTEXPR_ASSERT(succ(3) == 4);
        }
    }

    // Comparable
    {
        // equal
        {
            // same type
            BOOST_HANA_CONSTEXPR_ASSERT(equal(integer{0}, integer{0}));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(integer{0}, integer{1})));
            BOOST_HANA_CONSTANT_ASSERT(are<Comparable>(integer{0}, integer{0}));

            // mixed types
            BOOST_HANA_CONSTEXPR_ASSERT(equal(integer{0}, integer2{0}));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(integer{0}, integer2{1})));
            BOOST_HANA_CONSTANT_ASSERT(are<Comparable>(integer{0}, integer2{0}));
        }
    }

    // Orderable
    {
        // less
        {
            // same type
            BOOST_HANA_CONSTEXPR_ASSERT(less(integer{0}, integer{1}));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(less(integer{0}, integer{0})));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(less(integer{1}, integer{0})));
            BOOST_HANA_CONSTANT_ASSERT(are<Orderable, integer, integer>);

            // mixed types
            BOOST_HANA_CONSTEXPR_ASSERT(less(integer{0}, integer2{1}));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(less(integer{0}, integer2{0})));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(less(integer{1}, integer2{0})));
            BOOST_HANA_CONSTANT_ASSERT(are<Orderable, integer, integer2>);
        }
    }

    // Monoid
    {
        constexpr int x = 6, y = 4;

        // zero
        {
            BOOST_HANA_CONSTEXPR_ASSERT(zero<integer>.value == 0);
        }

        // plus
        {
            // same type
            BOOST_HANA_CONSTEXPR_ASSERT(plus(integer{x}, integer{y}).value == x + y);

            // mixed types
            BOOST_HANA_CONSTEXPR_ASSERT(plus(integer{x}, integer2{y}).value == x + y);
            BOOST_HANA_CONSTEXPR_ASSERT(plus(integer2{x}, integer{y}).value == x + y);
        }
    }

    // Group
    {
        // negate
        {
            BOOST_HANA_CONSTEXPR_ASSERT(negate(integer{1}).value == -1);
        }

        // minus
        {
            // same type
            BOOST_HANA_CONSTEXPR_ASSERT(minus(integer{1}, integer{3}).value == 1 - 3);

            // mixed types
            BOOST_HANA_CONSTEXPR_ASSERT(minus(integer{1}, integer2{3}).value == 1 - 3);
            BOOST_HANA_CONSTEXPR_ASSERT(minus(integer2{1}, integer{3}).value == 1 - 3);
        }
    }

    // Ring
    {
        constexpr int x = 6, y = 4;

        // one
        {
            BOOST_HANA_CONSTEXPR_ASSERT(one<integer>.value == 1);
        }

        // mult
        {
            // same type
            BOOST_HANA_CONSTEXPR_ASSERT(mult(integer{x}, integer{y}).value == x * y);

            // mixed types
            BOOST_HANA_CONSTEXPR_ASSERT(mult(integer{x}, integer2{y}).value == x * y);
            BOOST_HANA_CONSTEXPR_ASSERT(mult(integer2{x}, integer{y}).value == x * y);
        }
    }

    // IntegralDomain
    {
        // quot
        {
            // same type
            BOOST_HANA_CONSTEXPR_ASSERT(quot(integer{6}, integer{3}).value == 6 / 3);

            // mixed types
            BOOST_HANA_CONSTEXPR_ASSERT(quot(integer{6}, integer2{3}).value == 6 / 3);
            BOOST_HANA_CONSTEXPR_ASSERT(quot(integer2{6}, integer{3}).value == 6 / 3);
        }

        // mod
        {
            // same type
            BOOST_HANA_CONSTEXPR_ASSERT(mod(integer{6}, integer{4}).value == 6 % 4);

            // mixed types
            BOOST_HANA_CONSTEXPR_ASSERT(mod(integer{6}, integer2{4}).value == 6 % 4);
            BOOST_HANA_CONSTEXPR_ASSERT(mod(integer2{6}, integer{4}).value == 6 % 4);
        }
    }

    // Logical
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto x = test::numeric(0);
        BOOST_HANA_CONSTEXPR_LAMBDA auto y = test::numeric(1);
        BOOST_HANA_CONSTEXPR_LAMBDA auto t = [=](auto) { return x; };
        BOOST_HANA_CONSTEXPR_LAMBDA auto e = [=](auto) { return y; };

        // eval_if
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                eval_if(integer{true}, t, e), x
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                eval_if(integer{false}, t, e), y
            ));
        }

        // not_
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(not_(integer{true}), false));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(not_(integer{false}), true));
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


            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                unpack(a, f),
                f(1)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                unpack(b, f),
                f(1, 2)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                unpack(c, f),
                f(1, 2, 3)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                unpack(d, f),
                f(1, 2, 3, 4)
            ));
        }
    }
}

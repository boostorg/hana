/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/numeric.hpp>

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp> //! @todo Find a way to get rid of this
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/cnumeric.hpp>
#include <test/injection.hpp>

// tested instances
#include <test/auto/comparable.hpp>
#include <test/auto/enumerable.hpp>
#include <test/auto/group.hpp>
#include <test/auto/integral_domain.hpp>
#include <test/auto/logical.hpp>
#include <test/auto/monoid.hpp>
#include <test/auto/orderable.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Numeric> = tuple(
          type<Enumerable>
        , type<Monoid>
        , type<Group>
        , type<Ring>
        , type<IntegralDomain>
        , type<Comparable>
        , type<Orderable>
        , type<Logical>
    );

    template <>
    auto objects<Numeric> = tuple(
          numeric(0)
        , numeric(1)
        , numeric(2)
        , numeric(3)
    );
}}}

namespace boost { namespace hana {
    template <>
    struct enabled_operators<test::Numeric>
        : Monoid
        , Group
        , Ring
        , IntegralDomain
        , Comparable
        , Orderable
        , Logical
    { };
}}

struct invalid {
    template <typename T>
    operator T const() { throw; }
};

int main() {
    test::check_datatype<test::Numeric>();

    // Comparable
    {
        auto f = test::injection([]{});
        auto x = test::numeric(1);
        auto y = test::numeric(2);

        // equal
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(x, x));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(x, y)));
        }

        // not_equal
        {
            BOOST_HANA_CONSTEXPR_ASSERT(not_equal(x, y));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(not_equal(x, x)));
        }

        // comparing
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                comparing(f)(x, x),
                equal(f(x), f(x))
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                comparing(f)(x, y),
                equal(f(x), f(y))
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                x == y,
                equal(x, y)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                x != y,
                not_equal(x, y)
            ));
        }
    }

    // Orderable
    {
        auto ord = test::numeric;

        // test::injection is also monotonic
        auto f = test::injection([]{});

        // less
        {
            BOOST_HANA_CONSTEXPR_ASSERT(less(ord(0), ord(1)));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(less(ord(0), ord(0))));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(less(ord(1), ord(0))));
        }

        // less_equal
        {
            BOOST_HANA_CONSTEXPR_ASSERT(less_equal(ord(0), ord(1)));
            BOOST_HANA_CONSTEXPR_ASSERT(less_equal(ord(0), ord(0)));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(less_equal(ord(1), ord(0))));
        }

        // greater_equal
        {
            BOOST_HANA_CONSTEXPR_ASSERT(greater_equal(ord(1), ord(0)));
            BOOST_HANA_CONSTEXPR_ASSERT(greater_equal(ord(0), ord(0)));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(greater_equal(ord(0), ord(1))));
        }

        // greater
        {
            BOOST_HANA_CONSTEXPR_ASSERT(greater(ord(1), ord(0)));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(greater(ord(0), ord(0))));
            BOOST_HANA_CONSTEXPR_ASSERT(not_(greater(ord(0), ord(1))));
        }

        // max
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                max(ord(0), ord(0)), ord(0)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                max(ord(1), ord(0)), ord(1)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                max(ord(0), ord(1)), ord(1)
            ));
        }

        // min
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                min(ord(0), ord(0)),
                ord(0)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                min(ord(1), ord(0)),
                ord(0)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                min(ord(0), ord(1)),
                ord(0)
            ));
        }

        // ordering
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                ordering(f)(ord(1), ord(0)),
                less(f(ord(1)), f(ord(0)))
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                ordering(f)(ord(0), ord(1)),
                less(f(ord(0)), f(ord(1)))
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                ordering(f)(ord(0), ord(0)),
                less(f(ord(0)), f(ord(0)))
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                ord(0) < ord(1),
                less(ord(0), ord(1))
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                ord(0) <= ord(1),
                less_equal(ord(0), ord(1))
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                ord(0) > ord(1),
                greater(ord(0), ord(1))
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                ord(0) >= ord(1),
                greater_equal(ord(0), ord(1))
            ));
        }
    }

    // Enumerable
    {
        // succ
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                succ(test::numeric(0)),
                test::numeric(1)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                succ(test::numeric(1)),
                test::numeric(2)
            ));
        }

        // pred
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                pred(test::numeric(1)),
                test::numeric(0)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                pred(test::numeric(2)),
                test::numeric(1)
            ));
        }
    }

    // Monoid
    {
        constexpr int x = 2, y = 3;

        // zero
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                zero<test::Numeric>, test::numeric(0)
            ));
        }

        // plus
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                plus(test::numeric(x), test::numeric(y)),
                test::numeric(x + y)
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                test::numeric(x) + test::numeric(y),
                plus(test::numeric(x), test::numeric(y))
            ));
        }
    }

    // Group
    {
        constexpr int x = 2, y = 3;

        // minus
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                minus(test::numeric(x), test::numeric(y)),
                test::numeric(x - y)
            ));
        }

        // negate
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                negate(test::numeric(x)),
                test::numeric(-x)
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                test::numeric(x) - test::numeric(y),
                minus(test::numeric(x), test::numeric(y))
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                -test::numeric(x),
                negate(test::numeric(x))
            ));
        }
    }

    // Ring
    {
        constexpr int x = 2, y = 3;

        // one
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                one<test::Numeric>,
                test::numeric(1)
            ));
        }

        // mult
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                mult(test::numeric(x), test::numeric(y)),
                test::numeric(x * y)
            ));
        }

        // power
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                power(test::numeric(x), zero<test::CNumeric>),
                one<test::Numeric>
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                power(test::numeric(x), one<test::CNumeric>),
                test::numeric(x)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                power(test::numeric(x), test::cnumeric<int, 2>),
                mult(test::numeric(x), test::numeric(x))
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                power(test::numeric(x), test::cnumeric<int, 3>),
                mult(mult(test::numeric(x), test::numeric(x)), test::numeric(x))
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                test::numeric(x) * test::numeric(y),
                mult(test::numeric(x), test::numeric(y))
            ));
        }
    }

    // IntegralDomain
    {
        constexpr int x = 6, y = 3, z = 4;

        // quot
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                quot(test::numeric(x), test::numeric(y)),
                test::numeric(x / y)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                quot(test::numeric(x), test::numeric(z)),
                 test::numeric(x/ z)
            ));
        }

        // mod
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                mod(test::numeric(x), test::numeric(y)),
                test::numeric(x % y)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                mod(test::numeric(x), test::numeric(z)),
                test::numeric(x % z)
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                test::numeric(x) % test::numeric(y),
                mod(test::numeric(x), test::numeric(y))
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                test::numeric(x) / test::numeric(y),
                quot(test::numeric(x), test::numeric(y))
            ));
        }
    }

    // Logical
    {
        auto logical = test::numeric;
        auto comparable = test::numeric;

        // not_
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                not_(logical(true)),
                logical(false)
            ));
        }

        // and_
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                and_(logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                and_(logical(false)),
                logical(false)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                and_(logical(true), logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                and_(logical(true), logical(false)),
                logical(false)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                and_(logical(false), invalid{}),
                logical(false)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                and_(logical(true), logical(true), logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                and_(logical(true), logical(true), logical(false)),
                logical(false)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                and_(logical(true), logical(false), invalid{}),
                logical(false)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                and_(logical(false), invalid{}, invalid{}),
                logical(false)
            ));
        }

        // or_
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                or_(logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                or_(logical(false)),
                logical(false)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                or_(logical(false), logical(false)),
                logical(false)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                or_(logical(false), logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                or_(logical(true), invalid{}),
                logical(true)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                or_(logical(false), logical(false), logical(false)),
                logical(false)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                or_(logical(false), logical(false), logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                or_(logical(false), logical(true), invalid{}),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                or_(logical(true), invalid{}, invalid{}),
                logical(true)
            ));
        }

        // if_
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                if_(logical(true), comparable(0), comparable(1)),
                comparable(0)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                if_(logical(false), comparable(0), comparable(1)),
                comparable(1)
            ));
        }

        // eval_if
        {
            auto t = [=](auto) { return comparable(0); };
            auto e = [=](auto) { return comparable(1); };

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                eval_if(logical(true), t, e),
                comparable(0)
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                eval_if(logical(false), t, e),
                comparable(1)
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                !logical(true),
                not_(logical(true))
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                logical(true) && logical(true),
                and_(logical(true), logical(true))
            ));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(
                logical(false) || logical(false),
                or_(logical(false), logical(false))
            ));
        }
    }
}

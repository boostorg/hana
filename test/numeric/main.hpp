/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/numeric.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/enumerable.hpp>
#include <laws/group.hpp>
#include <laws/integral_domain.hpp>
#include <laws/logical.hpp>
#include <laws/monoid.hpp>
#include <laws/orderable.hpp>
#include <test/cnumeric.hpp>

#include <vector>
using namespace boost::hana;


namespace boost { namespace hana {
    template <>
    struct operators::of<test::Numeric>
        : operators::of<
            Monoid, Group, Ring, IntegralDomain,
            Comparable, Orderable, Logical
        >
    { };
}}

struct invalid {
    template <typename T>
    operator T const() { throw; }
};

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};
        auto x = test::numeric(1);
        auto y = test::numeric(2);

        // equal
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(x, x));
            BOOST_HANA_CONSTEXPR_CHECK(not_(equal(x, y)));
        }

        // not_equal
        {
            BOOST_HANA_CONSTEXPR_CHECK(not_equal(x, y));
            BOOST_HANA_CONSTEXPR_CHECK(not_(not_equal(x, x)));
        }

        // comparing
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                comparing(f)(x, x),
                equal(f(x), f(x))
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                comparing(f)(x, y),
                equal(f(x), f(y))
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                x == y,
                equal(x, y)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                x != y,
                not_equal(x, y)
            ));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        auto ord = test::numeric;

        // test::_injection is also monotonic
        test::_injection<0> f{};

        // less
        {
            BOOST_HANA_CONSTEXPR_CHECK(less(ord(0), ord(1)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(less(ord(0), ord(0))));
            BOOST_HANA_CONSTEXPR_CHECK(not_(less(ord(1), ord(0))));
        }

        // less_equal
        {
            BOOST_HANA_CONSTEXPR_CHECK(less_equal(ord(0), ord(1)));
            BOOST_HANA_CONSTEXPR_CHECK(less_equal(ord(0), ord(0)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(less_equal(ord(1), ord(0))));
        }

        // greater_equal
        {
            BOOST_HANA_CONSTEXPR_CHECK(greater_equal(ord(1), ord(0)));
            BOOST_HANA_CONSTEXPR_CHECK(greater_equal(ord(0), ord(0)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(greater_equal(ord(0), ord(1))));
        }

        // greater
        {
            BOOST_HANA_CONSTEXPR_CHECK(greater(ord(1), ord(0)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(greater(ord(0), ord(0))));
            BOOST_HANA_CONSTEXPR_CHECK(not_(greater(ord(0), ord(1))));
        }

        // max
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                max(ord(0), ord(0)), ord(0)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                max(ord(1), ord(0)), ord(1)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                max(ord(0), ord(1)), ord(1)
            ));
        }

        // min
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                min(ord(0), ord(0)),
                ord(0)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                min(ord(1), ord(0)),
                ord(0)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                min(ord(0), ord(1)),
                ord(0)
            ));
        }

        // ordering
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                ordering(f)(ord(1), ord(0)),
                less(f(ord(1)), f(ord(0)))
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                ordering(f)(ord(0), ord(1)),
                less(f(ord(0)), f(ord(1)))
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                ordering(f)(ord(0), ord(0)),
                less(f(ord(0)), f(ord(0)))
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                ord(0) < ord(1),
                less(ord(0), ord(1))
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                ord(0) <= ord(1),
                less_equal(ord(0), ord(1))
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                ord(0) > ord(1),
                greater(ord(0), ord(1))
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                ord(0) >= ord(1),
                greater_equal(ord(0), ord(1))
            ));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Enumerable
    //////////////////////////////////////////////////////////////////////////
    {
        // succ
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                succ(test::numeric(0)),
                test::numeric(1)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                succ(test::numeric(1)),
                test::numeric(2)
            ));
        }

        // pred
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                pred(test::numeric(1)),
                test::numeric(0)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                pred(test::numeric(2)),
                test::numeric(1)
            ));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Monoid
    //////////////////////////////////////////////////////////////////////////
    {
        constexpr int x = 2, y = 3;

        // zero
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                zero<test::Numeric>(), test::numeric(0)
            ));
        }

        // plus
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                plus(test::numeric(x), test::numeric(y)),
                test::numeric(x + y)
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                test::numeric(x) + test::numeric(y),
                plus(test::numeric(x), test::numeric(y))
            ));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Group
    //////////////////////////////////////////////////////////////////////////
    {
        constexpr int x = 2, y = 3;

        // minus
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                minus(test::numeric(x), test::numeric(y)),
                test::numeric(x - y)
            ));
        }

        // negate
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                negate(test::numeric(x)),
                test::numeric(-x)
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                test::numeric(x) - test::numeric(y),
                minus(test::numeric(x), test::numeric(y))
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                -test::numeric(x),
                negate(test::numeric(x))
            ));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Ring
    //////////////////////////////////////////////////////////////////////////
    {
        constexpr int x = 2, y = 3;

        // one
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                one<test::Numeric>(),
                test::numeric(1)
            ));
        }

        // mult
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                mult(test::numeric(x), test::numeric(y)),
                test::numeric(x * y)
            ));
        }

        // power
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                power(test::numeric(x), zero<test::CNumeric<int>>()),
                one<test::Numeric>()
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                power(test::numeric(x), one<test::CNumeric<int>>()),
                test::numeric(x)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                power(test::numeric(x), test::cnumeric<int, 2>),
                mult(test::numeric(x), test::numeric(x))
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                power(test::numeric(x), test::cnumeric<int, 3>),
                mult(mult(test::numeric(x), test::numeric(x)), test::numeric(x))
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                test::numeric(x) * test::numeric(y),
                mult(test::numeric(x), test::numeric(y))
            ));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    {
        constexpr int x = 6, y = 3, z = 4;

        // quot
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                quot(test::numeric(x), test::numeric(y)),
                test::numeric(x / y)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                quot(test::numeric(x), test::numeric(z)),
                 test::numeric(x/ z)
            ));
        }

        // rem
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                rem(test::numeric(x), test::numeric(y)),
                test::numeric(x % y)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                rem(test::numeric(x), test::numeric(z)),
                test::numeric(x % z)
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                test::numeric(x) % test::numeric(y),
                rem(test::numeric(x), test::numeric(y))
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                test::numeric(x) / test::numeric(y),
                quot(test::numeric(x), test::numeric(y))
            ));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Logical
    //////////////////////////////////////////////////////////////////////////
    {
        auto logical = test::numeric;
        auto comparable = test::numeric;

        // not_
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                not_(logical(true)),
                logical(false)
            ));
        }

        // and_
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                and_(logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                and_(logical(false)),
                logical(false)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                and_(logical(true), logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                and_(logical(true), logical(false)),
                logical(false)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                and_(logical(false), invalid{}),
                logical(false)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                and_(logical(true), logical(true), logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                and_(logical(true), logical(true), logical(false)),
                logical(false)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                and_(logical(true), logical(false), invalid{}),
                logical(false)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                and_(logical(false), invalid{}, invalid{}),
                logical(false)
            ));
        }

        // or_
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                or_(logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                or_(logical(false)),
                logical(false)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                or_(logical(false), logical(false)),
                logical(false)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                or_(logical(false), logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                or_(logical(true), invalid{}),
                logical(true)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                or_(logical(false), logical(false), logical(false)),
                logical(false)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                or_(logical(false), logical(false), logical(true)),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                or_(logical(false), logical(true), invalid{}),
                logical(true)
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                or_(logical(true), invalid{}, invalid{}),
                logical(true)
            ));
        }

        // if_
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                if_(logical(true), comparable(0), comparable(1)),
                comparable(0)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                if_(logical(false), comparable(0), comparable(1)),
                comparable(1)
            ));
        }

        // eval_if
        {
            auto t = [=](auto) { return comparable(0); };
            auto e = [=](auto) { return comparable(1); };

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                eval_if(logical(true), t, e),
                comparable(0)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                eval_if(logical(false), t, e),
                comparable(1)
            ));
        }

        // while_
        {
            auto smaller_than = [](auto n) {
                return [n](auto v) { return v.size() < n; };
            };
            auto f = [](auto v) {
                v.push_back(v.size());
                return v;
            };

            BOOST_HANA_RUNTIME_CHECK(equal(
                while_(smaller_than(0u), std::vector<int>{}, f),
                std::vector<int>{}
            ));

            BOOST_HANA_RUNTIME_CHECK(equal(
                while_(smaller_than(1u), std::vector<int>{}, f),
                std::vector<int>{0}
            ));

            BOOST_HANA_RUNTIME_CHECK(equal(
                while_(smaller_than(2u), std::vector<int>{}, f),
                std::vector<int>{0, 1}
            ));

            BOOST_HANA_RUNTIME_CHECK(equal(
                while_(smaller_than(3u), std::vector<int>{}, f),
                std::vector<int>{0, 1, 2}
            ));

            BOOST_HANA_RUNTIME_CHECK(equal(
                while_(smaller_than(4u), std::vector<int>{}, f),
                std::vector<int>{0, 1, 2, 3}
            ));

            // Make sure it can be called with an lvalue state:
            std::vector<int> v{};
            BOOST_HANA_RUNTIME_CHECK(equal(
                while_(smaller_than(4u), v, f),
                std::vector<int>{0, 1, 2, 3}
            ));
        }

        // until
        {
            auto has_length = [](auto n) {
                return [n](auto v) { return v.size() == n; };
            };
            auto f = [](auto v) {
                v.push_back(v.size());
                return v;
            };

            BOOST_HANA_RUNTIME_CHECK(equal(
                until(has_length(0u), std::vector<int>{}, f),
                std::vector<int>{}
            ));

            BOOST_HANA_RUNTIME_CHECK(equal(
                until(has_length(1u), std::vector<int>{}, f),
                std::vector<int>{0}
            ));

            BOOST_HANA_RUNTIME_CHECK(equal(
                until(has_length(2u), std::vector<int>{}, f),
                std::vector<int>{0, 1}
            ));

            BOOST_HANA_RUNTIME_CHECK(equal(
                until(has_length(3u), std::vector<int>{}, f),
                std::vector<int>{0, 1, 2}
            ));

            BOOST_HANA_RUNTIME_CHECK(equal(
                until(has_length(4u), std::vector<int>{}, f),
                std::vector<int>{0, 1, 2, 3}
            ));

            // Make sure it can be called with an lvalue state:
            std::vector<int> v{};
            BOOST_HANA_RUNTIME_CHECK(equal(
                until(has_length(4u), v, f),
                std::vector<int>{0, 1, 2, 3}
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                !logical(true),
                not_(logical(true))
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                logical(true) && logical(true),
                and_(logical(true), logical(true))
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                logical(false) || logical(false),
                or_(logical(false), logical(false))
            ));
        }
    }
}

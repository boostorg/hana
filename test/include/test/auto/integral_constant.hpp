/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_TEST_TEST_AUTO_INTEGRAL_CONSTANT_HPP

#include <boost/hana/integral_constant.hpp>

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp> // for comparison with builtin types
#include <boost/hana/functional/always.hpp>
#include <boost/hana/logical.hpp>

// provided instances
#include <test/auto/comparable.hpp>
#include <test/auto/enumerable.hpp>
#include <test/auto/group.hpp>
#include <test/auto/integral_domain.hpp>
#include <test/auto/logical.hpp>
#include <test/auto/monoid.hpp>
#include <test/auto/orderable.hpp>
#include <test/auto/ring.hpp>

// required instances
#include <test/auto/constant.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>

#include <type_traits>


namespace boost { namespace hana { namespace test {
    template <typename I, typename T, typename U>
    void IntegralConstant_laws() {
        // Comparable
        {
            laws<Comparable, I>();

            // equal
            {
                // IntegralConstant == IntegralConstant
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    integral_constant<I, T, 0>,
                    integral_constant<I, U, 0>
                ));
                BOOST_HANA_CONSTANT_ASSERT(not_(equal(
                    integral_constant<I, T, 0>,
                    integral_constant<I, U, 1>
                )));


                // IntegralConstant == other
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    integral_constant<I, T, 0>, U{0}
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(
                    integral_constant<I, T, 0>, U{1}
                )));


                // other == IntegralConstant
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    U{0}, integral_constant<I, T, 0>
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(
                    U{1}, integral_constant<I, T, 0>
                )));
            }
        }

        // Orderable
        {
            laws<Orderable, I>();

            // less
            {
                // IntegralConstant < IntegralConstant
                BOOST_HANA_CONSTANT_ASSERT(less(
                    integral_constant<I, T, 0>,
                    integral_constant<I, U, 1>
                ));
                BOOST_HANA_CONSTANT_ASSERT(not_(less(
                    integral_constant<I, T, 0>,
                    integral_constant<I, U, 0>
                )));
                BOOST_HANA_CONSTANT_ASSERT(not_(less(
                    integral_constant<I, T, 1>,
                    integral_constant<I, U, 0>
                )));

                // IntegralConstant < other
                BOOST_HANA_CONSTEXPR_ASSERT(less(integral_constant<I, T, 0>, U{1}));
                BOOST_HANA_CONSTEXPR_ASSERT(not_(less(integral_constant<I, T, 0>, U{0})));
                BOOST_HANA_CONSTEXPR_ASSERT(not_(less(integral_constant<I, T, 1>, U{0})));

                // other < IntegralConstant
                BOOST_HANA_CONSTEXPR_ASSERT(less(T{0}, integral_constant<I, U, 1>));
                BOOST_HANA_CONSTEXPR_ASSERT(not_(less(T{0}, integral_constant<I, U, 0>)));
                BOOST_HANA_CONSTEXPR_ASSERT(not_(less(T{1}, integral_constant<I, U, 0>)));
            }
        }

        // Enumerable
        {
            laws<Enumerable, I>();

            // succ
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    succ(integral_constant<I, T, 0>),
                    integral_constant<I, T, 1>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    succ(integral_constant<I, T, 1>),
                    integral_constant<I, T, 2>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    succ(integral_constant<I, T, 2>),
                    integral_constant<I, T, 3>
                ));
            }

            // pred
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    pred(integral_constant<I, T, 1>),
                    integral_constant<I, T, 0>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    pred(integral_constant<I, T, 2>),
                    integral_constant<I, T, 1>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    pred(integral_constant<I, T, 3>),
                    integral_constant<I, T, 2>
                ));
            }
        }

        // Monoid
        {
            laws<Monoid, I>();
            using C = std::common_type_t<T, U>;

            // zero
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(zero<I>, integral_constant<I, T, 0>));
                BOOST_HANA_CONSTANT_ASSERT(equal(zero<I>, integral_constant<I, U, 0>));
            }

            // plus
            {
                constexpr T x = 6;
                constexpr U y = 4;

                // IntegralConstant + IntegralConstant
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    plus(integral_constant<I, T, x>, integral_constant<I, U, y>),
                    integral_constant<I, C, x + y>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    plus(integral_constant<I, T, x>, integral_constant<I, U, y>),
                    integral_constant<I, C, x + y>
                ));

                // IntegralConstant + other
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    plus(integral_constant<I, T, x>, U{y}),
                    C{x + y}
                ));

                // other + IntegralConstant
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    plus(T{x}, integral_constant<I, U, y>),
                    C{x + y}
                ));
            }
        }

        // Group
        {
            laws<Group, I>();

            // negate
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    negate(integral_constant<I, T, 1>),
                    integral_constant<I, T, -1>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    negate(integral_constant<I, U, 3>),
                    integral_constant<I, U, -3>
                ));
            }

            // minus
            {
                using C = std::common_type_t<T, U>;

                // IntegralConstant - IntegralConstant
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    minus(integral_constant<I, T, 1>, integral_constant<I, U, 3>),
                    integral_constant<I, C, 1 - 3>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    minus(integral_constant<I, T, 4>, integral_constant<I, U, 2>),
                    integral_constant<I, C, 4 - 2>
                ));

                // IntegralConstant - other
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    minus(integral_constant<I, T, 1>, U{3}),
                    C{1 - 3}
                ));

                // other - IntegralConstant
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    minus(T{1}, integral_constant<I, U, 3>),
                    C{1 - 3}
                ));
            }
        }

        // Ring
        {
            laws<Ring, I>();
            using C = std::common_type_t<T, U>;

            // one
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(one<I>, integral_constant<I, T, 1>));
                BOOST_HANA_CONSTANT_ASSERT(equal(one<I>, integral_constant<I, U, 1>));
            }

            // mult
            {
                constexpr T x = 6;
                constexpr U y = 4;

                // IntegralConstant + IntegralConstant
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    mult(integral_constant<I, T, x>, integral_constant<I, U, y>),
                    integral_constant<I, C, x * y>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    mult(integral_constant<I, T, x>, integral_constant<I, U, y>),
                    integral_constant<I, C, x * y>
                ));

                // IntegralConstant * other
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    mult(integral_constant<I, T, x>, U{y}),
                    C{x * y}
                ));

                // other * IntegralConstant
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    mult(T{x}, integral_constant<I, U, y>),
                    C{x * y}
                ));
            }
        }

        // IntegralDomain
        {
            laws<IntegralDomain, I>();
            using C = std::common_type_t<T, U>;

            // quot
            {
                // IntegralConstant / IntegralConstant
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    quot(integral_constant<I, T, 6>, integral_constant<I, U, 3>),
                    integral_constant<I, C, 6 / 3>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    quot(integral_constant<I, T, 4>, integral_constant<I, U, 3>),
                    integral_constant<I, C, 4 / 3>
                ));

                // IntegralConstant / other
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    quot(integral_constant<I, T, 6>, U{3}),
                    C{6 / 3}
                ));

                // other / IntegralConstant
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    quot(T{6}, integral_constant<I, U, 3>),
                    C{6 / 3}
                ));
            }

            // mod
            {
                // IntegralConstant % IntegralConstant
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    mod(integral_constant<I, T, 6>, integral_constant<I, U, 4>),
                    integral_constant<I, C, 6 % 4>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    mod(integral_constant<I, T, 6>, integral_constant<I, U, 4>),
                    integral_constant<I, C, 6 % 4>
                ));

                // IntegralConstant % other
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    mod(integral_constant<I, T, 6>, U{4}),
                    C{6 % 4}
                ));

                // other % IntegralConstant
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    mod(T{6}, integral_constant<I, U, 4>),
                    C{6 % 4}
                ));
            }
        }

        // Logical
        {
            laws<Logical, I>();
            auto t = always(x<0>);
            auto e = always(x<1>);

            // eval_if
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    eval_if(integral_constant<I, bool, true>, t, e), x<0>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    eval_if(integral_constant<I, bool, false>, t, e), x<1>
                ));
            }

            // not_
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    not_(integral_constant<I, bool, true>),
                    integral_constant<I, bool, false>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    not_(integral_constant<I, bool, false>),
                    not_(not_(integral_constant<I, bool, true>))
                ));
            }
        }
    }

    template <typename I>
    auto laws<IntegralConstant, I> = [] {
        IntegralConstant_laws<I, int, int>();
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_INTEGRAL_CONSTANT_HPP

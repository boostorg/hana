/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_TEST_TEST_AUTO_INTEGRAL_CONSTANT_HPP

#include <boost/hana/integral_constant.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
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
#include <test/cnumeric.hpp>
#include <test/injection.hpp>

#include <type_traits>


namespace boost { namespace hana { namespace test {
    template <template <typename ...> class C, typename T, typename U>
    void IntegralConstant_laws() {
        using Common = std::common_type_t<T, U>;

        // laws
        {
            for_each(objects<C<T>>, [](auto x) {
                constexpr auto v = value(x);
                BOOST_HANA_CONSTANT_CHECK(equal(
                    x, to<C<T>>(cnumeric<T, v>)
                ));

                for_each(objects<C<T>>, [=](auto y) {
                    constexpr auto vx = value(x);
                    constexpr auto vy = value(y);

                    BOOST_HANA_CONSTANT_CHECK(
                        equal(x, y)
                            ^iff^
                        equal(integral_constant<C<T>, vx>, integral_constant<C<T>, vy>)
                    );
                });
            });
        }

        // common
        {
            static_assert(::std::is_same<
                common_t<C<T>, T>, T
            >{}, "");

            static_assert(::std::is_same<
                common_t<T, C<T>>, T
            >{}, "");

            static_assert(::std::is_same<
                common_t<C<T>, U>, ::std::common_type_t<T, U>
            >{}, "");

            static_assert(::std::is_same<
                common_t<U, C<T>>, ::std::common_type_t<T, U>
            >{}, "");

            static_assert(::std::is_same<
                common_t<C<T>, C<U>>, C< ::std::common_type_t<T, U>>
            >{}, "");
        }

        // conversions
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                to<T>(integral_constant<C<T>, 1>),
                T{1}
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                to<T>(integral_constant<C<T>, 0>),
                T{0}
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                to<U>(integral_constant<C<U>, 1>),
                U{1}
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                to<U>(integral_constant<C<U>, 0>),
                U{0}
            ));
        }

        // Comparable
        {
            laws<Comparable, C<T>>();

            // equal
            {
                // IntegralConstant == IntegralConstant
                BOOST_HANA_CONSTANT_CHECK(equal(
                    integral_constant<C<T>, 0>,
                    integral_constant<C<U>, 0>
                ));
                BOOST_HANA_CONSTANT_CHECK(not_(equal(
                    integral_constant<C<T>, 0>,
                    integral_constant<C<U>, 1>
                )));


                // IntegralConstant == other
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    integral_constant<C<T>, 0>, U{0}
                ));
                BOOST_HANA_CONSTEXPR_CHECK(not_(equal(
                    integral_constant<C<T>, 0>, U{1}
                )));


                // other == IntegralConstant
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    U{0}, integral_constant<C<T>, 0>
                ));
                BOOST_HANA_CONSTEXPR_CHECK(not_(equal(
                    U{1}, integral_constant<C<T>, 0>
                )));
            }
        }

        // Orderable
        {
            laws<Orderable, C<T>>();

            // less
            {
                // IntegralConstant < IntegralConstant
                BOOST_HANA_CONSTANT_CHECK(less(
                    integral_constant<C<T>, 0>,
                    integral_constant<C<U>, 1>
                ));
                BOOST_HANA_CONSTANT_CHECK(not_(less(
                    integral_constant<C<T>, 0>,
                    integral_constant<C<U>, 0>
                )));
                BOOST_HANA_CONSTANT_CHECK(not_(less(
                    integral_constant<C<T>, 1>,
                    integral_constant<C<U>, 0>
                )));

                // IntegralConstant < other
                BOOST_HANA_CONSTEXPR_CHECK(less(integral_constant<C<T>, 0>, U{1}));
                BOOST_HANA_CONSTEXPR_CHECK(not_(less(integral_constant<C<T>, 0>, U{0})));
                BOOST_HANA_CONSTEXPR_CHECK(not_(less(integral_constant<C<T>, 1>, U{0})));

                // other < IntegralConstant
                BOOST_HANA_CONSTEXPR_CHECK(less(T{0}, integral_constant<C<U>, 1>));
                BOOST_HANA_CONSTEXPR_CHECK(not_(less(T{0}, integral_constant<C<U>, 0>)));
                BOOST_HANA_CONSTEXPR_CHECK(not_(less(T{1}, integral_constant<C<U>, 0>)));
            }
        }

        // Enumerable
        {
            laws<Enumerable, C<T>>();

            // succ
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    succ(integral_constant<C<T>, 0>),
                    integral_constant<C<T>, 1>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    succ(integral_constant<C<T>, 1>),
                    integral_constant<C<T>, 2>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    succ(integral_constant<C<T>, 2>),
                    integral_constant<C<T>, 3>
                ));
            }

            // pred
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    pred(integral_constant<C<T>, 1>),
                    integral_constant<C<T>, 0>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    pred(integral_constant<C<T>, 2>),
                    integral_constant<C<T>, 1>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    pred(integral_constant<C<T>, 3>),
                    integral_constant<C<T>, 2>
                ));
            }
        }

        // Monoid
        {
            laws<Monoid, C<T>>();

            // zero
            {
                BOOST_HANA_CONSTANT_CHECK(equal(zero<C<T>>, integral_constant<C<T>, 0>));
                BOOST_HANA_CONSTANT_CHECK(equal(zero<C<T>>, integral_constant<C<U>, 0>));
            }

            // plus
            {
                constexpr T x = 6;
                constexpr U y = 4;

                // IntegralConstant + IntegralConstant
                BOOST_HANA_CONSTANT_CHECK(equal(
                    plus(integral_constant<C<T>, x>, integral_constant<C<U>, y>),
                    integral_constant<C<Common>, x + y>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    plus(integral_constant<C<T>, x>, integral_constant<C<U>, y>),
                    integral_constant<C<Common>, x + y>
                ));

                // IntegralConstant + other
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    plus(integral_constant<C<T>, x>, U{y}),
                    Common{x + y}
                ));

                // other + IntegralConstant
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    plus(T{x}, integral_constant<C<U>, y>),
                    Common{x + y}
                ));
            }
        }

        // Group
        {
            laws<Group, C<T>>();

            // negate
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    negate(integral_constant<C<T>, 1>),
                    integral_constant<C<T>, T{-1}>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    negate(integral_constant<C<U>, 3>),
                    integral_constant<C<U>, U{-3}>
                ));
            }

            // minus
            {
                // IntegralConstant - IntegralConstant
                BOOST_HANA_CONSTANT_CHECK(equal(
                    minus(integral_constant<C<T>, 1>, integral_constant<C<U>, 3>),
                    integral_constant<C<Common>, Common{1 - 3}>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    minus(integral_constant<C<T>, 4>, integral_constant<C<U>, 2>),
                    integral_constant<C<Common>, Common{4 - 2}>
                ));

                // IntegralConstant - other
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    minus(integral_constant<C<T>, 1>, U{3}),
                    Common{1 - 3}
                ));

                // other - IntegralConstant
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    minus(T{1}, integral_constant<C<U>, 3>),
                    Common{1 - 3}
                ));
            }
        }

        // Ring
        {
            laws<Ring, C<T>>();

            // one
            {
                BOOST_HANA_CONSTANT_CHECK(equal(one<C<T>>, integral_constant<C<T>, 1>));
                BOOST_HANA_CONSTANT_CHECK(equal(one<C<T>>, integral_constant<C<U>, 1>));
            }

            // mult
            {
                constexpr T x = 6;
                constexpr U y = 4;

                // IntegralConstant + IntegralConstant
                BOOST_HANA_CONSTANT_CHECK(equal(
                    mult(integral_constant<C<T>, x>, integral_constant<C<U>, y>),
                    integral_constant<C<Common>, x * y>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    mult(integral_constant<C<T>, x>, integral_constant<C<U>, y>),
                    integral_constant<C<Common>, x * y>
                ));

                // IntegralConstant * other
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    mult(integral_constant<C<T>, x>, U{y}),
                    Common{x * y}
                ));

                // other * IntegralConstant
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    mult(T{x}, integral_constant<C<U>, y>),
                    Common{x * y}
                ));
            }
        }

        // IntegralDomain
        {
            laws<IntegralDomain, C<T>>();

            // quot
            {
                // IntegralConstant / IntegralConstant
                BOOST_HANA_CONSTANT_CHECK(equal(
                    quot(integral_constant<C<T>, 6>, integral_constant<C<U>, 3>),
                    integral_constant<C<Common>, 6 / 3>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    quot(integral_constant<C<T>, 4>, integral_constant<C<U>, 3>),
                    integral_constant<C<Common>, 4 / 3>
                ));

                // IntegralConstant / other
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    quot(integral_constant<C<T>, 6>, U{3}),
                    Common{6 / 3}
                ));

                // other / IntegralConstant
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    quot(T{6}, integral_constant<C<U>, 3>),
                    Common{6 / 3}
                ));
            }

            // mod
            {
                // IntegralConstant % IntegralConstant
                BOOST_HANA_CONSTANT_CHECK(equal(
                    mod(integral_constant<C<T>, 6>, integral_constant<C<U>, 4>),
                    integral_constant<C<Common>, 6 % 4>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    mod(integral_constant<C<T>, 6>, integral_constant<C<U>, 4>),
                    integral_constant<C<Common>, 6 % 4>
                ));

                // IntegralConstant % other
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    mod(integral_constant<C<T>, 6>, U{4}),
                    Common{6 % 4}
                ));

                // other % IntegralConstant
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    mod(T{6}, integral_constant<C<U>, 4>),
                    Common{6 % 4}
                ));
            }
        }

        // Logical
        {
            laws<Logical, C<T>>();
            auto t = always(x<0>);
            auto e = always(x<1>);

            // eval_if
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    eval_if(integral_constant<C<bool>, true>, t, e), x<0>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    eval_if(integral_constant<C<bool>, false>, t, e), x<1>
                ));
            }

            // not_
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    not_(integral_constant<C<bool>, true>),
                    integral_constant<C<bool>, false>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    not_(integral_constant<C<bool>, false>),
                    not_(not_(integral_constant<C<bool>, true>))
                ));
            }
        }
    }

    template <template <typename ...> class C, typename T>
    auto laws<IntegralConstant, C<T>> = [] {
        IntegralConstant_laws<C, T, T>();
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_INTEGRAL_CONSTANT_HPP

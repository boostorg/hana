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
                        equal(integral_constant<C<T>, vx>(),
                              integral_constant<C<T>, vy>())
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
        }

        // Comparable
        {
            laws<Comparable, C<T>>();

            // equal
            {
                // IntegralConstant == IntegralConstant
                BOOST_HANA_CONSTANT_CHECK(equal(
                    integral_constant<C<T>, 0>(),
                    integral_constant<C<U>, 0>()
                ));
                BOOST_HANA_CONSTANT_CHECK(not_(equal(
                    integral_constant<C<T>, 0>(),
                    integral_constant<C<U>, 1>()
                )));


                // IntegralConstant == other
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    integral_constant<C<T>, 0>(), U{0}
                ));
                BOOST_HANA_CONSTEXPR_CHECK(not_(equal(
                    integral_constant<C<T>, 0>(), U{1}
                )));


                // other == IntegralConstant
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    U{0}, integral_constant<C<T>, 0>()
                ));
                BOOST_HANA_CONSTEXPR_CHECK(not_(equal(
                    U{1}, integral_constant<C<T>, 0>()
                )));
            }
        }
    }

    template <template <typename ...> class C, typename T>
    auto laws<IntegralConstant, C<T>> = [] {
        IntegralConstant_laws<C, T, T>();
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_INTEGRAL_CONSTANT_HPP

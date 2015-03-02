/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_TEST_LAWS_INTEGRAL_DOMAIN_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/integral_domain.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename D, typename = when<true>>
    struct TestIntegralDomain : TestIntegralDomain<D, laws> {
        using TestIntegralDomain<D, laws>::TestIntegralDomain;
    };

    template <typename D>
    struct TestIntegralDomain<D, laws> {
        static_assert(models<IntegralDomain(D)>{}, "");

        template <typename Xs>
        TestIntegralDomain(Xs xs) {
            foreach3(xs, [](auto a, auto b, auto k) {

                // commutativity
                BOOST_HANA_CHECK(hana::equal(
                    hana::mult(a, b),
                    hana::mult(b, a)
                ));

                only_when_(hana::not_equal(b, zero<D>()), [=](auto _) {
                    auto b_ = _(b);
                    BOOST_HANA_CHECK(hana::equal(
                        hana::plus(
                            hana::mult(hana::quot(a, b_), b_),
                            hana::mod(a, b_)
                        ),
                        a
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::mod(zero<D>(), b_),
                        zero<D>()
                    ));

                    // canonicity
                    BOOST_HANA_CHECK(hana::equal(
                        hana::mod(a, b_),
                        hana::mod(hana::plus(a, hana::mult(k, b_)), b_)
                    ));

                    // operators
                    only_when_(bool_<has_operator<D, decltype(mod)>{}>, [=](auto _) {
                        BOOST_HANA_CHECK(hana::equal(
                            hana::mod(a, _(b)),
                            _(a) % _(b)
                        ));
                    });

                    only_when_(bool_<has_operator<D, decltype(quot)>{}>, [=](auto _) {
                        BOOST_HANA_CHECK(hana::equal(
                            hana::quot(a, _(b)),
                            _(a) / _(b)
                        ));
                    });
                });

            });
        }
    };

    template <typename C>
    struct TestIntegralDomain<C, when<models<Constant(C)>{}>>
        : TestIntegralDomain<C, laws>
    {
        template <typename Xs>
        TestIntegralDomain(Xs xs) : TestIntegralDomain<C, laws>{xs} {
            foreach2(xs, [](auto x, auto y) {
                hana::eval_if(hana::equal(zero<C>(), y), [](auto){}, [=](auto _) {
                    auto y_ = _(y);

                    BOOST_HANA_CHECK(hana::equal(
                        hana::quot(hana::value(x), hana::value(y_)),
                        hana::value(hana::quot(x, y_))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::mod(hana::value(x), hana::value(y_)),
                        hana::value(hana::mod(x, y_))
                    ));

                });
            });
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_INTEGRAL_DOMAIN_HPP

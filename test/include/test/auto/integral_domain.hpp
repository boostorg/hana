/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_TEST_TEST_AUTO_INTEGRAL_DOMAIN_HPP

#include <boost/hana/integral_domain.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/ring.hpp>

#include <test/auto/base.hpp>

// required instances
#include <test/auto/ring.hpp>


namespace boost { namespace hana { namespace test {
    template <typename D>
    auto laws<IntegralDomain, D> = [] {
        BOOST_HANA_CONSTANT_CHECK(models<IntegralDomain, D>);
        laws<Ring, D>();

        // Instance laws
        {
            for_each(objects<D>, [](auto a) {
                for_each(objects<D>, [=](auto b) {
                    BOOST_HANA_CHECK(equal(
                        mult(a, b),
                        mult(b, a)
                    ));

                    eval_if(equal(b, zero<D>()), [](auto){}, [=](auto _) {

                        BOOST_HANA_CHECK(equal(
                            a,
                            plus(
                                mult(_(quot)(a, b), b),
                                _(mod)(a, b)
                            )
                        ));

                        BOOST_HANA_CHECK(equal(
                            _(mod)(zero<D>(), b),
                            zero<D>()
                        ));

                        for_each(objects<D>, [=](auto k) {
                            BOOST_HANA_CHECK(equal(
                                _(mod)(a, b),
                                mod(plus(a, _(mult)(k, b)), b)
                            ));
                        });
                    });
                });
            });
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_INTEGRAL_DOMAIN_HPP

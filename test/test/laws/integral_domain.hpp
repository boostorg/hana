/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_TEST_TEST_LAWS_INTEGRAL_DOMAIN_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/infix.hpp>
#include <boost/hana/integral_domain/integral_domain.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monoid/monoid.hpp>
#include <boost/hana/ring/ring.hpp>


template <typename D, typename ...Domain>
constexpr auto IntegralDomain_laws(Domain ...d) {
    using namespace boost::hana;
    auto lazy_implies = infix([](auto p, auto q) {
        return eval_if(p, q, always(not_(p)));
    });

    return and_([=](auto a) {
        return and_([=](auto b) {
            return and_([=](auto k) {

                return and_(
                    equal(mult(a, b), mult(b, a)),

                    not_equal(b, zero<D>) ^lazy_implies^ [=](auto _) { return
                        and_(
                            equal(plus(mult(_(quot)(a, b), b), _(mod)(a, b)), a),
                            equal(_(mod)(plus(a, mult(k, b)), b), _(mod)(a, b)),
                            equal(_(mod)(zero<D>, b), zero<D>)
                        );
                    }
                );

            }(d)...);
        }(d)...);
    }(d)...);
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_INTEGRAL_DOMAIN_HPP

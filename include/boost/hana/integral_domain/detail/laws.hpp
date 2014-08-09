/*!
@file
Defines `boost::hana::IntegralDomain::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_DOMAIN_DETAIL_LAWS_HPP
#define BOOST_HANA_INTEGRAL_DOMAIN_DETAIL_LAWS_HPP

#include <boost/hana/integral_domain/integral_domain.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monoid/monoid.hpp>
#include <boost/hana/ring/ring.hpp>
#include <boost/hana/searchable/searchable.hpp>


namespace boost { namespace hana {
    struct IntegralDomain::laws {
        template <typename Domains>
        static constexpr auto check(Domains ds) {
            return all(ds, [=](auto a) {
                return all(ds, [=](auto b) {
                    return all(ds, [=](auto k) {
                        using D = datatype_t<decltype(a)>;

                        auto nonzero_conditions = eval_if(not_equal(b, zero<D>),
                            [=](auto _) {
                                return and_(
                                    equal(
                                        plus(mult(_(quot)(a, b), b), _(mod)(a, b)),
                                        a
                                    ),
                                    equal(
                                        _(mod)(plus(a, mult(k, b)), b),
                                        _(mod)(a, b)
                                    ),
                                    equal(_(mod)(zero<D>, b), zero<D>)
                                );
                            },
                            [](auto _) { return true_; }
                        );

                        return and_(
                            equal(mult(a, b), mult(b, a)),
                            nonzero_conditions
                        );
                    });
                });
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_DOMAIN_DETAIL_LAWS_HPP

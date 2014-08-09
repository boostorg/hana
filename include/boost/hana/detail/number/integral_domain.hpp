/*!
@file
Defines the `boost::hana::IntegralDomain` instance for `boost::hana::detail::Number`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_NUMBER_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_DETAIL_NUMBER_INTEGRAL_DOMAIN_HPP

#include <boost/hana/detail/number/number.hpp>

#include <boost/hana/detail/number/ring.hpp>
#include <boost/hana/integral_domain/mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct IntegralDomain::instance<detail::Number<Mcd>, detail::Number<Mcd>>
        : IntegralDomain::mcd
    {
        template <typename X, typename Y>
        static constexpr auto quot_impl(X x, Y y) {
            return x.wrap(x.value / y.value);
        }

        template <typename X, typename Y>
        static constexpr auto mod_impl(X x, Y y) {
            return x.wrap(x.value % y.value);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_NUMBER_INTEGRAL_DOMAIN_HPP

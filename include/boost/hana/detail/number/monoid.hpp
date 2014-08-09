/*!
@file
Defines the `boost::hana::Monoid` instance for `boost::hana::detail::Number`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_NUMBER_MONOID_HPP
#define BOOST_HANA_DETAIL_NUMBER_MONOID_HPP

#include <boost/hana/detail/number/number.hpp>

#include <boost/hana/monoid/mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Monoid::instance<detail::Number<Mcd>, detail::Number<Mcd>>
        : Monoid::mcd
    {
        template <typename X, typename Y>
        static constexpr auto plus_impl(X x, Y y) {
            return x.wrap(x.value + y.value);
        }

        static constexpr auto zero_impl() {
            return detail::number<Mcd>(0);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_NUMBER_MONOID_HPP

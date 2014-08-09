/*!
@file
Defines the `boost::hana::Ring` instance for `boost::hana::detail::Number`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_NUMBER_RING_HPP
#define BOOST_HANA_DETAIL_NUMBER_RING_HPP

#include <boost/hana/detail/number/number.hpp>

#include <boost/hana/detail/number/group.hpp>
#include <boost/hana/ring/mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Ring::instance<detail::Number<Mcd>, detail::Number<Mcd>>
        : Ring::mcd
    {
        template <typename X, typename Y>
        static constexpr auto mult_impl(X x, Y y) {
            return x.wrap(x.value * y.value);
        }

        static constexpr auto one_impl() {
            return detail::number<Mcd>(1);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_NUMBER_RING_HPP

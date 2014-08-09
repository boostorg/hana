/*!
@file
Defines the `boost::hana::Orderable` instance for `boost::hana::detail::Number`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_NUMBER_ORDERABLE_HPP
#define BOOST_HANA_DETAIL_NUMBER_ORDERABLE_HPP

#include <boost/hana/detail/number/number.hpp>

#include <boost/hana/orderable/less_mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Orderable::instance<detail::Number<Mcd>, detail::Number<Mcd>>
        : Orderable::less_mcd
    {
        template <typename X, typename Y>
        static constexpr auto less_impl(X x, Y y) {
            return x.value < y.value;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_NUMBER_ORDERABLE_HPP

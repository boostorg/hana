/*!
@file
Defines the `boost::hana::Group` instance for `boost::hana::detail::Number`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_NUMBER_GROUP_HPP
#define BOOST_HANA_DETAIL_NUMBER_GROUP_HPP

#include <boost/hana/detail/number/number.hpp>

#include <boost/hana/detail/number/monoid.hpp>
#include <boost/hana/group/minus_mcd.hpp>
#include <boost/hana/group/negate_mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Group::instance<detail::Number<Mcd>, detail::Number<Mcd>>
        : Group::minus_mcd<detail::Number<Mcd>, detail::Number<Mcd>>
    {
        template <typename X, typename Y>
        static constexpr auto minus_impl(X x, Y y) {
            return x.wrap(x.value - y.value);
        }
    };

    template <>
    struct Group::instance<
        detail::Number<detail::quote<Group::negate_mcd>>,
        detail::Number<detail::quote<Group::negate_mcd>>
    >
        : Group::negate_mcd<
            detail::Number<detail::quote<Group::negate_mcd>>,
            detail::Number<detail::quote<Group::negate_mcd>>
        >
    {
        template <typename X>
        static constexpr auto negate_impl(X x) {
            return x.wrap(-x.value);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_NUMBER_GROUP_HPP

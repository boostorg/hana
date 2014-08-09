/*!
@file
Defines the `boost::hana::Comparable` instance for `boost::hana::detail::Number`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_NUMBER_COMPARABLE_HPP
#define BOOST_HANA_DETAIL_NUMBER_COMPARABLE_HPP

#include <boost/hana/detail/number/number.hpp>

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/comparable/not_equal_mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Comparable::instance<detail::Number<Mcd>, detail::Number<Mcd>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y) {
            return x.value == y.value;
        }
    };

    template <>
    struct Comparable::instance<
        detail::Number<Comparable::not_equal_mcd>,
        detail::Number<Comparable::not_equal_mcd>
    >
        : Comparable::not_equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto not_equal_impl(X x, Y y) {
            return x.value != y.value;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_NUMBER_COMPARABLE_HPP

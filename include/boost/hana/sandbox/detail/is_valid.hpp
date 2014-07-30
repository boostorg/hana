/*!
@file
Defines `boost::hana::detail::is_valid`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_IS_VALID_HPP
#define BOOST_HANA_DETAIL_IS_VALID_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana { namespace detail {
    template <typename F, typename ...X>
    constexpr auto is_valid_impl(int, F f, X ...x) -> decltype(f(x...), true_)
    { return true_; }

    template <typename F, typename ...X>
    constexpr auto is_valid_impl(long, F, X...)
    { return false_; }

    //! @ingroup group-details
    //! `is_valid(f)(x...)` is an `Integral` representing whether  `f(x...)`
    //! is a valid expression.
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_valid = [](auto f) {
        return [=](auto ...x) { return is_valid_impl(int{0}, f, x...); };
    };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_IS_VALID_HPP

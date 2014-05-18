/*!
 * @file
 * Defines `boost::hana::if_`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_IF_HPP
#define BOOST_HANA_IF_HPP

#include <boost/hana/integral.hpp>


namespace boost { namespace hana {
    constexpr struct _if {
        template <typename Then, typename Else>
        constexpr auto operator()(Bool<true>, Then t, Else) const
        { return t; }

        template <typename Then, typename Else>
        constexpr auto operator()(Bool<false>, Then, Else e) const
        { return e; }
    } if_{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_IF_HPP

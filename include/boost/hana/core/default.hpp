/*!
@file
Defines `boost::hana::default_` and `boost::hana::is_default`.

Copyright Louis Dionne 2013-2022
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_DEFAULT_HPP
#define BOOST_HANA_CORE_DEFAULT_HPP

#include <boost/hana/fwd/core/default.hpp>

#include <boost/hana/config.hpp>
#include <boost/hana/detail/integral_constant.hpp>


namespace boost { namespace hana {
    template <typename Method, typename>
    struct is_default : hana::integral_constant<bool, false> { };

    template <typename Method>
    struct is_default<Method, decltype((void)
        static_cast<default_>(*(Method*)0)
    )>
        : hana::integral_constant<bool, true>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_DEFAULT_HPP

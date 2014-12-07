/*!
@file
Defines `boost::hana::Constant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONSTANT_HPP
#define BOOST_HANA_CONSTANT_HPP

#include <boost/hana/fwd/constant.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/method.hpp>


namespace boost { namespace hana {
    template <typename C>
    constexpr auto is_a<Constant, C> = bool_<
        is_implemented<value_impl<C>>
    >;
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONSTANT_HPP

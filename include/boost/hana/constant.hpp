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

#include <boost/hana/core/method.hpp>
#include <boost/hana/core/models.hpp>


namespace boost { namespace hana {
    template <>
    struct models_impl<Constant> {
        template <typename C, typename Context>
        static constexpr bool apply = is_implemented<value_impl<C>, Context>;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONSTANT_HPP

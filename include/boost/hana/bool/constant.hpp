/*!
@file
Defines the instance of `boost::hana::Constant` for `boost::hana::Bool`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BOOL_CONSTANT_HPP
#define BOOST_HANA_BOOL_CONSTANT_HPP

#include <boost/hana/bool/bool.hpp>
#include <boost/hana/constant/mcd.hpp>


namespace boost { namespace hana {
    template <>
    struct Constant::instance<Bool> : Constant::mcd {
        template <typename C>
        static constexpr auto value_impl(C const&)
        { return C::value; }
    };
}}

#endif // !BOOST_HANA_BOOL_CONSTANT_HPP

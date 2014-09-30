/*!
@file
Defines the instance of `boost::hana::Constant` for `boost::hana::String`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_CONSTANT_HPP
#define BOOST_HANA_STRING_CONSTANT_HPP

#include <boost/hana/constant/mcd.hpp>
#include <boost/hana/string/string.hpp>


namespace boost { namespace hana {
    //! Instance of `Constant` for `String`s.
    //!
    //! A `String`'s compile-time value is a `constexpr char*` to its
    //! internal data.
    //!
    //! ### Example
    //! @snippet example/string/constant.cpp main
    template <>
    struct Constant::instance<String> : Constant::mcd {
        template <typename S>
        static constexpr auto value_impl(S const&) {
            return S::get();
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_CONSTANT_HPP

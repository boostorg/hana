/*
@file
Defines `boost::hana::experimental::type_name`.

@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXPERIMENTAL_TYPE_NAME_HPP
#define BOOST_HANA_EXPERIMENTAL_TYPE_NAME_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/experimental/detail/type_name_pretty_function.hpp>

#include <utility>


BOOST_HANA_NAMESPACE_BEGIN  namespace experimental {
    namespace type_name_details {
        template <typename T, std::size_t ...i>
        auto type_name_impl(std::index_sequence<i...>) {
            constexpr auto name = type_name_details::type_name_impl_stringliteral<T>();
            return boost::hana::string<*(name.ptr + i)...>{};
        }
    } // end namespace type_name_details

    //! @ingroup group-experimental
    //! Returns a `hana::string` representing the name of the given type, at
    //! compile-time. This works on Clang and MSVC 2017.
    //!
    //! It also works on GCC, but only at run time and it will return
    //! a `std::string`.
    //!
    //! Other compilers are not supported as of writing this.
    //! Original idea taken from https://github.com/Manu343726/ctti.
    template <typename T>
    auto type_name() {
        #ifdef _HANA_TN_CAN_CONSTEXPR
            constexpr auto name = type_name_details::type_name_impl_stringliteral<T>();
            return type_name_details::type_name_impl<T>(std::make_index_sequence<name.length>{});
        #else
            return  type_name_details::stringliteral_to_string(
                type_name_details::type_name_impl_stringliteral<T>()
            );
        #endif
    }
} BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_EXPERIMENTAL_TYPE_NAME_HPP

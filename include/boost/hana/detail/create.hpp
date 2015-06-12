/*!
@file
Defines `boost::hana::detail::create`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CREATE_HPP
#define BOOST_HANA_DETAIL_CREATE_HPP

#include <type_traits>


namespace boost { namespace hana { namespace detail {
    //! @ingroup group-details
    //! Implementation of the generic `std::make_xxx` pattern for arbitrary
    //! `xxx`s.
    template <template <typename ...> class T>
    struct create {
        template <typename ...X>
        constexpr T<typename std::decay<X>::type...>
        operator()(X&& ...x) const {
            return T<typename std::decay<X>::type...>{
                static_cast<X&&>(x)...
            };
        }
    };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_CREATE_HPP

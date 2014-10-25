/*!
@file
Defines `boost::hana::id`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_ID_HPP
#define BOOST_HANA_FUNCTIONAL_ID_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    namespace id_detail {
        struct id_impl {
            template <typename T>
            constexpr T operator()(T&& t) const {
                return detail::std::forward<T>(t);
            }
        };
    }

    //! @ingroup group-functional
    //! The identity function -- returns its argument unchanged.
    //!
    //! ### Example
    //! @snippet example/functional/id.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    BOOST_HANA_CONSTEXPR_LAMBDA auto id = [](auto&& x) -> decltype(auto) {
        return std::forward<decltype(x)>(x);
    };
#else
    constexpr id_detail::id_impl id{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_ID_HPP

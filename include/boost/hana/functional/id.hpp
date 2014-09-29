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
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_rvalue_reference.hpp>
#include <boost/hana/detail/std/remove_reference.hpp>


namespace boost { namespace hana {
    namespace id_detail {
        template <typename X>
        decltype(auto) id_impl(X&& x, detail::std::integral_constant<bool, false>)
        { return detail::std::forward<X>(x); }

        template <typename X>
        typename detail::std::remove_reference<X>::type
        id_impl(X&& x, detail::std::integral_constant<bool, true>)
        { return detail::std::forward<X>(x); }
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
    BOOST_HANA_CONSTEXPR_LAMBDA auto id = [](auto&& x) -> decltype(auto) {
        return id_detail::id_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::is_rvalue_reference<decltype(x)>{}
        );
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_ID_HPP

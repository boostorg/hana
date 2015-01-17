/*!
@file
Defines `boost::hana::is_implemented`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_IS_IMPLEMENTED_HPP
#define BOOST_HANA_CORE_IS_IMPLEMENTED_HPP

#include <boost/hana/core/dispatch.hpp>


namespace boost { namespace hana {
    namespace dispatch_detail {
        template <typename Method>
        constexpr auto is_implemented_impl(not_implemented<Method>)
        { return false; }

        constexpr auto is_implemented_impl(...)
        { return true; }
    }

    //! @ingroup group-core
    //! Returns whether a method is implemented for the given data type(s).
    //!
    //! ...
    //!
    //! ### Example
    //! @snippet example/core/dispatch.cpp is_implemented
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Method, typename Context = context<>>
    constexpr auto is_implemented = unspecified;
#else
    template <typename Method, typename Context = context<>>
    constexpr auto is_implemented =
        dispatch_detail::is_implemented_impl(dispatch<Method, Context>{});
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_IS_IMPLEMENTED_HPP

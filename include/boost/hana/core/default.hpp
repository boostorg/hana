/*!
@file
Defines `boost::hana::default_` and `boost::hana::is_default`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_DEFAULT_HPP
#define BOOST_HANA_CORE_DEFAULT_HPP

#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana {
    //! @ingroup group-core
    //! Mark a method implementation as a default implementation.
    //!
    //! @todo
    //! Document this.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/core/default.cpp default_
    struct default_ { };

    //! @ingroup group-core
    //! Returns whether a method implementation is a default implementation.
    //!
    //! @todo
    //! Document this.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/core/default.cpp is_default
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Method>
    struct is_default : unspecified boolean integral constant { };
#else
    template <typename T, typename = void>
    struct is_default : detail::std::false_type { };

    template <typename T>
    struct is_default<T, decltype((void)static_cast<default_>(*(T*)0))>
        : detail::std::true_type
    { };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_DEFAULT_HPP

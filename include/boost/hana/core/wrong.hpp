/*!
@file
Defines `boost::hana::wrong`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_WRONG_HPP
#define BOOST_HANA_CORE_WRONG_HPP

#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana {
    //! @ingroup group-core
    //! Equivalent to a type-dependent `std::false_type`.
    //!
    //! This is useful for making a static assertion that would otherwise
    //! always fire up dependent on some template parameters.
    //!
    //! ### Example
    //! @snippet example/core/wrong.cpp main
    //!
    //! @todo Improve the documentation.
    template <typename ...>
    struct wrong : detail::std::false_type { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_WRONG_HPP

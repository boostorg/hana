/*!
@file
Forward declares `boost::hana::size`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_SIZE_HPP
#define BOOST_HANA_FWD_SIZE_HPP

#include <boost/hana/fwd/length.hpp>


namespace boost { namespace hana {
    //! Equivalent to `length`; provided for consistency with the
    //! standard library.
    //! @relates Foldable
    //!
    //! This method is an alias to `length` provided for convenience and
    //! consistency with the standard library. As an alias, `size` is not
    //! tag-dispatched on its own and `length` should be customized instead.
    //!
    //!
    //! Example
    //! -------
    //! @include example/size.cpp
    constexpr auto size = hana::length;
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SIZE_HPP

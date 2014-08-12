/*!
@file
Defines `boost::hana::disable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_DISABLE_HPP
#define BOOST_HANA_CORE_DISABLE_HPP

namespace boost { namespace hana {
    //! @ingroup group-core
    //! Explicitly disable a type class instance.
    //!
    //! This is meant as a way to disable a type class instance provided
    //! through a predicate which, for some reason, is not desirable for
    //! a given data type.
    //!
    //! ### Example
    //! @include example/core/disable.cpp
    struct disable { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_DISABLE_HPP

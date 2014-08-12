/*!
@file
Defines `boost::hana::always`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_ALWAYS_HPP
#define BOOST_HANA_FUNCTIONAL_ALWAYS_HPP

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Return a constant function returning `x` regardless of the
    //! argument(s) it is invoked with.
    //!
    //! Specifically, `always(x)` is a function such that
    //! @code
    //!     always(x)(y...) == x
    //! @endcode
    //! for any `y...`.
    //!
    //! @note
    //! This can be useful to make an expression dependent in order to delay
    //! the instantiation of its type by the compiler.
    //! @snippet example/functional/always/dependent.cpp main
    //!
    //!
    //! ### Example
    //! @snippet example/functional/always/basic.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto always = [](auto x) {
        return [=](auto ...y) { return x; };
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_ALWAYS_HPP

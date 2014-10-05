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
#include <boost/hana/detail/std/move.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Return a constant function returning `x` regardless of the
    //! argument(s) it is invoked with.
    //!
    //! Specifically, `always(x)` is a function such that
    //! @code
    //!     always(x)(y...) == x
    //! @endcode
    //! for any `y...`. A copy of `x` is made and it is owned by the
    //! `always(x)` function. When `always(x)` is called, it will return
    //! a reference to the `x` it owns. This reference is valid as long
    //! as `always(x)` is in scope.
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
        return [x(detail::std::move(x))](auto const& ...y) -> decltype(x) {
            return x;
        };
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_ALWAYS_HPP

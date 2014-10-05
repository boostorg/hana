/*!
@file
Defines `boost::hana::flip`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_FLIP_HPP
#define BOOST_HANA_FUNCTIONAL_FLIP_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Invoke a function with its two first arguments reversed.
    //!
    //! Specifically, `flip(f)` is a function such that
    //! @code
    //!     flip(f)(x, y, z...) == f(y, x, z...)
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/functional/flip.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto flip = [](auto f) {
        return [f(detail::std::move(f))](auto&& x, auto&& y, auto&& ...z) -> decltype(auto) {
            return f(
                detail::std::forward<decltype(y)>(y),
                detail::std::forward<decltype(x)>(x),
                detail::std::forward<decltype(z)>(z)...
            );
        };
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_FLIP_HPP

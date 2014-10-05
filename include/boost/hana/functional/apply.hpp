/*!
@file
Defines `boost::hana::apply`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_APPLY_HPP
#define BOOST_HANA_FUNCTIONAL_APPLY_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Invoke `f` with `x...` as arguments.
    //!
    //!
    //! @param f
    //! A function called as `f(x...)` whose result is the return
    //! value of `apply`.
    //!
    //! @param x...
    //! The arguments to call `f` with. The number of `x...` must match the
    //! arity of `f`.
    //!
    //!
    //! ### Example
    //! @snippet example/functional/apply.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto apply = [](auto&& f, auto&& ...x) -> decltype(auto) {
        return detail::std::forward<decltype(f)>(f)(
            detail::std::forward<decltype(x)>(x)...
        );
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_APPLY_HPP

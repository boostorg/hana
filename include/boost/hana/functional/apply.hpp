/*!
@file
Defines `boost::hana::apply`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_APPLY_HPP
#define BOOST_HANA_FUNCTIONAL_APPLY_HPP

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
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto apply = [](auto&& f, auto&& ...x) -> decltype(auto) {
        return forwarded(f)(forwarded(x)...);
    };
#else
    struct _apply {
        template <typename F, typename ...Args>
        constexpr decltype(auto) operator()(F&& f, Args&& ...args) const {
            return static_cast<F&&>(f)(
                static_cast<Args&&>(args)...
            );
        }
    };

    constexpr _apply apply{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_APPLY_HPP

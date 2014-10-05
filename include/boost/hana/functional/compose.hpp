/*!
@file
Defines `boost::hana::compose`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_COMPOSE_HPP
#define BOOST_HANA_FUNCTIONAL_COMPOSE_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/variadic/foldl.hpp>


namespace boost { namespace hana {
    namespace functional_detail {
        BOOST_HANA_CONSTEXPR_LAMBDA auto compose2 = [](auto f, auto g) -> decltype(auto) {
            return [f(detail::std::move(f)), g(detail::std::move(g))]
                   (auto&& x, auto&& ...xs) -> decltype(auto) {
                return f(
                    g(detail::std::forward<decltype(x)>(x)),
                    detail::std::forward<decltype(xs)>(xs)...
                );
            };
        };
    }

    //! @ingroup group-functional
    //! Return the composition of two functions or more.
    //!
    //! `compose` is defined inductively. When given more than two functions,
    //! `compose(f, g, h...)` is equivalent to `compose(f, compose(g, h...))`.
    //! When given two functions, `compose(f, g)` is a function such that
    //! @code
    //!     compose(f, g)(x, y...) == f(g(x), y...)
    //! @endcode
    //!
    //! If you need composition of the form `f(g(x, y...))`, use `demux` instead.
    //!
    //! @note
    //! `compose` is an associative operation; `compose(f, compose(g, h))`
    //! is equivalent to `compose(compose(f, g), h)`.
    //!
    //! @internal
    //! ### Proof of associativity
    //!
    //! @code
    //!     compose(f, compose(g, h))(x, xs...) == f(compose(g, h)(x), xs...)
    //!                                         == f(g(h(x)), xs...)
    //!
    //!     compose(compose(f, g), h)(x, xs...) == compose(f, g)(h(x), xs...)
    //!                                         == f(g(h(x)), xs...)
    //! @endcode
    //! @endinternal
    //!
    //! ### Example
    //! @snippet example/functional/compose.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto compose = [](auto f, auto g, auto ...h) -> decltype(auto) {
        unspecified
    };
#else
    BOOST_HANA_CONSTEXPR_LAMBDA auto compose = [](auto&& f, auto&& g, auto&& ...h) -> decltype(auto) {
        return detail::variadic::foldl(
            functional_detail::compose2,
            detail::std::forward<decltype(f)>(f),
            detail::std::forward<decltype(g)>(g),
            detail::std::forward<decltype(h)>(h)...
        );
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_COMPOSE_HPP

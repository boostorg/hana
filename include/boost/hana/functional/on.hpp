/*!
@file
Defines `boost::hana::on`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_ON_HPP
#define BOOST_HANA_FUNCTIONAL_ON_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/functional/infix.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Invoke a function with the result of invoking another function on
    //! each argument.
    //!
    //! Specifically, `on(f, g)` is a function such that
    //! @code
    //!     on(f, g)(x...) == f(g(x)...)
    //! @endcode
    //!
    //! For convenience, `on` also supports infix application as provided
    //! by `infix`.
    //! @snippet example/functional/on/infix.cpp main
    //!
    //! @note
    //! `on` is associative, i.e. `on(f, on(g, h))` is equivalent to
    //! `on(on(f, g), h)`.
    //!
    //! @internal
    //! ### Proof of associativity
    //!
    //! @code
    //!     on(f, on(g, h))(xs...) == f(on(g, h)(xs)...)
    //!                            == f(g(h(xs))...)
    //!
    //!     on(on(f, g), h)(xs...) == on(f, g)(h(xs)...)
    //!                            == f(g(h(xs))...)
    //! @endcode
    //! @endinternal
    //!
    //! ### Example
    //! @snippet example/functional/on/multiply.cpp main
    //!
    //! ### Example
    //! @snippet example/functional/on/plus.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto on = infix([](auto f, auto g) {
        return [f(detail::std::move(f)), g(detail::std::move(g))]
               (auto&& ...x) -> decltype(auto) {
            return f(g(detail::std::forward<decltype(x)>(x))...);
        };
    });
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_ON_HPP

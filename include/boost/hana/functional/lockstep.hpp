/*!
@file
Defines `boost::hana::lockstep`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_LOCKSTEP_HPP
#define BOOST_HANA_FUNCTIONAL_LOCKSTEP_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/closure.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Invoke a function with the result of invoking other functions on its
    //! arguments, in lockstep.
    //!
    //! Specifically, `lockstep(f)(g1, ..., gN)` is a function such that
    //! @code
    //!     lockstep(f)(g1, ..., gN)(x1, ..., xN) == f(g1(x1), ..., gN(xN))
    //! @endcode
    //!
    //! Since each `g` is invoked on its corresponding argument in lockstep,
    //! the number of arguments must match the number of `g`s.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/functional.cpp lockstep
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto lockstep = [](auto&& f, auto&& ...g) {
        return [perfect-capture](auto&& ...x) -> decltype(auto) {
            return forwarded(f)(forwarded(g)(forwarded(x))...);
        };
    };
#else
    template <typename Indices, typename F, typename ...G>
    struct _lockstep;

    template <std::size_t ...n, typename F, typename ...G>
    struct _lockstep<std::index_sequence<n...>, F, G...>
        : detail::closure<F, G...>
    {
        using detail::closure<F, G...>::closure;

        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) const& {
            return detail::get<0>(*this)(
                detail::get<n+1>(*this)(static_cast<X&&>(x))...
            );
        }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) & {
            return detail::get<0>(*this)(
                detail::get<n+1>(*this)(static_cast<X&&>(x))...
            );
        }
#endif

        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) && {
            return static_cast<F&&>(detail::get<0>(*this))(
                static_cast<G&&>(detail::get<n+1>(*this))(static_cast<X&&>(x))...
            );
        }
    };

    template <typename F>
    struct _pre_lockstep {
        F f;

        template <typename ...G>
        constexpr _lockstep<std::make_index_sequence<sizeof...(G)>, F,
                            typename std::decay<G>::type...>
        operator()(G&& ...g) const& {
            return {this->f, static_cast<G&&>(g)...};
        }

        template <typename ...G>
        constexpr _lockstep<std::make_index_sequence<sizeof...(G)>, F,
                            typename std::decay<G>::type...>
        operator()(G&& ...g) && {
            return {static_cast<F&&>(this->f), static_cast<G&&>(g)...};
        }
    };

    constexpr detail::create<_pre_lockstep> lockstep{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_LOCKSTEP_HPP

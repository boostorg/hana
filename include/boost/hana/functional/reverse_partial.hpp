/*!
@file
Defines `boost::hana::reverse_partial`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_REVERSE_PARTIAL_HPP
#define BOOST_HANA_FUNCTIONAL_REVERSE_PARTIAL_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/closure.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Partially apply a function to some arguments.
    //!
    //! Given a function `f` and some arguments, `reverse_partial` returns a
    //! new function corresponding to `f` whose last arguments are partially
    //! applied. Specifically, `reverse_partial(f, x...)` is a function such
    //! that
    //! @code
    //!     reverse_partial(f, x...)(y...) == f(y..., x...)
    //! @endcode
    //!
    //! @note
    //! The arity of `f` must match the total number of arguments passed to
    //! it, i.e. `sizeof...(x) + sizeof...(y)`.
    //!
    //!
    //! Example
    //! -------
    //! @include example/functional/reverse_partial.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto reverse_partial = [](auto&& f, auto&& ...x) {
        return [perfect-capture](auto&& ...y) -> decltype(auto) {
            return forwarded(f)(forwarded(y)..., forwarded(x)...);
        };
    };
#else
    template <typename Indices, typename F, typename ...X>
    struct reverse_partial_t;

    template <std::size_t ...n, typename F, typename ...X>
    struct reverse_partial_t<std::index_sequence<n...>, F, X...>
        : detail::closure<F, X...>
    {
        using detail::closure<F, X...>::closure;

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) const& {
            return detail::get<0>(*this)(
                static_cast<Y&&>(y)...,
                detail::get<n+1>(*this)...
            );
        }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) & {
            return detail::get<0>(*this)(
                static_cast<Y&&>(y)...,
                detail::get<n+1>(*this)...
            );
        }
#endif

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) && {
            return static_cast<F&&>(detail::get<0>(*this))(
                static_cast<Y&&>(y)...,
                static_cast<X&&>(detail::get<n+1>(*this))...
            );
        }
    };

    struct make_reverse_partial_t {
        template <typename F, typename ...X>
        constexpr reverse_partial_t<
            std::make_index_sequence<sizeof...(X)>,
            typename std::decay<F>::type,
            typename std::decay<X>::type...
        > operator()(F&& f, X&& ...x) const {
            return {static_cast<F&&>(f), static_cast<X&&>(x)...};
        }
    };

    constexpr make_reverse_partial_t reverse_partial{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_REVERSE_PARTIAL_HPP

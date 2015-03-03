/*!
@file
Defines `boost::hana::detail::variadic::foldl`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_FOLDL_HPP
#define BOOST_HANA_DETAIL_VARIADIC_FOLDL_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/variadic/foldl1.hpp>
#include <boost/hana/fwd/type.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
#ifndef BOOST_HANA_CONFIG_HAS_CXX1Z_FOLD_EXPRESSIONS
    template <typename ...Xs, typename F, typename S>
    constexpr decltype(auto) foldl_impl(F&& f, S&& s, ...) {
        return foldl1(
            detail::std::forward<F>(f),
            detail::std::forward<S>(s),
            type<Xs>...
        );
    }

    template <typename ...Xs, typename F, typename S>
    constexpr decltype(auto) foldl_impl(F&& f, S, Type*) {
        return foldl1_t<typename S::type, Xs...>(
            detail::std::forward<F>(f)
        );
    }

    template <typename ...Xs, typename F, typename S>
    constexpr decltype(auto) foldl_t(F&& f, S&& s) {
        return foldl_impl<Xs...>(
            detail::std::forward<F>(f),
            detail::std::forward<S>(s),
            (datatype_t<S>*)0
        );
    }

    struct _foldl {
        template <typename F, typename S, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, S&& s, Xs&& ...xs) const {
            return foldl1(
                detail::std::forward<F>(f),
                detail::std::forward<S>(s),
                detail::std::forward<Xs>(xs)...
            );
        }
    };

    constexpr _foldl foldl{};

#else

    // Experiment with fold expressions; see N4191 at
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4191.html
    template <typename F, typename State>
    struct accumulator {
        F f;
        State state;

        template <typename X>
        constexpr decltype(auto) operator+(X&& x) const& {
            return detail::create<variadic::accumulator>{}(
                f, f(state, detail::std::forward<X>(x))
            );
        }

        template <typename X>
        constexpr decltype(auto) operator+(X&& x) & {
            return detail::create<variadic::accumulator>{}(
                f, f(state, detail::std::forward<X>(x))
            );
        }

        template <typename X>
        constexpr decltype(auto) operator+(X&& x) && {
            decltype(auto) result = f(detail::std::move(state),
                                      detail::std::forward<X>(x));
            return detail::create<variadic::accumulator>{}(
                detail::std::move(f),
                detail::std::forward<decltype(result)>(result)
            );
        }

        constexpr State const& get() const& { return state; }
        constexpr State & get() & { return state; }
        constexpr State get() && { return detail::std::move(state); }
    };

    struct _foldl {
        template <typename ...Xs>
        static constexpr decltype(auto) helper(Xs&& ...xs)
        { return (... + detail::std::forward<Xs>(xs)); }

        template <typename F, typename State, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, State&& state, Xs&& ...xs) const {
            return helper(
                detail::create<accumulator>{}(
                    detail::std::forward<F>(f),
                    detail::std::forward<State>(state)
                ),
                detail::std::forward<Xs>(xs)...
            ).get();
        }
    };

    constexpr _foldl foldl{};
#endif
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_FOLDL_HPP

/*!
@file
Defines `boost::hana::detail::left_folds::variadic`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_HPP
#define BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/left_folds/variadic_meta.hpp>
#include <boost/hana/detail/left_folds/variadic_unrolled.hpp>
#include <boost/hana/type.hpp>


namespace boost { namespace hana { namespace detail { namespace left_folds {
    template <bool is_mfc, typename ...xs>
    struct variadic_impl;

    template <>
    struct variadic_impl<false> {
        template <typename F, typename State, typename ...Xs>
        static constexpr auto apply(F f, State s, Xs ...xs)
        { return variadic_unrolled(f, s, xs...); }
    };

    template <>
    struct variadic_impl<true> {
        template <typename F, typename State, typename ...xs>
        static constexpr auto apply(F f, State state, xs...)
        { return variadic_impl<true, typename xs::type...>::apply(f, state); }

        template <typename F, typename State>
        static constexpr auto apply(F f, State state)
        { return state; }
    };

    template <typename ...xs>
    struct variadic_impl<false, xs...> {
        template <typename F, typename State>
        static constexpr auto apply(F f, State s)
        { return variadic_impl<false>::apply(f, s, type<xs>...); }
    };

    template <typename ...xs>
    struct variadic_impl<true, xs...> {
        template <typename f, typename state>
        static constexpr auto apply(f, state) {
            return type<
                variadic_meta<f::template apply, typename state::type, xs...>
            >;
        }
    };

    template <typename ...xs>
    struct variadic_t_impl {
        template <typename F, typename State>
        constexpr auto operator()(F f, State s) const {
            return variadic_impl<
                detail::is_metafunction_class<F>::value, xs...
            >::apply(f, s);
        }
    };

    template <typename ...xs>
    constexpr variadic_t_impl<xs...> variadic_t{};

    BOOST_HANA_CONSTEXPR_LAMBDA auto variadic = [](auto f, auto state, auto ...xs) {
        return variadic_impl<
            detail::is_metafunction_class<decltype(f)>::value
        >::apply(f, state, xs...);
    };
}}}} // end namespace boost::hana::detail::left_folds

#endif // !BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_HPP

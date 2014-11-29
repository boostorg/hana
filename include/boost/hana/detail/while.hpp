/*!
@file
Defines `boost::hana::detail::while_` and `boost::hana::detail::until`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_WHILE_HPP
#define BOOST_HANA_DETAIL_WHILE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/detail/std/decay.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana { namespace detail {
    template <bool is_constant>
    struct while_impl;

    struct _while {
        template <typename Pred, typename State, typename F>
        constexpr decltype(auto)
        operator()(Pred&& pred, State&& state, F&& f) const {
            decltype(auto) cond = pred(state);
            constexpr bool constant = is_a<Constant, datatype_t<decltype(cond)>>;
            return while_impl<constant>::apply(std::forward<decltype(cond)>(cond),
                                               std::forward<Pred>(pred),
                                               std::forward<State>(state),
                                               std::forward<F>(f));
        }
    };

    constexpr _while while_{};

    struct _until {
        template <typename Pred, typename State, typename F>
        constexpr decltype(auto)
        operator()(Pred&& pred, State&& state, F&& f) const {
            return while_(compose(not_, std::forward<Pred>(pred)),
                          std::forward<State>(state),
                          std::forward<F>(f));
        }
    };

    constexpr _until until{};

    template <>
    struct while_impl<true> {
        template <typename Cond, typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        apply(Cond cond, Pred&& pred, State&& state, F&& f) {
            constexpr bool cond_ = value(cond);
            return apply(bool_<cond_>,
                         std::forward<Pred>(pred),
                         std::forward<State>(state),
                         std::forward<F>(f));
        }

        template <typename Pred, typename State, typename F>
        static constexpr State
        apply(decltype(false_), Pred&& pred, State&& state, F&& f) {
            return std::forward<State>(state);
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        apply(decltype(true_), Pred&& pred, State&& state, F&& f) {
            decltype(auto) r = f(std::forward<State>(state));
            return while_(std::forward<Pred>(pred),
                          std::forward<decltype(r)>(r),
                          std::forward<F>(f));
        }
    };

    template <>
    struct while_impl<false> {
        template <typename Pred, typename State, typename F>
        static auto
        apply(bool condition, Pred&& pred, State&& state, F&& f)
            -> decltype(
                true ? f(std::forward<State>(state))
                     : std::forward<State>(state)
            )
        {
            if (condition) {
                decltype(auto) r = f(std::forward<State>(state));
                return while_(std::forward<Pred>(pred),
                              std::forward<decltype(r)>(r),
                              std::forward<F>(f));
            }
            else {
                return std::forward<State>(state);
            }
        }
    };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_WHILE_HPP

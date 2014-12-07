/*!
@file
Defines `boost::hana::Bool`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BOOL_HPP
#define BOOST_HANA_BOOL_HPP

#include <boost/hana/fwd/bool.hpp>

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/id.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    template <typename _>
    struct equal_impl<Bool, Bool, _> {
        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&)
        { return bool_<X::value == Y::value>; }
    };

    template <>
    struct Constant::instance<Bool> : Constant::mcd {
        template <typename C>
        static constexpr auto value_impl(C const&)
        { return C::value; }
    };

    template <>
    struct Logical::instance<Bool> : Logical::mcd {
        //! @bug
        //! We can't use perfect forwarding because of this bug:
        //! http://llvm.org/bugs/show_bug.cgi?id=20619
        template <typename T, typename E>
        static constexpr auto eval_if_impl(decltype(true_), T t, E)
        { return t(id); }

        template <typename T, typename E>
        static constexpr auto eval_if_impl(decltype(false_), T, E e)
        { return e(id); }

        template <typename C>
        static constexpr auto not_impl(C const&)
        { return bool_<!C::value>; }

        template <typename Pred, typename State, typename F>
        static constexpr State
        while_helper(decltype(false_), Pred&& pred, State&& state, F&& f) {
            return detail::std::forward<State>(state);
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        while_helper(decltype(true_), Pred&& pred, State&& state, F&& f) {
            decltype(auto) r = f(detail::std::forward<State>(state));
            return while_(detail::std::forward<Pred>(pred),
                          detail::std::forward<decltype(r)>(r),
                          detail::std::forward<F>(f));
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        while_impl(Pred&& pred, State&& state, F&& f) {
            auto cond = pred(state);
            return while_helper(cond,
                                detail::std::forward<Pred>(pred),
                                detail::std::forward<State>(state),
                                detail::std::forward<F>(f));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_BOOL_HPP

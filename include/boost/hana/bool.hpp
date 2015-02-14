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

#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct enabled_operators<Bool>
        : Comparable, Logical
    { };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Bool, Bool> {
        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&)
        { return bool_<X::value == Y::value>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct value_impl<Bool> {
        template <typename C>
        static constexpr auto apply(C const&)
        { return C::value; }
    };

    template <typename C>
    struct to_impl<Bool, C, when<
        models<Constant(C)>{} &&
        detail::std::is_same<typename C::value_type, bool>{}
    >>
        : embedding<>
    {
        template <typename X>
        static constexpr auto apply(X x) {
            constexpr bool v = hana::value(x);
            return bool_<v>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Logical
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct eval_if_impl<Bool> {
        //! @bug
        //! We can't use perfect forwarding because of this bug:
        //! http://llvm.org/bugs/show_bug.cgi?id=20619
        template <typename T, typename E>
        static constexpr auto apply(decltype(true_), T t, E)
        { return t(id); }

        template <typename T, typename E>
        static constexpr auto apply(decltype(false_), T, E e)
        { return e(id); }
    };

    template <>
    struct not_impl<Bool> {
        template <typename C>
        static constexpr auto apply(C const&)
        { return bool_<!C::value>; }
    };

    template <>
    struct while_impl<Bool> {
        template <typename Pred, typename State, typename F>
        static constexpr State
        while_helper(decltype(false_), Pred&& pred, State&& state, F&& f) {
            return detail::std::forward<State>(state);
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        while_helper(decltype(true_), Pred&& pred, State&& state, F&& f) {
            decltype(auto) r = f(detail::std::forward<State>(state));
            return hana::while_(detail::std::forward<Pred>(pred),
                                detail::std::forward<decltype(r)>(r),
                                detail::std::forward<F>(f));
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        apply(Pred&& pred, State&& state, F&& f) {
            auto cond = pred(state);
            return while_helper(cond,
                                detail::std::forward<Pred>(pred),
                                detail::std::forward<State>(state),
                                detail::std::forward<F>(f));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_BOOL_HPP

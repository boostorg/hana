/*!
@file
Defines `boost::hana::Logical`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_HPP
#define BOOST_HANA_LOGICAL_HPP

#include <boost/hana/fwd/logical.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>


namespace boost { namespace hana {
    struct Logical::mcd {
        //! @todo How to forward `x` here? Since the arguments to `if_` can be
        //! evaluated in any order, we have to be careful not to use `x` in
        //! a moved-from state.
        template <typename X, typename Y>
        static constexpr decltype(auto) or_impl(X&& x, Y&& y) {
            return if_(x, x, detail::std::forward<Y>(y));
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) and_impl(X&& x, Y&& y) {
            return if_(x, detail::std::forward<Y>(y), x);
        }

        //! @todo By using `always` here, we create a copy of both `t` and `e`,
        //! which is not very smart.
        template <typename C, typename T, typename E>
        static constexpr decltype(auto) if_impl(C&& c, T&& t, E&& e) {
            return eval_if(detail::std::forward<C>(c),
                always(detail::std::forward<T>(t)),
                always(detail::std::forward<E>(e))
            );
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        until_impl(Pred&& pred, State&& state, F&& f) {
            return while_(compose(not_, detail::std::forward<Pred>(pred)),
                          detail::std::forward<State>(state),
                          detail::std::forward<F>(f));
        }
    };

    namespace operators {
        //! Equivalent to `and_`.
        //! @relates boost::hana::Logical
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<Logical, datatype_t<X>>::value ||
            enable_operators<Logical, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator&&(X&& x, Y&& y) {
            return and_(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }

        //! Equivalent to `or_`.
        //! @relates boost::hana::Logical
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<Logical, datatype_t<X>>::value ||
            enable_operators<Logical, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator||(X&& x, Y&& y) {
            return or_(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }

        //! Equivalent to `not_`.
        //! @relates boost::hana::Logical
        template <typename X, typename = typename detail::std::enable_if<
            enable_operators<Logical, datatype_t<X>>::value
        >::type>
        constexpr decltype(auto) operator!(X&& x) {
            return not_(detail::std::forward<X>(x));
        }
    }

    //! Instance of `Logical` for objects of foreign types that can be
    //! implicitly converted to `bool`.
    //!
    //! Any foreign object that can be converted to `bool` implicitly is an
    //! instance of `Logical` by converting that object to `bool` and then
    //! using the obvious instance for `bool`.
    //!
    //! @bug
    //! We can't use perfect forwarding because of this bug:
    //! http://llvm.org/bugs/show_bug.cgi?id=20619
    template <typename L>
    struct Logical::instance<L, when_valid<
        decltype(detail::std::declval<L>() ? void() : void())
    >>
        : Logical::mcd
    {
        template <typename T, typename E>
        static constexpr auto eval_if_impl(bool cond, T t, E e) {
            return cond ? t(id) : e(id);
        }

        static constexpr bool not_impl(bool cond)
        { return !cond; }

        template <typename Pred, typename State, typename F>
        static auto while_impl(Pred&& pred, State&& state, F&& f)
            -> decltype(
                true ? f(detail::std::forward<State>(state))
                     : detail::std::forward<State>(state)
            )
        {
            if (pred(state)) {
                decltype(auto) r = f(detail::std::forward<State>(state));
                return while_(detail::std::forward<Pred>(pred),
                              detail::std::forward<decltype(r)>(r),
                              detail::std::forward<F>(f));
            }
            else {
                return detail::std::forward<State>(state);
            }
        }
    };

    template <typename T>
    struct Logical::instance<T, when<models<Logical(T)>{}>>
        : Logical::mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_HPP

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
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        //! Equivalent to `and_`.
        //! @relates boost::hana::Logical
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<Logical, datatype_t<X>>::value ||
            enable_operators<Logical, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator&&(X&& x, Y&& y) {
            return hana::and_(detail::std::forward<X>(x),
                              detail::std::forward<Y>(y));
        }

        //! Equivalent to `or_`.
        //! @relates boost::hana::Logical
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<Logical, datatype_t<X>>::value ||
            enable_operators<Logical, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator||(X&& x, Y&& y) {
            return hana::or_(detail::std::forward<X>(x),
                             detail::std::forward<Y>(y));
        }

        //! Equivalent to `not_`.
        //! @relates boost::hana::Logical
        template <typename X, typename = typename detail::std::enable_if<
            enable_operators<Logical, datatype_t<X>>::value
        >::type>
        constexpr decltype(auto) operator!(X&& x) {
            return hana::not_(detail::std::forward<X>(x));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // if_
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct if_impl : if_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct if_impl<L, when<condition>> {
        //! @todo By using `always` here, we create a copy of both `t` and `e`,
        //! which is not very smart.
        template <typename C, typename T, typename E>
        static constexpr decltype(auto) apply(C&& c, T&& t, E&& e) {
            return hana::eval_if(detail::std::forward<C>(c),
                hana::always(detail::std::forward<T>(t)),
                hana::always(detail::std::forward<E>(e))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // eval_if
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct eval_if_impl : eval_if_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct eval_if_impl<L, when<condition>> {
        static_assert(wrong<eval_if_impl<L>>{},
        "no definition of boost::hana::eval_if for the given data type");
    };

    //////////////////////////////////////////////////////////////////////////
    // while_
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct while_impl : while_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct while_impl<L, when<condition>> {
        static_assert(wrong<while_impl<L>>{},
        "no definition of boost::hana::while_ for the given data type");
    };

    //////////////////////////////////////////////////////////////////////////
    // until
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct until_impl : until_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct until_impl<L, when<condition>> {
        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto) apply(Pred&& pred, State&& state, F&& f) {
            return while_(compose(not_, detail::std::forward<Pred>(pred)),
                          detail::std::forward<State>(state),
                          detail::std::forward<F>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // not_
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct not_impl : not_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct not_impl<L, when<condition>> {
        static_assert(wrong<not_impl<L>>{},
        "no definition of boost::hana::not_ for the given data type");
    };

    //////////////////////////////////////////////////////////////////////////
    // and_
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct and_impl : and_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct and_impl<L, when<condition>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::if_(x, detail::std::forward<Y>(y), x);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // or_
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct or_impl : or_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct or_impl<L, when<condition>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            //! @todo How to forward `x` here? Since the arguments to `if_` can be
            //! evaluated in any order, we have to be careful not to use `x` in
            //! a moved-from state.
            return hana::if_(x, x, detail::std::forward<Y>(y));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for "implicitly convertible-to-bool" data types
    //////////////////////////////////////////////////////////////////////////
    template <typename L>
    struct models<Logical(L), when_valid<
        decltype(detail::std::declval<L>() ? void() : void())
    >>
        : detail::std::true_type
    { };

    template <typename L>
    struct eval_if_impl<L, when_valid<
        decltype(detail::std::declval<L>() ? void() : void())
    >> {
        template <typename T, typename E>
        static constexpr auto apply(bool cond, T t, E e)
        { return cond ? t(id) : e(id); }
    };

    template <typename L>
    struct not_impl<L, when_valid<
        decltype(detail::std::declval<L>() ? void() : void())
    >> {
        static constexpr bool apply(bool cond)
        { return !cond; }
    };

    template <typename L>
    struct while_impl<L, when_valid<
        decltype(detail::std::declval<L>() ? void() : void())
    >> {
        template <typename Pred, typename State, typename F>
        static auto apply(Pred&& pred, State&& state, F&& f)
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_HPP

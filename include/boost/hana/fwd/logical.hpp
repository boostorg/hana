/*!
@file
Forward declares `boost::hana::Logical`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_LOGICAL_HPP
#define BOOST_HANA_FWD_LOGICAL_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/variadic/foldl.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Logical` type class is for data types acting like a boolean.
    //!
    //! @anchor Logical_terminology
    //! ### Terminology
    //! Let `x` be a `Logical`. Then, we say that `x` is true-valued if and
    //! only if `or_(x, y) == x` for all `Logical`s `y`. Conversely, we say
    //! that `x` is false-valued if and only if `and_(x, y) == y` for all
    //! `Logical`s `y`.
    //!
    //! Also, we say that `x` is a _compile-time_ `Logical` if and only if
    //! it can have values of different types in both branches of the `if_`
    //! method.
    //!
    //!
    //! Minimal complete definition
    //! ----------------------------
    //! `eval_if`, `not_` and `while_`
    //!
    //!
    //! @bug
    //! We don't short-circuit right now. Don't forget to change the examples
    //! and unit tests when that's implemented.
    //!
    //! @todo
    //! Consider making this a real boolean algebra.
    struct Logical {
        BOOST_HANA_TYPECLASS(Logical);
        struct mcd;
        template <typename I>
        struct integral_constant_mcd;
    };

    //! Conditionally return one of two values based on a condition.
    //! @relates Logical
    //!
    //! Specifically, `then_` is returned iff `logical` is true-valued, and
    //! `else_` is returned otherwise. Note that some `Logical` instances
    //! may allow values of different types while others may require both
    //! values to have the same type.
    //!
    //!
    //! @param logical
    //! The condition determining which of the two values is returned.
    //!
    //! @param then_
    //! The value returned when `logical` is true-valued.
    //!
    //! @param else_
    //! The value returned when `logical` is false-valued.
    //!
    //!
    //! ### Example
    //! @snippet example/logical/if.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto if_ = [](auto&& logical, auto&& then_, auto&& else_) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _if {
        template <typename L, typename T, typename E>
        constexpr decltype(auto) operator()(L&& l, T&& t, E&& e) const {
            return Logical::instance<
                datatype_t<L>
            >::if_impl(
                detail::std::forward<L>(l),
                detail::std::forward<T>(t),
                detail::std::forward<E>(e)
            );
        }
    };

    constexpr _if if_{};
#endif

    //! Conditionally execute one of two branches based on a condition.
    //! @relates Logical
    //!
    //! The selected branch will be invoked with an identity function, wich
    //! allows making types and values dependent inside a lambda and achieve
    //! a lazy-like behavior. However, type instantiation laziness can only
    //! be achieved with `Integral` conditions or equivalent. The result of
    //! the `eval_if` is the result of the invoked branch.
    //!
    //!
    //! @param logical
    //! The condition determining which of the two branches is selected.
    //!
    //! @param then_branch
    //! A function called as `then_branch([](auto x) { return x; })` iff
    //! `logical` is true-valued.
    //!
    //! @param else_branch
    //! A function called as `else_branch([](auto x) { return x; })` iff
    //! `logical` is false-valued.
    //!
    //!
    //! ### Example (purely compile-time condition)
    //! @snippet example/logical/eval_if.cpp heterogeneous
    //!
    //! ### Example (runtime or `constexpr` condition)
    //! @snippet example/logical/eval_if.cpp homogeneous
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto eval_if = [](auto&& logical, auto&& then_branch, auto&& else_branch) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _eval_if {
        template <typename L, typename T, typename E>
        constexpr decltype(auto) operator()(L&& l, T&& t, E&& e) const {
            return Logical::instance<
                datatype_t<L>
            >::eval_if_impl(
                detail::std::forward<L>(l),
                detail::std::forward<T>(t),
                detail::std::forward<E>(e)
            );
        }
    };

    constexpr _eval_if eval_if{};
#endif

    //! Apply a function to an initial state while some predicate is satisfied.
    //! @relates Logical
    //!
    //! Specifically, `while_(pred, state, f)` is equivalent to
    //! @code
    //!     f(...f(f(state)))
    //! @endcode
    //! where `f` is iterated as long as `pred(f(...))` is a true-valued
    //! `Logical`.
    //!
    //!
    //! @param pred
    //! A predicate called on the state or on the result of applying `f` a
    //! certain number of times to the state, and returning whether `f`
    //! should be applied one more time.
    //!
    //! @param state
    //! The initial state on which `f` is applied.
    //!
    //! @param f
    //! A function that is iterated on the initial state. Note that the
    //! return type of `f` may change from one iteration to the other,
    //! but only while `pred` returns a compile-time `Logical`. In other
    //! words, `decltype(f(stateN))` may differ from `decltype(f(stateN+1))`,
    //! but only if `pred(f(stateN))` returns a compile-time Logical.
    //!
    //!
    //! ### Example (purely compile-time condition)
    //! @snippet example/logical/while.cpp heterogeneous
    //!
    //! ### Example (runtime or `constexpr` condition)
    //! @snippet example/logical/while.cpp homogeneous
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto while_ = [](auto&& pred, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _while {
        template <typename Pred, typename State, typename F>
        constexpr decltype(auto) operator()(Pred&& pred, State&& state, F&& f) const {
            return Logical::instance<
                typename datatype<decltype(pred(state))>::type
            >::while_impl(
                detail::std::forward<Pred>(pred),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _while while_{};
#endif

    //! Apply a function to an initial state until some predicate is satisfied.
    //! @relates Logical
    //!
    //! Specifically, `until(pred, state, f)` is equivalent to
    //! @code
    //!     f(...f(f(state)))
    //! @endcode
    //! where `f` is iterated until `pred(f(...))` is a true-valued `Logical`.
    //!
    //!
    //! @param pred
    //! A predicate called on the state or on the result of applying `f` a
    //! certain number of times to the state, and returning whether `f`
    //! should stop being applied.
    //!
    //! @param state
    //! The initial state on which `f` is applied.
    //!
    //! @param f
    //! A function that is iterated on the initial state. Note that the
    //! return type of `f` may change from one iteration to the other,
    //! but only while `pred` returns a compile-time `Logical`. In other
    //! words, `decltype(f(stateN))` may differ from `decltype(f(stateN+1))`,
    //! but only if `pred(f(stateN))` returns a compile-time Logical.
    //!
    //!
    //! ### Example (purely compile-time condition)
    //! @snippet example/logical/until.cpp heterogeneous
    //!
    //! ### Example (runtime or `constexpr` condition)
    //! @snippet example/logical/until.cpp homogeneous
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto until = [](auto&& pred, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _until {
        template <typename Pred, typename State, typename F>
        constexpr decltype(auto) operator()(Pred&& pred, State&& state, F&& f) const {
            return Logical::instance<
                typename datatype<decltype(pred(state))>::type
            >::until_impl(
                detail::std::forward<Pred>(pred),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _until until{};
#endif

    //! Negates a `Logical`.
    //! @relates Logical
    //!
    //! Specifically, `not_(x)` returns a false-valued `Logical` iff `x`
    //! is a true-valued `Logical`, and a true-valued one otherwise.
    //!
    //! ### Example
    //! @snippet example/logical/not.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto not_ = [](auto&& logical) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _not {
        template <typename L>
        constexpr decltype(auto) operator()(L&& l) const {
            return Logical::instance<
                datatype_t<L>
            >::not_impl(
                detail::std::forward<L>(l)
            );
        }
    };

    constexpr _not not_{};
#endif

    //! Return whether all the arguments are true-valued.
    //! @relates Logical
    //!
    //! `and_` can be called with one argument or more. When called with
    //! two arguments, `and_` dispatches to the type class implementation.
    //! Otherwise,
    //! @code
    //!     and_(x) == x
    //!     and_(x, y, ...z) == and_(and_(x, y), z...)
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/logical/and.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto and_ = [](auto&& x, auto&& ...y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _and {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return Logical::instance<datatype_t<X>>::and_impl(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }

        template <typename X, typename ...Y>
        constexpr decltype(auto) operator()(X&& x, Y&& ...y) const {
            return detail::variadic::foldl(
                *this,
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)...
            );
        }
    };

    constexpr _and and_{};
#endif

    //! Return whether any of the arguments is true-valued.
    //! @relates Logical
    //!
    //! `or_` can be called with one argument or more. When called with
    //! two arguments, `or_` dispatches to the type class implementation.
    //! Otherwise,
    //! @code
    //!     or_(x) == x
    //!     or_(x, y, ...z) == or_(or_(x, y), z...)
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/logical/or.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto or_ = [](auto&& x, auto&& ...y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _or {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return Logical::instance<datatype_t<X>>::or_impl(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }

        template <typename X, typename ...Y>
        constexpr decltype(auto) operator()(X&& x, Y&& ...y) const {
            return detail::variadic::foldl(
                *this,
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)...
            );
        }
    };

    constexpr _or or_{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_LOGICAL_HPP

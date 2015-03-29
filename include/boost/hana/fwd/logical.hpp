/*!
@file
Forward declares `boost::hana::Logical`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_LOGICAL_HPP
#define BOOST_HANA_FWD_LOGICAL_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/fwd/core/operators.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Logical` concept represents types with a truth value.
    //!
    //! Intuitively, a `Logical` is just a `bool`, or something that can act
    //! like one. However, in the context of programming with heterogeneous
    //! objects, it becomes extremely important to distinguish between those
    //! objects whose truth value is known at compile-time, and those whose
    //! truth value is only known at runtime. The reason why this is so
    //! important is because it is possible to branch at compile-time on
    //! a condition whose truth value is known at compile-time, and hence
    //! the return type of the enclosing function can depend on that truth
    //! value. However, if the truth value is only known at runtime, then
    //! the compiler has to compile both branches (because any or both of
    //! them may end up being used), which creates the additional requirement
    //! that both branches must evaluate to the same type.
    //!
    //! Specifically, `Logical` (almost) represents a [boolean algebra][1],
    //! which is a mathematical structure encoding the usual properties that
    //! allow us to reason with `bool`. The exact properties that must be
    //! satisfied by any model of `Logical` are rigorously stated in the laws
    //! below.
    //!
    //!
    //! Truth, falsity and logical equivalence
    //! --------------------------------------
    //! A `Logical` `x` is said to be _true-valued_, or sometimes also just
    //! _true_ as an abuse of notation, if
    //! @code
    //!     if_(x, true, false) == true
    //! @endcode
    //!
    //! Similarly, `x` is _false-valued_, or sometimes just _false_, if
    //! @code
    //!     if_(x, true, false) == false
    //! @endcode
    //!
    //! This provides a standard way of converting any `Logical` to a straight
    //! `bool`. The notion of truth value suggests another definition, which
    //! is that of logical equivalence. We will say that two `Logical`s `x`
    //! and `y` are _logically equivalent_ if they have the same truth value.
    //! To denote that some expressions `p` and `q` of a Logical data type are
    //! logically equivalent, we will sometimes also write
    //! @code
    //!     p   if and only if   q
    //! @endcode
    //! which is very common in mathematics. The intuition behind this notation
    //! is that whenever `p` is true-valued, then `q` should be; but when `p`
    //! is false-valued, then `q` should be too. Hence, `p` should be
    //! true-valued when (and only when) `q` is true-valued.
    //!
    //!
    //! Laws
    //! ----
    //! As outlined above, the `Logical` concept almost represents a boolean
    //! algebra. The rationale for this laxity is to allow things like integers
    //! to act like `Logical`s, which is aligned with C++, even though they do
    //! not form a boolean algebra. Even though we depart from the usual
    //! axiomatization of boolean algebras, we have found through experience
    //! that the definition of a Logical given here is largely compatible with
    //! intuition.
    //!
    //! The following laws must be satisfied for any data type `L` modeling
    //! the `Logical` concept. Let `a`, `b` and `c` be objects of a `Logical`
    //! data type, and let `t` and `f` be arbitrary _true-valued_ and
    //! _false-valued_ `Logical`s of that data type, respectively. Then,
    //! @code
    //!     // associativity
    //!     or_(a, or_(b, c))   == or_(or_(a, b), c)
    //!     and_(a, and_(b, c)) == and_(and_(a, b), c)
    //!
    //!     // equivalence through commutativity
    //!     or_(a, b)   if and only if   or_(b, a)
    //!     and_(a, b)  if and only if   and_(b, a)
    //!
    //!     // absorption
    //!     or_(a, and_(a, b)) == a
    //!     and_(a, or_(a, b)) == a
    //!
    //!     // left identity
    //!     or_(a, f)  == a
    //!     and_(a, t) == a
    //!
    //!     // distributivity
    //!     or_(a, and_(b, c)) == and_(or_(a, b), or_(a, c))
    //!     and_(a, or_(b, c)) == or_(and_(a, b), and_(a, c))
    //!
    //!     // complements
    //!     or_(a, not_(a))  is true-valued
    //!     and_(a, not_(a)) is false-valued
    //! @endcode
    //!
    //! > #### Why is the above not a boolean algebra?
    //! > If you look closely, you will find that we depart from the usual
    //! > boolean algebras because:
    //! > 1. we do not require the elements representing truth and falsity to
    //! >    be unique
    //! > 2. we do not enforce commutativity of the `and_` and `or_` operations
    //! > 3. because we do not enforce commutativity, the identity laws become
    //! >    left-identity laws
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! 1. `eval_if`, `not_` and `while_`\n
    //! @todo
    //!
    //!
    //! Provided models
    //! ---------------
    //! 1. For arithmetic data types\n
    //! A data type `T` is arithmetic if `std::is_arithmetic<T>::value` is
    //! true. For an arithmetic data type `T`, a model of `Logical` is
    //! provided automatically by using the result of the builtin implicit
    //! conversion to `bool` as a truth value. Specifically, the minimal
    //! complete definition for those data types is
    //! @code
    //!     eval_if(cond, then, else_) = cond ? then(id) : else(id)
    //!     not_(cond) = static_cast<T>(cond ? false : true)
    //!     while_(pred, state, f) = equivalent to a normal while loop
    //! @endcode
    //!
    //! > #### Rationale for not providing a model for all contextually convertible to bool data types
    //! > The `not_` method can not be implemented in a meaningful way for all
    //! > of those types. For example, one can not cast a pointer type `T*`
    //! > to bool and then back again to `T*` in a meaningful way. With an
    //! > arithmetic type `T`, however, it is possible to cast from `T` to
    //! > bool and then to `T` again; the result will be `0` or `1` depending
    //! > on the truth value. If you want to use a pointer type or something
    //! > similar in a conditional, it is suggested to explicitly convert it
    //! > to bool by using `to<bool>`.
    //!
    //!
    //! Operators
    //! ---------
    //! For convenience, the following operators are provided as an
    //! equivalent way of calling the corresponding method:
    //! @code
    //!     &&  ->  and_
    //!     ||  ->  or_
    //!     !   -> not_
    //! @endcode
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Boolean_algebra_(structure)
    struct Logical { };

    //! Conditionally return one of two values based on a condition.
    //! @relates Logical
    //!
    //! Specifically, `then` is returned iff `cond` is true-valued, and
    //! `else_` is returned otherwise. Note that some `Logical` models may
    //! allow `then` and `else_` to have different types, while others may
    //! require both values to have the same type.
    //!
    //!
    //! @param cond
    //! The condition determining which of the two values is returned.
    //!
    //! @param then
    //! The value returned when `cond` is true-valued.
    //!
    //! @param else_
    //! The value returned when `cond` is false-valued.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/logical.cpp if_
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto if_ = [](auto&& cond, auto&& then, auto&& else_) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename L, typename = void>
    struct if_impl;

    struct _if {
        template <typename Cond, typename Then, typename Else>
        constexpr decltype(auto) operator()(Cond&& cond, Then&& then, Else&& else_) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Logical, typename datatype<Cond>::type>{},
            "hana::if_(cond, then, else) requires cond to be a Logical");
#endif
            return if_impl<typename datatype<Cond>::type>::apply(
                static_cast<Cond&&>(cond),
                static_cast<Then&&>(then),
                static_cast<Else&&>(else_)
            );
        }
    };

    constexpr _if if_{};
#endif

    //! Conditionally execute one of two branches based on a condition.
    //! @relates Logical
    //!
    //! Given a condition and two branches in the form of lambdas or Lazy
    //! expressions, `eval_if` will evaluate the branch selected by the
    //! condition with `eval` and return the result. The exact requirements
    //! for what the branches may be are the same requirements as those for
    //! the `eval` function.
    //!
    //!
    //! Deferring compile-time evaluation inside `eval_if`
    //! --------------------------------------------------
    //! By passing a unary callable to `eval_if`, it is possible to defer
    //! the compile-time evaluation of selected expressions inside the
    //! lambda. This is useful when instantiating a branch would trigger
    //! a compile-time error; we only want the branch to be instantiated
    //! when that branch is selected. Here's how it can be achieved.
    //!
    //! For simplicity, we'll use a unary lambda as our unary callable.
    //! Our lambda must accept a parameter (usually called `_`), which
    //! can be used to defer the compile-time evaluation of expressions
    //! as required. Here's an example:
    //! @snippet example/logical.cpp eval_if.fact
    //!
    //! What happens here is that `eval_if` will call `eval` on the selected
    //! branch. In turn, `eval` will call the selected branch either with
    //! nothing -- for the _then_ branch -- or with `hana::id` -- for the
    //! _else_ branch. Hence, `_(x)` is always the same as `x`, but the
    //! compiler can't tell until the lambda has been called! Hence, the
    //! compiler has to wait before it instantiates the body of the lambda
    //! and no infinite recursion happens. However, this trick to delay the
    //! instantiation of the lambda's body can only be used when the condition
    //! is known at compile-time, because otherwise both branches have to be
    //! instantiated inside the `eval_if` anyway.
    //!
    //! There are several caveats to note with this approach to lazy branching.
    //! First, because we're using lambdas, it means that the function's
    //! result can't be used in a constant expression. This is a limitation
    //! of the current language.
    //!
    //! The second caveat is that compilers currently have several bugs
    //! regarding deeply nested lambdas with captures. So you always risk
    //! crashing the compiler, but this is a question of time before it is
    //! not a problem anymore.
    //!
    //! Finally, it means that conditionals can't be written directly inside
    //! unevaluated contexts. The reason is that a lambda can't appear in an
    //! unevaluated context, for example in `decltype`. One way to workaround
    //! this is to completely lift your type computations into variable
    //! templates instead. For example, instead of writing
    //! @code
    //!     template <typename T>
    //!     struct pointerize : decltype(eval_if(traits::is_pointer(type<T>),
    //!             [] { return type<T>; },
    //!             [](auto _) { return _(traits::add_pointer)(type<T>); }
    //!         ))
    //!     { };
    //! @endcode
    //!
    //! you could instead write
    //!
    //! @snippet example/logical.cpp eval_if.pointerize
    //!
    //! > __Note__: This example would actually be implemented more easily
    //! > with partial specializations, but my bag of examples is empty
    //! > at the time of writing this.
    //!
    //! Now, this hoop-jumping only has to be done in one place, because
    //! you should use normal function notation everywhere else in your
    //! metaprogram to perform type computations. So the syntactic
    //! cost is amortized over the whole program.
    //!
    //!
    //! @param cond
    //! The condition determining which of the two branches is selected.
    //!
    //! @param then
    //! An expression called as `eval(then)` if `cond` is true-valued.
    //!
    //! @param else_
    //! A function called as `eval(else_)` if `cond` is false-valued.
    //!
    //!
    //! Example (purely compile-time condition)
    //! ---------------------------------------
    //! @snippet example/logical.cpp eval_if.heterogeneous
    //!
    //! Example (runtime or `constexpr` condition)
    //! ------------------------------------------
    //! @snippet example/logical.cpp eval_if.homogeneous
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto eval_if = [](auto&& cond, auto&& then, auto&& else_) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename L, typename = void>
    struct eval_if_impl;

    struct _eval_if {
        template <typename Cond, typename Then, typename Else>
        constexpr decltype(auto) operator()(Cond&& cond, Then&& then, Else&& else_) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Logical, typename datatype<Cond>::type>{},
            "hana::eval_if(cond, then, else) requires cond to be a Logical");
#endif
            return eval_if_impl<typename datatype<Cond>::type>::apply(
                static_cast<Cond&&>(cond),
                static_cast<Then&&>(then),
                static_cast<Else&&>(else_)
            );
        }
    };

    constexpr _eval_if eval_if{};
#endif

    //! Apply a function to an initial state while some predicate is satisfied.
    //! @relates Logical
    //!
    //! This method is a natural extension of the `while` language construct
    //! to manipulate a state whose type may change from one iteration to
    //! another. However, note that having a state whose type changes from
    //! one iteration to the other is only possible as long as the predicate
    //! returns a `Logical` whose truth value is known at compile-time.
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
    //! but only if `pred(f(stateN))` returns a compile-time `Logical`.
    //!
    //!
    //! Example (purely compile-time condition)
    //! ---------------------------------------
    //! @snippet example/logical.cpp while.heterogeneous
    //!
    //! Example (runtime or `constexpr` condition)
    //! ------------------------------------------
    //! @snippet example/logical.cpp while.homogeneous
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto while_ = [](auto&& pred, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename L, typename = void>
    struct while_impl;

    struct _while {
        template <typename Pred, typename State, typename F>
        constexpr decltype(auto) operator()(Pred&& pred, State&& state, F&& f) const {
            using Cond = decltype(pred(state));

#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Logical, typename datatype<Cond>::type>{},
            "hana::while_(pred, state, f) requires pred(state) to be a Logical");
#endif
            return while_impl<typename datatype<Cond>::type>::apply(
                static_cast<Pred&&>(pred),
                static_cast<State&&>(state),
                static_cast<F&&>(f)
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
    //! but only if `pred(f(stateN))` returns a compile-time `Logical`.
    //!
    //!
    //! Example (purely compile-time condition)
    //! ---------------------------------------
    //! @snippet example/logical.cpp until.heterogeneous
    //!
    //! Example (runtime or `constexpr` condition)
    //! ------------------------------------------
    //! @snippet example/logical.cpp until.homogeneous
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto until = [](auto&& pred, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename L, typename = void>
    struct until_impl;

    struct _until {
        template <typename Pred, typename State, typename F>
        constexpr decltype(auto) operator()(Pred&& pred, State&& state, F&& f) const {
            using Cond = decltype(pred(state));

#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Logical, typename datatype<Cond>::type>{},
            "hana::until(pred, state, f) requires pred(state) to be a Logical");
#endif
            return until_impl<typename datatype<Cond>::type>::apply(
                static_cast<Pred&&>(pred),
                static_cast<State&&>(state),
                static_cast<F&&>(f)
            );
        }
    };

    constexpr _until until{};
#endif

    //! Negates a `Logical`.
    //! @relates Logical
    //!
    //! This method returns a `Logical` of the same data type, but whose
    //! truth-value is negated. Specifically, `not_(x)` returns a false-valued
    //! `Logical` if `x` is a true-valued `Logical`, and a true-valued one
    //! otherwise.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/logical.cpp not_
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto not_ = [](auto&& x) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename L, typename = void>
    struct not_impl;

    struct _not {
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const {
            return not_impl<typename datatype<X>::type>::apply(
                static_cast<X&&>(x)
            );
        }
    };

    constexpr _not not_{};
#endif

    //! Return whether all the arguments are true-valued.
    //! @relates Logical
    //!
    //! `and_` can be called with one argument or more. When called with
    //! two arguments, `and_` uses tag-dispatching to find the right
    //! implementation. Otherwise,
    //! @code
    //!     and_(x) == x
    //!     and_(x, y, ...z) == and_(and_(x, y), z...)
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/logical.cpp and_
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto and_ = [](auto&& x, auto&& ...y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename L, typename = void>
    struct and_impl;

    struct _and {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const;

        template <typename X, typename ...Y>
        constexpr decltype(auto) operator()(X&& x, Y&& ...y) const;
    };

    constexpr _and and_{};
#endif

    //! Return whether any of the arguments is true-valued.
    //! @relates Logical
    //!
    //! `or_` can be called with one argument or more. When called with
    //! two arguments, `or_` uses tag-dispatching to find the right
    //! implementation. Otherwise,
    //! @code
    //!     or_(x) == x
    //!     or_(x, y, ...z) == or_(or_(x, y), z...)
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/logical.cpp or_
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto or_ = [](auto&& x, auto&& ...y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename L, typename = void>
    struct or_impl;

    struct _or {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const;

        template <typename X, typename ...Y>
        constexpr decltype(auto) operator()(X&& x, Y&& ...y) const;
    };

    constexpr _or or_{};
#endif

    template <>
    struct operators::of<Logical>
        : decltype(and_), decltype(or_), decltype(not_)
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_LOGICAL_HPP

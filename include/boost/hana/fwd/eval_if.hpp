/*!
@file
Forward declares `boost::hana::eval_if`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EVAL_IF_HPP
#define BOOST_HANA_FWD_EVAL_IF_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
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
    struct eval_if_impl : eval_if_impl<L, when<true>> { };

    struct eval_if_t {
        template <typename Cond, typename Then, typename Else>
        constexpr decltype(auto) operator()(Cond&& cond, Then&& then, Else&& else_) const;
    };

    constexpr eval_if_t eval_if{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EVAL_IF_HPP

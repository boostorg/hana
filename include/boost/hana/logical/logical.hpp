/*!
@file
Forward declares `boost::hana::Logical`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_LOGICAL_HPP
#define BOOST_HANA_LOGICAL_LOGICAL_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
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
    //! @bug
    //! We don't short-circuit right now. Don't forget to change the examples
    //! and unit tests when that's implemented.
    //!
    //! @todo
    //! Consider making this a real boolean algebra.
    struct Logical {
        BOOST_HANA_TYPECLASS(Logical);
        struct mcd;
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
    BOOST_HANA_CONSTEXPR_LAMBDA auto if_ = [](auto&& logical, auto&& then_, auto&& else_) -> decltype(auto) {
        return Logical::instance<
            datatype_t<decltype(logical)>
        >::if_impl(
            detail::std::forward<decltype(logical)>(logical),
            detail::std::forward<decltype(then_)>(then_),
            detail::std::forward<decltype(else_)>(else_)
        );
    };

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
    BOOST_HANA_CONSTEXPR_LAMBDA auto eval_if = [](auto&& logical, auto&& then_branch, auto&& else_branch) -> decltype(auto) {
        return Logical::instance<
            datatype_t<decltype(logical)>
        >::eval_if_impl(
            detail::std::forward<decltype(logical)>(logical),
            detail::std::forward<decltype(then_branch)>(then_branch),
            detail::std::forward<decltype(else_branch)>(else_branch)
        );
    };

    //! Negates a `Logical`.
    //! @relates Logical
    //!
    //! Specifically, `not_(x)` returns a false-valued `Logical` iff `x`
    //! is a true-valued `Logical`, and a true-valued one otherwise.
    //!
    //! ### Example
    //! @snippet example/logical/not.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto not_ = [](auto&& logical) -> decltype(auto) {
        return Logical::instance<
            datatype_t<decltype(logical)>
        >::not_impl(detail::std::forward<decltype(logical)>(logical));
    };

    namespace logical_detail {
        BOOST_HANA_CONSTEXPR_LAMBDA auto and2 = [](auto&& x, auto&& y) -> decltype(auto) {
            return Logical::instance<datatype_t<decltype(x)>>::and_impl(
                detail::std::forward<decltype(x)>(x),
                detail::std::forward<decltype(y)>(y)
            );
        };

        BOOST_HANA_CONSTEXPR_LAMBDA auto or2 = [](auto&& x, auto&& y) -> decltype(auto) {
            return Logical::instance<datatype_t<decltype(x)>>::or_impl(
                detail::std::forward<decltype(x)>(x),
                detail::std::forward<decltype(y)>(y)
            );
        };
    }

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
    BOOST_HANA_CONSTEXPR_LAMBDA auto and_ = [](auto&& x, auto&& ...y) -> decltype(auto) {
        return detail::variadic::foldl(
            logical_detail::and2,
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)...
        );
    };

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
    BOOST_HANA_CONSTEXPR_LAMBDA auto or_ = [](auto&& x, auto&& ...y) -> decltype(auto) {
        return detail::variadic::foldl(
            logical_detail::or2,
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)...
        );
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_LOGICAL_HPP

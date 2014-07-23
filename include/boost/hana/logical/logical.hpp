/*!
@file
Forward declares `boost::hana::Logical`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_LOGICAL_HPP
#define BOOST_HANA_LOGICAL_LOGICAL_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    /*!
    @ingroup typeclasses
    The `Logical` type class is for data types acting like a boolean.

    -------------------------------------------------------------------------

    @todo
    - Use a non-naive implementation for variadic `and_` and `or_`.
    - Consider making this a real boolean algebra.

    @bug
    We don't short-circuit right now. Don't forget to change the examples and
    unit tests when that's implemented.
     */
    struct Logical {
        BOOST_HANA_TYPECLASS(Logical);
        struct mcd;
    };

    //! Conditionally return one of two values based on a condition.
    //! @method{Logical}
    BOOST_HANA_CONSTEXPR_LAMBDA auto if_ = [](auto logical, auto then_, auto else_) {
        return Logical::instance<
            datatype_t<decltype(logical)>
        >::if_impl(logical, then_, else_);
    };

    //! Conditionally execute one of two branches based on a condition.
    //! @method{Logical}
    //!
    //! The selected branch will be invoked with an identity function, wich
    //! allows making types and values dependent inside a lambda and achieve
    //! a lazy-like behavior. However, type instantiation laziness can only
    //! be achieved with `Integral` conditions or equivalent.
    //!
    //! ### Example (purely compile-time condition)
    //! @snippet example/integral/logical/eval_if.cpp main
    //!
    //! ### Example (runtime or `constexpr` condition)
    //! @snippet example/logical/default_instance/eval_if.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto eval_if = [](auto logical, auto then_branch, auto else_branch) {
        return Logical::instance<
            datatype_t<decltype(logical)>
        >::eval_if_impl(logical, then_branch, else_branch);
    };

    //! Negates a `Logical`.
    //! @method{Logical}
    BOOST_HANA_CONSTEXPR_LAMBDA auto not_ = [](auto logical) {
        return Logical::instance<
            datatype_t<decltype(logical)>
        >::not_impl(logical);
    };

    struct _and {
        template <typename X, typename Y>
        constexpr auto operator()(X x, Y y) const
        { return Logical::instance<datatype_t<decltype(x)>>::and_impl(x, y); }

        template <typename X, typename Y, typename ...Z>
        constexpr auto operator()(X x, Y y, Z ...z) const
        { return (*this)(x, (*this)(y, z...)); }

        template <typename X>
        constexpr auto operator()(X x) const
        { return x; }
    };

    //! Return whether all the arguments are true-valued.
    //! @method{Logical}
    //!
    //! `and_` can be called with one argument or more. When called with
    //! two arguments, `and_` dispatches to the type class implementation.
    //! Otherwise,
    //! @code
    //!     and_(x) == x
    //!     and_(x, y, ...z) == and_(x, and_(y, z...))
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/integral/logical/and.cpp main
    constexpr _and and_{};

    struct _or {
        template <typename X, typename Y>
        constexpr auto operator()(X x, Y y) const
        { return Logical::instance<datatype_t<decltype(x)>>::or_impl(x, y); }

        template <typename X, typename Y, typename ...Z>
        constexpr auto operator()(X x, Y y, Z ...z) const
        { return (*this)(x, (*this)(y, z...)); }

        template <typename X>
        constexpr auto operator()(X x) const
        { return x; }
    };

    //! Return whether any of the arguments is true-valued.
    //! @method{Logical}
    //!
    //! `or_` can be called with one argument or more. When called with
    //! two arguments, `or_` dispatches to the type class implementation.
    //! Otherwise,
    //! @code
    //!     or_(x) == x
    //!     or_(x, y, ...z) == or_(x, or_(y, z...))
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/integral/logical/or.cpp main
    constexpr _or or_{};

    namespace operators {
        //! Equivalent to `and_`.
        //! @method{boost::hana::Logical}
        template <typename X, typename Y>
        constexpr auto operator&&(X x, Y y)
        { return and_(x, y); }

        //! Equivalent to `or_`.
        //! @method{boost::hana::Logical}
        template <typename X, typename Y>
        constexpr auto operator||(X x, Y y)
        { return or_(x, y); }

        //! Equivalent to `not_`.
        //! @method{boost::hana::Logical}
        template <typename X>
        constexpr auto operator!(X x)
        { return not_(x); }
    }

    //! Minimal complete definition: `eval_if` and `not_`
    struct Logical::mcd {
        template <typename X, typename Y>
        static constexpr auto or_impl(X x, Y y)
        { return if_(x, x, y); }

        template <typename X, typename Y>
        static constexpr auto and_impl(X x, Y y)
        { return if_(x, y, x); }

        template <typename C, typename T, typename E>
        static constexpr auto if_impl(C c, T t, E e) {
            return eval_if(c,
                [=](auto) { return t; },
                [=](auto) { return e; }
            );
        }
    };

    template <typename T>
    struct Logical::instance<T, when_valid<decltype(*(T*)0 ? (void)0 : (void)0)>>
        : Logical::mcd
    {
        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(bool cond, Then t, Else e) {
            auto id = [](auto x) { return x; };
            return cond ? t(id) : e(id);
        }

        static constexpr auto not_impl(bool cond)
        { return !cond; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_LOGICAL_HPP

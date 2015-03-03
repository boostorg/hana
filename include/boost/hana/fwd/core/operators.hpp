/*!
@file
Forward declares `boost::hana::operators` and related utilities.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CORE_OPERATORS_HPP
#define BOOST_HANA_FWD_CORE_OPERATORS_HPP

namespace boost { namespace hana {
    //! @ingroup group-core
    //! Namespace containing operators provided as an alias to some methods.
    //!
    //! Some methods provide equivalent ways of being called by using
    //! operators, which are sometimes more expressive. Those operators
    //! are contained in the namespace `boost::hana::operators`. In all
    //! cases, the operators must be explicitly asked for in order to be
    //! enabled for objects of a data type. This works by specializing the
    //! `has_operator` metafunction, or by doing anything equivalent as
    //! documented by `has_operator`. However, unary and binary operators are
    //! handled a bit differently, as is explained next.
    //!
    //! For a unary operator `@` aliasing a method `m` and an argument
    //! of a data type `T`, the operator is enabled only when
    //! `has_operator<T, decltype(m)>` is true. For a binary operator `@`
    //! aliasing a method `m` and two arguments of data type `T` and `U`,
    //! the operator is enabled when either `has_operator<T, decltype(m)>`
    //! or `has_operator<U, decltype(m)>` is true.
    //!
    //! To use the operators in this namespace, one may either use
    //! `operators::adl` or make them visible to name lookup by importing
    //! them all explicitly
    //! @code
    //!     using namespace boost::hana::operators;
    //! @endcode
    //! or by importing them selectively
    //! @code
    //!     using boost::hana::operators::operator+;
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @include example/core/operators.cpp
    namespace operators {
        //! @ingroup group-core
        //! Makes the operators from `boost::hana::operators` visible to ADL.
        //!
        //! Given a type `T`, the operators in `boost::hana::operators` can
        //! be made usable with objects of type `T` in two different ways.
        //! First, making `operators::adl` a base class of the type `T` will
        //! add the `hana::operators` namespace to the set of associated
        //! namespaces that are considered during argument dependent lookup,
        //! which has the effect of making those operators available for
        //! objects of type `T`. Otherwise, using `operators::adl` as a
        //! template parameter of a template will also achieve the same
        //! effect. For example, if one has a template type `T` declared as
        //! @code
        //!     template <typename ...>
        //!     struct T;
        //! @endcode
        //! , using `operators::adl` as a template parameter will cause
        //! `T<..., operators::adl, ...>` to have the `hana::operators`
        //! namespace in its set of associated namespaces for ADL.
        //!
        //! While `operators::adl` makes the operators visible to lookup for
        //! the objects of a type, it is still required to enable them
        //! explicitly with `has_operator`.
        //!
        //!
        //! Example
        //! -------
        //! @include example/core/operators.cpp
        struct adl { };

        //! @ingroup group-core
        //! Controls the operator aliases that are enabled for a data type.
        //!
        //! Given a data type `T`, `operators::of<T>` should inherit from all
        //! the methods whose operator aliases are enabled for objects of data
        //! type `T`. The methods inherited by this trait are checked by the
        //! `has_operator` metafunction; if `operators::of<T>` inherits from
        //! `decltype(m)` for some method `m`, then
        //! `has_operator<T, decltype(m)>` will return true.
        //!
        //! However, while explicit specialization of `operators::of` is
        //! possible, another way of customizing this trait is provided.
        //! If a data type `T` (or a concept, see below) provides a nested
        //! `hana::operators` type, then `operators::of<T>` will inherit
        //! this instead. This makes it possible to enable operators for
        //! a data type without ever leaving the comfort of your namespace.
        //!
        //! Also, when a concept `Concept` has methods for which equivalent
        //! operators are provided, `operators::of<Concept>` should be
        //! specialized so it inherits from all its methods that possess an
        //! operator equivalent. For example, if `Concept` has methods `m1`,
        //! `m2` and `m3`, and if `m1` and `m2` have associated operators in
        //! the `hana::operators` namespace, then `Concept` should specialize
        //! `operators::of` as follows, or provide an equivalent
        //! `hana::operators` member type:
        //! @code
        //!     template <>
        //!     struct operators::of<Concept>
        //!         : decltype(m1), decltype(m2)
        //!     { };
        //! @endcode
        //!
        //! Hence, if a data type `T` wants to enable all the operators
        //! provided by `Concept`, `operators::of<T>` only needs to inherit
        //! `operators::of<Concept>`:
        //! @code
        //!     template <>
        //!     struct operators::of<T>
        //!         : operators::of<Concept>
        //!     { };
        //! @endcode
        //! instead of writing the following code, which is otherwise
        //! equivalent:
        //! @code
        //!     template <>
        //!     struct operators::of<T>
        //!         : decltype(m1), decltype(m2)
        //!     { };
        //! @endcode
        //!
        //! Also, given more than a single concept, `operators::of` will
        //! behave as
        //! @code
        //!     template <typename ...Concepts>
        //!     struct operators
        //!         : operators<Concepts>...
        //!     { };
        //! @endcode
        //! This makes it possible for a data type `T` to enable the operators
        //! provided by several concepts easily. For example, if `T` wants to
        //! enable the operators provided by concepts `C1`, `C2` and `C3`, the
        //! `operators::of` specialization only needs to look like:
        //! @code
        //!     template <>
        //!     struct operators::of<T>
        //!         : operators::of<C1, C2, C3>
        //!     { };
        //! @endcode
        //!
        //! instead of the following code, which is otherwise equivalent:
        //! @code
        //!     template <>
        //!     struct operators::of<T>
        //!         : operators::of<C1>, operators::of<C2>, operators::of<C3>
        //!     { };
        //! @endcode
        //!
        //!
        //! @note
        //! This trait only controls which operators are enabled when they
        //! are found by name lookup. To make the operators findable by name
        //! lookup, one should use `operators::adl` or import the operators
        //! from the operators` namespace explicitly.
        //!
        //!
        //! Example
        //! -------
        //! @include example/core/operators.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
        template <see documentation>
        struct of;
#else
        template <typename ...Concepts>
        struct of;
#endif
    } // end namespace operators

    //! @ingroup group-core
    //! Returns whether the operator associated to a given method is enabled
    //! for the given data type.
    //!
    //! Given a data type `T` and the type `decltype(m)` of a method `m`,
    //! `has_operator` returns whether `m` has an operator associated to it
    //! and whether that operator is enabled for objects of data type `T`,
    //! as documented by `operators::of`.
    //!
    //!
    //! Example
    //! -------
    //! @include example/core/operators.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Datatype, typename Method>
    struct has_operator;
#else
    template <typename Datatype, typename Method, typename = void>
    struct has_operator;
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CORE_OPERATORS_HPP

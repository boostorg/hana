/*!
@file
Defines `boost::hana::operators`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_OPERATORS_HPP
#define BOOST_HANA_CORE_OPERATORS_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! @ingroup group-core
    //! Namespace containing custom operators provided by some type classes.
    //!
    //! Operators in this namespace are enabled only when explicitly asked
    //! for with the `enabled_operators` trait. To use the operators in this
    //! namespace, one may either use `operators::enable_adl` or make them
    //! visible to name lookup by importing them all explicitly
    //! @code
    //!     using namespace boost::hana::operators;
    //! @endcode
    //!
    //! or by importing them selectively
    //! @code
    //!     using boost::hana::operators::operator+;
    //! @endcode
    //!
    //!
    //! ### Example
    //! @include example/core/operators.cpp
    namespace operators {
        //! @ingroup group-core
        //! Allows custom Hana operators to be used with objects of a type.
        //!
        //! Specifically, by inheriting from `enable_adl` or by using it as a
        //! template parameter, one makes the custom operators defined in the
        //! `operators` namespace accessible via argument-dependent lookup for
        //! objects of that type.
        //!
        //! Then, operators enabled for the data type of those objects can be
        //! used without importing the operators explicitly in the scope of
        //! usage.
        struct enable_adl { };
    }

    namespace core_detail {
        template <typename Datatype, typename = void*>
        struct default_enabled_operators { };

        template <typename Datatype>
        struct default_enabled_operators<Datatype, decltype((void*)(
            (typename Datatype::hana_enabled_operators*)0
        ))>
            : Datatype::hana_enabled_operators
        { };

        template <typename Typeclass>
        constexpr bool enable_operators_impl(Typeclass*) { return true; }

        template <typename Typeclass>
        constexpr bool enable_operators_impl(...) { return false; }
    }

    //! @ingroup group-core
    //! Trait describing the custom operators enabled for a data type.
    //!
    //! Specifically, `enabled_operators<T>` inherits from all the type
    //! classes for which custom operators (in namespace `operators`)
    //! should be provided for objects of data type `T`.
    //!
    //! The trait may be specialized for user-defined data types. Note that
    //! specializations may use `when` to provide flexible specializations.
    //! If the trait is not specialized, it is equivalent to
    //! `T::hana_enabled_operators` if that expression is valid,
    //! and to an empty struct otherwise.
    //!
    //! This trait only controls which operators are enabled when they are
    //! found by name lookup. To make the operators findable, one should use
    //! `operators::enable_adl` or import the operators from the `operators`
    //! namespace explicitly.
    template <typename Datatype, typename = void>
    struct enabled_operators
        //! @cond
        : enabled_operators<Datatype, when<true>>
        //! @endcond
    { };

    template <typename Datatype, bool condition>
    struct enabled_operators<Datatype, when<condition>>
        : core_detail::default_enabled_operators<Datatype>
    { };

    //! @ingroup group-core
    //! Return whether the custom operators of a type class are enabled for
    //! the given data type.
    //!
    //! Specifically, `enable_operators<Typeclass, Datatype>` provides a
    //! nested `static constexpr bool value` member representing whether
    //! `enabled_operators<Datatype>` inherits from `Typeclass`.
    template <typename Typeclass, typename Datatype>
    struct enable_operators {
        static constexpr bool value = core_detail::enable_operators_impl<Typeclass>(
            (enabled_operators<Datatype>*)0
        );
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_OPERATORS_HPP

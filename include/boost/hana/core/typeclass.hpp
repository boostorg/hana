/*!
@file
Defines utilities to create type classes.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_TYPECLASS_HPP
#define BOOST_HANA_CORE_TYPECLASS_HPP

namespace boost { namespace hana {
    struct disable;

    namespace core_detail {
        template <typename Void, typename Typeclass, typename ...Args>
        struct default_instance {
            using type = disable;
        };

        template <typename Typeclass, typename ...Args>
        struct default_instance<
            decltype((void)(typename Typeclass::template default_instance<Args...>*)0),
            Typeclass, Args...
        > {
            using type = typename Typeclass::template default_instance<Args...>;
        };
    }

    //! @ingroup group-core
    //! Defines a unary type class.
    //! @hideinitializer
    //!
    //! Use this macro at public scope when defining a type class to create
    //! the boilerplate necessary for a unary type class.
    //!
    //! ### Example
    //! @include example/core/typeclass/unary_typeclass.cpp
    #define BOOST_HANA_TYPECLASS(NAME)                                      \
        /** @cond */                                                        \
        template <typename T, typename ...>                                 \
        struct instance                                                     \
            : instance<T, ::boost::hana::when<true>>                        \
        { };                                                                \
                                                                            \
        template <typename T, bool condition>                               \
        struct instance<T, ::boost::hana::when<condition>>                  \
            : ::boost::hana::core_detail::default_instance<                 \
                void, NAME, T                                               \
            >::type                                                         \
        { }                                                                 \
        /** @endcond */                                                     \
    /**/

    //! @ingroup group-core
    //! Defines a binary type class.
    //! @hideinitializer
    //!
    //! This is equivalent to `BOOST_HANA_TYPECLASS`, except it creates a
    //! type class accepting two data types. This is useful for type classes
    //! like `Comparable`, whose methods are binary and should be dispatched
    //! using the data types of both arguments.
    //!
    //! ### Example
    //! @include example/core/typeclass/binary_typeclass.cpp
    #define BOOST_HANA_BINARY_TYPECLASS(NAME)                               \
        /** @cond */                                                        \
        template <typename T, typename U, typename ...>                     \
        struct instance                                                     \
            : instance<T, U, ::boost::hana::when<true>>                     \
        { };                                                                \
                                                                            \
        template <typename T, typename U, bool condition>                   \
        struct instance<T, U, ::boost::hana::when<condition>>               \
            : ::boost::hana::core_detail::default_instance<                 \
                void, NAME, T, U                                            \
            >::type                                                         \
        { }                                                                 \
        /** @endcond */                                                     \
    /**/

    //! @ingroup group-core
    //! Enable a type class instance only if a boolean condition is true.
    //!
    //! @internal
    //! ### Rationale for using `when` instead of `std::enable_if`
    //! Using `when` is necessary for two reasons. First, a non-type template
    //! argument may not depend on a template parameter of a partial
    //! specialization, so we need to wrap the `bool` condition into a type.
    //! Second, `when` is used to implement the priority of partially
    //! specialized instances over predicated instances, but we could also
    //! achieve the same by replacing `when<true>` with `void` and letting
    //! people use `enable_if`. Hence, I'd say it boils down to preference
    //! at this point; I'm open for discussion about this.
    //! @endinternal
    //!
    //! ### Example
    //! @include example/core/typeclass/when.cpp
    template <bool condition>
    struct when { };

    //! @ingroup group-core
    //! Enable a type class instance only if an expression is well-formed.
    //!
    //! Specifically, this is equivalent to `when<true>`, but SFINAE will
    //! cause the partial specialization to fail when the expression is
    //! ill-formed.
    //!
    //! ### Example
    //! @include example/core/typeclass/when_valid.cpp
    template <typename ...>
    using when_valid = when<true>;

    //! @ingroup group-core
    //! Explicitly disable a type class instance.
    //!
    //! This is meant as a way to disable a type class instance provided
    //! through a predicate which, for some reason, is not desirable for
    //! a given data type.
    //!
    //! ### Example
    //! @include example/core/typeclass/disable.cpp
    struct disable { };

    namespace operators {
        //! @ingroup group-core
        //! Allows operators in the `boost::hana::operators` namespace to be
        //! found by ADL.
        //!
        //! Use this as a dummy template parameter or base class to make
        //! operators in the `boost::hana::operators` namespace ADL-findable
        //! for a type.
        //!
        //! @note
        //! Nothing except operators should be defined in this namespace;
        //! otherwise, ambiguities can arise when `using namespace operators`.
        //!
        //! @todo
        //! Is ADL really the best way of providing custom operators? This has
        //! (at least) the problem that templated types which have nothing to
        //! do with Boost.Hana could have their set of associated namespaces
        //! augmented with `boost::hana::operators` in an undesirable way:
        //! @code
        //!     template <typename T>
        //!     struct nothing_to_do_with_hana { };
        //!
        //!     template <typename T, typename = operators::enable>
        //!     struct something_to_do_with_hana { };
        //!
        //!     nothing_to_do_with_hana<something_to_do_with_hana<int>> x{};
        //!     x == x; // tries to use Comparable::equal_impl
        //! @endcode
        struct enable { };
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_TYPECLASS_HPP

/*!
@file
Internal header to break circular dependencies.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_TYPECLASSES_HPP
#define BOOST_HANA_DETAIL_TYPECLASSES_HPP

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

    //! @ingroup core
    //! Defines a unary type class
    //!
    //! Use this macro at public scope when defining a type class to create
    //! the boilerplate necessary for a unary type class.
    //!
    //! ### Example
    //! @include example/core/typeclass.cpp
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

    //! @ingroup core
    //! Defines a binary type class
    //!
    //! This is equivalent to `BOOST_HANA_TYPECLASS`, except it creates a
    //! type class with two arguments.
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

    //! @ingroup core
    //! Used to instantiate a type class based on the value of a predicate.
    //!
    //! @note
    //! `when` is provided whenever the header of a type class is included;
    //! including boost/hana/core.hpp is not necessary in that case.
    //!
    //! @internal
    //! Using `when` is necessary for two reasons. First, a non-type
    //! template argument may not depend on a template parameter of a
    //! partial specialization, so we need to wrap the `bool` result of
    //! the predicate into a type. Second, `when` is used to implement the
    //! priority of partially specialized instances over predicated instances,
    //! but we could also achieve the same by replacing `when<true>` with
    //! `void` and letting people use `enable_if`.
    template <bool condition>
    struct when { };

    //! @ingroup core
    //! Used to instantiate a type class based on the validity of
    //! an expression.
    //!
    //! Specifically, this is equivalent to `when<true>`, but SFINAE will
    //! cause the partial specialization to fail when the expression is
    //! ill-formed.
    template <typename ...>
    using when_valid = when<true>;

    //! @ingroup core
    //! Explicitly disable a type class instance.
    //!
    //! This is meant as a way to disable a type class instance provided
    //! through a predicate which, for some reason, is not desirable for
    //! a given data type.
    //!
    //! ### Example
    //! @include example/core/disable.cpp
    struct disable { };

    namespace core_detail {
        template <typename T, typename Enable = void*>
        struct default_datatype { using type = T; };

        template <typename T>
        struct default_datatype<T, decltype((void*)(typename T::hana_datatype*)0)> {
            using type = typename T::hana_datatype;
        };
    }

    //! @ingroup core
    //! Metafunction returning the data type associated to `T`.
    //!
    //! By default, this metafunction returns `T::hana_datatype` if that
    //! expression is valid, and `T` otherwise. It can also be specialized
    //! to customize the data type of `T` without requiring `T` to have a
    //! nested `hana_datatype` type. A dummy parameter is also provided to
    //! allow `datatype` to be specialized for all types satisfying a predicate
    //! using `std::enable_if`.
    //!
    //! @todo
    //! - Could this be related to `decltype_`? If so, how? It is a valid
    //!   question whether `decltype_(list(...))` should be `List` or `<garbage>`.
    //! - Consider using two layers of specializations to improve performance
    //!   if this is an issue. I suspect that using the enabler will hurt
    //!   performance a lot because it requires the compiler to look at all
    //!   the enablers each time `datatype` is instantiated.
    template <typename T, typename Enable = void>
    struct datatype {
        using type = typename core_detail::default_datatype<T>::type;
    };

    //! @ingroup core
    //! Alias to `datatype<T>::%type`.
    template <typename T>
    using datatype_t = typename datatype<T>::type;

    namespace operators {
        //! @ingroup core
        //! Allows operators in the `boost::hana::operators` namespace to be
        //! found by ADL.
        //!
        //! Use this as a dummy template parameter or base class to make
        //! operators in the `boost::hana::operators` namespace ADL-findable
        //! for a type.
        //!
        //! @note
        //! boost/hana/core.hpp does not need to be included when the header
        //! of a type class providing operators has been included.
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

#endif // !BOOST_HANA_DETAIL_TYPECLASSES_HPP

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
    //! Machinery for creating a unary type class.
    //!
    //!
    //! ### Creating a type class
    //! Creating a new type class is done by using the `BOOST_HANA_TYPECLASS`
    //! macro inside a struct with the desired name:
    //! @code
    //!     struct Typeclass {
    //!         BOOST_HANA_TYPECLASS(Typeclass);
    //!     };
    //! @endcode
    //!
    //! If desired, methods can be provided by putting them inside a nested
    //! type of `Typeclass`:
    //! @code
    //!     struct Typeclass {
    //!         BOOST_HANA_TYPECLASS(Typeclass);
    //!         struct some_member {
    //!             // methods
    //!         };
    //!     };
    //! @endcode
    //!
    //! In this library, type classes with a single minimal complete definition
    //! provide the other methods, if any, in the nested type named `mcd`. In
    //! the case where multiple minimal complete definitions exist, each set of
    //! provided methods is in a different nested type with a descriptive name.
    //! In all cases, the minimal complete definition(s) and the location of
    //! their associated set of provided methods are documented.
    //!
    //! It is also possible to define a default instance for all data types.
    //! To do so, provide a nested `default_instance` template inside the
    //! type class:
    //! @code
    //!     struct Typeclass {
    //!         BOOST_HANA_TYPECLASS(Typeclass);
    //!         template <typename ...TypeclassArguments>
    //!         struct default_instance {
    //!             // default instance for all data types
    //!         };
    //!     };
    //! @endcode
    //!
    //! The nested `default_instance` should be just like a normal instance;
    //! see below for how to instantiate a type class. This can be used to
    //! provide a default behavior for all data types while still allowing
    //! this behavior to be customized by instantiating the type class. However,
    //! this should seldom be used because methods with a meaningful behavior
    //! for all data types are rare. This feature is provided for flexibility,
    //! but it should be a hint to reconsider your type class design if you
    //! are about to use it.
    //!
    //! ### Example
    //! @include example/core/default_instance.cpp
    //!
    //!
    //! ### Instantiating a type class
    //! Instantiating a type class is done by specializing the associated
    //! `instance` nested type:
    //! @code
    //!     template <>
    //!     struct Typeclass::instance<Datatype> : Typeclass::mcd {
    //!         // minimal complete definition at least
    //!     };
    //! @endcode
    //!
    //! Inheriting from `Typeclass::mcd` makes the default methods associated
    //! to that minimal complete definition available. If multiple minimal
    //! complete definitions are provided, one has to choose and inherit from
    //! the corresponding set of default methods.
    //!
    //! It is possible to over-define a type class, i.e. provide more methods
    //! than strictly necessary to fulfill the minimal complete definition.
    //! Simply implement those methods as if they were part of the minimal
    //! complete definition:
    //! @code
    //!     template <>
    //!     struct Typeclass::instance<Datatype> : Typeclass::mcd {
    //!         // minimal complete definition
    //!
    //!         // more methods
    //!     };
    //! @endcode
    //!
    //! Inheriting from a set of default methods is recommended even if default
    //! methods are not actually required, i.e. all the methods of the type
    //! class are implemented in the instance. This allows methods to be added
    //! to the type class without breaking the instance, provided the type
    //! class does not change its minimal complete definition(s).
    //!
    //! Type classes can also be instantiated for all specializations of a
    //! parametric data type in the most natural way:
    //! @code
    //!     template <typename T>
    //!     struct Typeclass::instance<SomeDatatype<T>> : Typeclass::mcd {
    //!         // ...
    //!     };
    //! @endcode
    //!
    //! Finally, type classes can be instantiated for all data types
    //! satisfying some predicate:
    //! @code
    //!     template <typename T>
    //!     struct Typeclass::instance<T, when<Predicate(T)>>
    //!         : Typeclass::mcd
    //!     {
    //!         // ...
    //!     };
    //! @endcode
    //!
    //! Note that instances provided through partial specialization have the
    //! priority over instances provided through a predicate. This allows
    //! data types (parametric or not) to instantiate a type class even if
    //! an instance for the same type class is provided through a predicate.
    //!
    //! ### Example
    //! @include example/core/typeclass.cpp
    //!
    //! @internal
    //! Using `when` is necessary for two reasons. First, a non-type
    //! template argument may not depend on a template parameter of a
    //! partial specialization, so we need to wrap the `bool` result of
    //! the predicate into a type. Second, `when` is used to implement the
    //! priority of partially specialized instances over predicated instances,
    //! but we could also achieve the same by replacing `when<true>` with
    //! `void` and letting people use `enable_if`.
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
    //! Machinery for creating a binary type class.
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
        struct enable { };
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_TYPECLASSES_HPP

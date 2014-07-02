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
    //! @ingroup core
    //! Machinery for creating a unary type class.
    //!
    //!
    //! ### Creating a type class
    //! Creating a new type class is done by inheriting from `typeclass`:
    //! @code
    //!     struct Typeclass : typeclass<Typeclass> { };
    //! @endcode
    //!
    //! If desired, default methods can be provided by putting them inside
    //! a nested type of `Typeclass`:
    //! @code
    //!     struct Typeclass : typeclass<Typeclass> {
    //!         struct some_member {
    //!             // default methods
    //!         };
    //!     };
    //! @endcode
    //!
    //! In this library, type classes with a single minimal complete definition
    //! provide their default methods, if any, in the nested type named `mcd`.
    //! In the case where multiple minimal complete definitions exist, each set
    //! of default methods is in a different nested type with a descriptive
    //! name. In all cases, the minimal complete definition(s) and the location
    //! of their associated set of default methods are documented.
    //!
    //! One can also provide a default instance for all data types by defining
    //! a `default_` member:
    //! @code
    //!     struct Typeclass : typeclass<Typeclass> {
    //!         struct default_ {
    //!             // default instance for all data types
    //!         };
    //!     };
    //! @endcode
    //!
    //! `default_` should be just like a normal instance; see below for how
    //! to instantiate a type class. This can be used to provide a default
    //! behavior for all data types while still allowing this behavior to
    //! be customized by instantiating the type class. However, this should
    //! seldom be used because methods with a meaningful behavior for all
    //! data types are rare. This feature is provided for flexibility, but
    //! it should be a hint to reconsider your type class design if you are
    //! about to use it.
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
    //! One can also instantiate a type class for all data types satisfying a
    //! predicate:
    //! @code
    //!     template <typename T>
    //!     struct Typeclass::instance<T, std::enable_if_t<Predicate(T)>>
    //!         : Typeclass::mcd
    //!     {
    //!         // ...
    //!     };
    //! @endcode
    //!
    //! This uses the well-known C++ trick of providing a dummy template
    //! parameter allowing SFINAE.
    //!
    //! ### Example
    //! @include example/core/typeclass.cpp
    template <typename Typeclass>
    struct typeclass;

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
        template <typename x, typename ...>
        struct dependent { using type = x; };
    }

    #define BOOST_HANA_TYPECLASS_BOILERPLATE(NAME)                          \
        template <>                                                         \
        struct typeclass<NAME> {                                            \
            using default_ = disable;                                       \
                                                                            \
            template <typename T, typename Enable = void>                   \
            struct instance                                                 \
                : core_detail::dependent<NAME, Enable>::type::default_      \
            { };                                                            \
        };                                                                  \
    /**/

    //! @ingroup core
    //! Machinery for creating a binary type class.
    //!
    //! This is equivalent to `typeclass`, except it creates a type class
    //! with two arguments.
    template <typename Typeclass>
    struct binary_typeclass;

    #define BOOST_HANA_BINARY_TYPECLASS_BOILERPLATE(NAME)                   \
        template <>                                                         \
        struct binary_typeclass<NAME> {                                     \
            using default_ = disable;                                       \
                                                                            \
            template <typename T, typename U, typename Enable = void>       \
            struct instance                                                 \
                : core_detail::dependent<NAME, Enable>::type::default_      \
            { };                                                            \
        };                                                                  \
    /**/

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
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_TYPECLASSES_HPP

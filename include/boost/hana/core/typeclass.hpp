/*!
@file
Defines utilities to create type classes.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_TYPECLASS_HPP
#define BOOST_HANA_CORE_TYPECLASS_HPP

#include <boost/hana/core/datatype.hpp>


namespace boost { namespace hana {
    //! @defgroup group-typeclasses Type classes
    //! General purpose type classes provided by the library.
    //!
    //! @todo
    //! - Implement better law checking and check them for all instances in the
    //!   unit tests. Since we're modularized, it's OK to resort to heavy stuff
    //!   in the law-checking because it's presumably only done in unit testing
    //!   contexts.
    //! - Provide a way to check type class requirements like
    //!   `Functor f => Applicative f`.
    //! - Find a better way to provide instances between type classes; consider
    //!   something like `Foldable::instance<Iterable>`.
    //! - Consider inheriting from a base class even when no mcd is required.
    //!   That would allow us to _not_ include a useless mcd.
    //! - Document the purpose of minimal instances; they are meant to provide an
    //!   easy to use archetype for testing and their tests are meant to exercise
    //!   the basic dispatching code of type classes (hence it makes sense to test
    //!   even the mcd of a minimal instance). In particular, they are not meant
    //!   to be _the_ minimal instance, which does not exist in general
    //!   (what does it even mean to be a _minimal_ instance?)

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
    //! @include example/core/unary_typeclass.cpp
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
    //! @include example/core/binary_typeclass.cpp
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
    //! Explicitly disable a type class instance.
    //!
    //! This is meant as a way to disable a type class instance provided
    //! through a predicate which, for some reason, is not desirable for
    //! a given data type.
    //!
    //! ### Example
    //! @include example/core/disable.cpp
    struct disable { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_TYPECLASS_HPP

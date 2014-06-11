/*!
@file
Defines the @ref Core module.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_HPP
#define BOOST_HANA_CORE_HPP

namespace boost { namespace hana {
    //! @defgroup Core Core
    //! Miscellaneous core utilities.
    //! @{

    /*!
    Contains the default methods of a type class.

    By default, no default methods are defined. It should be specialized for
    type classes wishing to provide a default implementation for some methods:
    @code
        template <>
        struct defaults<Typeclass> {
            template <typename ...Args>
            struct with {
                // provide a default implementation for methods outside
                // of the minimal complete definition
            };
        };
    @endcode

    The `Args...` are specific to each type class; one should consult the
    documentation of the type class to know what arguments are expected.
    */
    template <template <typename ...> class Typeclass>
    struct defaults {
        template <typename ...>
        using with = defaults;
    };

    /*!
    Contains complimentary type class instantiations for instances of
    other type classes.

    By default, no complimentary instances are provided. Type classes wishing
    to provide a complimentary instance must do so by specializing `instance`:
    @code
        template <typename ...Args>
        struct instance<Typeclass>::with<Args...> {
            // provide complimentary methods or even a full instance
        };
    @endcode

    The `Args...` are specific to each type class; one should consult the
    documentation of the type class to know what arguments are expected.
     */
    template <template <typename ...> class Typeclass>
    struct instance {
        template <typename ...>
        using with = instance;
    };

    namespace core_detail {
        template <typename T> typename T::hana_datatype datatype_impl(void*);
        template <typename T> T datatype_impl(...);
    }

    /*!
    Trait returning the data type associated to `T`.

    By default, this metafunction returns `T::hana_datatype` if that
    expression is valid, and `T` otherwise. It can also be specialized
    to customize the data type of `T` without requiring `T` to have a
    nested `hana_datatype` type.

    @todo
    Is there a more efficient way of performing SFINAE which would not require
    an overload resolution?
     */
    template <typename T>
    struct datatype {
        using type = decltype(core_detail::datatype_impl<T>(nullptr));
    };

    //! Alias to `datatype<T>::%type`.
    template <typename T>
    using datatype_t = typename datatype<T>::type;

    //! @}

    namespace operators {
        //! @ingroup Core
        //! Allows operators in the `boost::hana::operators` namespace to be
        //! found by ADL.
        //!
        //! Use this as a dummy template parameter or base class to make
        //! operators in the `boost::hana::operators` namespace ADL-findable
        //! for a type.
        struct enable { };
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_HPP

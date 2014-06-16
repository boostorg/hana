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

    Every type class instance must inherit exactly one of `defaults<>::%with<>`
    and `instance<>::%with<>`. Since `instance<>::%with<>` specializations are
    required to inherit `defaults<>::%with<>`, it is always inherited from.
    This is important since it allows default methods to be added to the type
    class without breaking existing user code, provided the new methods are
    not part of the minimal complete definition.

    When implementing a new type class, `defaults` __must__ be specialized
    even if default methods are not provided. Specialization is done as
    follows:
    @code
        template <>
        struct defaults<Typeclass> {
            template <typename ...Args>
            struct with {
                // provide a default implementation for methods outside
                // of the minimal complete definition if desired
            };
        };
    @endcode

    The `Args...` are specific to each type class; the documentation should
    explain their purpose.

    @todo
    - Document best practices with Enablers.
    - Show examples with enablers and other useful stuff.
    */
    template <template <typename ...> class Typeclass>
    struct defaults;

    /*!
    Allows complimentary type class instances to be provided.

    Every type class instance must inherit exactly one of `defaults<>::%with<>`
    and `instance<>::%with<>`. The latter may not be provided, in which case
    the only choice is to inherit `defaults<>::%with<>`. Complimentary
    instances, if any, should be documented for each type class.

    When implementing a new type class, one has to inherit from
    `instance<>::%with<>` as follows:
    @code
        template <typename ...>
        struct Typeclass : instance<Typeclass>::template with<...> { };
    @endcode

    This allows complimentary instances to be found when no explicit instance
    is provided for a data type. Also, `instance` __must__ be specialized for
    every type class even if no complimentary instances are provided.
    Specialization must be done as follows:
    @code
        template <>
        struct instance<Typeclass> {
            template <typename ...Args>
            struct with;
        };
    @endcode

    The `Args...` are specific to each type class; the documentation should
    explain their purpose. Then, to provide a complimentary instance, one can
    specialize `instance` as follows:
    @code
        template <...>
        struct instance<Typeclass>::with<...>
            : defaults<Typeclass>::with<...>
        {
            // whatever you want
        };
    @endcode

    Inheriting from `defaults` in that specialization is mandatory.

    @note
    Unless you want to break the world, always make sure that users opt-in
    __explicitly__ into a complimentary type class instance. Otherwise, code
    relying on the fact that a data type is _not_ an instance of a given type
    class could break when the complimentary instance is made available.

    @todo
    - Document best practices with Enablers.
    - Show examples with enablers and other useful stuff.
     */
    template <template <typename ...> class Typeclass>
    struct instance;

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
    Is there a more efficient way of performing SFINAE which would not
    require an overload resolution?

    @bug
    The data type of `std::is_pointer<int>{}` is `std::is_pointer<int>`
    instead of `Integral` because `std::is_pointer` only _inherits_ from
    `std::integral_constant`.
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

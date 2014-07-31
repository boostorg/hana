/*!
@file
Defines utilities to create data types.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_DATATYPE_HPP
#define BOOST_HANA_CORE_DATATYPE_HPP

namespace boost { namespace hana {
    //! @defgroup group-datatypes Data types
    //! General purpose data types provided by the library.

    //! @ingroup group-core
    //! Enable a type class instance or a data type specialization only if a
    //! boolean condition is true.
    //!
    //! @internal
    //! ### Rationale for using `when` instead of `std::enable_if`
    //! Using `when` is necessary for two reasons. First, a non-type template
    //! argument may not depend on a template parameter of a partial
    //! specialization, so we need to wrap the `bool` condition into a
    //! type. Second, `when` is used to control the priority of partial
    //! specializations in a finer grained manner than what can be achieved
    //! with the usual `typename Enable = void` and `std::enable_if` pattern.
    //! @endinternal
    //!
    //! ### Example
    //! @include example/core/when.cpp
    template <bool condition>
    struct when;

    //! @ingroup group-core
    //! Enable a type class instance or a data type specialization only if an
    //! expression is well-formed.
    //!
    //! Specifically, this is equivalent to `when<true>`, but SFINAE will
    //! cause the partial specialization to fail when the expression is
    //! ill-formed.
    //!
    //! ### Example
    //! @include example/core/when_valid.cpp
    template <typename ...>
    using when_valid = when<true>;

    namespace core_detail {
        template <typename T, typename Enable = void>
        struct default_datatype { using type = T; };

        template <typename T>
        struct default_datatype<T, decltype((void)(typename T::hana_datatype*)0)> {
            using type = typename T::hana_datatype;
        };
    }

    //! @ingroup group-core
    //! Metafunction returning the data type associated to `T`.
    //!
    //! Specifically, let's denote by `U` the type `T` after being stripped
    //! of all references and cv-qualifiers. Then, this metafunction returns
    //! @code
    //!     U::hana_datatype
    //! @endcode
    //! if that expression is valid, and `U` otherwise. It can also be
    //! specialized to customize the data type of `T` without requiring `T`
    //! to have a nested `hana_datatype` type. Finally, it is also possible
    //! to use `when` and `when_valid` to enable the a `datatype`
    //! specialization only when some boolean condition is true or when some
    //! expression is well-formed.
    //!
    //! ### Example
    //! @snippet example/core/datatype.cpp main
    //!
    //! @todo
    //! Could this be related to `decltype_`? If so, how? It is a valid
    //! question whether `decltype_(list(...))` should be `List` or
    //! `<garbage>`.
    template <typename T, typename ...>
    struct datatype
    //! @cond
        : datatype<T, when<true>>
    //! @endcond
    { };

    template <typename T, bool condition>
    struct datatype<T, when<condition>> {
        using type = typename core_detail::default_datatype<T>::type;
    };

    template <typename T> struct datatype<T const> : datatype<T> { };
    template <typename T> struct datatype<T volatile> : datatype<T> { };
    template <typename T> struct datatype<T const volatile> : datatype<T> { };
    template <typename T> struct datatype<T&> : datatype<T> { };
    template <typename T> struct datatype<T&&> : datatype<T> { };

    //! @ingroup group-core
    //! Alias to `datatype<T>::%type`.
    template <typename T>
    using datatype_t = typename datatype<T>::type;

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

#endif // !BOOST_HANA_CORE_DATATYPE_HPP

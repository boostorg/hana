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
    //! Used in conjunction with `when` to enable a type class instance or a
    //! data type specialization only if an expression is well-formed.
    //!
    //! Specifically, `is_valid<...>` is always equivalent to the constant
    //! expression `true`. When used as `when<is_valid<...>>` inside a type
    //! class or data type specialization, SFINAE will cause the partial
    //! specialization to fail when the expression is ill-formed.
    //!
    //! ### Example
    //! @include example/core/is_valid.cpp
    template <typename ...>
    constexpr bool is_valid = true;

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
    //! to use `when` to enable the a `datatype` specialization only when some
    //! boolean condition is true or when some expression is well-formed with
    //! `is_valid`.
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

    //! @ingroup group-core
    //! Allows the operators associated to the given type classes to be
    //! found by ADL.
    //!
    //! Specifically, `operators` takes zero or more type classes providing
    //! custom operators and makes them findable by argument-dependent lookup
    //! for any class `C` such that:
    //! 1. `operators<...>` is a base class of `C`
    //! 2. `C` is a template specialization and `operators<...>` appears in
    //!    its template argument list.
    //!
    //! In other words, either inherit from `operators<T1, ..., Tn>` or use
    //! it as a dummy template parameter to make the custom operators defined
    //! by the type classes `T1`, ..., `Tn` available to objects of that type.
    //!
    //! @attention
    //! A data type using `operators` __may not__ rely on the instance
    //! provided by some type classes which rely on the presence of
    //! specific operators to implement a method. For example, one may
    //! not use `operators` and then rely on the instance of `Comparable`
    //! for all objects that can be compared using `==`, because that is
    //! redundant. Failure to respect this will result in undefined behavior.
    //!
    //! @tparam Typeclasses...
    //! Type classes whose custom operators should be made visible to ADL.
    //! Note that all the type classes must provide custom operators; a
    //! compile-time error is triggered otherwise.
    //!
    //! ### Example
    //! @include example/core/operators.cpp
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
    //!     template <typename T, typename = operators<Comparable>>
    //!     struct something_to_do_with_hana { };
    //!
    //!     nothing_to_do_with_hana<something_to_do_with_hana<int>> x{};
    //!     x == x; // tries to use Comparable::equal_impl
    //! @endcode
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename ...Typeclasses>
    using operators = unspecified;
#else
    namespace core_detail {
        template <typename ...T>
        struct enable_adl : T... { };
    }

    template <typename ...Typeclasses>
    using operators = core_detail::enable_adl<
        typename Typeclasses::operators...
    >;
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_DATATYPE_HPP

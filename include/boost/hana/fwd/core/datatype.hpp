/*!
@file
Forward declares `boost::hana::datatype` and `boost::hana::datatype_t`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 FWD_*/

#ifndef BOOST_HANA_FWD_CORE_DATATYPE_HPP
#define BOOST_HANA_FWD_CORE_DATATYPE_HPP

namespace boost { namespace hana {
    //! @ingroup group-core
    //! %Metafunction returning the generalized type associated to `T`.
    //!
    //! There are several ways to specify the data type of a C++ type. If it's
    //! a user-defined type, one can define a nested `hana::dataype` alias
    //! inside of it:
    //! @code
    //!     struct MyUserDefinedType {
    //!         struct hana { using datatype = MyDatatype; };
    //!     };
    //! @endcode
    //!
    //! Sometimes, however, the C++ type can't be modified (if it's in a
    //! foreign library) or simply can't have nested types (if it's not a
    //! struct or class). In those cases, using a nested alias is impossible
    //! and so ad-hoc customization is also supported by specializing
    //! `datatype` in the `boost::hana` namespace:
    //! @code
    //!     struct i_cant_modify_this;
    //!
    //!     namespace boost { namespace hana {
    //!         template <>
    //!         struct datatype<i_cant_modify_this> {
    //!             using type = MyDatatype;
    //!         };
    //!     }}
    //! @endcode
    //!
    //! `datatype` can also be specialized for all C++ types satisfying some
    //! boolean condition using `when`. `when` accepts a single compile-time
    //! boolean and enables the specialization of `datatype` if and only if
    //! that boolean is `true`. This is similar to the well known C++ idiom
    //! of using a dummy template parameter with `std::enable_if` and relying
    //! on SFINAE. For example, we could specify the data type of all
    //! `fusion::vector`s by doing:
    //! @code
    //!     struct BoostFusionVector;
    //!
    //!     namespace boost { namespace hana {
    //!         template <typename T>
    //!         struct datatype<T, when<
    //!             std::is_same<
    //!                 typename fusion::traits::tag_of<T>::type,
    //!                 fusion::traits::tag_of<fusion::vector<>>::type
    //!             >{}
    //!         >> {
    //!             using type = BoostFusionVector;
    //!         };
    //!     }}
    //! @endcode
    //!
    //! Also, when it is not specialized and when the given C++ type does not
    //! have a nested `hana::dataype` alias, `datatype<T>` returns `T` itself.
    //! This makes data types a simple extension of normal C++ types. This is
    //! _super_ useful, mainly for two reasons. First, this allows Hana to
    //! adopt a reasonable default behavior for some operations involving
    //! types that have no notion of data type. For example, Hana allows
    //! comparing with `equal` any two objects for which a valid `operator==`
    //! is defined, and that without any work on the user side. Second, it
    //! also means that you can ignore data types completely if you don't need
    //! their functionality; just use the normal C++ type of your objects and
    //! everything will "just work".
    //!
    //! Finally, also note that `datatype<T>` is always equivalent to
    //! `datatype<U>`, where `U` is the type `T` after being stripped of
    //! all references and cv-qualifiers. This makes it unnecessary to
    //! specialize `datatype` for all reference and cv combinations, which
    //! would be a real pain.
    //!
    //! > __Tip 1__\n
    //! > If compile-time performance is a serious concern, consider specializing
    //! > the `datatype` metafunction in Hana's namespace. When unspecialized,
    //! > the metafunction has to use SFINAE, which tends to incur a larger
    //! > compile-time overhead. For heavily used templated types, this can
    //! > potentially make a difference.
    //!
    //! > __Tip 2__\n
    //! > Consider using `datatype_t` alias instead of `datatype`, which
    //! > reduces the amount of typing in dependent contexts.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/core/datatype.cpp datatype
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename T, optional when-based enabler>
    struct datatype { unspecified };
#else
    template <typename T, typename = void>
    struct datatype;
#endif

    //! @ingroup group-core
    //! Alias to `datatype<T>::%type`, provided for convenience.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/core/datatype.cpp datatype_t
    template <typename T>
    using datatype_t = typename datatype<T>::type;
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CORE_DATATYPE_HPP

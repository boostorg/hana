/*!
@file
Forward declares `boost::hana::datatype` and `boost::hana::datatype_t`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 FWD_*/

#ifndef BOOST_HANA_FWD_CORE_DATATYPE_HPP
#define BOOST_HANA_FWD_CORE_DATATYPE_HPP

namespace boost { namespace hana {
    //! @defgroup group-datatypes Data types
    //! General purpose data types provided by the library.

    //! @ingroup group-core
    //! %Metafunction returning the data type associated to `T`.
    //!
    //! First, note that `datatype<T>` is always equivalent to `datatype<U>`,
    //! where `U` is the type `T` after being stripped of all references and
    //! cv-qualifiers. Then, `datatype<U>` returns
    //! @code
    //!     U::hana::datatype
    //! @endcode
    //! if that expression is valid, and `U` otherwise. If `U` does not
    //! have a nested `%hana::datatype` type, the metafunction can also be
    //! specialized in the `boost::hana` namespace, optionally using `when`
    //! to allow flexible specializations.
    //!
    //!
    //! > #### Tip
    //! > If compile-time performance is a serious concern, consider specializing
    //! > the `datatype` metafunction in Hana's namespace. When unspecialized,
    //! > the metafunction has to use SFINAE, which tends to incur a larger
    //! > compile-time overhead. For heavily used templated types, this can
    //! > potentially make a difference.
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

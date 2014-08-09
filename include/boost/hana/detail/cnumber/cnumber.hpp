/*!
@file
Defines the `boost::hana::detail::CNumber` data type.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CNUMBER_CNUMBER_HPP
#define BOOST_HANA_DETAIL_CNUMBER_CNUMBER_HPP

#include <boost/hana/core/datatype.hpp>


namespace boost { namespace hana { namespace detail {
    struct unspecified;

    template <template <typename ...> class>
    struct quote;

    //! @ingroup group-details
    //! The `CNumber` data type.
    //!
    //! This is a trivial instance of a couple of type classes for which
    //! some methods can either return compile-time or runtime results.
    //! While `Number` handles the runtime part, `CNumber` handles the
    //! compile-time part.
    //!
    //! ## Instance of
    //! `Constant` and `Logical`.
    template <typename Mcd = unspecified>
    struct CNumber { };

    namespace cnumber_detail {
        template <typename T, T v, typename Mcd, typename Operators>
        struct cnumber_type {
            using hana_datatype = CNumber<Mcd>;
            static constexpr T value = v;

            template <typename U, U u>
            static constexpr cnumber_type<U, u, Mcd, Operators> wrap{};
        };
    }

    //! Create a `CNumber` with the given value and type.
    //! @relates CNumber
    //!
    //! By default, the minimal complete definitions used by a `CNumber` are
    //! left unspecified. However, it is possible to specify a minimal complete
    //! definition that must absolutely be used for some type class. This is
    //! useful for unit testing purposes, where one wants to check that the
    //! minimal complete definition works as expected.
    //!
    //!
    //! @tparam T
    //! The type of the compile-time value held by the `CNumber`.
    //!
    //! @param v
    //! The compile-time value held by the `CNumber`.
    //!
    //! @tparam Mcd
    //! A minimal complete definition that should absolutely be used. This
    //! parameter is optional; if nothing is specified, then all the MCDs
    //! that are used are left unspecified.
    //!
    //! @tparam Operators
    //! An `operators<>` specialization representing the operators that can
    //! be used with the returned object. This parameter is optional; if
    //! nothing is specified, then no operators are provided.
    template <typename T, T v,
              typename Mcd = unspecified,
              typename Operators = operators<>>
    constexpr cnumber_detail::cnumber_type<T, v, Mcd, Operators> cnumber{};
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_CNUMBER_CNUMBER_HPP

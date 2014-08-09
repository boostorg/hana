/*!
@file
Defines the `boost::hana::detail::Number` data type.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_NUMBER_NUMBER_HPP
#define BOOST_HANA_DETAIL_NUMBER_NUMBER_HPP

#include <boost/hana/core/datatype.hpp>


namespace boost { namespace hana { namespace detail {
    struct unspecified;

    template <template <typename ...> class>
    struct quote;

    //! @ingroup group-details
    //! The `Number` data type.
    //!
    //! This is a trivial instance of several type classes. It is useful
    //! for unit testing purposes, where one wants a lightweight and
    //! easy-to-reason-with instance of a type class. Also, all its
    //! instances are guaranteed to be minimal, i.e. they only define
    //! the strict minimum to satisfy the minimal complete definition.
    //!
    //! ## Instance of
    //! `Comparable`, `Orderable`, `Logical`, `Monoid`, `Group`, and `Ring`.
    template <typename Mcd = unspecified>
    struct Number { };

    namespace number_detail {
        template <typename X, typename Mcd, typename Operators>
        struct number_type {
            X value;
            using hana_datatype = Number<Mcd>;

            template <typename Y>
            constexpr auto wrap(Y y) const
            { return number_type<Y, Mcd, Operators>{y}; }

            constexpr explicit operator X() const { return value; }
        };

        template <typename Mcd, typename Operators>
        struct make_number {
            template <typename X>
            constexpr auto operator()(X x) const
            { return number_type<X, Mcd, Operators>{x}; }
        };
    }

    //! Create a `Number` with the given value.
    //! @relates Number
    //!
    //! By default, the minimal complete definitions used by a `Number` are
    //! left unspecified. However, it is possible to specify a minimal complete
    //! definition that must absolutely be used for some type class. This is
    //! useful for unit testing purposes, where one wants to check that the
    //! minimal complete definition works as expected.
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
    //!
    //! @param x
    //! The integer value to wrap in a `Number` data type.
    template <typename Mcd = unspecified,
              typename Operators = operators<>>
    constexpr number_detail::make_number<Mcd, Operators> number{};
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_NUMBER_NUMBER_HPP

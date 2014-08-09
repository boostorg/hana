/*!
@file
Defines the `boost::hana::detail::Identity` data type.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_IDENTITY_IDENTITY_HPP
#define BOOST_HANA_DETAIL_IDENTITY_IDENTITY_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/orderable/less_mcd.hpp>


namespace boost { namespace hana {
namespace detail {
    struct unspecified;

    template <template <typename ...> class>
    struct quote;

    //! @ingroup group-details
    //! The identity data type.
    //!
    //! This is a trivial instance of several type classes. It is useful
    //! for unit testing purposes, where one wants a lightweight and
    //! easy-to-reason-with instance of a type class. Also, all its
    //! instances are guaranteed to be minimal, i.e. they only define
    //! the strict minimum to satisfy the minimal complete definition.
    //!
    //! ## Instance of
    //! `Comparable`, `Orderable`, `Functor`, `Applicative`, `Monad`,
    //! `Foldable`, `Traversable` and `Searchable`.
    template <typename Mcd = unspecified>
    struct Identity { };

    namespace identity_detail {
        template <typename X, typename Mcd, typename Operators>
        struct identity_type {
            X value;
            using hana_datatype = Identity<Mcd>;

            template <typename Y>
            constexpr auto wrap(Y y) const
            { return identity_type<Y, Mcd, Operators>{y}; }
        };

        template <typename Mcd, typename Operators>
        struct make_identity {
            template <typename X>
            constexpr auto operator()(X x) const
            { return identity_type<X, Mcd, Operators>{x}; }
        };
    }

    //! Wrap a value in the `Identity` data type.
    //! @relates Identity
    //!
    //! By default, the minimal complete definitions used by an `Identity`
    //! are left unspecified. However, it is possible to specify a minimal
    //! complete definition that must absolutely be used for some type class.
    //! This is useful for unit testing purposes, where one wants to check
    //! that the minimal complete definition works as expected.
    //!
    //! Also note that instances for `Comparable` and `Orderable` are always
    //! unspecified, and so their MCDs can't be specified.
    //!
    //! @tparam Mcd
    //! A minimal complete definition that should absolutely be used. This
    //! parameter is optional; if nothing is specified, or if `unspecified`
    //! is passed, then all the MCDs that are used are left unspecified.
    //!
    //! @tparam Operators
    //! An `operators<>` specialization representing the operators that can
    //! be used with the returned object. This parameter is optional; if
    //! nothing is specified, then no operators are provided.
    //!
    //! @param x
    //! The object to lift into the `Identity` data type.
    template <typename Mcd = unspecified,
              typename Operators = operators<>>
    constexpr identity_detail::make_identity<Mcd, Operators> identity{};
}

template <typename Mcd>
struct Comparable::instance<detail::Identity<Mcd>, detail::Identity<Mcd>>
    : Comparable::equal_mcd
{
    template <typename Id1, typename Id2>
    static constexpr auto equal_impl(Id1 x, Id2 y) {
        return equal(x.value, y.value);
    }
};

template <typename Mcd>
struct Orderable::instance<detail::Identity<Mcd>, detail::Identity<Mcd>>
    : Orderable::less_mcd
{
    template <typename Id1, typename Id2>
    static constexpr auto less_impl(Id1 x, Id2 y) {
        return less(x.value, y.value);
    }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_IDENTITY_IDENTITY_HPP

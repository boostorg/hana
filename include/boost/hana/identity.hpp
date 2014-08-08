/*!
@file
Defines the `boost::hana::Identity` data type.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_IDENTITY_HPP
#define BOOST_HANA_IDENTITY_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foldable/unpack_mcd.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>
#include <boost/hana/orderable/less_mcd.hpp>
#include <boost/hana/traversable/traverse_mcd.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! The identity data type.
    //!
    //! This is a trivial instance of several type classes. It is especially
    //! useful for unit testing purposes, where one wants a lightweight and
    //! easy-to-reason-with instance of a type class.
    //!
    //! ## Instance of
    //! `Comparable`, `Orderable`, `Functor`, `Applicative`, `Monad`,
    //! `Foldable` and `Traversable`.
    struct Identity { };

    namespace detail { namespace repr {
        template <typename X, typename = operators<Comparable, Orderable, Monad>>
        struct identity {
            X value;
            using hana_datatype = Identity;
        };
    }}

    //! Wrap a value in the `Identity` data type.
    BOOST_HANA_CONSTEXPR_LAMBDA auto identity = [](auto x) {
        return detail::repr::identity<decltype(x)>{x};
    };

    template <>
    struct Comparable::instance<Identity, Identity> : Comparable::equal_mcd {
        template <typename Id1, typename Id2>
        static constexpr auto equal_impl(Id1 x, Id2 y) {
            return equal(x.value, y.value);
        }
    };

    template <>
    struct Orderable::instance<Identity, Identity> : Orderable::less_mcd {
        template <typename Id1, typename Id2>
        static constexpr auto less_impl(Id1 x, Id2 y) {
            return less(x.value, y.value);
        }
    };

    template <>
    struct Functor::instance<Identity> : Functor::fmap_mcd {
        template <typename F, typename Id>
        static constexpr auto fmap_impl(F f, Id self) {
            return identity(f(self.value));
        }
    };

    template <>
    struct Applicative::instance<Identity> : Applicative::mcd {
        template <typename X>
        static constexpr auto lift_impl(X x) {
            return identity(x);
        }

        template <typename F, typename X>
        static constexpr auto ap_impl(F f, X x) {
            return identity(f.value(x.value));
        }
    };

    template <>
    struct Monad::instance<Identity> : Monad::flatten_mcd<Identity> {
        template <typename Id>
        static constexpr auto flatten_impl(Id self) {
            return self.value;
        }
    };

    template <>
    struct Foldable::instance<Identity> : Foldable::unpack_mcd {
        template <typename Id, typename F>
        static constexpr auto unpack_impl(Id self, F f) {
            return f(self.value);
        }
    };

    template <>
    struct Traversable::instance<Identity> : Traversable::traverse_mcd {
        template <typename A, typename F, typename Id>
        static constexpr auto traverse_impl(F f, Id self) {
            return fmap(identity, f(self.value));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_IDENTITY_HPP

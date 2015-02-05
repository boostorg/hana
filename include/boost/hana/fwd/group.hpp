/*!
@file
Forward declares `boost::hana::Group`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_GROUP_HPP
#define BOOST_HANA_FWD_GROUP_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Group` type class is used for `Monoid`s where all objects have
    //! an inverse w.r.t. the `Monoid`'s binary operation.
    //!
    //! The method names refer to the group of numbers under addition.
    //!
    //! ### Requires
    //! `Monoid`
    //!
    //! ### Laws
    //! For all objects `x` of a `Group` `G`, the following laws must be
    //! satisfied:
    //! @code
    //!     plus(x, negate(x)) == zero<G>() // right inverse
    //!     plus(negate(x), x) == zero<G>() // left inverse
    //! @endcode
    struct Group {
        BOOST_HANA_BINARY_TYPECLASS(Group);
        template <typename G1, typename G2>
        struct negate_mcd;
        template <typename G1, typename  G2>
        struct minus_mcd;
        template <typename I1, typename I2>
        struct integral_constant_mcd;
        template <typename T, typename U>
        struct default_instance;
    };

    //! Subtract two elements of a group.
    //! @relates Group
    //!
    //! Specifically, this performs the `Monoid` operation on the first
    //! argument and on the inverse of the second argument, thus being
    //! equivalent to:
    //! @code
    //!     minus(x, y) == plus(x, negate(y))
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/group.cpp minus
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto minus = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct minus_impl : minus_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct minus_impl<T, U, when<condition>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return Group::instance<T, U>::minus_impl(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    struct _minus {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return minus_impl<datatype_t<X>, datatype_t<Y>>::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _minus minus{};
#endif

    //! Return the inverse of an element of a group.
    //! @relates Group
    //!
    //! Since `Group` is a binary type class and `negate` is a unary method,
    //! `negate(x)` is dispatched to the type class instance for `G` and `G`,
    //! i.e. `Group::instance<G, G>`, where `G` is the data type of `x`.
    //!
    //! ### Example
    //! @snippet example/group.cpp negate
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto negate = [](auto&& x) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct negate_impl : negate_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct negate_impl<T, when<condition>> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x) {
            return Group::instance<T, T>::negate_impl(
                detail::std::forward<X>(x)
            );
        }
    };

    struct _negate {
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const {
            return negate_impl<datatype_t<X>>::apply(
                detail::std::forward<X>(x)
            );
        }
    };

    constexpr _negate negate{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_GROUP_HPP

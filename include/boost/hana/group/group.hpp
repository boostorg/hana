/*!
@file
Forward declares `boost::hana::Group`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_GROUP_HPP
#define BOOST_HANA_GROUP_GROUP_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
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
    //!     plus(x, negate(x)) == zero<G> // right inverse
    //!     plus(negate(x), x) == zero<G> // left inverse
    //! @endcode
    struct Group {
        BOOST_HANA_BINARY_TYPECLASS(Group);
        template <typename G1, typename G2>
        struct negate_mcd;
        template <typename G1, typename  G2>
        struct minus_mcd;
        template <typename I1, typename I2>
        struct integral_constant_mcd;
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
    BOOST_HANA_CONSTEXPR_LAMBDA auto minus = [](auto&& x, auto&& y) -> decltype(auto) {
        return Group::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::minus_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    };

    //! Return the inverse of an element of a group.
    //! @relates Group
    //!
    //! Since `Group` is a binary type class and `negate` is a unary method,
    //! `negate(x)` is dispatched to the type class instance for `G` and `G`,
    //! i.e. `Group::instance<G, G>`, where `G` is the data type of `x`.
    //!
    //! ### Example
    //! @snippet example/group.cpp negate
    BOOST_HANA_CONSTEXPR_LAMBDA auto negate = [](auto&& x) -> decltype(auto) {
        return Group::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(x)>
        >::negate_impl(detail::std::forward<decltype(x)>(x));
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_GROUP_GROUP_HPP

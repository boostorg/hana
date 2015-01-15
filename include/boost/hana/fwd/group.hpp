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
#include <boost/hana/core/method.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Group` type class is used for `Monoid`s where all objects have
    //! an inverse w.r.t. the `Monoid`'s binary operation.
    //!
    //! The method names refer to the group of numbers under addition.
    //!
    //!
    //! Superclass
    //! ----------
    //! `Monoid`
    //!
    //!
    //! Laws
    //! ----
    //! For all objects `x` of a `Group` `G`, the following laws must be
    //! satisfied:
    //! @code
    //!     plus(x, negate(x)) == zero<G>() // right inverse
    //!     plus(negate(x), x) == zero<G>() // left inverse
    //! @endcode
    //!
    //!
    //! Minimal complete definitions
    //! ----------------------------
    //! 1. `minus`
    //! @todo
    //!
    //! 2. `negate`
    //! @todo
    //!
    //! 3. `operator-`
    //! @todo
    struct Group { };

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
    BOOST_HANA_BINARY_METHOD(minus_impl);

    struct _minus {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return dispatch<minus_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >>::apply(
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
    //! ### Example
    //! @snippet example/group.cpp negate
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto negate = [](auto&& x) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_METHOD(negate_impl);

    struct _negate {
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const {
            return dispatch<negate_impl<typename datatype<X>::type>>::apply(
                detail::std::forward<X>(x)
            );
        }
    };

    constexpr _negate negate{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_GROUP_HPP

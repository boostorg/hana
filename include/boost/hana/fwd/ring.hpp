/*!
@file
Forward declares `boost::hana::Ring`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_RING_HPP
#define BOOST_HANA_FWD_RING_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Ring` type class is used for `Group`s that also form a `Monoid`
    //! under a second binary operation that distributes over the first.
    //!
    //! The method names refer to the ring of numbers under multiplication.
    //!
    //!
    //! Superclass
    //! ----------
    //! `Group`
    //!
    //!
    //! Laws
    //! ----
    //! For all objects `x`, `y`, `z` of a `Ring` `R`, the following laws must
    //! be satisfied:
    //! @code
    //!     mult(x, mult(y, z)) == mult(mult(x, y), z)          // associativity
    //!     mult(x, one<R>()) == x                              // right identity
    //!     mult(one<R>(), x) == x                              // left identity
    //!     mult(x, plus(y, z)) == plus(mult(x, y), mult(x, z)) // distributivity
    //! @endcode
    //!
    //!
    //! Minimal complete definintions
    //! -----------------------------
    //! 1. `one` and `mult`
    //! @todo
    //!
    //! 2. `operator*` and `static_cast`ability to 1
    //! Any data type whose objects can be multiplied with the usual
    //! `operator*` is given the `mult` method using this multiplication.
    //! The `one` method is implemented as `static_cast<T>(1)`.
    struct Ring { };

    //! Associative operation of a `Ring`.
    //! @relates Ring
    //!
    //! ### Example
    //! @snippet example/ring.cpp mult
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto mult = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_BINARY_METHOD(mult_impl);

    struct _mult {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return dispatch<mult_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >>::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _mult mult{};
#endif

    //! Identity of `mult`.
    //! @relates Ring
    //!
    //! @tparam R
    //! The data type (a `Ring`) of the returned identity.
    //!
    //! ### Example
    //! @snippet example/ring.cpp one
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename R>
    constexpr auto one = []() -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_METHOD(one_impl);

    template <typename R>
    struct _one {
        constexpr decltype(auto) operator()() const {
            return dispatch<one_impl<R>>::apply();
        }
    };

    template <typename R>
    constexpr _one<R> one{};
#endif

    //! Power of a `Ring` element.
    //! @relates Ring
    //!
    //! Specifically, `power(r, p)`, is equivalent to multiplying `r` with
    //! itself `p` times, by decrementing `p` until it is equal to `zero`
    //! If `p` is equal to `zero` from the beginning, `one` is returned.
    //!
    //! @note
    //! Dispatching for this method is done on the data type of `r` only.
    //!
    //!
    //! @param r
    //! A `Ring` element that is multiplied with itself `p` times.
    //!
    //! @param p
    //! An `IntegralConstant` representing the number of times `r` should be
    //! multiplied by itself.
    //!
    //!
    //! ### Example
    //! @snippet example/ring.cpp power
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto power = [](auto&& r, auto&& p) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_METHOD(power_impl);

    struct _power {
        template <typename R, typename P>
        constexpr decltype(auto) operator()(R&& r, P&& p) const {
            return dispatch<power_impl<
                typename datatype<R>::type
            >>::apply(
                detail::std::forward<R>(r),
                detail::std::forward<P>(p)
            );
        }
    };

    constexpr _power power{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_RING_HPP

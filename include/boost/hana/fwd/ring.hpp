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
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Ring` type class is used for `Group`s that also form a `Monoid`
    //! under a second binary operation that distributes over the first.
    //!
    //! The method names refer to the ring of numbers under multiplication.
    //!
    //! ### Requires
    //! `Group`
    //!
    //! ### Laws
    //! For all objects `x`, `y`, `z` of a `Ring` `R`, the following laws must
    //! be satisfied:
    //! @code
    //!     mult(x, mult(y, z)) == mult(mult(x, y), z)          // associativity
    //!     mult(x, one<R>) == x                                // right identity
    //!     mult(one<R>, x) == x                                // left identity
    //!     mult(x, plus(y, z)) == plus(mult(x, y), mult(x, z)) // distributivity
    //! @endcode
    struct Ring {
        BOOST_HANA_BINARY_TYPECLASS(Ring);
        struct mcd;
        template <typename I1, typename I2>
        struct integral_constant_mcd;
    };

    //! Associative operation of a `Ring`.
    //! @relates Ring
    //!
    //! ### Example
    //! @snippet example/ring.cpp mult
    BOOST_HANA_CONSTEXPR_LAMBDA auto mult = [](auto&& x, auto&& y) -> decltype(auto) {
        return Ring::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::mult_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    };

    //! Identity of `mult`.
    //! @relates Ring
    //!
    //! Since `Ring` is a binary type class and `one` is a nullary method,
    //! `one<R>` is dispatched to the type class instance for `R` and `R`,
    //! i.e. `Ring::instance<R, R>`.
    //!
    //! @tparam R
    //! The data type (a `Ring`) of the returned identity.
    //!
    //! ### Example
    //! @snippet example/ring.cpp one
    template <typename R>
    BOOST_HANA_CONSTEXPR_LAMBDA auto one = Ring::instance<R, R>::one_impl();

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
    BOOST_HANA_CONSTEXPR_LAMBDA auto power = [](auto&& r, auto&& p) -> decltype(auto) {
        return Ring::instance<
            datatype_t<decltype(r)>, datatype_t<decltype(r)>
        >::power_impl(
            detail::std::forward<decltype(r)>(r),
            detail::std::forward<decltype(p)>(p)
        );
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_RING_HPP

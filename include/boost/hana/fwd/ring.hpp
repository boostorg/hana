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
#include <boost/hana/core/when.hpp>
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
    //!     mult(x, one<R>()) == x                              // right identity
    //!     mult(one<R>(), x) == x                              // left identity
    //!     mult(x, plus(y, z)) == plus(mult(x, y), mult(x, z)) // distributivity
    //! @endcode
    struct Ring {
        BOOST_HANA_BINARY_TYPECLASS(Ring);
        struct mcd;
        template <typename I1, typename I2>
        struct integral_constant_mcd;
        template <typename T, typename U>
        struct default_instance;
    };

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
    template <typename T, typename U, typename = void>
    struct mult_impl : mult_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct mult_impl<T, U, when<condition>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return Ring::instance<T, U>::mult_impl(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    struct _mult {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return mult_impl<datatype_t<X>, datatype_t<Y>>::apply(
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
    //! Since `Ring` is a binary type class and `one` is a nullary method,
    //! `one<R>()` is dispatched to the type class instance for `R` and `R`,
    //! i.e. `Ring::instance<R, R>`.
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
    template <typename T, typename = void>
    struct one_impl : one_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct one_impl<T, when<condition>> {
        static constexpr decltype(auto) apply() {
            return Ring::instance<T, T>::one_impl();
        }
    };

    template <typename R>
    struct _one {
        constexpr decltype(auto) operator()() const {
            return one_impl<R>::apply();
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
    template <typename T, typename = void>
    struct power_impl : power_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct power_impl<T, when<condition>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return Ring::instance<T, T>::power_impl(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    struct _power {
        template <typename R, typename P>
        constexpr decltype(auto) operator()(R&& r, P&& p) const {
            return power_impl<datatype_t<R>>::apply(
                detail::std::forward<R>(r),
                detail::std::forward<P>(p)
            );
        }
    };

    constexpr _power power{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_RING_HPP

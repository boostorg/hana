/*!
@file
Forward declares `boost::hana::IntegralDomain`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_FWD_INTEGRAL_DOMAIN_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `IntegralDomain` concept represents a commutative `Ring` whose
    //! multiplication has some "nice" properties.
    //!
    //! In a Ring `(R, +, *)`, it follows quite easily from the axioms that
    //! `x * 0 == 0` for any ring element `x`. However, there is nothing that
    //! mandates `0` to be the only ring element sending other elements to `0`.
    //! Hence, in some Rings, it is possible to have elements `x` and `y` such
    //! that `x * y == 0` while not having `x == 0` nor `y == 0`. We call these
    //! elements divisors of zero, or zero divisors. For example, this situation
    //! arises in the Ring of integers modulo 4 (the set `{0, 1, 2, 3}`)
    //! with addition and multiplication `mod 4` as binary operations.
    //! In this case, we have that
    //! @code
    //!     2 * 2 == 4
    //!           == 0 (mod 4)
    //! @endcode
    //! even though `2 != 0 (mod 4)`.
    //!
    //! Following this line of thought, an [IntegralDomain][1] is simply a
    //! `Ring` whose only zero divisor is zero itself. In other words, an
    //! `IntegralDomain` is a `Ring` whose multiplication won't send
    //! non-zero elements to zero. Also note that since multiplication in a
    //! `Ring` is not necessarily commutative, it is not always the case that
    //! @code
    //!     x * y == 0  implies  y * x == 0
    //! @endcode
    //! To be rigorous, we should then distinguish between elements that are
    //! zero divisors when multiplied to the right and to the left.
    //! Fortunately, the concept of an `IntegralDomain` requires the Ring
    //! multiplication to be commutative. Hence,
    //! @code
    //!     x * y == y * x
    //! @endcode
    //! and we do not have to distinguish between left and right zero divisors.
    //!
    //! Typical examples of `IntegralDomain`s include integers, integers
    //! modulo a prime number and polynomials over a field. The method
    //! names used here refer to the integral domain of integers under
    //! the usual addition and multiplication.
    //!
    //!
    //! Superclasses
    //! ------------
    //! `Monoid`, `Group`, `Ring`
    //!
    //!
    //! Laws
    //! ----
    //! For all objects `a`, `b` and `k` of an `IntegralDomain` `D`, the
    //! following laws must be satisfied:
    //! @code
    //!     mult(a, b) == mult(b, a)                                   // commutativity
    //!     plus(mult(quot(a, b), b), mod(a, b)) == a if b is non-zero
    //!     mod(plus(a, mult(k, b)), b) == mod(a, b)  if b is non-zero // canonicity
    //!     mod(zero<D>(), b) == zero<D>()            if b is non-zero
    //! @endcode
    //!
    //!
    //! Minimal complete definitions
    //! ----------------------------
    //! `mod` and `quot` satisfying the above laws
    //!
    //!
    //! Provided models
    //! ---------------
    //! 1. For non-boolean integral data types\n
    //! A data type `T` is integral if `std::is_integral<T>::%value` is true.
    //! For a non-boolean integral data type `T`, a model of `IntegralDomain`
    //! is automatically defined by using the `Ring` model provided for
    //! arithmetic data types and setting
    //! @code
    //!     quot(x, y) = (x / y)
    //!     mod(x, y) = (x % y)
    //! @endcode
    //!
    //! @note
    //! The rationale for not providing an IntegralDomain model for `bool` is
    //! the same as for not providing Monoid, Group and Ring models.
    //!
    //!
    //! Operators
    //! ---------
    //! For convenience, the following operators are provided as an
    //! equivalent way of calling the corresponding method:
    //! @code
    //!     /  ->  quot
    //!     %  ->  mod
    //! @endcode
    //!
    //!
    //! @todo
    //! Implement other utility methods like `divides`, `even` and `odd`.
    //! Also ponder adding `div_mod`, which might allow optimizations.
    //!
    //! [1]: http://en.wikipedia.org/wiki/Integral_domain
    struct IntegralDomain { };

    //! Generalized integer quotient.
    //! @relates IntegralDomain
    //!
    //!
    //! Cross-type version of the method
    //! --------------------------------
    //! The `quot` method is "overloaded" to handle distinct data types
    //! with certain properties. Specifically, `quot` is defined for
    //! _distinct_ data types `A` and `B` such that
    //! 1. `A` and `B` share a common data type `C`, as determined by the
    //!    `common` metafunction
    //! 2. `A`, `B` and `C` are all `IntegralDomain`s when taken individually
    //! 3. `to<C> : A -> B` and `to<C> : B -> C` are `Ring`-embeddings, as
    //!    determined by the `is_embedding` metafunction.
    //!
    //! In that case, the `quot` method is defined as
    //! @code
    //!     quot(x, y) = quot(to<C>(x), to<C>(y))
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/integral_domain.cpp quot
    //!
    //!
    //! @todo
    //! Consider moving this to a `Field` concept?
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto quot = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct quot_impl;

    struct _quot {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return quot_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _quot quot{};
#endif

    //! Generalized integer remainder.
    //! @relates IntegralDomain
    //!
    //! Specifically, `mod(x, y)` picks a canonical element of the equivalence
    //! class of `x` in the ideal generated by `y`. If that does not ring
    //! a bell, just think of it as the `%` operator.
    //!
    //!
    //! Cross-type version of the method
    //! --------------------------------
    //! The `mod` method is "overloaded" to handle distinct data types
    //! with certain properties. Specifically, `mod` is defined for
    //! _distinct_ data types `A` and `B` such that
    //! 1. `A` and `B` share a common data type `C`, as determined by the
    //!    `common` metafunction
    //! 2. `A`, `B` and `C` are all `IntegralDomain`s when taken individually
    //! 3. `to<C> : A -> B` and `to<C> : B -> C` are `Ring`-embeddings, as
    //!    determined by the `is_embedding` metafunction.
    //!
    //! In that case, the `mod` method is defined as
    //! @code
    //!     mod(x, y) = mod(to<C>(x), to<C>(y))
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/integral_domain.cpp mod
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto mod = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct mod_impl;

    struct _mod {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return mod_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _mod mod{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_INTEGRAL_DOMAIN_HPP

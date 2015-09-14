/*!
@file
Forward declares `boost::hana::IntegralDomain`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CONCEPT_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_FWD_CONCEPT_INTEGRAL_DOMAIN_HPP

namespace boost { namespace hana {
    //! @ingroup group-concepts
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
    //! Minimal complete definition
    //! ---------------------------
    //! `div` and `mod` satisfying the laws below
    //!
    //!
    //! Laws
    //! ----
    //! To simplify the reading, we will use the `+`, `*`, `/` and `%`
    //! operators with infix notation to denote the application of the
    //! corresponding methods in Monoid, Group, Ring and IntegralDomain.
    //! For all objects `a` and `b` of an `IntegralDomain` `D`, the
    //! following laws must be satisfied:
    //! @code
    //!     a * b == b * a // commutativity
    //!     (a / b) * b + a % b == a    if b is non-zero
    //!     zero<D>() % b == zero<D>()  if b is non-zero
    //! @endcode
    //!
    //!
    //! Refined concepts
    //! ----------------
    //! `Monoid`, `Group`, `Ring`
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `hana::integral_constant`
    //!
    //!
    //! Free model for non-boolean integral data types
    //! ----------------------------------------------
    //! A data type `T` is integral if `std::is_integral<T>::%value` is true.
    //! For a non-boolean integral data type `T`, a model of `IntegralDomain`
    //! is automatically defined by using the `Ring` model provided for
    //! arithmetic data types and setting
    //! @code
    //!     div(x, y) = (x / y)
    //!     mod(x, y)  = (x % y)
    //! @endcode
    //!
    //! @note
    //! The rationale for not providing an IntegralDomain model for `bool` is
    //! the same as for not providing Monoid, Group and Ring models.
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Integral_domain
    template <typename D>
    struct IntegralDomain;
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CONCEPT_INTEGRAL_DOMAIN_HPP

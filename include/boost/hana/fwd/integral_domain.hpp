/*!
@file
Forward declares `boost::hana::IntegralDomain`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_FWD_INTEGRAL_DOMAIN_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/default.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/fwd/core/operators.hpp>


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
    //! `quot` and `rem` satisfying the laws below
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
    //! Superclasses
    //! ------------
    //! `Monoid`, `Group`, `Ring`
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `IntegralConstant`
    //!
    //!
    //! Free model for non-boolean integral data types
    //! ----------------------------------------------
    //! A data type `T` is integral if `std::is_integral<T>::%value` is true.
    //! For a non-boolean integral data type `T`, a model of `IntegralDomain`
    //! is automatically defined by using the `Ring` model provided for
    //! arithmetic data types and setting
    //! @code
    //!     quot(x, y) = (x / y)
    //!     rem(x, y)  = (x % y)
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
    //!     %  ->  rem
    //! @endcode
    //!
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
            using T = typename datatype<X>::type;
            using U = typename datatype<Y>::type;
            using Quot = BOOST_HANA_DISPATCH_IF(decltype(quot_impl<T, U>{}),
                _models<IntegralDomain, T>{}() &&
                _models<IntegralDomain, U>{}() &&
                !is_default<quot_impl<T, U>>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<IntegralDomain, T>{},
            "hana::quot(x, y) requires x to be an IntegralDomain");

            static_assert(_models<IntegralDomain, U>{},
            "hana::quot(x, y) requires y to be an IntegralDomain");

            static_assert(!is_default<quot_impl<T, U>>{}(),
            "hana::quot(x, y) requires x and y to be embeddable "
            "in a common IntegralDomain");
        #endif

            return Quot::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
        }
    };

    constexpr _quot quot{};
#endif

    //! Generalized integer remainder.
    //! @relates IntegralDomain
    //!
    //! Given two elements of an IntegralDomain `x` and `y`, with `y`
    //! nonzero, `rem` returns the remainder of the division of `x` by `y`.
    //! In other words, `rem` can be seen as an equivalent to `%`.
    //!
    //! Cross-type version of the method
    //! --------------------------------
    //! The `rem` method is "overloaded" to handle distinct data types
    //! with certain properties. Specifically, `rem` is defined for
    //! _distinct_ data types `A` and `B` such that
    //! 1. `A` and `B` share a common data type `C`, as determined by the
    //!    `common` metafunction
    //! 2. `A`, `B` and `C` are all `IntegralDomain`s when taken individually
    //! 3. `to<C> : A -> B` and `to<C> : B -> C` are `Ring`-embeddings, as
    //!    determined by the `is_embedding` metafunction.
    //!
    //! In that case, `rem` is defined as
    //! @code
    //!     rem(x, y) = rem(to<C>(x), to<C>(y))
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/integral_domain.cpp rem
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto rem = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct rem_impl;

    struct _rem {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            using T = typename datatype<X>::type;
            using U = typename datatype<Y>::type;
            using Rem = BOOST_HANA_DISPATCH_IF(decltype(rem_impl<T, U>{}),
                _models<IntegralDomain, T>{}() &&
                _models<IntegralDomain, U>{}() &&
                !is_default<rem_impl<T, U>>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<IntegralDomain, T>{},
            "hana::rem(x, y) requires x to be an IntegralDomain");

            static_assert(_models<IntegralDomain, U>{},
            "hana::rem(x, y) requires y to be an IntegralDomain");

            static_assert(!is_default<rem_impl<T, U>>{},
            "hana::rem(x, y) requires x and y to be embeddable "
            "in a common IntegralDomain");
        #endif

            return Rem::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
        }
    };

    constexpr _rem rem{};
#endif

    namespace operators {
        template <>
        struct of<IntegralDomain>
            : decltype(quot), decltype(rem)
        { };
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_INTEGRAL_DOMAIN_HPP

/*!
@file
Forward declares `boost::hana::Monoid`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MONOID_HPP
#define BOOST_HANA_FWD_MONOID_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/default.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Monoid` concept represents data types with an associative
    //! binary operation that has an identity.
    //!
    //! Specifically, a [Monoid][1] is a basic algebraic structure typically
    //! used in mathematics to construct more complex algebraic structures
    //! like `Group`s, `Ring`s and so on. They are useful in several contexts,
    //! notably to define the properties of numbers in a granular way. At its
    //! core, a `Monoid` is a set `S` of objects along with a binary operation
    //! (let's say `+`) that is associative and that has an identity in `S`.
    //! There are many examples of `Monoid`s:
    //! - strings with concatenation and the empty string as the identity
    //! - integers with addition and `0` as the identity
    //! - integers with multiplication and `1` as the identity
    //! - many others...
    //!
    //! As you can see with the integers, there are some sets that can be
    //! viewed as a monoid in more than one way, depending on the choice
    //! of the binary operation and identity. The method names used here
    //! refer to the monoid of integers under addition; `plus` is the binary
    //! operation and `zero` is the identity element of that operation.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `plus` and `zero` satisfying the laws
    //!
    //!
    //! Laws
    //! ----
    //! For all objects `x`, `y` and `z` of a `Monoid` `M`, the following
    //! laws must be satisfied:
    //! @code
    //!     plus(zero<M>(), x) == x                    // left zero
    //!     plus(x, zero<M>()) == x                    // right zero
    //!     plus(x, plus(y, z)) == plus(plus(x, y), z) // associativity
    //! @endcode
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `IntegralConstant`
    //!
    //!
    //! Free model for non-boolean arithmetic data types
    //! ------------------------------------------------
    //! A data type `T` is arithmetic if `std::is_arithmetic<T>::%value` is
    //! true. For a non-boolean arithmetic data type `T`, a model of `Monoid`
    //! is automatically defined by setting
    //! @code
    //!     plus(x, y) = (x + y)
    //!     zero<T>() = static_cast<T>(0)
    //! @endcode
    //!
    //! > #### Rationale for not making `bool` a `Monoid` by default
    //! > First, it makes no sense whatsoever to define an additive `Monoid`
    //! > over the `bool` type. Also, it could make sense to define a `Monoid`
    //! > with logical conjunction or disjunction. However, C++ allows `bool`s
    //! > to be added, and the method names of this concept really suggest
    //! > addition. In line with the principle of least surprise, no model
    //! > is provided by default.
    //!
    //!
    //! Structure-preserving functions
    //! ------------------------------
    //! Let `A` and `B` be two `Monoid`s. A function `f : A -> B` is said
    //! to be a [Monoid morphism][2] if it preserves the monoidal structure
    //! between `A` and `B`. Rigorously, for all objects `x, y` of data
    //! type `A`,
    //! @code
    //!     f(plus(x, y)) == plus(f(x), f(y))
    //!     f(zero<A>()) == zero<B>()
    //! @endcode
    //! Functions with these properties interact nicely with `Monoid`s, which
    //! is why they are given such a special treatment.
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Monoid
    //! [2]: http://en.wikipedia.org/wiki/Monoid#Monoid_homomorphisms
    struct Monoid { };

    //! Associative binary operation on a `Monoid`.
    //! @relates Monoid
    //!
    //! @param x, y
    //! Two objects to combine with the `Monoid`'s binary operation.
    //!
    //!
    //! Cross-type version of the method
    //! --------------------------------
    //! The `plus` method is "overloaded" to handle distinct data types
    //! with certain properties. Specifically, `plus` is defined for
    //! _distinct_ data types `A` and `B` such that
    //! 1. `A` and `B` share a common data type `C`, as determined by the
    //!    `common` metafunction
    //! 2. `A`, `B` and `C` are all `Monoid`s when taken individually
    //! 3. `to<C> : A -> B` and `to<C> : B -> C` are `Monoid`-embeddings, as
    //!    determined by the `is_embedding` metafunction.
    //!
    //! The definition of `plus` for data types satisfying the above
    //! properties is obtained by setting
    //! @code
    //!     plus(x, y) = plus(to<C>(x), to<C>(y))
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monoid.cpp plus
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto plus = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct plus_impl;

    struct _plus {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            using T = typename datatype<X>::type;
            using U = typename datatype<Y>::type;
            using Plus = BOOST_HANA_DISPATCH_IF(decltype(plus_impl<T, U>{}),
                _models<Monoid, T>{}() &&
                _models<Monoid, U>{}() &&
                !is_default<plus_impl<T, U>>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Monoid, T>{},
            "hana::plus(x, y) requires 'x' to be a Monoid");

            static_assert(_models<Monoid, U>{},
            "hana::plus(x, y) requires 'y' to be a Monoid");

            static_assert(!is_default<plus_impl<T, U>>{}(),
            "hana::plus(x, y) requires 'x' and 'y' to be embeddable "
            "in a common Monoid");
        #endif

            return Plus::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
        }
    };

    constexpr _plus plus{};
#endif

    //! Identity of `plus`.
    //! @relates Monoid
    //!
    //! @tparam M
    //! The data type (a `Monoid`) of the returned identity.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monoid.cpp zero
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto zero = []() -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct zero_impl;

    template <typename M>
    struct _zero {

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Monoid, M>{},
        "hana::zero<M>() requires M to be a Monoid");
    #endif

        constexpr decltype(auto) operator()() const {
            using Zero = BOOST_HANA_DISPATCH_IF(zero_impl<M>,
                _models<Monoid, M>{}()
            );

            return Zero::apply();
        }
    };

    template <typename M>
    constexpr _zero<M> zero{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MONOID_HPP

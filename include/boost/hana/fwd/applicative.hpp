/*!
@file
Forward declares `boost::hana::Applicative`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_APPLICATIVE_HPP
#define BOOST_HANA_FWD_APPLICATIVE_HPP

#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Applicative` concept represents `Functor`s with the ability
    //! to lift values and combine computations.
    //!
    //! A `Functor` can only take a normal function and map it over a
    //! structure containing values to obtain a new structure containing
    //! values. Intuitively, an `Applicative` can also take a value and
    //! lift it into the structure. In addition, an `Applicative` can take
    //! a structure containing functions and apply it to a structure
    //! containing values to obtain a new structure containing values.
    //! By currying the function(s) inside the structure, it is then
    //! also possible to apply n-ary functions to n structures containing
    //! values.
    //!
    //!
    //! Laws
    //! ----
    //! Given an Applicative `F`, the following laws must be satisfied:
    //!
    //! 1. Identity\n
    //! For all objects `xs` of tag `F(A)`,
    //! @code
    //!     ap(lift<F>(id), xs) == xs
    //! @endcode
    //!
    //! 2. Composition\n
    //! For all objects `xs` of tag `F(A)` and functions-in-an-applicative
    //! @f$ fs : F(B \to C) @f$,
    //! @f$ gs : F(A \to B) @f$,
    //! @code
    //!     ap(ap(lift<F>(compose), fs, gs), xs) == ap(fs, ap(gs, xs))
    //! @endcode
    //!
    //! 3. Homomorphism\n
    //! For all objects `x` of tag `A` and functions @f$ f : A \to B @f$,
    //! @code
    //!     ap(lift<F>(f), lift<F>(x)) == lift<F>(f(x))
    //! @endcode
    //!
    //! 4. Interchange\n
    //! For all objects `x` of tag `A` and functions-in-an-applicative
    //! @f$ fs : F(A \to B) @f$,
    //! @code
    //!     ap(fs, lift<F>(x)) == ap(lift<F>(apply(-, x)), fs)
    //! @endcode
    //! where `apply(-, x)` denotes the partial application of the `apply`
    //! function from the @ref group-functional module to the `x` argument.
    //!
    //! As a consequence of these laws, the model of `Functor` for `F` will
    //! satisfy the following for all objects `xs` of tag `F(A)` and functions
    //! @f$ f : A \to B @f$:
    //! @code
    //!     transform(xs, f) == ap(lift<F>(f), xs)
    //! @endcode
    //!
    //!
    //! Superclass
    //! ----------
    //! 1. `Functor` (free model)\n
    //! As a consequence of the above laws, any `Applicative F` can be made a
    //! `Functor` by setting
    //! @code
    //!     transform(xs, f) = ap(lift<F>(f), xs)
    //! @endcode
    //! This implementation of `transform` is provided as
    //! `Applicative::transform_impl`. To use it, simply inherit
    //! `Applicative::transform_impl` in your definition of `transform_impl`:
    //! @code
    //!     template <>
    //!     struct transform_impl<YourApplicative>
    //!         : Applicative::transform_impl<YourApplicative>
    //!     { };
    //! @endcode
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `lift` and `ap` satisfying the above laws
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `Either`, `Lazy`, `Optional`, `Tuple`
    //!
    //!
    //! @anchor applicative-transformation
    //! Structure-preserving functions
    //! ------------------------------
    //! An _applicative transformation_ is a function @f$ t : F(X) \to G(X) @f$
    //! between two Applicatives `F` and `G`, where `X` can be any tag, and
    //! which preserves the operations of an Applicative. In other words, for
    //! all objects `x` of tag `X`, functions-in-an-applicative
    //! @f$ fs : F(X \to Y) @f$ and objects `xs` of tag `F(X)`,
    //! @code
    //!     t(lift<F>(x)) == lift<G>(x)
    //!     t(ap(fs, xs)) == ap(t(fs), t(xs))
    //! @endcode
    struct Applicative {
        template <typename A>
        struct transform_impl;
    };

    //! Lifted application.
    //! @relates Applicative
    //!
    //! Specifically, `ap` applies a structure containing functions to a
    //! structure containing values, and returns a new structure containing
    //! values. The exact way in which the functions are applied to the values
    //! depends on the `Applicative`.
    //!
    //! `ap` can be called with two arguments or more; the functions in the `f`
    //! structure are curried and then applied to the values in each `x...`
    //! structure using the binary form of `ap`. Note that this requires the
    //! number of `x...` must match the arity of the functions in the `f`
    //! structure. In other words, `ap(f, x1, ..., xN)` is equivalent to
    //! @code
    //!     ((f' <ap> x1) <ap> x2) ... <ap> xN
    //! @endcode
    //! where `f'` is `f` but containing curried functions instead and
    //! `x <ap> y` is just `ap(x, y)` written in infix notation to emphasize
    //! the left associativity.
    //!
    //!
    //! Signature
    //! ---------
    //! Given an Applicative `A`, the signature is
    //! @f$ \mathrm{ap} : A(T_1 \times \cdots \times T_n \to U)
    //!                   \times A(T_1) \times \cdots \times A(T_n)
    //!                   \to A(U) @f$.
    //!
    //! @param f
    //! A structure containing function(s).
    //!
    //! @param x...
    //! Structure(s) containing value(s) and on which `f` is applied. The
    //! number of structures must match the arity of the functions in the
    //! `f` structure.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/applicative.cpp ap
    //!
    //! @todo
    //! Consider giving access to all the arguments to the tag-dispatched
    //! implementation for performance purposes.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto ap = [](auto&& f, auto&& ...x) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename A, typename = void>
    struct ap_impl;

    struct _ap {
        template <typename F, typename X>
        constexpr decltype(auto) operator()(F&& f, X&& x) const;

        template <typename F, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, Xs&& ...xs) const;
    };

    constexpr _ap ap{};
#endif

    //! Lift a value into an `Applicative` structure.
    //! @relates Applicative
    //!
    //! `lift<A>` takes a normal value and embeds it into a structure whose
    //! shape is represented by the `A` `Applicative`. Note that the value
    //! may be a function, in which case the created structure may be
    //! `ap`plied to another `Applicative` structure containing values.
    //!
    //!
    //! Signature
    //! ---------
    //! Given an Applicative `A`, the signature is
    //! @f$ \mathrm{lift}_A : T \to A(T) @f$.
    //!
    //! @tparam A
    //! A tag representing the `Applicative` into which the value is lifted.
    //!
    //! @param x
    //! The value to lift into the applicative.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/applicative.cpp lift
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename A>
    constexpr auto lift = [](auto&& x) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename A, typename = void>
    struct lift_impl;

    template <typename A>
    struct _lift {
    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Applicative, A>{},
        "hana::lift<A> requires A to be an Applicative");
    #endif

        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const {
            using Lift = BOOST_HANA_DISPATCH_IF(
                lift_impl<A>,
                _models<Applicative, A>{}()
            );

            return Lift::apply(static_cast<X&&>(x));
        }
    };

    template <typename A>
    constexpr _lift<A> lift{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_APPLICATIVE_HPP

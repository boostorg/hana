/*!
@file
Forward declares `boost::hana::Applicative`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_APPLICATIVE_HPP
#define BOOST_HANA_FWD_APPLICATIVE_HPP

#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
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
    //! For any `Applicative` `A`, the following laws must be satisfied:
    //! @code
    //!     ap(lift<A>(id), u) == u                          // identity
    //!     ap(lift<A>(compose), u, v, w) == ap(u, ap(v, w)) // composition
    //!     ap(lift<A>(f), lift<A>(x)) == lift<A>(f(x))      // homomorphism
    //!     ap(u, lift<A>(y)) == ap(lift<A>(apply(y)), u)    // interchange
    //! @endcode
    //!
    //! As a consequence of these laws, the model of `Functor` for `A` will
    //! satisfy
    //! @code
    //!     fmap(x, f) == ap(lift<A>(f), x)
    //! @endcode
    //!
    //!
    //! Superclass
    //! ----------
    //! 1. `Functor` (model provided)\n
    //! As a consequence of the above laws, any `Applicative A` can be made a
    //! `Functor` by setting
    //! @code
    //!     fmap(x, f) = ap(lift<A>(f), x)
    //! @endcode
    //! This implementation of `fmap` is provided as `Applicative::fmap_impl`.
    //! To use it, simply inherit `Applicative::fmap_impl` in your definition
    //! of `fmap_impl`:
    //! @code
    //!     template <>
    //!     struct fmap_impl<YourApplicative>
    //!         : Applicative::fmap_impl<YourApplicative>
    //!     { };
    //! @endcode
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `lift` and `ap` satisfying the above laws
    //!
    //!
    //! Structure-preserving functions
    //! ------------------------------
    //! An _applicative transformation_ is a function `f : A -> B` from an
    //! `Applicative A` to an `Applicative B`, which preserves the operations
    //! of an `Applicative`:
    //! @code
    //!     f(lift<A>(x)) == lift<B>(x)
    //!     f(ap(x, y)) == ap(f(x), f(y))
    //! @endcode
    struct Applicative {
        template <typename A>
        struct fmap_impl;
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
    //! Consider giving access to all the arguments to the type class
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
    //! @param x
    //! The value to lift into the applicative.
    //!
    //! @tparam A
    //! The data type (an `Applicative`) into which the value is lifted.
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
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const {
            return lift_impl<A>::apply(detail::std::forward<X>(x));
        }
    };

    template <typename A>
    constexpr _lift<A> lift{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_APPLICATIVE_HPP

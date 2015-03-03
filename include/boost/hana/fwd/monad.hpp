/*!
@file
Forward declares `boost::hana::Monad`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MONAD_HPP
#define BOOST_HANA_FWD_MONAD_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Monad` concept represents `Applicative`s with the ability to
    //! flatten nested levels of structure.
    //!
    //! Intuitively, using the Functor-as-a-box analogy, a `Monad` is able
    //! to flatten a box containing a box into just a single box. In other
    //! words, it is able to remove one level of monadic structure. This
    //! allows Monads to combine not only structures, but also functions
    //! returning structures, which turns out to be pretty useful.
    //!
    //! Historically, Monads are a construction coming from category theory,
    //! an abstract branch of mathematics. The functional programming
    //! community eventually discovered how Monads could be used to
    //! formalize several useful things like side effects, which led
    //! to the wide adoption of Monads in that community. However, even
    //! in a multi-paradigm language like C++, there are several constructs
    //! which turn out to be Monads, like `std::optional`, `std::vector` and
    //! others.
    //!
    //! Everybody tries to introduce Monads with a different analogy, and
    //! almost everybody fails. This is called the [Monad tutorial fallacy][1].
    //! We won't fall into this trap here and will let readers who are new to
    //! this concept read one of the many excellent tutorials available online.
    //! Understanding Monads might take time at first, but once you get it, a
    //! lot of patterns will become obvious Monads; this enlightening will be
    //! your reward for the hard work. That being said, there are different
    //! ways of defining a Monad; some will use the `bind` function (`>>=` in
    //! Haskell), others will use `join` (called `flatten` in this library),
    //! and there are probably other ways. In Hana, the Monad laws are written
    //! in term of the monadic composition operator, called `mcompose`, which
    //! we think gives the best intuition on the relation between a Monad and
    //! a Monoid.
    //!
    //!
    //! Laws
    //! ----
    //! In what follows, we use equality between functions as meaning
    //! @code
    //!     f == g  if and only if  f(x) == g(x) for all x
    //! @endcode
    //! This is to simplify writing the laws, which would be tedious otherwise.
    //! For a data type `M` to be a `Monad`, the following must be satisfied
    //! for all functions `f, g, h` with a suitable signature for the
    //! composition to make sense:
    //! @code
    //!     mcompose<M>(lift<M>, f)           == f                                 // left identity
    //!     mcompose<M>(f, lift<M>)           == f                                 // right identity
    //!     mcompose<M>(mcompose<M>(f, g), h) == mcompose<M>(f, mcompose<M>(g, h)) // associativity
    //! @endcode
    //! By comparing with the `Monoid` laws, one can clearly see how these
    //! two structures have something in common.
    //!
    //!
    //! Superclasses
    //! ------------
    //! `Functor` and `Applicative`
    //!
    //!
    //! Provided superclass methods
    //! ---------------------------
    //! 1. `ap`\n
    //! When the minimal complete definition for `Monad` and `Functor` are
    //! both satisfied, it is possible to automatically implement `ap` by
    //! setting
    //! @code
    //!     ap(fs, x) = bind(fs, [](auto f) {
    //!         return transform(x, f);
    //!     })
    //! @endcode
    //!
    //! This implementation is provided through the `Monad::ap_impl` type,
    //! which you can use as follows:
    //! @code
    //!     template <>
    //!     struct ap_impl<YourDatatype>
    //!         : Monad::ap_impl<YourDatatype>
    //!     { };
    //! @endcode
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! 1. `flatten`\n
    //! When `flatten` is defined, `bind` is defined by setting:
    //! @code
    //!     bind(xs, f) = flatten(transform(xs, f))
    //! @endcode
    //!
    //! 2. `bind`\n
    //! When `bind` is defined, `flatten` is defined by setting:
    //! @code
    //!     flatten(xs) = bind(xs, id)
    //! @endcode
    //!
    //!
    //! Operators
    //! ---------
    //! For convenience, the following operator is provided as an
    //! equivalent way of calling the corresponding method:
    //! @code
    //!     xs | f  ->  bind(xs, f)
    //! @endcode
    //!
    //! > Note: `xs | f` was preferred over `xs >>= f` (as in Haskell),
    //! > because `>>=` is right associative in C++, which would make it
    //! > impossible to chain computations.
    //!
    //!
    //! [1]: https://byorgey.wordpress.com/2009/01/12/abstraction-intuition-and-the-monad-tutorial-fallacy/
    struct Monad {
        template <typename M>
        struct ap_impl;
    };

    //! Apply a function returning a monadic value to the value(s)
    //! inside a monad.
    //! @relates Monad
    //!
    //!
    //! @param xs
    //! A structure containing value(s) to transform with `f`.
    //!
    //! @param f
    //! A function taking a value inside the `xs` structure and returning a
    //! monadic value. It will be called as `f(x)` for an element `x` of the
    //! structure `xs`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad.bind.cpp bind
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto bind = [](auto&& xs, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct bind_impl;

    struct _bind {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return bind_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _bind bind{};
#endif

    //! Collapse two levels of monadic structure into a single level.
    //! @relates Monad
    //!
    //! @param xs
    //! An object with two levels of monadic structure, which should be
    //! collapsed into a single level of structure.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad.cpp flatten
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto flatten = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct flatten_impl;

    struct _flatten {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return flatten_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _flatten flatten{};
#endif

    //! Composition of monadic functions.
    //! @relates Monad
    //!
    //! Specifically, this is equivalent to the `compose` function for
    //! functions that return `Monad`s.
    //!
    //! @param f
    //! A function with pseudo-signature `f :: A -> M<B>`.
    //!
    //! @param g
    //! A function with pseudo-signature `g :: B -> M<C>`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad.cpp mcompose
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto mcompose = [](auto&& f, auto&& g) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct mcompose_impl;

    template <typename M>
    struct _mcompose {
        template <typename F, typename G>
        constexpr decltype(auto) operator()(F&& f, G&& g) const {
            return mcompose_impl<M>::apply(
                detail::std::forward<F>(f),
                detail::std::forward<G>(g)
            );
        }
    };

    template <typename M>
    constexpr _mcompose<M> mcompose{};
#endif

    //! Sequentially compose two monadic actions, discarding any value
    //! produced by the first but not its effects.
    //! @relates Monad
    //!
    //! @note
    //! This is equivalent to Haskell's `>>`.
    //!
    //!
    //! @param before
    //! The first `Monad` in the monadic composition chain. The result of
    //! this monad is ignored, but its effects are combined with that of the
    //! second monad.
    //!
    //! @param xs
    //! The second `Monad` in the monadic composition chain.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad.cpp then
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto then = [](auto&& before, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct then_impl;

    struct _then {
        template <typename Before, typename Xs>
        constexpr decltype(auto) operator()(Before&& before, Xs&& xs) const {
            return then_impl<typename datatype<Before>::type>::apply(
                detail::std::forward<Before>(before),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _then then{};
#endif

    //! Tap inside a monadic chain.
    //! @relates Monad
    //!
    //! Specifically, `tap<M>(f)(x)` performs `f(x)` and returns `lift<M>(x)`.
    //! Combined with the law saying that `bind(m, lift<M>) == m`, this
    //! provides a way of executing an action inside a monadic chain without
    //! influencing its result. This is useful to e.g. insert debug statements
    //! or perform actions that are not tied to the chain but that need to be
    //! executed inside of it.
    //!
    //!
    //! @tparam M
    //! The data type (a `Monad`) of the monads in the tapped monadic chain.
    //!
    //! @param f
    //! A function to be executed inside a monadic chain. It will be called
    //! as `f(x)`, where `x` is a value inside the previous monad in the
    //! chain. The result of `f` is always discarded.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad.cpp tap
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto tap = [](auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct tap_impl;

    template <typename M>
    struct _tap {
        template <typename F>
        constexpr decltype(auto) operator()(F&& f) const {
            return tap_impl<M>::apply(detail::std::forward<decltype(f)>(f));
        }
    };

    template <typename M>
    constexpr _tap<M> tap{};
#endif

    template <>
    struct operators::of<Monad>
        : decltype(bind)
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MONAD_HPP

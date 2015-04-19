/*!
@file
Forward declares `boost::hana::Monad`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MONAD_HPP
#define BOOST_HANA_FWD_MONAD_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/fwd/core/operators.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Monad` concept represents `Applicative`s with the ability to
    //! flatten nested levels of structure.
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
    //! Everybody tries to introduce `Monad`s with a different analogy, and
    //! most people fail. This is called the [Monad tutorial fallacy][1]. We
    //! will try to avoid this trap by not presenting a specific intuition,
    //! and we will instead present what monads are mathematically.
    //! For specific intuitions, we will let readers who are new to this
    //! concept read one of the many excellent tutorials available online.
    //! Understanding Monads might take time at first, but once you get it,
    //! a lot of patterns will become obvious Monads; this enlightening will
    //! be your reward for the hard work.
    //!
    //! There are different ways of defining a Monad; Haskell uses a function
    //! called `bind` (`>>=`) and another one called `return` (it has nothing
    //! to do with C++'s `return` statement). They then introduce relationships
    //! that must be satisfied for a type to be a Monad with those functions.
    //! Mathematicians sometimes use a function called `join` and another one
    //! called `unit`, or they also sometimes use other category theoretic
    //! constructions like functor adjunctions and the Kleisli category.
    //!
    //! This library uses a composite approach. First, we use the `flatten`
    //! function (equivalent to `join`) along with the `lift` function from
    //! `Applicative` (equivalent to `unit`) to introduce the notion of
    //! monadic function composition. We then write the properties that must
    //! be satisfied by a Monad using this monadic composition operator,
    //! because we feel it shows the link between Monads and Monoids more
    //! clearly than other approaches.
    //!
    //! Roughly speaking, we will say that a `Monad` is an `Applicative` which
    //! also defines a way to compose functions returning a monadic result,
    //! as opposed to only being able to compose functions returning a normal
    //! result. We will then ask for this composition to be associative and to
    //! have a neutral element, just like normal function composition. For
    //! usual composition, the neutral element is the identity function `id`.
    //! For monadic composition, the neutral element is the `lift` function
    //! defined by `Applicative`. This construction is made clearer in the
    //! laws below.
    //!
    //!
    //! Minimal complete definitions
    //! ----------------------------
    //! First, a `Monad` must be both a `Functor` and an `Applicative`.
    //! Also, an implementation of `flatten` or `chain` satisfying the
    //! laws below for monadic composition must be provided.
    //!
    //! @note
    //! The `ap` method for `Applicatives` may be derived from the minimal
    //! complete definition of `Monad` and `Functor`; see below for more
    //! information.
    //!
    //!
    //! Laws
    //! ----
    //! To simplify writing the laws, we use the comparison between functions.
    //! For two functions `f` and `g`, we define
    //! @code
    //!     f == g  if and only if  f(x) == g(x) for all x
    //! @endcode
    //!
    //! With the usual composition of functions, we are given two functions
    //! @f$ f : A \to B @f$ and @f$ g : B \to C @f$, and we must produce a
    //! new function @f$ compose(g, f) : A \to C @f$. This composition of
    //! functions is associative, which means that
    //! @code
    //!     compose(h, compose(g, f)) == compose(compose(h, g), f)
    //! @endcode
    //!
    //! Also, this composition has an identity element, which is the identity
    //! function. This simply means that
    //! @code
    //!     compose(f, id) == compose(id, f) == f
    //! @endcode
    //!
    //! This is probably nothing new if you are reading the `Monad` laws.
    //! Now, we can observe that the above is equivalent to saying that
    //! functions with the composition operator form a `Monoid`, where the
    //! neutral element is the identity function.
    //!
    //! Given an `Applicative` `F`, what if we wanted to compose two functions
    //! @f$ f : A \to F(B) @f$ and @f$ g : B \to F(C) @f$? When the
    //! `Applicative` `F` is also a `Monad`, such functions taking normal
    //! values but returning monadic values are called _monadic functions_.
    //! To compose them, we obviously can't use normal function composition,
    //! since the domains and codomains of `f` and `g` do not match properly.
    //! Instead, we'll need a new operator -- let's call it `monadic_compose`:
    //! @f[
    //!     \mathrm{monadic\_compose} :
    //!         (B \to F(C)) \times (A \to F(B)) \to (A \to F(C))
    //! @f]
    //!
    //! How could we go about implementing this function? Well, since we know
    //! `F` is an `Applicative`, the only functions we have are `transform`
    //! (from `Functor`), and `lift` and `ap` (from `Applicative`). Hence,
    //! the only thing we can do at this point while respecting the signatures
    //! of `f` and `g` is to set (for `x` of type `A`)
    //! @code
    //!     monadic_compose(g, f)(x) = transform(f(x), g)
    //! @endcode
    //!
    //! Indeed, `f(x)` is of type `F(B)`, so we can map `g` (which takes `B`'s)
    //! on it. Doing so will leave us with a result of type `F(F(C))`, but what
    //! we wanted was a result of type `F(C)` to respect the signature of
    //! `monadic_compose`. If we had a joker of type @f$ F(F(C)) \to F(C) @f$,
    //! we could simply set
    //! @code
    //!     monadic_compose(g, f)(x) = joker(transform(f(x), g))
    //! @endcode
    //!
    //! and we would be happy. It turns out that `flatten` is precisely this
    //! joker. Now, we'll want our joker to satisfy some properties to make
    //! sure this composition is associative, just like our normal composition
    //! was. These properties are slightly cumbersome to specify, so we won't
    //! do it here. Also, we'll need some kind of neutral element for the
    //! composition. This neutral element can't be the usual identity function,
    //! because it does not have the right type: our neutral element needs to
    //! be a function of type @f$ X \to F(X) @f$ but the identity function has
    //! type @f$ X \to X @f$. It is now the right time to observe that `lift`
    //! from `Applicative` has exactly the right signature, and so we'll take
    //! this for our neutral element.
    //!
    //! We are now ready to formulate the `Monad` laws using this composition
    //! operator. For a `Monad` `M` and functions @f$ f : A \to M(B) @f$,
    //! @f$ g : B \to M(C) @f$ and @f$ h : C \to M(D) @f$, the following
    //! must be satisfied:
    //! @code
    //!     // associativity
    //!     monadic_compose(h, monadic_compose(g, f)) == monadic_compose(monadic_compose(h, g), f)
    //!
    //!     // right identity
    //!     monadic_compose(f, lift<M(A)>) == f
    //!
    //!     // left identity
    //!     monadic_compose(lift<M(B)>, f) == f
    //! @endcode
    //!
    //! which is to say that `M` along with monadic composition is a Monoid
    //! where the neutral element is `lift`.
    //!
    //!
    //! Provided superclass methods
    //! ---------------------------
    //! 1. `ap` from `Applicative`\n
    //! When the minimal complete definition for `Monad` and `Functor` are
    //! both satisfied, it is possible to automatically implement `ap` by
    //! setting
    //! @code
    //!     ap(fs, xs) = chain(fs, [](auto f) {
    //!         return transform(xs, f);
    //!     })
    //! @endcode
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
    //! [1]: https://byorgey.wordpress.com/2009/01/12/abstraction-intuition-and-the-monad-tutorial-fallacy/
    struct Monad {
        template <typename M>
        struct ap_impl;
    };

    //! Feed a monadic value into a monadic computation.
    //! @relates Monad
    //!
    //! Given a monadic value and a monadic function, `chain` feeds the
    //! monadic value into the function, thus performing some Monad-specific
    //! effects, and returns the result. An implementation of `chain` must
    //! satisfy
    //! @code
    //!     chain(xs, f) == flatten(transform(xs, f))
    //! @endcode
    //!
    //!
    //! Signature
    //! ---------
    //! For a monad `M`, given a monadic value of type `M(A)` and a monadic
    //! function @f$ f : A \to M(B) @f$, `chain` has the signature
    //! @f$
    //!     \mathrm{chain} : M(A) \times (A \to M(B)) \to M(B)
    //! @f$.
    //!
    //! @param xs
    //! A monadic value to be fed to the function `f`.
    //!
    //! @param f
    //! A function taking a normal value in the `xs` structure, and returning
    //! a monadic value. This function is called as  `f(x)`, where `x` is an
    //! element of the structure `xs`.
    //!
    //!
    //! Operator-form
    //! -------------
    //! For convenience, the `chain` method can be applied to `Monad`s that
    //! support it by using the `|` operator. Hence, if `xs` supports the
    //! operator,
    //! @code
    //!     xs | f == chain(xs, f)
    //! @endcode
    //!
    //! Furthermore, this operator is left associative, which means that
    //! @code
    //!     xs | f | g == (xs | f) | g == chain(chain(xs, f), g)
    //! @endcode
    //!
    //! This allows creating chains of monadic computations easily.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad.chain.cpp chain
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto chain = [](auto&& xs, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct chain_impl;

    struct _chain {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            using M = typename datatype<Xs>::type;
            using Chain = chain_impl<M>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Monad, M>{},
            "hana::chain(xs, f) requires xs to be a Monad");
        #endif

            return Chain::apply(static_cast<Xs&&>(xs), static_cast<F&&>(f));
        }
    };

    constexpr _chain chain{};
#endif

    //! Collapse two levels of monadic structure into a single level.
    //! @relates Monad
    //!
    //! Given a monadic value wrapped into two levels of monad, `flatten`
    //! removes one such level. An implementation of `flatten` must satisfy
    //! @code
    //!     flatten(xs) == chain(xs, id)
    //! @endcode
    //!
    //!
    //! Signature
    //! ---------
    //! For a `Monad` `M` and an arbitrary data type `T`, `flatten` has a
    //! signature of
    //! @f$
    //!     \mathrm{flatten} : M(M(T)) \to M(T)
    //! @f$
    //!
    //! @param xs
    //! A value with two levels of monadic structure, which should be
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
            using M = typename datatype<Xs>::type;
            using Flatten = flatten_impl<M>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Monad, M>{},
            "hana::flatten(xs) requires xs to be a Monad");
        #endif

            return Flatten::apply(static_cast<Xs&&>(xs));
        }
    };

    constexpr _flatten flatten{};
#endif

    //! Composition of monadic functions.
    //! @relates Monad
    //!
    //! Given two monadic functions `f` and `g`, `monadic_compose` returns
    //! a new function equivalent to the composition of `f` with `g`, except
    //! the result of `g` is `chain`ed into `f` instead of simply passed to
    //! it, as with normal composition. `monadic_compose` satisfies
    //! @code
    //!     monadic_compose(f, g)(x) == chain(g(x), f)
    //! @endcode
    //!
    //!
    //! @note
    //! Unlike `compose`, `monadic_compose` does not generalize nicely to
    //! arities higher than one. Hence, only unary functions may be used
    //! with `monadic_compose`.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a `Monad` `M` and two functions @f$ f : B \to M(C) @f$ and
    //! @f$ g : A \to M(B) @f$, the signature is
    //! @f$
    //!     \mathrm{monadic_compose}
    //!         : (B \to M(C)) \times (A \to M(B)) \to (A \to M(C))
    //! @f$.
    //!
    //! @param f
    //! A monadic function with signature @f$ B \to M(C) @f$.
    //!
    //! @param g
    //! A monadic function with signature @f$ A \to M(B) @f$.
    //!
    //!
    //! @note
    //! This method is not tag-dispatched, so it can't be customized directly.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad.cpp monadic_compose
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto monadic_compose = [](auto&& f, auto&& g) {
        return [perfect-capture](auto&& x) -> decltype(auto) {
            return hana::chain(forwarded(g)(forwarded(x)), forwarded(f));
        };
    };
#else
    struct _monadic_compose {
        template <typename F, typename G>
        constexpr decltype(auto) operator()(F&& f, G&& g) const;
    };

    constexpr _monadic_compose monadic_compose{};
#endif

    //! Sequentially compose two monadic actions, discarding any value
    //! produced by the first but not its effects.
    //! @relates Monad
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
        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Monad, typename datatype<Before>::type>{},
            "hana::then(before, xs) requires before to be a Monad");

            static_assert(_models<Monad, typename datatype<Xs>::type>{},
            "hana::then(before, xs) requires xs to be a Monad");
        #endif

            return then_impl<typename datatype<Before>::type>::apply(
                static_cast<Before&&>(before),
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _then then{};
#endif

    //! Tap inside a monadic chain.
    //! @relates Monad
    //!
    //! Given a function `f`, `tap<M>` returns a new function which performs
    //! `f` on its argument and then returns the argument lifted in the `M`
    //! `Monad`. Combined with the property that `chain(m, lift<M>) == m`,
    //! this provides a way of executing an action inside a monadic chain
    //! without influencing its overall result. This is useful to e.g. insert
    //! debug statements or perform actions that are not tied to the chain but
    //! that need to be executed inside of it.
    //!
    //! @note
    //! Since C++ is not a pure language, it is possible to perform side
    //! effects inside the `f` function. Actually, side effects are the
    //! only reason why one might want to use `tap`. However, one should
    //! not rely on the side effects being done in any specific order.
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
    #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(_models<Monad, M>{},
        "hana::tap<M> requires M to be a Monad");
    #endif
        template <typename F>
        constexpr decltype(auto) operator()(F&& f) const
        { return tap_impl<M>::apply(static_cast<F&&>(f)); }
    };

    template <typename M>
    constexpr _tap<M> tap{};
#endif

    template <>
    struct operators::of<Monad>
        : decltype(chain)
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MONAD_HPP

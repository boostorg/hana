/*!
@file
Forward declares `boost::hana::Comonad`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_COMONAD_HPP
#define BOOST_HANA_FWD_COMONAD_HPP

#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    // Note: We use a multiline C++ comment because there's a double backslash
    // symbol in the documentation (for LaTeX), which triggers
    //      warning: multi-line comment [-Wcomment]
    // on GCC.

    /*!
    @ingroup group-concepts
    The `Comonad` concept represents context-sensitive computations and
    data.

    Formally, the Comonad concept is dual to the Monad concept.
    But unless you're a mathematician, you don't care about that and it's
    fine. So intuitively, a Comonad represents context sensitive values
    and computations. First, Comonads make it possible to extract
    context-sensitive values from their context with `extract`.
    In contrast, Monads make it possible to wrap raw values into
    a given context with `lift` (from Applicative).

    Second, Comonads make it possible to apply context-sensitive values
    to functions accepting those, and to return the result as a
    context-sensitive value using `extend`. In contrast, Monads make
    it possible to apply a monadic value to a function accepting a normal
    value and returning a monadic value, and to return the result as a
    monadic value (with `chain`).

    Finally, Comonads make it possible to wrap a context-sensitive value
    into an extra layer of context using `duplicate`, while Monads make
    it possible to take a value with an extra layer of context and to
    strip it with `flatten`.

    Whereas `lift`, `chain` and `flatten` from Applicative and Monad have
    types
    \f{align*}{
        \mathrm{lift}_M &: T \to M(T) \\
        \mathrm{chain} &: M(T) \times (T \to M(U)) \to M(U) \\
        \mathrm{flatten} &: M(M(T)) \to M(T)
    \f}

    `extract`, `extend` and `duplicate` from Comonad have types
    \f{align*}{
        \mathrm{extract} &: W(T) \to T \\
        \mathrm{extend} &: W(T) \times (W(T) \to U) \to W(U) \\
        \mathrm{duplicate} &: W(T) \to W(W(T))
    \f}

    Notice how the "arrows" are reversed. This symmetry is essentially
    what we mean by Comonad being the _dual_ of Monad.


    Minimal complete definition
    ---------------------------
    `extract` and (`extend` or `duplicate`) satisfying the laws below.
    A `Comonad` must also be a `Functor`.


    Laws
    ----
    For all Comonads `w`, the following laws must be satisfied:
    @code
        extract(duplicate(w)) == w
        transform(duplicate(w), extract) == w
        duplicate(duplicate(w)) == transform(duplicate(w), duplicate)
    @endcode

    @note
    There are several equivalent ways of defining Comonads, and this one
    is just one that was picked arbitrarily for simplicity.


    Superclass
    ----------
    1. Functor\n
    Every Comonad is also required to be a Functor. At first, one might think
    that it should instead be some imaginary concept CoFunctor. However, it
    turns out that a CoFunctor is the same as a `Functor`, hence the
    requirement that a `Comonad` also is a `Functor`.


    Concrete models
    ---------------
    `Lazy`

    */
    struct Comonad { };

    //! Extract a value in a given comonadic context.
    //! @relates Comonad
    //!
    //! Given a value inside a comonadic context, extract it from that
    //! context, performing whatever effects are mandated by that context.
    //! This can be seen as the dual operation to the `lift` method of the
    //! Applicative concept.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Comonad `W`, the signature is
    //! \f$
    //!     \mathrm{extract} : W(T) \to T
    //! \f$
    //!
    //! @param w
    //! The value to be extracted inside a comonadic context.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/comonad.cpp extract
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto extract = [](auto&& w) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename W, typename = void>
    struct extract_impl;

    struct _extract {
        template <typename W_>
        constexpr decltype(auto) operator()(W_&& w) const {
            using W = typename datatype<W_>::type;
            using Extract = BOOST_HANA_DISPATCH_IF(
                extract_impl<W>,
                _models<Comonad, W>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Comonad, W>{},
            "hana::extract(w) requires w to be a Comonad");
        #endif

            return Extract::apply(static_cast<W_&&>(w));
        }
    };

    constexpr _extract extract{};
#endif

    //! Add an extra layer of comonadic context to a comonadic value.
    //! @relates Comonad
    //!
    //! Given a value already in a comonadic context, `duplicate` wraps this
    //! value with an additional layer of comonadic context. This can be seen
    //! as the dual operation to `flatten` from the Monad concept.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Comonad `W`, the signature is
    //! \f$
    //!     \mathrm{duplicate} : W(T) \to W(W(T))
    //! \f$
    //!
    //! @param w
    //! The value to wrap in an additional level of comonadic context.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/comonad.cpp duplicate
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto duplicate = [](auto&& w) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename W, typename = void>
    struct duplicate_impl;

    struct _duplicate {
        template <typename W_>
        constexpr decltype(auto) operator()(W_&& w) const {
            using W = typename datatype<W_>::type;
            using Duplicate = BOOST_HANA_DISPATCH_IF(
                duplicate_impl<W>,
                _models<Comonad, W>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Comonad, W>{},
            "hana::duplicate(w) requires w to be a Comonad");
        #endif

            return Duplicate::apply(static_cast<W_&&>(w));
        }
    };

    constexpr _duplicate duplicate{};
#endif

    //! Comonadic application of a function to a comonadic value.
    //! @relates Comonad
    //!
    //! Given a comonadic value and a function accepting a comonadic input,
    //! `extend` returns the result of applying the function to that input
    //! inside the comonadic context.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Comonad `W` and a function of type \f$ W(T) \to U \f$, the
    //! signature is
    //! \f$
    //!     \mathrm{extend} : W(T) \times (W(T) \to U) \to W(U)
    //! \f$
    //!
    //! @param w
    //! A comonadic value to call the function with.
    //!
    //! @param f
    //! A function of signature \f$ W(T) \to U \f$ to be applied to its
    //! comonadic argument inside the comonadic context.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/comonad.cpp extend
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto extend = [](auto&& w, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename W, typename = void>
    struct extend_impl;

    struct _extend {
        template <typename W_, typename F>
        constexpr decltype(auto) operator()(W_&& w, F&& f) const {
            using W = typename datatype<W_>::type;
            using Extend = BOOST_HANA_DISPATCH_IF(
                extend_impl<W>,
                _models<Comonad, W>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Comonad, W>{},
            "hana::extend(w, f) requires w to be a Comonad");
        #endif

            return Extend::apply(static_cast<W_&&>(w), static_cast<F&&>(f));
        }
    };

    constexpr _extend extend{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_COMONAD_HPP

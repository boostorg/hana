/*!
@file
Forward declares `boost::hana::Lazy`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_LAZY_HPP
#define BOOST_HANA_FWD_LAZY_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/create.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! The `Lazy` data type implements superficial laziness via a monadic
    //! interface.
    //!
    //! It is important to understand that the laziness implemented by `Lazy`
    //! is only superficial; only function applications made inside the `Lazy`
    //! monad can be made lazy, not all their subexpressions.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Functor`\n
    //! Applying a function over a lazy value with `transform` returns the
    //! result of applying the function, as a lazy value.
    //! @snippet example/lazy.cpp functor
    //!
    //! 2. `Applicative`\n
    //! A normal value can be lifted into a lazy value by using `lift<Lazy>`.
    //! A lazy function can be lazily applied to a lazy value by using `ap`.
    //!
    //! 3. `Monad` (operators provided)\n
    //! The `Lazy` monad allows combining lazy computations into larger
    //! lazy computations.
    //! @snippet example/lazy.monad.cpp monad
    //!
    //!
    //! @note
    //! `Lazy` only models a few concepts because providing more functionality
    //! would require evaluating the lazy values in most cases. Since this
    //! raises some issues such as side effects and memoization, the interface
    //! is kept minimal.
    struct Lazy { };

    //! Evaluate a lazy value and return it.
    //! @relates Lazy
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto eval = [](auto&& lx) -> decltype(auto) {
        return unspecified;
    };
#else
    struct _eval {
        template <typename Lx>
        constexpr decltype(auto) operator()(Lx&& lx) const;
    };

    constexpr _eval eval{};
#endif

    //! Lifts a normal value to a lazy one.
    //! @relates Lazy
    //!
    //! Additionally, `lazy(f)` is a function such that `lazy(f)(x1, ..., xN)`
    //! is equivalent to `ap(lazy(f), lift<Lazy>(x1), ..., lift<Lazy>(xN))`,
    //! which in turn is equivalent to `lazy(f(x1, ..., xN))`, except for the
    //! fact that the inner call to `f` is evaluated lazily. Note that
    //! `lazy(f)()` is equivalent to `lazy(f())`, with the inner call to
    //! `f` being evaluated lazily. This is provided for convenience even
    //! though `ap(lazy(f))` would be invalid because `ap` requires 2
    //! arguments or more.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/lazy.cpp lazy
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto lazy = [](auto&& x) {
        return unspecified-type;
    };
#else
    template <typename F, typename X, typename = operators::adl>
    struct _lazy_call;

    template <typename X, typename = operators::adl>
    struct _lazy;

    constexpr detail::create<_lazy> lazy{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_LAZY_HPP

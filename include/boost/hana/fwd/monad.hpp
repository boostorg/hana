/*!
@file
Forward declares `boost::hana::Monad`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MONAD_HPP
#define BOOST_HANA_FWD_MONAD_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! `Monad`s are `Applicative`s with the ability to flatten values that
    //! were lifted more than once.
    //!
    //! ### Requires
    //! `Functor`, `Applicative`
    //!
    //! ### Laws
    //! Instances of `Monad` must satisfy the following laws:
    //! @code
    //!     bind(lift<M>(x), f) == f(x)
    //!     bind(m, lift<M>) == m
    //!     bind(m, [](auto x){ return bind(f(x), g); }) == bind(bind(m, f), g)
    //! @endcode
    struct Monad {
        BOOST_HANA_TYPECLASS(Monad);
        template <typename M>
        struct bind_mcd;

        template <typename M>
        struct flatten_mcd;

        template <typename M>
        struct list_mcd;
    };

    //! Apply a function returning a monad to the value(s) inside a monad.
    //! @relates Monad
    //!
    //!
    //! @param monad
    //! A `Monad` to compose monadically with the given function.
    //!
    //! @param f
    //! A function taking a value inside the `monad` and returning a `Monad`.
    //! It will be called as `f(x)`, where `x` is the same argument as `f`
    //! would be called with when doing `fmap(monad, f)`.
    //!
    //!
    //! ### Example
    //! @include example/monad/bind.cpp
    BOOST_HANA_CONSTEXPR_LAMBDA auto bind = [](auto&& monad, auto&& f) -> decltype(auto) {
        return Monad::instance<
            datatype_t<decltype(monad)>
        >::bind_impl(
            detail::std::forward<decltype(monad)>(monad),
            detail::std::forward<decltype(f)>(f)
        );
    };

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
    //! @param monad
    //! The second `Monad` in the monadic composition chain.
    //!
    //!
    //! ### Example
    //! @snippet example/monad/then.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto then = [](auto&& before, auto&& monad) -> decltype(auto) {
        return Monad::instance<
            datatype_t<decltype(before)>
        >::then_impl(
            detail::std::forward<decltype(before)>(before),
            detail::std::forward<decltype(monad)>(monad)
        );
    };

    //! Flatten two levels of monadic wrapping into a single level.
    //! @relates Monad
    //!
    //! @param monad
    //! A `Monad` containing a `Monad` that should be flattened to a `Monad`.
    //!
    //! ### Example
    //! @snippet example/monad/flatten.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto flatten = [](auto&& monad) -> decltype(auto) {
        return Monad::instance<
            datatype_t<decltype(monad)>
        >::flatten_impl(detail::std::forward<decltype(monad)>(monad));
    };

    namespace monad_detail {
        template <typename M>
        struct tap {
            template <typename F>
            constexpr decltype(auto) operator()(F&& f) const {
                return Monad::instance<M>::tap_impl(
                    detail::std::forward<decltype(f)>(f)
                );
            }
        };
    }

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
    //! ### Example
    //! @snippet example/monad/tap.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto tap = [](auto&& f) -> decltype(auto) {
        return Monad::instance<M>::tap_impl(std::forward<decltype(f)>(f));
    };
#else
    template <typename M>
    constexpr monad_detail::tap<M> tap{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MONAD_HPP

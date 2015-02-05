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
#include <boost/hana/core/when.hpp>
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
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto bind = [](auto&& monad, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct bind_impl : bind_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct bind_impl<T, when<condition>> {
        template <typename M, typename F>
        static constexpr decltype(auto) apply(M&& m, F&& f) {
            return Monad::instance<T>::bind_impl(
                detail::std::forward<M>(m),
                detail::std::forward<F>(f)
            );
        }
    };

    struct _bind {
        template <typename M, typename F>
        constexpr decltype(auto) operator()(M&& m, F&& f) const {
            return bind_impl<datatype_t<M>>::apply(
                detail::std::forward<M>(m),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _bind bind{};
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
    //! @param monad
    //! The second `Monad` in the monadic composition chain.
    //!
    //!
    //! ### Example
    //! @snippet example/monad/then.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto then = [](auto&& before, auto&& monad) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct then_impl : then_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct then_impl<T, when<condition>> {
        template <typename Before, typename M>
        static constexpr decltype(auto) apply(Before&& before, M&& m) {
            return Monad::instance<T>::then_impl(
                detail::std::forward<Before>(before),
                detail::std::forward<M>(m)
            );
        }
    };

    struct _then {
        template <typename Before, typename M>
        constexpr decltype(auto) operator()(Before&& before, M&& m) const {
            return then_impl<datatype_t<Before>>::apply(
                detail::std::forward<Before>(before),
                detail::std::forward<M>(m)
            );
        }
    };

    constexpr _then then{};
#endif

    //! Flatten two levels of monadic wrapping into a single level.
    //! @relates Monad
    //!
    //! @param monad
    //! A `Monad` containing a `Monad` that should be flattened to a `Monad`.
    //!
    //! ### Example
    //! @snippet example/monad/flatten.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto flatten = [](auto&& monad) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct flatten_impl : flatten_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct flatten_impl<T, when<condition>> {
        template <typename M>
        static constexpr decltype(auto) apply(M&& m) {
            return Monad::instance<T>::flatten_impl(
                detail::std::forward<M>(m)
            );
        }
    };

    struct _flatten {
        template <typename M>
        constexpr decltype(auto) operator()(M&& m) const {
            return flatten_impl<datatype_t<M>>::apply(
                detail::std::forward<M>(m)
            );
        }
    };

    constexpr _flatten flatten{};
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
    //! ### Example
    //! @snippet example/monad/tap.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto tap = [](auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct tap_impl : tap_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct tap_impl<T, when<condition>> {
        template <typename F>
        static constexpr decltype(auto) apply(F&& f) {
            return Monad::instance<T>::tap_impl(
                detail::std::forward<decltype(f)>(f)
            );
        }
    };

    template <typename M>
    struct _tap {
        template <typename F>
        constexpr decltype(auto) operator()(F&& f) const {
            return tap_impl<M>::apply(
                detail::std::forward<decltype(f)>(f)
            );
        }
    };

    template <typename M>
    constexpr _tap<M> tap{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MONAD_HPP

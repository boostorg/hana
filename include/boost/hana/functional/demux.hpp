/*!
@file
Defines `boost::hana::demux`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_DEMUX_HPP
#define BOOST_HANA_FUNCTIONAL_DEMUX_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/create.hpp>

#include <utility>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Invoke a function with the results of invoking other functions
    //! on its arguments.
    //!
    //! Specifically, `demux(f)(g...)` is a function such that
    //! @code
    //!     demux(f)(g...)(x...) == f(g(x...)...)
    //! @endcode
    //!
    //! Each `g` is called with all the arguments, and then `f` is called
    //! with the result of each `g`. Hence, the arity of `f` must match
    //! the number of `g`s.
    //!
    //! This is called `demux` because of a vague similarity between this
    //! device and a demultiplexer in signal processing. `demux` takes what
    //! can be seen as a continuation (`f`), a bunch of functions to split a
    //! signal (`g...`) and zero or more arguments representing the signal
    //! (`x...`). Then, it calls the continuation with the result of
    //! splitting the signal with whatever functions where given.
    //!
    //! @note
    //! When used with two functions only, `demux` is associative. In other
    //! words (and noting `demux(f, g) = demux(f)(g)` to ease the notation),
    //! it is true that `demux(demux(f, g), h) == demux(f, demux(g, h))`.
    //!
    //!
    //! Signature
    //! ---------
    //! The signature of `demux` is
    //! \f[
    //!     \mathrm{demux} :
    //!         (B_1 \times \dotsb \times B_n \to C)
    //!             \to ((A_1 \times \dotsb \times A_n \to B_1)
    //!                 \times \dotsb
    //!                 \times (A_1 \times \dotsb \times A_n \to B_n))
    //!             \to (A_1 \times \dotsb \times A_n \to C)
    //! \f]
    //!
    //! This can be rewritten more tersely as
    //! \f[
    //!     \mathrm{demux} :
    //!         \left(\prod_{i=1}^n B_i \to C \right)
    //!         \to \prod_{j=1}^n \left(\prod_{i=1}^n A_i \to B_j \right)
    //!         \to \left(\prod_{i=1}^n A_i \to C \right)
    //! \f]
    //!
    //!
    //! Link with normal composition
    //! ----------------------------
    //! The signature of `compose` is
    //! \f[
    //!     \mathrm{compose} : (B \to C) \times (A \to B) \to (A \to C)
    //! \f]
    //!
    //! A valid observation is that this coincides exactly with the type
    //! of `demux` when used with a single unary function. Actually, both
    //! functions are equivalent:
    //! @code
    //!     demux(f)(g)(x) == compose(f, g)(x)
    //! @endcode
    //!
    //! However, let's now consider the curried version of `compose`,
    //! `curry<2>(compose)`:
    //! \f[
    //!     \mathrm{curry_2(compose)} : (B \to C) \to ((A \to B) \to (A \to C))
    //! \f]
    //!
    //! For the rest of this explanation, we'll just consider the curried
    //! version of `compose` and so we'll use `compose` instead of
    //! `curry<2>(compose)` to lighten the notation. With currying, we can
    //! now consider `compose` applied to itself:
    //! \f[
    //!     \mathrm{compose(compose, compose)} :
    //!         (B \to C) \to (A_1 \to A_2 \to B) \to (A_1 \to A_2 \to C)
    //! \f]
    //!
    //! If we uncurry deeply the above expression, we obtain
    //! \f[
    //!     \mathrm{compose(compose, compose)} :
    //!         (B \to C) \times (A_1 \times A_2 \to B) \to (A_1 \times A_2 \to C)
    //! \f]
    //!
    //! This signature is exactly the same as that of `demux` when given a
    //! single binary function, and indeed they are equivalent definitions.
    //! We can also generalize this further by considering
    //! `compose(compose(compose, compose), compose)`:
    //! \f[
    //!     \mathrm{compose(compose(compose, compose), compose)} :
    //!         (B \to C) \to (A_1 \to A_2 \to A_3 \to B)
    //!             \to (A_1 \to A_2 \to A_3 \to C)
    //! \f]
    //!
    //! which uncurries to
    //! \f[
    //!     \mathrm{compose(compose(compose, compose), compose)} :
    //!         (B \to C) \times (A_1 \times A_2 \times A_3 \to B)
    //!             \to (A_1 \times A_2 \times A_3 \to C)
    //! \f]
    //!
    //! This signature is exactly the same as that of `demux` when given a
    //! single ternary function. Hence, for a single n-ary function `g`,
    //! `demux(f)(g)` is equivalent to the n-times composition of `compose`
    //! with itself, applied to `g` and `f`:
    //! @code
    //!     demux(f)(g) == fold.left([compose, ..., compose], id, compose)(g, f)
    //!                           //  ^^^^^^^^^^^^^^^^^^^^^ n times
    //! @endcode
    //!
    //! More information on this insight can be seen [here][1]. Also, I'm
    //! not sure how this insight could be generalized to more than one
    //! function `g`, or if that is even possible.
    //!
    //!
    //! Proof of associativity in the binary case
    //! -----------------------------------------
    //! As explained above, `demux` is associative when it is used with
    //! two functions only. Indeed, given functions `f`, `g` and `h` with
    //! suitable signatures, we have
    //! @code
    //!     demux(f)(demux(g)(h))(x...) == f(demux(g)(h)(x...))
    //!                                 == f(g(h(x...)))
    //! @endcode
    //!
    //! On the other hand, we have
    //! @code
    //!     demux(demux(f)(g))(h)(x...) == demux(f)(g)(h(x...))
    //!                                 == f(g(h(x...)))
    //! @endcode
    //!
    //! and hence `demux` is associative in the binary case.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/functional.cpp demux
    //!
    //! [1]: http://stackoverflow.com/q/5821089/627587
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto demux = [](auto&& f) {
        return [perfect-capture](auto&& ...g) {
            return [perfect-capture](auto&& ...x) -> decltype(auto) {
                // g... and x... can't be forwarded, or we could double-move!
                return forwarded(f)(g(x...)...);
            };
        };
    };
#else
    template <typename F, typename Closure>
    struct _demux;

    template <typename F>
    struct _pre_demux {
        F f;

        template <typename ...G>
        constexpr decltype(auto) operator()(G&& ...g) const& {
            return detail::create<_demux>{}(f,
                detail::create<detail::closure>{}(static_cast<G&&>(g)...)
            );
        }

        template <typename ...G>
        constexpr decltype(auto) operator()(G&& ...g) && {
            return detail::create<_demux>{}(std::move(f),
                detail::create<detail::closure>{}(static_cast<G&&>(g)...)
            );
        }
    };

    template <typename F, typename ...G>
    struct _demux<F, detail::closure_impl<G...>> {
        F f;
        detail::closure_impl<G...> g;

        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) const& {
            return f(static_cast<G const&>(g).get(x...)...);
        }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) & {
            return f(static_cast<G&>(g).get(x...)...);
        }
#endif

        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) && {
            // Not moving from G cause we would double-move.
            return std::move(f)(static_cast<G&>(g).get(x...)...);
        }
    };

    constexpr detail::create<_pre_demux> demux{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_DEMUX_HPP

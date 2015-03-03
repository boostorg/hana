/*!
@file
Defines `boost::hana::demux`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_DEMUX_HPP
#define BOOST_HANA_FUNCTIONAL_DEMUX_HPP

#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>


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
    //! words, `demux(f)(demux(g)(h))` is equivalent to `demux(demux(f)(g))(h)`.
    //!
    //! @internal
    //! ### Proof of associativity in the binary case
    //!
    //! @code
    //!     demux(f)(demux(g)(h))(x...) == f(demux(g)(h)(x...))
    //!                                 == f(g(h(x...)))
    //!
    //!     demux(demux(f)(g))(h)(x...) == demux(f)(g)(h(x...))
    //!                                 == f(g(h(x...)))
    //! @endcode
    //! @endinternal
    //!
    //! ### Example
    //! @snippet example/functional/demux.cpp main
    //!
    //! @todo
    //! I think this is equivalent to `transform . transform`.
    //! See http://stackoverflow.com/q/5821089/627587
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
                detail::create<detail::closure>{}(detail::std::forward<G>(g)...)
            );
        }

        template <typename ...G>
        constexpr decltype(auto) operator()(G&& ...g) && {
            return detail::create<_demux>{}(detail::std::move(f),
                detail::create<detail::closure>{}(detail::std::forward<G>(g)...)
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

        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) & {
            return f(static_cast<G&>(g).get(x...)...);
        }

        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) && {
            // Not moving from G cause we would double-move.
            return detail::std::move(f)(static_cast<G&>(g).get(x...)...);
        }
    };

    constexpr detail::create<_pre_demux> demux{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_DEMUX_HPP

/*!
@file
Defines `boost::hana::demux`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_DEMUX_HPP
#define BOOST_HANA_FUNCTIONAL_DEMUX_HPP

#include <boost/hana/detail/constexpr.hpp>
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
    //! I think this is equivalent to `fmap . fmap`.
    //! See http://stackoverflow.com/q/5821089/627587
    BOOST_HANA_CONSTEXPR_LAMBDA auto demux = [](auto f) {
        return [f(detail::std::move(f))](auto ...g) {
            return [=](auto&& ...x) -> decltype(auto) {
                // Can't forward, because that could cause double-moves.
                return f(g(x...)...);
            };
        };
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_DEMUX_HPP

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_MONAD_MINIMAL_HPP
#define BOOST_HANA_TEST_MONAD_MINIMAL_HPP

#include <boost/hana/applicative.hpp>
#include <boost/hana/functor.hpp>


struct MinimalMonad;

template <typename T>
struct monad {
    T value;
    using hana_datatype = MinimalMonad;

    // Required to check Monad laws.
    friend constexpr bool operator==(monad<T> a, monad<T> b)
    { return a.value == b.value; }
};

struct MonadInstance {
    template <typename M, typename F>
    static constexpr auto bind_impl(M m, F f)
    { return f(m.value); }

    template <typename M>
    static constexpr auto join_impl(M m)
    { return m.value; }
};

namespace boost { namespace hana {
    template <>
    struct Functor<MinimalMonad> : defaults<Functor>::with<MinimalMonad> {
        template <typename F, typename M>
        static constexpr auto fmap_impl(F f, M m)
        { return monad<decltype(f(m.value))>{f(m.value)}; }
    };

    template <>
    struct Applicative<MinimalMonad> : defaults<Applicative>::with<MinimalMonad> {
        template <typename T>
        static constexpr auto unit_impl(T t)
        { return monad<T>{t}; }

        template <typename F, typename X>
        static constexpr auto ap_impl(F f, X x)
        { return fmap(f.value, x); }
    };
}}

#endif // !BOOST_HANA_TEST_MONAD_MINIMAL_HPP

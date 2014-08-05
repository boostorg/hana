/*!
@file
Defines `boost::hana::Maybe`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_HPP
#define BOOST_HANA_MAYBE_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/dependent_on.hpp>
#include <boost/hana/detail/maybe_fwd.hpp>
#include <boost/hana/foldable/mcd.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>
#include <boost/hana/searchable/mcd.hpp>
#include <boost/hana/traversable/traverse_mcd.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for `Maybe`s.
    //!
    //! Two `Maybe`s are equal if and only if they are both empty or they
    //! both contain a value and those values are equal.
    //!
    //! ### Example
    //! @snippet example/maybe/comparable.cpp main
    template <>
    struct Comparable::instance<Maybe, Maybe> : Comparable::equal_mcd {
        template <typename T, typename U>
        static constexpr auto
        equal_impl(maybe_detail::just<T> t, maybe_detail::just<U> u)
        { return equal(t.val, u.val); }

        template <bool tv, typename T, bool uv, typename U>
        static constexpr auto
        equal_impl(maybe_detail::maybe<tv, T>, maybe_detail::maybe<uv, U>)
        { return bool_<tv == uv>; }
    };

    //! Instance of `Functor` for `Maybe`s.
    //!
    //! A `Maybe` can be seen as a `List` containing either one element
    //! (`just(x)`) or no elements at all (`nothing`). As such, `fmap` for
    //! `Maybe`s returns `nothing` when applied to `nothing` and `just(f(x))`
    //! when applied to `just(x)`.
    //!
    //! ### Example
    //! @snippet example/maybe/functor.cpp main
    template <>
    struct Functor::instance<Maybe> : Functor::fmap_mcd {
        template <typename F, typename M>
        static constexpr auto fmap_impl(F f, M m)
        { return maybe(nothing, [=](auto x) { return just(f(x)); }, m); }
    };

    //! Instance of `Applicative` for `Maybe`s.
    //!
    //! First, a value can be made optional with `lift<Maybe>`, which is
    //! equivalent to `just`. Second, one can feed an optional value to an
    //! optional function with `ap`, which will return `just(f(x))` if there
    //! is both a function _and_ a value, and `nothing` otherwise.
    //!
    //! ### Example
    //! @snippet example/maybe/applicative_simple.cpp main
    //!
    //! ### Example
    //! @include example/maybe/applicative.cpp
    template <>
    struct Applicative::instance<Maybe> : Applicative::mcd {
        template <typename T>
        static constexpr auto lift_impl(T x)
        { return just(x); }

        template <typename Mf, typename Mx>
        static constexpr auto ap_impl(Mf mf, Mx mx)
        { return maybe(nothing, [=](auto f) { return fmap(f, mx); }, mf); }
    };

    //! Instance of `Monad` for `Maybe`s.
    //!
    //! The `Maybe` `Monad` makes it easy to compose actions that might fail.
    //! One can feed an optional value if there is one into a function with
    //! `bind`, which will return `nothing` if there is no value. Finally,
    //! optional-optional values can have their redundant level of
    //! `Maybe`ness removed with `flatten`.
    //!
    //! ### Example
    //! @include example/maybe/monad.cpp
    template <>
    struct Monad::instance<Maybe> : Monad::flatten_mcd<Maybe> {
        template <typename MMX>
        static constexpr auto flatten_impl(MMX mmx)
        { return maybe(nothing, [](auto mx) { return mx; }, mmx); }
    };

    //! Instance of `Foldable` for `Maybe`s.
    //!
    //! Folding a `Maybe` is equivalent to folding a `List` containing either
    //! no elements (for `nothing`) or `x` (for `just(x)`). The `Foldable`
    //! instance follows from that.
    //!
    //! ### Example
    //! @snippet example/maybe/foldable.cpp main
    template <typename ...Nothing>
    struct Foldable::instance<Maybe, Nothing...>
        : detail::dependent_on_t<(bool)sizeof...(Nothing), Foldable::mcd>
    {
        template <typename F, typename S, typename M>
        static constexpr auto foldr_impl(F f, S s, M m)
        { return maybe(s, [=](auto x) { return f(x, s); }, m); }

        template <typename F, typename S, typename M>
        static constexpr auto foldl_impl(F f, S s, M m)
        { return maybe(s, [=](auto x) { return f(s, x); }, m); }
    };

    //! Instance of `Traversable` for `Maybe`s.
    //!
    //! Traversing `nothing` yields `nothing` in the new applicative, and
    //! traversing `just(x)` applies the function and maps `just` inside
    //! the resulting applicative.
    //!
    //! ### Example
    //! @snippet example/maybe/traversable.cpp main
    template <>
    struct Traversable::instance<Maybe> : Traversable::traverse_mcd {
        template <typename A, typename F, typename M>
        static constexpr auto traverse_impl(F f, M mx) {
            return maybe(
                lift<A>(nothing),
                [=](auto x) { return fmap(just, f(x)); },
                mx
            );
        }
    };

    //! Instance of `Searchable` for `Maybe`s.
    //!
    //! Searching a `Maybe` is equivalent to searching a list containing
    //! `x` for `just(x)` and an empty list for `nothing`.
    //!
    //! ### Example
    //! @snippet example/maybe/searchable.cpp main
    template <>
    struct Searchable::instance<Maybe> : Searchable::mcd {
        template <typename M, typename Pred>
        static constexpr auto find_impl(M m, Pred p) {
            return maybe(
                nothing,
                [=](auto x) { return if_(p(x), just(x), nothing); },
                m
            );
        }

        template <typename M, typename Pred>
        static constexpr auto any_impl(M m, Pred p)
        { return maybe(false_, p, m); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_HPP

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
#include <boost/hana/detail/maybe_fwd.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/traversable.hpp>


namespace boost { namespace hana {
    //! @details
    //! Two `Maybe`s are equal if and only if they are both empty or they
    //! both contain a value and those values are equal.
    //!
    //! ### Example
    //! @snippet example/maybe/comparable.cpp main
    template <>
    struct Comparable::instance<Maybe, Maybe> : Comparable::equal_mcd {
        template <typename T, typename U>
        static constexpr auto
        equal_impl(operators::_just<T> t, operators::_just<U> u)
        { return equal(t.val, u.val); }

        template <bool tv, typename T, bool uv, typename U>
        static constexpr auto
        equal_impl(operators::_maybe<tv, T>, operators::_maybe<uv, U>)
        { return bool_<tv == uv>; }
    };

    //! @details
    //! A `Maybe` can be seen as a `List` containing either one element
    //! (`just(x)`) or no elements at all (`nothing`). As such, `fmap` for
    //! `Maybe`s returns `nothing` when applied to `nothing` and `just(f(x))`
    //! when applied to `just(x)`.
    //!
    //! ### Example
    //! @snippet example/maybe/functor.cpp main
    template <>
    struct Functor::instance<Maybe> : Functor::fmap_mcd {
        template <typename F, typename T>
        static constexpr auto fmap_impl(F f, operators::_just<T> j)
        { return just(f(j.val)); }

        template <typename F>
        static constexpr auto fmap_impl(F, operators::_nothing)
        { return nothing; }
    };

    //! @details
    //! First, a value can be made optional with `lift<Maybe>`, which is
    //! equivalent to `just`. Second, one can feed an optional value to an
    //! optional function with `ap`, which will return `just(f(x))` if there
    //! is a function _and_ a value and `nothing` otherwise.
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

    //! @details
    //! The `Maybe` `Monad` makes it easy to compose actions that might fail.
    //! One can feed an optional value if there is one into a function with
    //! `bind`, which will return `nothing` if there is no value. Finally,
    //! optional-optional values can have their redundant level of
    //! `Maybe`ness removed with `flatten`.
    //!
    //! ### Example
    //! @snippet example/maybe/monad.cpp main
    template <>
    struct Monad::instance<Maybe> : Monad::flatten_mcd {
        template <typename T>
        static constexpr auto flatten_impl(operators::_just<operators::_just<T>> j)
        { return j.val; }

        static constexpr auto flatten_impl(operators::_nothing)
        { return nothing; }

        static constexpr auto flatten_impl(operators::_just<operators::_nothing>)
        { return nothing; }
    };

    //! @details
    //! Folding a `Maybe` is equivalent to folding a `List` containing either
    //! no elements (for `nothing`) or `x` (for `just(x)`). The `Foldable`
    //! instance follows from that.
    //!
    //! ### Example
    //! @snippet example/maybe/foldable.cpp main
    template <>
    struct Foldable::instance<Maybe> : Foldable::lazy_foldr_mcd {
        template <typename F, typename S, typename M>
        static constexpr auto lazy_foldr_impl(F f, S s, M m) {
            auto go = [=](auto x) {
                return f([=] { return x; }, [=] { return s; });
            };
            return maybe(s, go, m);
        }
    };

    //! @details
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_HPP

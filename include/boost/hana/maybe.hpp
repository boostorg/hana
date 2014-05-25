/*!
 * @file
 * Defines `boost::hana::Maybe`.
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_HPP
#define BOOST_HANA_MAYBE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>


namespace boost { namespace hana {
    template <bool is_valid, typename T>
    struct Maybe { };

    template <typename T>
    struct Maybe<true, T> { T val; };

    template <typename T>
    using Just = Maybe<true, T>;

    using Nothing = Maybe<false, struct _nothing>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto just = [](auto x) {
        return Just<decltype(x)>{x};
    };

    constexpr Nothing nothing{};

    template <typename T, typename U>
    struct Comparable<Just<T>, Just<U>> : defaults<Comparable> {
        static constexpr auto equal_impl(Just<T> t, Just<U> u)
        { return equal(t.val, u.val); }
    };

    template <bool tv, typename T, bool uv, typename U>
    struct Comparable<Maybe<tv, T>, Maybe<uv, U>> : defaults<Comparable> {
        static constexpr auto equal_impl(Maybe<tv, T>, Maybe<uv, U>)
        { return bool_<tv == uv>; }
    };

    template <bool tv, typename T, bool uv, typename U>
    constexpr auto operator==(Maybe<tv, T> t, Maybe<uv, U> u)
    { return equal(t, u); }

    template <bool tv, typename T, bool uv, typename U>
    constexpr auto operator!=(Maybe<tv, T> t, Maybe<uv, U> u)
    { return not_equal(t, u); }

    constexpr struct _maybe {
        template <typename Default, typename F>
        constexpr auto operator()(Default default_, F, Nothing) const
        { return default_; }

        template <typename Default, typename F, typename T>
        constexpr auto operator()(Default, F f, Just<T> j) const
        { return f(j.val); }
    } maybe{};

    template <>
    struct Functor<Nothing> {
        template <typename F>
        static constexpr auto fmap_impl(F, Nothing)
        { return nothing; }
    };

    template <typename T>
    struct Functor<Just<T>> {
        template <typename F>
        static constexpr auto fmap_impl(F f, Just<T> j)
        { return just(f(j.val)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_HPP

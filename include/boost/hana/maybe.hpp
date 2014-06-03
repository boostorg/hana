/*!
@file
Defines `boost::hana::Maybe`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_HPP
#define BOOST_HANA_MAYBE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
    /*!
    @datatype{Maybe}
    @{
    Represents an optional value.

    A `Maybe` either contains a value (represented as `just(x)`), or it is
    empty (represented as `nothing`).

    @instantiates{Comparable, Functor, Monad}

    @todo
    - `Comparable` should only be instantiated when the content of the `Maybe`
    is `Comparable`.
     */
    struct _Maybe { };

    //! @}

    namespace operators {
        template <bool is_valid, typename T>
        struct Maybe {
            using hana_datatype = _Maybe;
        };

        template <typename T>
        struct Maybe<true, T> {
            using hana_datatype = _Maybe;
            T val;
        };
    }
    using operators::Maybe;

    template <typename T>
    using Just = Maybe<true, T>;

    using Nothing = Maybe<false, struct _nothing>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto just = [](auto x) {
        return Just<decltype(x)>{x};
    };

    constexpr Nothing nothing{};

    template <>
    struct Comparable<_Maybe, _Maybe> : defaults<Comparable> {
        template <typename T, typename U>
        static constexpr auto equal_impl(Just<T> t, Just<U> u)
        { return equal(t.val, u.val); }

        template <bool tv, typename T, bool uv, typename U>
        static constexpr auto equal_impl(Maybe<tv, T>, Maybe<uv, U>)
        { return bool_<tv == uv>; }
    };

    constexpr struct _maybe {
        template <typename Default, typename F>
        constexpr auto operator()(Default default_, F, Nothing) const
        { return default_; }

        template <typename Default, typename F, typename T>
        constexpr auto operator()(Default, F f, Just<T> j) const
        { return f(j.val); }
    } maybe{};

    template <>
    struct Functor<_Maybe> : defaults<Functor> {
        template <typename F, typename T>
        static constexpr auto fmap_impl(F f, Just<T> j)
        { return just(f(j.val)); }

        template <typename F>
        static constexpr auto fmap_impl(F, Nothing)
        { return nothing; }
    };

    template <>
    struct Monad<_Maybe> : defaults<Monad> {
        template <typename T>
        static constexpr auto unit_impl(T x)
        { return just(x); }

        template <typename T>
        static constexpr auto join_impl(Just<Just<T>> j)
        { return j.val; }

        template <typename AnythingElse>
        static constexpr auto join_impl(AnythingElse)
        { return nothing; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_HPP

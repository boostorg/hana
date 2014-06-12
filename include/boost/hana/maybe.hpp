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
    Represents an optional value.

    A `Maybe` either contains a value (represented as `just(x)`), or it is
    empty (represented as `nothing`).

    @instantiates{Comparable, Functor, Monad}

    @todo
    - Document how the type classes are instantiated?
    - Show examples? Might be overkill for API functions, but not for
      type class instances.
     */
    struct Maybe { };

    namespace operators {
        template <bool is_valid, typename T>
        struct _maybe {
            using hana_datatype = Maybe;
        };

        template <typename T>
        struct _maybe<true, T> {
            using hana_datatype = Maybe;
            T val;
        };

        template <typename T>
        using _just = _maybe<true, T>;
        using _nothing = _maybe<false, void>;
    }

    namespace maybe_detail {
        struct _maybe_func {
            template <typename Default, typename F>
            constexpr auto operator()(Default default_, F, operators::_nothing) const
            { return default_; }

            template <typename Default, typename F, typename T>
            constexpr auto operator()(Default, F f, operators::_just<T> j) const
            { return f(j.val); }
        };
    }

    //! Applies a function to the contents of a `Maybe`, with a fallback
    //! result.
    //! @relates Maybe
    //!
    //! Specifically, `maybe` takes a default value, a function and an
    //! optional value. If the optional value is `nothing`, the default
    //! value is returned. Otherwise, the function is applied to the
    //! content of the `just`.
    constexpr maybe_detail::_maybe_func maybe{};

    //! Creates an optional value containing `x`.
    //! @relates Maybe
    //! @hideinitializer
    BOOST_HANA_CONSTEXPR_LAMBDA auto just = [](auto x) {
        return operators::_just<decltype(x)>{x};
    };

    //! Returns whether a `Maybe` contains a value.
    //! @relates Maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_just = [](auto m) {
        return maybe(false_, [](auto) { return true_; }, m);
    };

    //! Creates an empty optional value.
    //! @relates Maybe
    //! @hideinitializer
    constexpr operators::_nothing nothing{};

    //! Returns whether a `Maybe` is empty.
    //! @relates Maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_nothing = [](auto m) {
        return !is_just(m);
    };

    //! Returns the contents of a `Maybe`, or a default value if the `Maybe`
    //! is `nothing`.
    //! @relates Maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto from_maybe = [](auto default_, auto m) {
        return maybe(default_, [](auto x) { return x; }, m);
    };

    //! Extract the value out of a `just`, or trigger a compile-time error
    //! if the argument is `nothing`.
    //! @relates Maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto from_just = [](auto m) {
        auto err = [](auto ...dum) {
            constexpr bool always_false = sizeof...(dum) != 0;
            static_assert(always_false,
            "trying to extract the value inside a boost::hana::nothing "
            "with boost::hana::from_just");
        };
        return maybe(err, [](auto x) { return [=] { return x; }; }, m)();
    };

    template <>
    struct Comparable<Maybe, Maybe>
        : defaults<Comparable>::template with<Maybe, Maybe>
    {
        template <typename T, typename U>
        static constexpr auto
        equal_impl(operators::_just<T> t, operators::_just<U> u)
        { return equal(t.val, u.val); }

        template <bool tv, typename T, bool uv, typename U>
        static constexpr auto
        equal_impl(operators::_maybe<tv, T>, operators::_maybe<uv, U>)
        { return bool_<tv == uv>; }
    };

    template <>
    struct Functor<Maybe> : defaults<Functor>::with<Maybe> {
        template <typename F, typename T>
        static constexpr auto fmap_impl(F f, operators::_just<T> j)
        { return just(f(j.val)); }

        template <typename F>
        static constexpr auto fmap_impl(F, operators::_nothing)
        { return nothing; }
    };

    template <>
    struct Monad<Maybe> : defaults<Monad> {
        template <typename T>
        static constexpr auto unit_impl(T x)
        { return just(x); }

        template <typename T>
        static constexpr auto join_impl(operators::_just<operators::_just<T>> j)
        { return j.val; }

        template <typename AnythingElse>
        static constexpr auto join_impl(AnythingElse)
        { return nothing; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_HPP

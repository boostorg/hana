/*!
@file
Forward declares `boost::hana::Maybe`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MAYBE_HPP
#define BOOST_HANA_FWD_MAYBE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/fwd/logical.hpp>
#include <boost/hana/fwd/monad.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents an optional value.
    //!
    //! A `Maybe` either contains a value (represented as `just(x)`), or it
    //! is empty (represented as `nothing`).
    //!
    //! ## Instance of
    //! `Comparable`, `Functor`, `Applicative`, `Monad`, `Foldable`,
    //! `Searchable` and `Traversable`.
    struct Maybe {
        struct hana {
            struct enabled_operators
                : Comparable
                , Monad
            { };
        };
    };

    //! Create an optional value containing `x`.
    //! @relates Maybe
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp just
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto just = [](auto&& x) {
        return unspecified-type;
    };
#else
    template <typename T, typename = operators::enable_adl>
    struct _just {
        T val;
        struct hana { using datatype = Maybe; };
    };

    constexpr detail::create<_just> just{};
#endif

    //! An empty optional value.
    //! @relates Maybe
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp nothing
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr unspecified-type nothing{};
#else
    struct _nothing : operators::enable_adl {
        struct hana { using datatype = Maybe; };
    };

    constexpr _nothing nothing{};
#endif

    //! Create a `Maybe` with the result of a function, but only if a
    //! predicate is satisfied.
    //! @relates Maybe
    //!
    //! Specifically, returns `just(f(x))` if `predicate(x)` is a true-valued
    //! `Logical`, and `nothing` otherwise.
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x)` and returning a true-valued
    //! `Logical` if `just(f(x))` should be the resulting value, and a
    //! false-valued one if `nothing` should be the resulting value.
    //! In the current version of the library, the result of `predicate`
    //! has to be a [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //! @param f
    //! A function called as `f(x)` if the `predicate` returns a true-valued
    //! `Logical`, and not called at all otherwise. If the `predicate` returns
    //! a false-valued `Logical`, the `f(x)` expression is not even required
    //! to be well-formed.
    //!
    //! @param x
    //! The value to either transform and put in a `just`, or discard.
    //!
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp only_when
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto only_when = [](auto&& predicate, auto&& f, auto&& x) -> decltype(auto) {
        if (forwarded(predicate)(x))
            return just(forwarded(f)(forwarded(x)));
        else
            return nothing;
    };
#else
    struct _only_when {
        template <typename F, typename X>
        struct just_f_x {
            F f; X x;
            template <typename Id>
            constexpr decltype(auto) operator()(Id _) && {
                return just(_(detail::std::forward<F>(f))(
                    detail::std::forward<X>(x)
                ));
            }

            template <typename Id>
            constexpr decltype(auto) operator()(Id _) &
            { return just(_(f)(x)); }

            template <typename Id>
            constexpr decltype(auto) operator()(Id _) const&
            { return just(_(f)(x)); }
        };
        template <typename Pred, typename F, typename X>
        constexpr decltype(auto) operator()(Pred&& pred, F&& f, X&& x) const {
            return eval_if(detail::std::forward<Pred>(pred)(x),
                just_f_x<F, X>{detail::std::forward<F>(f), detail::std::forward<X>(x)},
                always(nothing)
            );
        }
    };

    constexpr _only_when only_when{};
#endif

    //! Apply a function to the contents of a `Maybe`, with a fallback
    //! result.
    //! @relates Maybe
    //!
    //! Specifically, `maybe` takes a default value, a function and an
    //! optional value. If the optional value is `nothing`, the default
    //! value is returned. Otherwise, the function is applied to the
    //! content of the `just`.
    //!
    //!
    //! @param default_
    //! A default value returned if `m` is `nothing`.
    //!
    //! @param f
    //! A function called as `f(x)` if and only if `m` is an optional value
    //! of the form `just(x)`. In that case, the result returend by `maybe`
    //! is the result of `f`.
    //!
    //! @param m
    //! An optional value.
    //!
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp maybe
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto maybe = [](auto&& default_, auto&& f, auto&& m) -> decltype(auto) {
        if (m is a just(x)) {
            return forwarded(f)(forwarded(x));
        else
            return forwarded(default_);
        }
    };
#else
    struct _maybe {
        template <typename Def, typename F, typename T>
        constexpr decltype(auto) operator()(Def&&, F&& f, _just<T> const& m) const
        { return detail::std::forward<F>(f)(m.val); }

        template <typename Def, typename F, typename T>
        constexpr decltype(auto) operator()(Def&&, F&& f, _just<T>& m) const
        { return detail::std::forward<F>(f)(m.val); }

        template <typename Def, typename F, typename T>
        constexpr decltype(auto) operator()(Def&&, F&& f, _just<T>&& m) const
        { return detail::std::forward<F>(f)(detail::std::move(m).val); }

        template <typename Def, typename F>
        constexpr Def operator()(Def&& def, F&&, _nothing) const
        { return detail::std::forward<Def>(def); }
    };

    constexpr _maybe maybe{};
#endif

    //! Return whether a `Maybe` contains a value.
    //! @relates Maybe
    //!
    //! Specifically, returns a [compile-time](@ref Logical_terminology)
    //! true-valued `Logical` if `m` is of the form `just(x)` for some `x`,
    //! and a false-valued one otherwise.
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp is_just
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto is_just = [](auto const& m) {
        return m is a just(x);
    };
#else
    constexpr auto is_just = partial(maybe, false_, always(true_));
#endif

    //! Return whether a `Maybe` is empty.
    //! @relates Maybe
    //!
    //! Specifically, returns a [compile-time](@ref Logical_terminology)
    //! true-valued `Logical` if `m` is of the form `nothing`, and a
    //! false-valued one otherwise.
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp is_nothing
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto is_nothing = [](auto const& m) {
        return m is a nothing;
    };
#else
    constexpr auto is_nothing = partial(maybe, true_, always(false_));
#endif

    //! Return the contents of a `Maybe`, with a fallback result.
    //! @relates Maybe
    //!
    //! Specifically, returns `x` if `m` of the form `just(x)`, and `default_`
    //! if `m` is of the form `nothing`.
    //!
    //!
    //! @param default_
    //! The default value to return if `m` is `nothing`.
    //!
    //! @param m
    //! The optional value to try to retrieve the value from.
    //!
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp from_maybe
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto from_maybe = [](auto&& default_, auto&& m) -> decltype(auto) {
        return maybe(forwarded(default_), id, forwarded(m));
    };
#else
    struct _from_maybe {
        template <typename Default, typename M>
        constexpr decltype(auto) operator()(Default&& default_, M&& m) const {
            return maybe(
                detail::std::forward<Default>(default_),
                id,
                detail::std::forward<M>(m)
            );
        }
    };

    constexpr _from_maybe from_maybe{};
#endif

    //! Extract the content of a `Maybe` or fail at compile-time.
    //! @relates Maybe
    //!
    //! Specifically, returns `x` if the optional value is of the form
    //! `just(x)`, and triggers a static assertion otherwise.
    //!
    //! ### Example
    //! @snippet example/maybe/maybe.cpp from_just
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto from_just = [](auto&& m) -> decltype(auto) {
        static_assert(m is a just(x),
        "can't use boost::hana::from_just on a boost::hana::nothing");
        return forwarded(x);
    };
#else
    struct _from_just {
        struct error {
            template <typename ...Dummy>
            constexpr void operator()(Dummy ...) const {
                constexpr bool always_false = sizeof...(Dummy) != 0;
                static_assert(always_false,
                "trying to extract the value inside a boost::hana::nothing "
                "with boost::hana::from_just");
            }
        };

        template <typename M>
        constexpr decltype(auto) operator()(M&& m) const {
            return maybe(
                error{}, compose(id, always), detail::std::forward<M>(m)
            )();
        }
    };

    constexpr _from_just from_just{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MAYBE_HPP

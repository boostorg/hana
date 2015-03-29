/*!
@file
Forward declares `boost::hana::Maybe`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MAYBE_HPP
#define BOOST_HANA_FWD_MAYBE_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents an optional value.
    //!
    //! A `Maybe` either contains a value (represented as `just(x)`), or it
    //! is empty (represented as `nothing`). In essence, `Maybe` is pretty
    //! much like a `boost::optional` or the upcoming `std::optional`. This
    //! can be particularly useful for returning from a function that might
    //! fail, when the reason of failure is unimportant. However, there is
    //! an important distinction to make between `Maybe` and `std::optional`:
    //! `just(x)` and `nothing` do not share the same type. Hence, whether a
    //! `just` or a `nothing` will be returned from a function has to be
    //! known at compile-time for the return type to be computable by the
    //! compiler. This makes `Maybe` well suited for static metaprogramming
    //! tasks but very poor for anything dynamic.
    //!
    //!
    //! Interoperation with `Type`s
    //! ---------------------------
    //! When a `just` contains an object of type `T` which is a `Type`,
    //! it has a nested `::%type` alias equivalent to `T::%type`. `nothing`,
    //! however, never has a nested `::%type` alias. If `t` is a `Type`,
    //! this allows `decltype(just(t))` to be seen as a nullary metafunction
    //! equivalent to `decltype(t)`. Along with the `sfinae` function,
    //! this allows `Maybe` to interact seamlessly with SFINAE-friendly
    //! metafunctions.
    //! Example:
    //! @snippet example/maybe.cpp sfinae_friendly_metafunctions
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! Two `Maybe`s are equal if and only if they are both empty or they
    //! both contain a value and those values are equal.
    //! @snippet example/maybe.cpp comparable
    //!
    //! 2. `Orderable` (operators provided)\n
    //! `Maybe`s can be ordered by considering the value they are holding,
    //! if any. To handle the case of an empty `Maybe`, we arbitrarily set
    //! `nothing` as being less than any other `just`. Hence,
    //! @code
    //!     just(x) < just(y) if and only if x < y
    //!     nothing < just(anything)
    //! @endcode
    //! Example:
    //! @snippet example/maybe.cpp orderable
    //!
    //! 3. `Functor`\n
    //! A `Maybe` can be seen as a `List` containing either one element
    //! (`just(x)`) or no elements at all (`nothing`). As such, mapping
    //! a function over a `Maybe` is equivalent to applying it to its value
    //! if there is one and to `nothing` otherwise:
    //! @code
    //!     transform(just(x), f) == just(f(x))
    //!     transform(nothing, f) == nothing
    //! @endcode
    //! Example:
    //! @snippet example/maybe.cpp functor
    //!
    //! 4. `Applicative`\n
    //! First, a value can be made optional with `lift<Maybe>`, which is
    //! equivalent to `just`. Second, one can feed an optional value to an
    //! optional function with `ap`, which will return `just(f(x))` if there
    //! is both a function _and_ a value, and `nothing` otherwise:
    //! @code
    //!     ap(just(f), just(x)) == just(f(x))
    //!     ap(nothing, just(x)) == nothing
    //!     ap(just(f), nothing) == nothing
    //!     ap(nothing, nothing) == nothing
    //! @endcode
    //! A simple example:
    //! @snippet example/maybe.cpp applicative
    //! A more complex example:
    //! @snippet example/maybe.complex.cpp applicative
    //!
    //! 5. `Monad` (operators provided)\n
    //! The `Maybe` `Monad` makes it easy to compose actions that might fail.
    //! One can feed an optional value if there is one into a function with
    //! `bind`, which will return `nothing` if there is no value. Finally,
    //! optional-optional values can have their redundant level of
    //! `Maybe`ness removed with `flatten`.
    //! Example:
    //! @snippet example/maybe.cpp monad
    //!
    //! 6. `MonadPlus`\n
    //! The `Maybe` MonadPlus makes it easy to choose the first valid value
    //! of two optional values. If both optional values are `nothing`s, then
    //! `concat` will return `nothing`.
    //! @snippet example/maybe.cpp monad_plus
    //!
    //! 7. `Traversable`\n
    //! Traversing `nothing` yields `nothing` in the new applicative, and
    //! traversing `just(x)` applies the function and maps `just` inside
    //! the resulting applicative.
    //! Example:
    //! @snippet example/maybe.cpp traversable
    //!
    //! 8. `Foldable`\n
    //! Folding a `Maybe` is equivalent to folding a `List` containing either
    //! no elements (for `nothing`) or `x` (for `just(x)`).
    //! Example:
    //! @snippet example/maybe.cpp foldable
    //!
    //! 9. `Searchable`\n
    //! Searching a `Maybe` is equivalent to searching a list containing
    //! `x` for `just(x)` and an empty list for `nothing`.
    //! Example:
    //! @snippet example/maybe.cpp searchable
    struct Maybe { };

    //! Create an optional value containing `x`.
    //! @relates Maybe
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/maybe.cpp just
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto just = [](auto&& x) {
        return unspecified-type;
    };
#else
    template <typename T>
    struct _just;

    struct _make_just {
        template <typename T>
        constexpr auto operator()(T&&) const;
    };

    constexpr _make_just just{};
#endif

    //! An empty optional value.
    //! @relates Maybe
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/maybe.cpp nothing
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr unspecified-type nothing{};
#else
    struct _nothing : operators::adl {
        static constexpr bool is_just = false;
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
    //! Since the type of `just` and `nothing` differ, the result of
    //! `predicate` has to be a compile-time `Logical`.
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
    //! Example
    //! -------
    //! @snippet example/maybe.cpp only_when
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto only_when = [](auto&& predicate, auto&& f, auto&& x) -> decltype(auto) {
        if (forwarded(predicate)(x))
            return just(forwarded(f)(forwarded(x)));
        else
            return nothing;
    };
#else
    struct _only_when {
        template <typename Pred, typename F, typename X>
        constexpr decltype(auto) operator()(Pred&& pred, F&& f, X&& x) const;
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
    //! Example
    //! -------
    //! @snippet example/maybe.cpp maybe
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
    //! Specifically, returns a compile-time true-valued `Logical` if `m` is
    //! of the form `just(x)` for some `x`, and a false-valued one otherwise.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/maybe.cpp is_just
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto is_just = [](auto const& m) {
        return m is a just(x);
    };
#else
    struct _is_just {
        template <typename M>
        constexpr decltype(auto) operator()(M const&) const;
    };

    constexpr _is_just is_just{};
#endif

    //! Return whether a `Maybe` is empty.
    //! @relates Maybe
    //!
    //! Specifically, returns a compile-time true-valued `Logical` if `m` is
    //! a `nothing`, and a false-valued one otherwise.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/maybe.cpp is_nothing
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto is_nothing = [](auto const& m) {
        return m is a nothing;
    };
#else
    struct _is_nothing {
        template <typename M>
        constexpr decltype(auto) operator()(M const&) const;
    };

    constexpr _is_nothing is_nothing{};
#endif

    //! Return the contents of a `Maybe`, with a fallback result.
    //! @relates Maybe
    //!
    //! Specifically, returns `x` if `m` is `just(x)`, and `default_`
    //! otherwise.
    //!
    //!
    //! @param default_
    //! The default value to return if `m` is `nothing`.
    //!
    //! @param m
    //! The optional value to try to retrieve the value from.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/maybe.cpp from_maybe
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto from_maybe = [](auto&& default_, auto&& m) -> decltype(auto) {
        return maybe(forwarded(default_), id, forwarded(m));
    };
#else
    struct _from_maybe {
        template <typename Default, typename M>
        constexpr decltype(auto) operator()(Default&& default_, M&& m) const;
    };

    constexpr _from_maybe from_maybe{};
#endif

    //! Extract the content of a `Maybe` or fail at compile-time.
    //! @relates Maybe
    //!
    //! Specifically, returns `x` if the optional value is `just(x)`, and
    //! triggers a static assertion otherwise.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/maybe.cpp from_just
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto from_just = [](auto&& m) -> decltype(auto) {
        static_assert(m is a just(x),
        "can't use boost::hana::from_just on a boost::hana::nothing");
        return forwarded(x);
    };
#else
    struct _from_just {
        template <typename M>
        constexpr decltype(auto) operator()(M&& m) const;
    };

    constexpr _from_just from_just{};
#endif

    //! Calls a function if the call expression is well-formed.
    //! @relates Maybe
    //!
    //! Given a function `f`, `sfinae` returns a new function applying `f`
    //! to its arguments and returning `just` the result if the call is
    //! well-formed, and `nothing` otherwise. In other words, `sfinae(f)(x...)`
    //! is `just(f(x...))` if that expression is well-formed, and `nothing`
    //! otherwise. Note, however, that it is possible for an expression
    //! `f(x...)` to be well-formed as far as SFINAE is concerned, but
    //! trying to actually compile `f(x...)` still fails. In this case,
    //! `sfinae` won't be able to detect it and a hard failure is likely
    //! to happen.
    //!
    //!
    //! @note
    //! The function given to `sfinae` must not return `void`, since
    //! `just(void)` does not make sense. A compilation error is
    //! triggered if the function returns void.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/maybe.cpp sfinae
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    auto sfinae = [](auto&& f) {
        return [perfect-capture](auto&& ...x) {
            if (decltype(forwarded(f)(forwarded(x)...)) is well-formed)
                return just(forwarded(f)(forwarded(x)...));
            else
                return nothing;
        };
    };
#else
    struct _sfinae {
        template <typename F>
        constexpr decltype(auto) operator()(F&& f) const;
    };

    constexpr _sfinae sfinae{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MAYBE_HPP

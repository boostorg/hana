/*!
@file
Forward declares `boost::hana::Iterable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ITERABLE_HPP
#define BOOST_HANA_FWD_ITERABLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/fwd/constant.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>

#include <cstddef>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Iterable` concept represents data structures supporting external
    //! iteration.
    //!
    //! Intuitively, an `Iterable` can be seen as a kind of container whose
    //! elements can be pulled out one at a time. An `Iterable` also provides
    //! a way to know when the _container_ is empty, i.e. when there are no
    //! more elements to pull out.
    //!
    //! Whereas `Foldable` represents data structures supporting internal
    //! iteration with the ability to accumulate a result, the `Iterable`
    //! concept allows inverting the control of the iteration. This is more
    //! flexible than `Foldable`, since it allows iterating over only some
    //! part of the structure. This, in turn, allows `Iterable` to work on
    //! infinite structures, while trying to fold such a structure would
    //! never finish.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `front`, `tail` and `is_empty`
    //!
    //!
    //! @anchor Iterable-lin
    //! The linearization of an `Iterable`
    //! ----------------------------------
    //! Intuitively, for an `Iterable` structure `xs`, the _linearization_ of
    //! `xs` is the sequence of all the elements in `xs` as if they had been
    //! put in a (possibly infinite) list:
    //! @code
    //!     linearization(xs) = [x1, x2, x3, ...]
    //! @endcode
    //!
    //! This notion is precisely the extension of the [linearization]
    //! (@ref Foldable-lin) notion of `Foldable`s to the infinite case.
    //! This notion is useful for expressing various properties of
    //! `Iterable`s, and is used for that throughout the documentation.
    //!
    //!
    //! Compile-time `Iterable`s
    //! ------------------------
    //! A _compile-time_ `Iterable` is an `Iterable` for which `is_empty`
    //! returns a compile-time `Logical`. These structures allow iteration
    //! to be done at compile-time, in the sense that the "loop" doing the
    //! iteration can be unrolled because the total length of the structure
    //! is kown at compile-time.
    //!
    //! In particular, note that being a compile-time `Iterable` has nothing
    //! to do with being finite or infinite. For example, it would be possible
    //! to create a sequence representing the Pythagorean triples as
    //! `IntegralConstant`s. Such a sequence would be infinite, but iteration
    //! on the sequence would still be done at compile-time. However, if one
    //! tried to iterate over _all_ the elements of the sequence, the compiler
    //! would loop indefinitely, in contrast to your program looping
    //! indefinitely if the sequence was a runtime one.
    //!
    //! __In the current version of the library, only compile-time `Iterable`s
    //! are supported.__ While it would be possible in theory to support
    //! runtime `Iterable`s, doing it efficiently is the subject of some
    //! research. In particular, follow [this issue][1] for the current
    //! status of runtime `Iterable`s.
    //!
    //!
    //! Laws
    //! ----
    //! First, we require the equality of two `Iterable`s to be related to the
    //! equality of the elements in their linearizations. More specifically,
    //! if `xs` and `ys` are two non-empty `Iterable`s of data type `It`, then
    //! @code
    //!     xs == ys  =>  front(xs) == front(ys) && tail(xs) == tail(ys)
    //! @endcode
    //! which conveys that two `Iterable`s must have the same linearization
    //! in order to have the chance of being considered equal. We then handle
    //! the empty case by saying that if any of `xs` and `ys` is empty, then
    //! @code
    //!     xs == ys  =>  is_empty(xs) && is_empty(ys)
    //! @endcode
    //!
    //! Second, since every `Iterable` is also a `Searchable`, we require the
    //! models of `Iterable` and `Searchable` to be consistent. This is made
    //! precise by the following laws. For any `Iterable` `xs` with a
    //! linearization of `[x1, x2, x3, ...]`,
    //! @code
    //!     any_of(xs, equal.to(z))  <=>  xi == z
    //! @endcode
    //! for some finite index `i`. Furthermore,
    //! @code
    //!     find_if(xs, pred) == just(the first xi such that pred(xi) is satisfied)
    //! @endcode
    //! or `nothing` if no such `xi` exists.
    //!
    //!
    //! Refined concepts
    //! ----------------
    //! 1. `Searchable` (free model)\n
    //! Any `Iterable` gives rise to a model of `Searchable`, where the keys
    //! and the values are both the elements in the structure. Searching for
    //! a key is just doing a linear search through the elements of the
    //! structure.
    //! @snippet example/iterable.cpp Searchable
    //!
    //! 2. `Foldable` for finite `Iterable`s (free model)\n
    //! Every finite `Iterable` gives rise to a model of  `Foldable`. Hence,
    //! finite `Iterable`s must satisfy additional laws to make sure that
    //! external iteration in `Iterable` and internal iteration in `Foldable`
    //! are consistent. These laws are expressed in terms of the `Foldable`'s
    //! [linearization](@ref Foldable-lin). For any finite `Iterable` `xs`
    //! with linearization `[x1, ..., xn]`, the following must be satisfied:
    //! @code
    //!     at(xs, i) == xi
    //!     is_empty(xs) <=> n == 0
    //! @endcode
    //! An equivalent way of writing this is
    //! @code
    //!     front(xs) == front(linearization(xs))
    //!               == x1
    //!
    //!     linearization(tail(xs)) == tail(linearization(xs))
    //!                             == [x2, ..., xn]
    //!
    //!     is_empty(xs)  <=>  is_empty(linearization(xs))
    //!                   <=>  n == 0
    //! @endcode
    //! This says that linearizing an `Iterable` and then iterating through
    //! it should be equivalent to just iterating through it.
    //! @note
    //! As explained above, `Iterable`s are also `Searchable`s and their
    //! models have to be consistent. By the laws presented here, it also
    //! means that the `Foldable` model for finite `Iterable`s has to be
    //! consistent with the `Searchable` model.
    //!
    //! For convenience, a default minimal complete definition for `Foldable`
    //! is provided for finite `Iterable`s via the `Iterable::fold_left_impl`
    //! class. The model of `Foldable` thus created is simple:\n
    //! Let `xs` be an `Iterable` and let `xi` denote the `i`-th element in
    //! its linearization. In other words, `xs` can be linearized as
    //! `[x1, ..., xN]`, where `N` is the (finite) number of elements in
    //! `xs`. Then, right-folding `xs` with a binary operation `*`
    //! (in infix notation for legibility) is equivalent to
    //! @code
    //!     x1 * (x2 * ( ... * (xN-1 * xN)))
    //! @endcode
    //! Similarly, left-folding `xs` is equivalent to
    //! @code
    //!     (((x1 * x2) * x3) * ...) * xN
    //! @endcode
    //! In both cases, notice the side of the parentheses. Left-folding
    //! applies `*` in a left-associative manner, whereas right-folding
    //! applies it in a right-associative manner. For associative operations,
    //! i.e. operations such that for all `a`, `b` and `c`,
    //! @code
    //!     (a * b) * c = a * (b * c)
    //! @endcode
    //! this makes no difference.
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `Tuple`, `String`, `Range`
    //!
    //!
    //! [1]: https://github.com/ldionne/hana/issues/40
    struct Iterable {
        template <typename It> struct fold_left_impl;

        template <typename It> struct find_if_impl;
        template <typename It> struct any_of_impl;
    };

    //! Returns the first element of a non-empty iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty Iterable `xs` with a linearization of `[x1, ..., xN]`,
    //! `front(xs)` is equal to `x1`. If `xs` is empty, it is an error to
    //! use this function.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp front
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto front = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct front_impl;

    struct _front {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using It = typename datatype<Xs>::type;
            using Front = BOOST_HANA_DISPATCH_IF(
                front_impl<It>,
                _models<Iterable, It>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Iterable, It>{},
            "hana::front(xs) requires 'xs' to be an Iterable");
        #endif

            return Front::apply(static_cast<Xs&&>(xs));
        }
    };

    constexpr _front front{};
#endif

    //! Returns a new iterable containing all but the first element of a
    //! non-empty iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty Iterable `xs` with a linearization of `[x1, ..., xN]`,
    //! `tail(xs)` is an Iterable of the same data type whose linearization is
    //! `[x2, ..., xN]`. In particular, `tail(xs)` is functionally equivalent
    //! to `drop(xs, size_t<1>)`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp tail
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto tail = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct tail_impl;

    struct _tail {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using It = typename datatype<Xs>::type;
            using Tail = BOOST_HANA_DISPATCH_IF(
                tail_impl<It>,
                _models<Iterable, It>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Iterable, It>{},
            "hana::tail(xs) requires xs to be an Iterable");
        #endif

            return Tail::apply(static_cast<Xs&&>(xs));
        }
    };

    constexpr _tail tail{};
#endif

    //! Returns whether the iterable is empty.
    //! @relates Iterable
    //!
    //! `is_empty` must return a compile-time `Logical` representing
    //! whether the iterable is empty.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp is_empty
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto is_empty = [](auto&& iterable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct is_empty_impl;

    struct _is_empty {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using It = typename datatype<Xs>::type;
            using IsEmpty = BOOST_HANA_DISPATCH_IF(
                is_empty_impl<It>,
                _models<Iterable, It>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Iterable, It>{},
            "hana::is_empty(xs) requires xs to be an Iterable");
        #endif

            return IsEmpty::apply(static_cast<Xs&&>(xs));
        }
    };

    constexpr _is_empty is_empty{};
#endif

    //! Returns the `n`th element of an iterable.
    //! @relates Iterable
    //!
    //! Given an `Iterable` and a `Constant` index, `at` returns the element
    //! located at the index in the linearization of the iterable.
    //! Specifically, given an iterable `xs` with a linearization of
    //! `[x1, ..., xN]`, `at(xs, k)` is equivalent to `xk`.
    //!
    //!
    //! @param xs
    //! The iterable in which an element is retrieved. The iterable must
    //! contain at least `n + 1` elements.
    //!
    //! @param n
    //! A (non-negative) `Constant` of an unsigned integral type representing
    //! the 0-based index of the element to return. It is an error to call
    //! `at` with an index that is either out of bounds for the iterable,
    //! not of an unsigned type or not a `Constant`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp at
    //!
    //!
    //! Benchmarks
    //! ----------
    //! <div class="benchmark-chart"
    //!      style="min-width: 310px; height: 400px; margin: 0 auto"
    //!      data-dataset="benchmark.at.compile.json">
    //! </div>
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto at = [](auto&& xs, auto&& n) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct at_impl;

    struct _at {
        template <typename Xs, typename N>
        constexpr decltype(auto) operator()(Xs&& xs, N&& n) const {
            using It = typename datatype<Xs>::type;
            using At = BOOST_HANA_DISPATCH_IF(
                at_impl<It>,
                _models<Iterable, It>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Iterable, It>{},
            "hana::at(xs, n) requires xs to be an Iterable");
        #endif

            return At::apply(static_cast<Xs&&>(xs), static_cast<N&&>(n));
        }
    };

    constexpr _at at{};
#endif

    //! Equivalent to `at`; provided for convenience.
    //! @relates Iterable
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp at_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto at_c = [](auto&& xs) -> decltype(auto) {
        return at(forwarded(xs), size_t<n>);
    };
#else
    template <std::size_t n>
    struct _at_c;

    template <std::size_t n>
    constexpr _at_c<n> at_c{};
#endif

    //! Returns the last element of a non-empty and finite iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty and finite iterable `xs` with a linearization of
    //! `[x1, ..., xN]`, `back(xs)` is equal to `xN`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp back
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto back = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct back_impl;

    struct _back {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using It = typename datatype<Xs>::type;
            using Back = BOOST_HANA_DISPATCH_IF(
                back_impl<It>,
                _models<Iterable, It>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Iterable, It>{},
            "hana::back(xs) requires 'xs' to be an Iterable");
        #endif

            return Back::apply(static_cast<Xs&&>(xs));
        }
    };

    constexpr _back back{};
#endif

    //! Drop the first `n` elements of an iterable, and return the rest.
    //! @relates Iterable
    //!
    //! Given an `Iterable` `xs` with a linearization of `[x1, x2, ...]` and
    //! a (non-negative) `Constant` `n` holding an unsigned integral value,
    //! `drop_front(xs, n)` is an iterable of the same data type whose
    //! linearization is `[xn+1, xn+2, ...]`. In particular, note that this
    //! function does not mutate the original iterable in any way. If `n` is
    //! not given, it defaults to a `Constant` with an unsigned integral value
    //! equal to `1`.
    //!
    //! In case `length(xs) <= n`, `drop_front` will simply drop the whole
    //! iterable without failing, thus returning an empty iterable. This is
    //! different from `drop_front_exactly`, which expects `n <= length(xs)`
    //! but can be better optimized because of this additional guarantee.
    //!
    //!
    //! @param xs
    //! The iterable from which elements are dropped.
    //!
    //! @param n
    //! A non-negative `Constant` holding an unsigned integral value
    //! representing the number of elements to be dropped from the iterable.
    //! If `n` is not given, it defaults to a `Constant` with an unsigned
    //! integral value equal to `1`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop_front
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop_front = [](auto&& xs[, auto&& n]) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct drop_front_impl;

    struct _drop_front {
        template <typename Xs, typename N>
        constexpr auto operator()(Xs&& xs, N&& n) const {
            using It = typename datatype<Xs>::type;
            using DropFront = BOOST_HANA_DISPATCH_IF(drop_front_impl<It>,
                _models<Iterable, It>{}() &&
                _models<Constant, N>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Iterable, It>{},
            "hana::drop_front(xs, n) requires 'xs' to be an Iterable");

            static_assert(_models<Constant, N>{},
            "hana::drop_front(xs, n) requires 'n' to be a Constant");
        #endif

            return DropFront::apply(static_cast<Xs&&>(xs), static_cast<N&&>(n));
        }

        template <typename Xs>
        constexpr auto operator()(Xs&& xs) const;
    };

    constexpr _drop_front drop_front{};
#endif

    //! Drop the first `n` elements of an iterable, and return the rest.
    //! @relates Iterable
    //!
    //! Given an `Iterable` `xs` with a linearization of `[x1, x2, ...]` and
    //! a (non-negative) `Constant` `n` holding an unsigned integral value,
    //! `drop_front_exactly(xs, n)` is an iterable of the same data type whose
    //! linearization is `[xn+1, xn+2, ...]`. In particular, note that this
    //! function does not mutate the original iterable in any way. If `n` is
    //! not given, it defaults to a `Constant` with an unsigned integral value
    //! equal to `1`.
    //!
    //! It is an error to use `drop_front_exactly` with `n > length(xs)`. This
    //! additional guarantee allows `drop_front_exactly` to be better optimized
    //! than the `drop_front` function, which allows `n > length(xs)`.
    //!
    //!
    //! @param xs
    //! The iterable from which elements are dropped.
    //!
    //! @param n
    //! A non-negative `Constant` holding an unsigned integral value
    //! representing the number of elements to be dropped from the iterable.
    //! `n` must be less than or equal to the number of elements in `xs`.
    //! If `n` is not given, it defaults to a `Constant` with an unsigned
    //! integral value equal to `1`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop_front_exactly
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop_front_exactly = [](auto&& xs[, auto&& n]) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct drop_front_exactly_impl;

    struct _drop_front_exactly {
        template <typename Xs, typename N>
        constexpr decltype(auto) operator()(Xs&& xs, N&& n) const {
            using It = typename datatype<Xs>::type;
            using DropFrontExactly = BOOST_HANA_DISPATCH_IF(
                drop_front_exactly_impl<It>,
                _models<Iterable, It>{}() &&
                _models<Constant, N>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Iterable, It>{},
            "hana::drop_front_exactly(xs, n) requires 'xs' to be an Iterable");

            static_assert(_models<Constant, N>{},
            "hana::drop_front_exactly(xs, n) requires 'n' to be a Constant");
        #endif

            return DropFrontExactly::apply(static_cast<Xs&&>(xs),
                                           static_cast<N&&>(n));
        }

        template <typename Xs>
        constexpr auto operator()(Xs&& xs) const;
    };

    constexpr _drop_front_exactly drop_front_exactly{};
#endif

    //! Drop elements from an iterable up to, but excluding, the first
    //! element for which the `predicate` is not satisfied.
    //! @relates Iterable
    //!
    //! Specifically, `drop_while` returns an iterable containing all the
    //! elements of the original iterable except for those in the range
    //! delimited by [`head`, `e`), where `head` is the first element and
    //! `e` is the first element for which the `predicate` is not satisfied.
    //! If the iterable is not finite, the `predicate` has to return a false-
    //! valued `Logical` at a finite index for this method to return.
    //!
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! structure, and returning a `Logical` representing whether `x` should
    //! be dropped from the structure. In the current version of the library,
    //! `predicate` should return a compile-time `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop_while
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop_while = [](auto&& iterable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct drop_while_impl;

    struct _drop_while {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            using It = typename datatype<Xs>::type;
            using DropWhile = BOOST_HANA_DISPATCH_IF(
                drop_while_impl<It>,
                _models<Iterable, It>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Iterable, It>{},
            "hana::drop_while(xs, pred) requires xs to be an Iterable");
        #endif

            return DropWhile::apply(static_cast<Xs&&>(xs), static_cast<Pred&&>(pred));
        }
    };

    constexpr _drop_while drop_while{};
#endif

    //! Drop elements from an iterable up to, but excluding, the first
    //! element for which the `predicate` is satisfied.
    //! @relates Iterable
    //!
    //! Specifically, `drop_until` returns an iterable containing all the
    //! elements of the original iterable except for those in the range
    //! delimited by [`head`, `e`), where `head` is the first element and
    //! `e` is the first element for which the `predicate` is satisfied.
    //! If the iterable is not finite, the `predicate` has to return a true-
    //! valued `Logical` at a finite index for this method to return.
    //!
    //! @note
    //! This is effectively equivalent to `drop_while` with a negated
    //! `predicate`.
    //!
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! structure, and returning a `Logical` representing whether `x` and
    //! subsequent elements should be kept in the structure. In the current
    //! version of the library, `predicate` should return a compile-time
    //! `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp drop_until
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop_until = [](auto&& iterable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct drop_until_impl;

    struct _drop_until {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            using It = typename datatype<Xs>::type;
            using DropUntil = BOOST_HANA_DISPATCH_IF(
                drop_until_impl<It>,
                _models<Iterable, It>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Iterable, It>{},
            "hana::drop_until(xs, pred) requires xs to be an Iterable");
        #endif

            return DropUntil::apply(static_cast<Xs&&>(xs),
                                    static_cast<Pred&&>(pred));
        }
    };

    constexpr _drop_until drop_until{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ITERABLE_HPP

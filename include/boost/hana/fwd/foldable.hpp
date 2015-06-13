/*!
@file
Forward declares `boost::hana::Foldable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_FOLDABLE_HPP
#define BOOST_HANA_FWD_FOLDABLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/by_fwd.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/fwd/integral_constant.hpp>
#include <boost/hana/fwd/monad.hpp>
#include <boost/hana/fwd/monoid.hpp>
#include <boost/hana/fwd/ring.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Foldable` concept represents data structures that can be reduced
    //! to a single value.
    //!
    //! Another way of seeing `Foldable` is as data structures supporting
    //! internal iteration with the ability to accumulate a result. By
    //! internal iteration, we mean that the _loop control_ is in the hand
    //! of the structure, not the caller. Hence, it is the structure who
    //! decides when the iteration stops, which is normally when the whole
    //! structure has been consumed. Since C++ is an eager language, this
    //! requires `Foldable` structures to be finite, or otherwise one would
    //! need to loop indefinitely to consume the whole structure.
    //!
    //! @note
    //! While the fact that `Foldable` only works for finite structures may
    //! seem overly restrictive in comparison to the Haskell definition of
    //! `Foldable`, a finer grained separation of the concepts should
    //! mitigate the issue. For iterating over possibly infinite data
    //! structures, see the `Iterable` concept. For searching a possibly
    //! infinite data structure, see the `Searchable` concept.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! (`fold.left` and `fold.right`) or `unpack`
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `Either`, `Map`, `Optional`, `Pair`, `Set`, `Range`, `Tuple`
    //!
    //!
    //! @anchor Foldable-lin
    //! The linearization of a `Foldable`
    //! ---------------------------------
    //! Intuitively, for a `Foldable` structure `xs`, the _linearization_ of
    //! `xs` is the sequence of all the elements in `xs` as if they had been
    //! put in a list:
    //! @code
    //!     linearization(xs) = [x1, x2, ..., xn]
    //! @endcode
    //!
    //! Note that it is always possible to produce such a linearization
    //! for a finite `Foldable` by setting
    //! @code
    //!     linearization(xs) = fold.left(xs, [], flip(prepend))
    //! @endcode
    //! for an appropriate definition of `[]` and `prepend`. The notion of
    //! linearization is useful for expressing various properties of
    //! `Foldable` structures, and is used across the documentation. Also
    //! note that `Iterable`s define an [extended version](@ref Iterable-lin)
    //! of this allowing for infinite structures.
    //!
    //!
    //! Compile-time Foldables
    //! ----------------------
    //! A compile-time `Foldable` is a `Foldable` whose total length is known
    //! at compile-time. In other words, it is a `Foldable` whose `length`
    //! method returns a `Constant` of an unsigned integral type. When
    //! folding a compile-time `Foldable`, the folding can be unrolled,
    //! because the final number of steps of the algorithm is known at
    //! compile-time.
    //!
    //! Additionally, the `unpack` method is only available to compile-time
    //! `Foldable`s. This is because the return _type_ of `unpack` depends
    //! on the number of objects in the structure. Being able to resolve
    //! `unpack`'s return type at compile-time hence requires the length of
    //! the structure to be known at compile-time too.
    //!
    //! __In the current version of the library, only compile-time `Foldable`s
    //! are supported.__ While it would be possible in theory to support
    //! runtime `Foldable`s too, doing so efficiently requires more research.
    //!
    //!
    //! Provided conversion to any `Sequence`
    //! -------------------------------------
    //! In the spirit of the notion of linearization presented above, any
    //! `Foldable` can be converted to a `Sequence`. The result is, as one
    //! might expect, simply the linearization of the `Foldable` structure.
    //! More specifically, given a `Foldable` `xs` with a linearization of
    //! `[x1, ..., xn]` and a `Sequence` `S`, `to<S>(xs)` is equivalent to
    //! `make<S>(x1, ..., xn)`.
    //! @snippet example/foldable.cpp conversion
    //!
    //!
    //! Free model for builtin arrays
    //! -----------------------------
    //! Builtin arrays whose size is known can be folded as-if they were
    //! homogeneous tuples. However, note that builtin arrays can't be
    //! made more than `Foldable` (e.g. `Iterable`) because they can't
    //! be empty and they also can't be returned from functions.
    struct Foldable { };

    //! Fold a structure using a binary operation and (optionally) an initial
    //! reduction state.
    //! @relates Foldable
    //!
    //! Generally speaking, folding refers to the concept of summarizing a
    //! complex structure as a single value, by successively applying a
    //! binary operation which reduces two elements of the structure to a
    //! single value. Folds come in many flavors; left folds, right folds,
    //! folds with and without an initial reduction state, and their monadic
    //! variants. This method can be used to access all of the non-monadic
    //! fold variants, by using the different syntaxes documented below.
    //! Here's a summary of the different folds:
    //! @code
    //!     fold.left(xs, state, f) = see below
    //!     fold.left(xs, f) = see below
    //!     fold = fold.left
    //!
    //!     fold.right(xs, state, f) = see below
    //!     fold.right(xs, f) = see below
    //! @endcode
    //!
    //! In the above, `xs` is always the structure to be folded. Similarly,
    //! `state` is an initial value to use as the accumulation state.
    //!
    //! When the structure is empty, two things may arise. If an initial
    //! state was provided, it is returned as-is. Otherwise, if the no-state
    //! version of the method was used, an error is triggered. When the
    //! stucture contains a single element and the no-state version of the
    //! method was used, that single element is returned as is. This behavior
    //! is consistent with what happens for empty structures with a provided
    //! initial state.
    //!
    //!
    //! ## Left folds (`fold.left`)
    //! `fold.left` is a left-associative fold using a binary operation.
    //! Given a structure containing `x1, ..., xn`, a function `f` and
    //! optionnally an initial state, `fold.left` applies `f` as follows
    //! @code
    //!     f(... f(f(f(x1, x2), x3), x4) ..., xn) // without state
    //!     f(... f(f(f(f(state, x1), x2), x3), x4) ..., xn) // with state
    //! @endcode
    //!
    //! ### Signature
    //! Given a `Foldable` `xs` of data type `F(T)`, a function
    //! \f$ f : S \times T \to S \f$ and an initial state `s` of
    //! data type `S`, the signatures for `fold.left` are
    //! \f[
    //!     \mathrm{fold.left} : F(T) \times S \times (S \times T \to S) \to S
    //! \f]
    //!
    //! for the variant with an initial state, and
    //! \f[
    //!     \mathrm{fold.left} : F(T) \times (T \times T \to T) \to T
    //! \f]
    //!
    //! for the variant without an initial state.
    //!
    //! @param xs
    //! The structure to fold.
    //!
    //! @param state
    //! The initial value used for folding.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the
    //! result accumulated so far and `x` is an element in the structure.
    //! For left folds without an initial state, the function is called as
    //! `f(x1, x2)`, where `x1` and `x2` are elements of the structure.
    //!
    //! @note
    //! `fold` is equivalent to `fold.left`, so `fold(xs, f)` and
    //! `fold(xs, state, f)` are equivalent to `fold.left(xs, f)` and
    //! `fold.left(xs, state, f)` respectively. This is provided solely
    //! for convenience.
    //!
    //! ### Example
    //! @snippet example/foldable.cpp fold.left
    //!
    //!
    //! ## Right folds (`fold.right`)
    //! `fold.right` is a right-associative fold using a binary operation.
    //! Given a structure containing `x1, ..., xn`, a function `f` and
    //! optionnally an initial state, `fold.right` applies `f` as follows
    //! @code
    //!     f(x1, f(x2, f(x3, f(x4, ... f(xn-1, xn) ... )))) // without state
    //!     f(x1, f(x2, f(x3, f(x4, ... f(xn, state) ... )))) // with state
    //! @endcode
    //!
    //! It is worth noting that the order in which the binary function should
    //! expect its arguments is reversed from `fold.left`.
    //!
    //! ### Signature
    //! Given a `Foldable` `xs` of data type `F(T)`, a function
    //! \f$ f : T \times S \to S \f$ and an initial state `s` of
    //! data type `S`, the signatures for `fold.right` are
    //! \f[
    //!     \mathrm{fold.right} : F(T) \times S \times (T \times S \to S) \to S
    //! \f]
    //!
    //! for the variant with an initial state, and
    //! \f[
    //!     \mathrm{fold.right} : F(T) \times (T \times T \to T) \to T
    //! \f]
    //!
    //! for the variant without an initial state.
    //!
    //! @param xs
    //! The structure to fold.
    //!
    //! @param state
    //! The initial value used for folding.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the
    //! result accumulated so far and `x` is an element in the structure.
    //! For right folds without an initial state, the function is called as
    //! `f(x1, x2)`, where `x1` and `x2` are elements of the structure.
    //!
    //! ### Example
    //! @snippet example/foldable.cpp fold.right
    //!
    //!
    //! ## Tag-dispatching
    //! All of the different fold variants are tag-dispatched methods
    //! and can be overridden individually. Here is how each variant is
    //! tag-dispatched (where `Xs` is the data type of `xs`):
    //! @code
    //!     fold.left(xs, state, f) -> fold_left_impl<Xs>::apply(xs, state, f)
    //!     fold.left(xs, f)        -> fold_left_nostate_impl<Xs>::apply(xs, f)
    //!
    //!     fold.right(xs, state, f) -> fold_right_impl<Xs>::apply(xs, state, f)
    //!     fold.right(xs, f)        -> fold_right_nostate_impl<Xs>::apply(xs, f)
    //! @endcode
    //! `fold` is not tag-dispatched because it is just an alias to `fold.left`.
    //!
    //!
    //! Benchmarks
    //! ----------
    //! <div class="benchmark-chart"
    //!      style="min-width: 310px; height: 400px; margin: 0 auto"
    //!      data-dataset="benchmark.fold_left.compile.json">
    //! </div>
    //! <div class="benchmark-chart"
    //!      style="min-width: 310px; height: 400px; margin: 0 auto"
    //!      data-dataset="benchmark.fold_right.compile.json">
    //! </div>
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto fold = see documentation;
#else
    template <typename Xs, typename = void>
    struct fold_left_impl;

    template <typename Xs, typename = void>
    struct fold_left_nostate_impl;

    template <typename Xs, typename = void>
    struct fold_right_impl;

    template <typename Xs, typename = void>
    struct fold_right_nostate_impl;


    struct _fold_left {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
            using S = typename datatype<Xs>::type;
            using FoldLeft = BOOST_HANA_DISPATCH_IF(fold_left_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::fold.left(xs, state, f) requires xs to be Foldable");
        #endif

            return FoldLeft::apply(static_cast<Xs&&>(xs),
                                   static_cast<State&&>(state),
                                   static_cast<F&&>(f));
        }

        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            using S = typename datatype<Xs>::type;
            using FoldLeft = BOOST_HANA_DISPATCH_IF(fold_left_nostate_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::fold.left(xs, f) requires xs to be Foldable");
        #endif

            return FoldLeft::apply(static_cast<Xs&&>(xs), static_cast<F&&>(f));
        }
    };

    struct _fold_right {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
            using S = typename datatype<Xs>::type;
            using FoldRight = BOOST_HANA_DISPATCH_IF(fold_right_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::fold.right(xs, state, f) requires xs to be Foldable");
        #endif

            return FoldRight::apply(static_cast<Xs&&>(xs),
                                    static_cast<State&&>(state),
                                    static_cast<F&&>(f));
        }

        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            using S = typename datatype<Xs>::type;
            using FoldRight = BOOST_HANA_DISPATCH_IF(fold_right_nostate_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::fold.right(xs, f) requires xs to be Foldable");
        #endif

            return FoldRight::apply(static_cast<Xs&&>(xs), static_cast<F&&>(f));
        }
    };

    template <typename ...AvoidODRViolation>
    struct _fold : _fold_left {
        static constexpr _fold_left left{};
        static constexpr _fold_right right{};
    };
    template <typename ...AvoidODRViolation>
    constexpr _fold_left _fold<AvoidODRViolation...>::left;
    template <typename ...AvoidODRViolation>
    constexpr _fold_right _fold<AvoidODRViolation...>::right;

    constexpr _fold<> fold{};
#endif

    //! Monadic fold of a structure with a binary operation and an optional
    //! initial reduction state.
    //! @relates Foldable
    //!
    //! @note
    //! This assumes the reader to be accustomed to non-monadic folds as
    //! explained by `hana::fold`.
    //!
    //! The `monadic_fold` method is used for left or right folding a
    //! structure with a monadic binary operation and an optional initial
    //! state. Basically, a monadic fold is a fold in which subsequent calls
    //! to the binary function are chained with the monadic `chain` operator
    //! of the corresponding Monad. This allows a structure to be folded in
    //! a custom monadic context. For example, performing a monadic fold with
    //! the `Optional` monad would require the binary function to return the
    //! result as an `Optional`, and the fold would abort and return nothing
    //! whenever one of the accumulation step would fail (i.e. return `nothing`).
    //! If, however, all the reduction steps succeed, then `just` the result
    //! would be returned. Different monads will of course result in
    //! different effects.
    //!
    //! This method can be used to access all of the monadic fold variants,
    //! by using the different syntaxes documented below. Here's a summary:
    //! @code
    //!     monadic_fold<M>.left(xs, state, f) = see below
    //!     monadic_fold<M>.left(xs, f) = see below
    //!     monadic_fold<M> = monadic_fold<M>.left
    //!
    //!     monadic_fold<M>.right(xs, state, f) = see below
    //!     monadic_fold<M>.right(xs, f) = see below
    //! @endcode
    //!
    //! In the above, `xs` is always the structure to be folded. Similarly,
    //! `state` is an initial value to use as the accumulation state.
    //!
    //! When the structure is empty, two things may arise. If an initial
    //! state was provided, it is lifted to the given Monad and returned
    //! as-is. Otherwise, if the no-state version of the method was used,
    //! an error is triggered. When the stucture contains a single element
    //! and the no-state version of the method was used, that single element
    //! is lifted into the given Monad and returned as is. This behavior is
    //! consistent with what happens for empty structures with a provided
    //! initial state.
    //!
    //!
    //! ## Monadic left folds (`monadic_fold<M>.left`)
    //! `monadic_fold<M>.left` is a left-associative monadic fold. Given a
    //! structure containing `x1, ..., xn`, a function `f` and an optional
    //! initial state, `monadic_fold<M>.left` applies `f` as follows
    //! @code
    //!     // with state
    //!     ((((f(state, x1) | f(-, x2)) | f(-, x3)) | ...) | f(-, xn))
    //!
    //!     // without state
    //!     ((((f(x1, x2) | f(-, x3)) | f(-, x4)) | ...) | f(-, xn))
    //! @endcode
    //!
    //! where `f(-, xk)` denotes the partial application of `f` to `xk`, and
    //! `|` is just the operator version of the monadic `chain`.
    //!
    //! ### Signature
    //! Given a Monad of data type `M`, a Foldable of data type `F(T)`,
    //! an initial state of data type `S` and a function
    //! \f$ f : S \times T \to M(S) \f$, the signatures of
    //! `monadic_fold<M>.left` are
    //! \f[
    //!     \mathrm{monadic\_fold}_M.\mathrm{left} :
    //!         F(T) \times S \times (S \times T \to M(S)) \to M(S)
    //! \f]
    //!
    //! for the version with an initial state, and
    //! \f[
    //!     \mathrm{monadic\_fold}_M.\mathrm{left} :
    //!         F(T) \times (T \times T \to M(T)) \to M(T)
    //! \f]
    //!
    //! for the version without an initial state.
    //!
    //! @tparam M
    //! The Monad representing the monadic context in which the fold happens.
    //! The return type of `f` must be in that Monad.
    //!
    //! @param xs
    //! The structure to fold.
    //!
    //! @param state
    //! The initial value used for folding. If the structure is empty, this
    //! value is lifted in to the `M` Monad and then returned as-is.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the result
    //! accumulated so far and `x` is an element in the structure. The
    //! function must return its result inside the `M` Monad.
    //!
    //! ### Example
    //! @snippet example/foldable.monadic_fold.left.cpp monadic_fold.left
    //!
    //!
    //! ## Monadic right folds (`monadic_fold<M>.right`)
    //! `monadic_fold<M>.right` is a right-associative monadic fold. Given a
    //! structure containing `x1, ..., xn`, a function `f` and an optional
    //! initial state, `monadic_fold<M>.right` applies `f` as follows
    //! @code
    //!     // with state
    //!     (f(x1, -) | (f(x2, -) | (f(x3, -) | (... | f(xn, state)))))
    //!
    //!     // without state
    //!     (f(x1, -) | (f(x2, -) | (f(x3, -) | (... | f(xn-1, xn)))))
    //! @endcode
    //!
    //! where `f(xk, -)` denotes the partial application of `f` to `xk`,
    //! and `|` is just the operator version of the monadic `chain`.
    //! It is worth noting that the order in which the binary function should
    //! expect its arguments is reversed from `monadic_fold<M>.left`.
    //!
    //! ### Signature
    //! Given a Monad of data type `M`, a Foldable of data type `F(T)`,
    //! an initial state of data type `S` and a function
    //! \f$ f : T \times S \to M(S) \f$, the signatures of
    //! `monadic_fold<M>.right` are
    //! \f[
    //!     \mathrm{monadic\_fold}_M.\mathrm{right} :
    //!         F(T) \times S \times (T \times S \to M(S)) \to M(S)
    //! \f]
    //!
    //! for the version with an initial state, and
    //! \f[
    //!     \mathrm{monadic\_fold}_M.\mathrm{right} :
    //!         F(T) \times (T \times T \to M(T)) \to M(T)
    //! \f]
    //!
    //! for the version without an initial state.
    //!
    //! @tparam M
    //! The Monad representing the monadic context in which the fold happens.
    //! The return type of `f` must be in that Monad.
    //!
    //! @param xs
    //! The structure to fold.
    //!
    //! @param state
    //! The initial value used for folding. If the structure is empty, this
    //! value is lifted in to the `M` Monad and then returned as-is.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the result
    //! accumulated so far and `x` is an element in the structure. The
    //! function must return its result inside the `M` Monad.
    //!
    //! ### Example
    //! @snippet example/foldable.cpp monadic_fold.right
    //!
    //!
    //! ## Tag-dispatching
    //! All of the different monadic fold variants are tag-dispatched methods
    //! and can be overridden individually. Here is how each variant is
    //! tag-dispatched (where `Xs` is the data type of `xs`):
    //! @code
    //!     monadic_fold<M>.left(xs, state, f) -> monadic_fold_left_impl<Xs>::apply<M>(xs, state, f)
    //!     monadic_fold<M>.left(xs, f)        -> monadic_fold_left_nostate_impl<Xs>::apply<M>(xs, f)
    //!
    //!     monadic_fold<M>.right(xs, state, f) -> monadic_fold_right_impl<Xs>::apply<M>(xs, state, f)
    //!     monadic_fold<M>.right(xs, f)        -> monadic_fold_right_nostate_impl<Xs>::apply<M>(xs, f)
    //! @endcode
    //! `monadic_fold<M>` is not tag-dispatched, because it is just an alias
    //! to `monadic_fold<M>.left`.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto monadic_fold = see documentation;
#else
    template <typename Xs, typename = void>
    struct monadic_fold_left_impl;

    template <typename Xs, typename = void>
    struct monadic_fold_left_nostate_impl;

    template <typename Xs, typename = void>
    struct monadic_fold_right_impl;

    template <typename Xs, typename = void>
    struct monadic_fold_right_nostate_impl;


    template <typename M>
    struct _monadic_fold_left {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
            using S = typename datatype<Xs>::type;
            using MonadicFoldLeft = BOOST_HANA_DISPATCH_IF(
                monadic_fold_left_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::monadic_fold<M>.left(xs, state, f) requires xs to be Foldable");
        #endif

            return MonadicFoldLeft::template apply<M>(static_cast<Xs&&>(xs),
                                                      static_cast<State&&>(state),
                                                      static_cast<F&&>(f));
        }

        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            using S = typename datatype<Xs>::type;
            using MonadicFoldLeft = BOOST_HANA_DISPATCH_IF(
                monadic_fold_left_nostate_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::monadic_fold<M>.left(xs, f) requires xs to be Foldable");
        #endif

            return MonadicFoldLeft::template apply<M>(static_cast<Xs&&>(xs),
                                                      static_cast<F&&>(f));
        }
    };

    template <typename M>
    struct _monadic_fold_right {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
            using S = typename datatype<Xs>::type;
            using MonadicFoldRight = BOOST_HANA_DISPATCH_IF(
                monadic_fold_right_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::monadic_fold<M>.right(xs, state, f) requires xs to be Foldable");
        #endif

            return MonadicFoldRight::template apply<M>(static_cast<Xs&&>(xs),
                                                       static_cast<State&&>(state),
                                                       static_cast<F&&>(f));
        }

        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            using S = typename datatype<Xs>::type;
            using MonadicFoldRight = BOOST_HANA_DISPATCH_IF(
                monadic_fold_right_nostate_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::monadic_fold<M>.right(xs, f) requires xs to be Foldable");
        #endif
            return MonadicFoldRight::template apply<M>(static_cast<Xs&&>(xs),
                                                       static_cast<F&&>(f));
        }
    };

    template <typename M>
    struct _monadic_fold : _monadic_fold_left<M> {
    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Monad, M>{},
        "hana::monadic_fold<M> requires M to be a Monad");
    #endif
        static constexpr _monadic_fold_left<M> left{};
        static constexpr _monadic_fold_right<M> right{};
    };
    template <typename M>
    constexpr _monadic_fold_left<M> _monadic_fold<M>::left;
    template <typename M>
    constexpr _monadic_fold_right<M> _monadic_fold<M>::right;

    template <typename M>
    constexpr _monadic_fold<M> monadic_fold{};
#endif

    //! Equivalent to `reverse_fold` in Boost.Fusion and Boost.MPL.
    //! @relates Foldable
    //!
    //! This method has the same semantics as `reverse_fold` in Boost.Fusion
    //! and Boost.MPL, with the extension that an initial state is not
    //! required. This method is equivalent to `fold.right`, except that
    //! the accumulating function must take its arguments in reverse order,
    //! to match the order used in Fusion. In other words,
    //! @code
    //!     reverse_fold(sequence, state, f) == fold.right(sequence, state, flip(f))
    //!     reverse_fold(sequence, f) == fold.right(sequence, flip(f))
    //! @endcode
    //!
    //!
    //! @note
    //! This method is a convenience alias to `fold.right`. As an alias,
    //! `reverse_fold` is not tag-dispatched on its own and `fold.right`
    //! should be customized instead.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp reverse_fold
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto reverse_fold = [](auto&& foldable[, auto&& state], auto&& f) -> decltype(auto) {
        return see-documentation;
    };
#else
    struct _reverse_fold {
        template <typename Xs, typename S, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, S&& s, F&& f) const {
            return hana::fold.right(static_cast<Xs&&>(xs),
                                    static_cast<S&&>(s),
                                    hana::flip(static_cast<F&&>(f)));
        }

        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return hana::fold.right(static_cast<Xs&&>(xs),
                                    hana::flip(static_cast<F&&>(f)));
        }
    };

    constexpr _reverse_fold reverse_fold{};
#endif

    //! Perform an action on each element of a foldable, discarding
    //! the result each time.
    //! @relates Foldable
    //!
    //! Iteration is done from left to right, i.e. in the same order as when
    //! using `fold.left`. If the structure is not finite, this method will
    //! not terminate.
    //!
    //!
    //! @param foldable
    //! The structure to iterate over.
    //!
    //! @param f
    //! A function called as `f(x)` for each element `x` of the structure.
    //! The result of `f(x)`, whatever it is, is ignored.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp for_each
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto for_each = [](auto&& foldable, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct for_each_impl;

    struct _for_each {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            using S = typename datatype<Xs>::type;
            using ForEach = BOOST_HANA_DISPATCH_IF(
                for_each_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::for_each(xs, f) requires xs to be Foldable");
        #endif

            return ForEach::apply(static_cast<Xs&&>(xs), static_cast<F&&>(f));
        }
    };

    constexpr _for_each for_each{};
#endif

    //! Return the number of elements in a finite structure.
    //! @relates Foldable
    //!
    //! Specifically, returns an object of an unsigned integral type, or
    //! a `Constant` holding such an object, which represents the number
    //! of elements in the structure. The structure has to be finite for
    //! this method to return.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp length
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto length = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct length_impl;

    struct _length {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using S = typename datatype<Xs>::type;
            using Length = BOOST_HANA_DISPATCH_IF(
                length_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::length(xs) requires xs to be Foldable");
        #endif

            return Length::apply(static_cast<Xs&&>(xs));
        }
    };

    constexpr _length length{};
#endif

    //! Equivalent to `length`; provided for consistency with the
    //! standard library.
    //! @relates Foldable
    //!
    //! This method is an alias to `length` provided for convenience and
    //! consistency with the standard library. As an alias, `size` is not
    //! tag-dispatched on its own and `length` should be customized instead.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp size
    constexpr auto size = length;

    //! Return the least element of a non-empty structure with respect to
    //! a `predicate`, by default `less`.
    //! @relates Foldable
    //!
    //! Given a non-empty structure and an optional binary predicate
    //! (`less` by default), `minimum` returns the least element of
    //! the structure, i.e. an element which is less than or equal to
    //! every other element in the structure, according to the predicate.
    //!
    //! If the structure contains heterogeneous objects, then the predicate
    //! must return a compile-time `Logical`. If no predicate is provided,
    //! the elements in the structure must be Orderable, or compile-time
    //! Orderable if the structure is heterogeneous.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Foldable `xs` of data type `F(T)`, a Logical `Bool` and a
    //! predicate \f$ pred : T \times T \to Bool \f$, `minimum` has the
    //! following signatures. For the variant with a provided predicate,
    //! \f[
    //!     \mathrm{minimum} : S(T) \times (T \times T \to Bool) \to T
    //! \f]
    //!
    //! for the variant without a custom predicate, the `T` data type is
    //! required to be Orderable. The signature is then
    //! \f[
    //!     \mathrm{minimum} : S(T) \to T
    //! \f]
    //!
    //! @param xs
    //! The structure to find the least element of.
    //!
    //! @param predicate
    //! A function called as `predicate(x, y)`, where `x` and `y` are elements
    //! of the structure. `predicate` should be a strict weak ordering on the
    //! elements of the structure and its return value should be a Logical,
    //! or a compile-time Logical if the structure is heterogeneous.
    //!
    //! ### Example
    //! @snippet example/foldable.cpp minimum
    //!
    //!
    //! Syntactic sugar (`minimum.by`)
    //! ------------------------------
    //! `minimum` can be called in a third way, which provides a nice syntax
    //! especially when working with the `ordering` combinator:
    //! @code
    //!     minimum.by(predicate, xs) == minimum(xs, predicate)
    //!     minimum.by(predicate) == minimum(-, predicate)
    //! @endcode
    //!
    //! where `minimum(-, predicate)` denotes the partial application of
    //! `minimum` to `predicate`.
    //!
    //! ### Example
    //! @snippet example/foldable.cpp minimum.by
    //!
    //!
    //! Tag dispatching
    //! ---------------
    //! Both the non-predicated version and the predicated versions of
    //! `minimum` are tag-dispatched methods, and hence they can be
    //! customized independently. One reason for this is that some
    //! structures are able to provide a much more efficient implementation
    //! of `minimum` when the `less` predicate is used. Here is how the
    //! different versions of `minimum` are dispatched:
    //! @code
    //!     minimum(xs) -> minimum_impl<data type of xs>::apply(xs)
    //!     minimum(xs, pred) -> minimum_pred_impl<data type of xs>::apply(xs, pred)
    //! @endcode
    //!
    //! Also note that `minimum.by` is not tag-dispatched on its own, since it
    //! is just syntactic sugar for calling the corresponding `minimum`.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto minimum = [](auto&& xs[, auto&& predicate]) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct minimum_impl;

    template <typename S, typename = void>
    struct minimum_pred_impl;

    struct _minimum : detail::by<_minimum> {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using S = typename datatype<Xs>::type;
            using Minimum = BOOST_HANA_DISPATCH_IF(
                minimum_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::minimum(xs) requires xs to be Foldable");
        #endif

            return Minimum::apply(static_cast<Xs&&>(xs));
        }

        template <typename Xs, typename Predicate>
        constexpr decltype(auto) operator()(Xs&& xs, Predicate&& pred) const {
            using S = typename datatype<Xs>::type;
            using Minimum = BOOST_HANA_DISPATCH_IF(
                minimum_pred_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::minimum(xs, predicate) requires xs to be Foldable");
        #endif

            return Minimum::apply(static_cast<Xs&&>(xs),
                                  static_cast<Predicate&&>(pred));
        }
    };

    constexpr _minimum minimum{};
#endif

    //! Return the greatest element of a non-empty structure with respect to
    //! a `predicate`, by default `less`.
    //! @relates Foldable
    //!
    //! Given a non-empty structure and an optional binary predicate
    //! (`less` by default), `maximum` returns the greatest element of
    //! the structure, i.e. an element which is greater than or equal to
    //! every other element in the structure, according to the predicate.
    //!
    //! If the structure contains heterogeneous objects, then the predicate
    //! must return a compile-time `Logical`. If no predicate is provided,
    //! the elements in the structure must be Orderable, or compile-time
    //! Orderable if the structure is heterogeneous.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Foldable `xs` of data type `F(T)`, a Logical `Bool` and a
    //! predicate \f$ pred : T \times T \to Bool \f$, `maximum` has the
    //! following signatures. For the variant with a provided predicate,
    //! \f[
    //!     \mathrm{maximum} : S(T) \times (T \times T \to Bool) \to T
    //! \f]
    //!
    //! for the variant without a custom predicate, the `T` data type is
    //! required to be Orderable. The signature is then
    //! \f[
    //!     \mathrm{maximum} : S(T) \to T
    //! \f]
    //!
    //! @param xs
    //! The structure to find the greatest element of.
    //!
    //! @param predicate
    //! A function called as `predicate(x, y)`, where `x` and `y` are elements
    //! of the structure. `predicate` should be a strict weak ordering on the
    //! elements of the structure and its return value should be a Logical,
    //! or a compile-time Logical if the structure is heterogeneous.
    //!
    //! ### Example
    //! @snippet example/foldable.cpp maximum
    //!
    //!
    //! Syntactic sugar (`maximum.by`)
    //! ------------------------------
    //! `maximum` can be called in a third way, which provides a nice syntax
    //! especially when working with the `ordering` combinator:
    //! @code
    //!     maximum.by(predicate, xs) == maximum(xs, predicate)
    //!     maximum.by(predicate) == maximum(-, predicate)
    //! @endcode
    //!
    //! where `maximum(-, predicate)` denotes the partial application of
    //! `maximum` to `predicate`.
    //!
    //! ### Example
    //! @snippet example/foldable.cpp maximum.by
    //!
    //!
    //! Tag dispatching
    //! ---------------
    //! Both the non-predicated version and the predicated versions of
    //! `maximum` are tag-dispatched methods, and hence they can be
    //! customized independently. One reason for this is that some
    //! structures are able to provide a much more efficient implementation
    //! of `maximum` when the `less` predicate is used. Here is how the
    //! different versions of `maximum` are dispatched:
    //! @code
    //!     maximum(xs) -> maximum_impl<data type of xs>::apply(xs)
    //!     maximum(xs, pred) -> maximum_pred_impl<data type of xs>::apply(xs, pred)
    //! @endcode
    //!
    //! Also note that `maximum.by` is not tag-dispatched on its own, since it
    //! is just syntactic sugar for calling the corresponding `maximum`.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto maximum = [](auto&& xs[, auto&& predicate]) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct maximum_impl;

    template <typename S, typename = void>
    struct maximum_pred_impl;

    struct _maximum : detail::by<_maximum> {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using S = typename datatype<Xs>::type;
            using Maximum = BOOST_HANA_DISPATCH_IF(maximum_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::maximum(xs) requires xs to be Foldable");
        #endif

            return Maximum::apply(static_cast<Xs&&>(xs));
        }

        template <typename Xs, typename Predicate>
        constexpr decltype(auto) operator()(Xs&& xs, Predicate&& pred) const {
            using S = typename datatype<Xs>::type;
            using Maximum = BOOST_HANA_DISPATCH_IF(maximum_pred_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::maximum(xs, predicate) requires xs to be Foldable");
        #endif

            return Maximum::apply(static_cast<Xs&&>(xs),
                                  static_cast<Predicate&&>(pred));
        }
    };

    constexpr _maximum maximum{};
#endif

    //! Compute the sum of the numbers of a structure.
    //! @relates Foldable
    //!
    //! More generally, `sum` will take any foldable structure containing
    //! objects forming a Monoid and reduce them using the Monoid's binary
    //! operation. The initial state for folding is the identity of the
    //! Monoid. It is sometimes necessary to specify the Monoid to use;
    //! this is possible by using `sum<M>`. If no Monoid is specified,
    //! the structure will use the Monoid formed by the elements it contains
    //! (if it knows it), or `IntegralConstant<int>` otherwise. Hence,
    //! @code
    //!     sum<M>(xs) = fold.left(xs, zero<M or inferred Monoid>(), plus)
    //!     sum<> = sum<IntegralConstant<int>>
    //! @endcode
    //!
    //! For numbers, this will just compute the sum of the numbers in the
    //! `xs` structure.
    //!
    //!
    //! @note
    //! The elements of the structure are not actually required to be in the
    //! same Monoid, but it must be possible to perform `plus` on any two
    //! adjacent elements of the structure, which requires each pair of
    //! adjacent element to at least have a common Monoid embedding. The
    //! meaning of "adjacent" as used here is that two elements of the
    //! structure `x` and `y` are adjacent if and only if they are adjacent
    //! in the linearization of that structure, as documented by the Iterable
    //! concept.
    //!
    //!
    //! Why must the Monoid be specified sometimes?
    //! -------------------------------------------
    //! This is because sequences like Tuple are not parameterized data
    //! types (by design). Hence, we do not know what kind of objects are
    //! in the sequence, and so that must be specified explicitly. Other
    //! foldable structures like Ranges will ignore the suggested Monoid
    //! because they know the data type of the objects they contain. This
    //! inconsistent behavior is a limitation of the current design of data
    //! types, and work is being done to resolve it.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp sum
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto sum = see documentation;
#else
    template <typename Xs, typename = void>
    struct sum_impl;

    template <typename M>
    struct _sum {
    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Monoid, M>{},
        "hana::sum<M> requires M to be a Monoid");
    #endif

        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using S = typename datatype<Xs>::type;
            using Sum = BOOST_HANA_DISPATCH_IF(sum_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::sum<M>(xs) requires xs to be Foldable");
        #endif

            return Sum::template apply<M>(static_cast<Xs&&>(xs));
        }
    };

    template <typename M = IntegralConstant<int>>
    constexpr _sum<M> sum{};
#endif

    //! Compute the product of the numbers of a structure.
    //! @relates Foldable
    //!
    //! More generally, `product` will take any foldable structure containing
    //! objects forming a Ring and reduce them using the Ring's binary
    //! operation. The initial state for folding is the identity of the
    //! Ring's operation. It is sometimes necessary to specify the Ring to
    //! use; this is possible by using `product<R>`. If no Ring is specified,
    //! the structure will use the Ring formed by the elements it contains
    //! (if it knows it), or `IntegralConstant<int>` otherwise.
    //! Hence,
    //! @code
    //!     product<R>(xs) = fold.left(xs, one<R or inferred Ring>(), mult)
    //!     product<> = product<IntegralConstant<int>>
    //! @endcode
    //!
    //! For numbers, this will just compute the product of the numbers in the
    //! `xs` structure.
    //!
    //! @note
    //! The elements of the structure are not actually required to be in the
    //! same Ring, but it must be possible to perform `mult` on any two
    //! adjacent elements of the structure, which requires each pair of
    //! adjacent element to at least have a common Ring embedding. The
    //! meaning of "adjacent" as used here is that two elements of the
    //! structure `x` and `y` are adjacent if and only if they are adjacent
    //! in the linearization of that structure, as documented by the Iterable
    //! concept.
    //!
    //! @note
    //! See the documentation for `sum` to understand why the Ring must
    //! sometimes be specified explicitly.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp product
    //!
    //!
    //! Benchmarks
    //! ----------
    //! <div class="benchmark-chart"
    //!      style="min-width: 310px; height: 400px; margin: 0 auto"
    //!      data-dataset="benchmark.product.compile.json">
    //! </div>
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto product = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct product_impl;

    template <typename R>
    struct _product {
    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Ring, R>{},
        "hana::product<R> requires R to be a Ring");
    #endif

        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using S = typename datatype<Xs>::type;
            using Product = BOOST_HANA_DISPATCH_IF(product_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::product<R>(xs) requires xs to be Foldable");
        #endif

            return Product::template apply<R>(static_cast<Xs&&>(xs));
        }
    };

    template <typename R = IntegralConstant<int>>
    constexpr _product<R> product{};
#endif

    //! Return the number of elements in the structure for which the
    //! `predicate` is satisfied.
    //! @relates Foldable
    //!
    //! Specifically, returns an object of an unsigned integral type, or
    //! a `Constant` holding such an object, which represents the number
    //! of elements in the structure satisfying the given `predicate`.
    //!
    //!
    //! @param foldable
    //! The structure whose elements are counted.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! structure, and returning a `Logical` representing whether `x` should
    //! be counted.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp count_if
    //!
    //! Benchmarks
    //! ----------
    //! <div class="benchmark-chart"
    //!      style="min-width: 310px; height: 400px; margin: 0 auto"
    //!      data-dataset="benchmark.count_if.compile.json">
    //! </div>
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto count_if = [](auto&& foldable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct count_if_impl;

    struct _count_if {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            using S = typename datatype<Xs>::type;
            using CountIf = BOOST_HANA_DISPATCH_IF(count_if_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::count_if(xs, pred) requires xs to be Foldable");
        #endif

            return CountIf::apply(static_cast<Xs&&>(xs),
                                  static_cast<Pred&&>(pred));
        }
    };

    constexpr _count_if count_if{};
#endif

    //! Return the number of elements in the structure that compare equal to
    //! a given value.
    //! @relates Foldable
    //!
    //! Given a Foldable structure `foldable` and a value `value`, `count`
    //! returns an unsigned integral, or a Constant thereof, representing the
    //! number of elements of `foldable` that compare equal to `value`. For
    //! this method to be well-defined, all the elements of the structure must
    //! be Comparable with the given value.
    //!
    //!
    //! @param foldable
    //! The structure whose elements are counted.
    //!
    //! @param value
    //! A value compared with each element in the structure. Elements
    //! that compare equal to this value are counted, others are not.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp count
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto count = [](auto&& foldable, auto&& value) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct count_impl;

    struct _count {
        template <typename Xs, typename Value>
        constexpr decltype(auto) operator()(Xs&& xs, Value&& value) const {
            using S = typename datatype<Xs>::type;
            using Count = BOOST_HANA_DISPATCH_IF(count_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::count(xs, value) requires xs to be Foldable");
        #endif

            return Count::apply(static_cast<Xs&&>(xs), static_cast<Value&&>(value));
        }
    };

    constexpr _count count{};
#endif

    //! Invoke a function with the elements of a structure as arguments.
    //! @relates Foldable
    //!
    //! Given a function and a foldable structure whose length can be known at
    //! compile-time, `unpack` invokes the function with the contents of that
    //! structure. In other words, `unpack(foldable, f)` is equivalent to
    //! `f(x...)`, where `x...` are the elements of the structure. The length
    //! of the structure must be known at compile-time, because the version of
    //! `f`'s `operator()` that will be compiled depends on the number of
    //! arguments it is called with, which has to be known at compile-time.
    //!
    //! To create a function that accepts a foldable instead of variadic
    //! arguments, see `fuse` instead.
    //!
    //!
    //! @param foldable
    //! The structure to expand into the function.
    //!
    //! @param f
    //! A function to be invoked as `f(x...)`, where `x...` are the elements
    //! of the structure as-if they had been linearized with `to<Tuple>`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp unpack
    //!
    //!
    //! Rationale: `unpack`'s name and parameter order
    //! ----------------------------------------------
    //! It has been suggested a couple of times that `unpack` be called
    //! `apply` instead, and that the parameter order be reversed to match
    //! that of the [proposed std::apply function][1]. However, the name
    //! `apply` is already used to denote normal function application, an use
    //! which is consistent with the Boost MPL library and with the rest of
    //! the world, especially the functional programming community.
    //! Furthermore, the author of this library considers the proposed
    //! `std::apply` to have both an unfortunate name and an unfortunate
    //! parameter order. Indeed, taking the function as the first argument
    //! means that using `std::apply` with a lambda function looks like
    //! @code
    //! std::apply([](auto ...args) {
    //!     use(args...);
    //! }, tuple);
    //! @endcode
    //!
    //! which is undeniably ugly because of the trailing `, tuple)` part
    //! on the last line. On the other hand, taking the function as a
    //! second argument allows one to write
    //! @code
    //! hana::unpack(tuple, [](auto ...args) {
    //!     use(args...);
    //! });
    //! @endcode
    //!
    //! which looks much nicer. Because of these observations, the author
    //! of this library feels justified to use `unpack` instead of `apply`,
    //! and to use a sane parameter order.
    //!
    //! [1]: http://en.cppreference.com/w/cpp/experimental/apply
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto unpack = [](auto&& foldable, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct unpack_impl;

    struct _unpack {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            using S = typename datatype<Xs>::type;
            using Unpack = BOOST_HANA_DISPATCH_IF(unpack_impl<S>,
                _models<Foldable, S>{}()
            );

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Foldable, S>{},
            "hana::unpack(xs, f) requires xs to be Foldable");
        #endif

            return Unpack::apply(static_cast<Xs&&>(xs), static_cast<F&&>(f));
        }
    };

    constexpr _unpack unpack{};
#endif

    //! Transform a function taking multiple arguments into a function that
    //! can be called with a compile-time `Foldable`.
    //! @relates Foldable
    //!
    //!
    //! This function is provided for convenience as a different way of
    //! calling `unpack`. Specifically, `fuse(f)` is a function such that
    //! @code
    //!     fuse(f)(foldable) == unpack(foldable, f)
    //!                       == f(x...)
    //! @endcode
    //! where `x...` are the elements in the foldable. This function is
    //! useful when one wants to create a function that accepts a foldable
    //! which is not known yet.
    //!
    //! @note
    //! This function is not tag-dispatched; customize `unpack` instead.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp fuse
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto fuse = [](auto&& f) -> decltype(auto) {
        return [perfect-capture](auto&& xs) -> decltype(auto) {
            return unpack(forwarded(xs), forwarded(f));
        };
    };
#else
    constexpr auto fuse = curry<2>(flip(unpack));
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_FOLDABLE_HPP

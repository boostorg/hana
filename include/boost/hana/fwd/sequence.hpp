/*!
@file
Forward declares `boost::hana::Sequence`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_SEQUENCE_HPP
#define BOOST_HANA_FWD_SEQUENCE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/fwd/foldable.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Sequence` concept represents generic index-based sequences.
    //!
    //! Compared to other abstract concepts, the Sequence concept is very
    //! specific. It represents generic index-based sequences. The reason
    //! why such a specific concept is provided is because there are a lot
    //! of models that behave exactly the same while being implemented in
    //! wildly different ways. It is useful to regroup all those data types
    //! under the same umbrella for the purpose of generic programming.
    //!
    //! In fact, models of this concept are not only _similar_. They are
    //! actually _isomorphic_, in a sense that we define below, which is
    //! a fancy way of rigorously saying that they behave exactly the same
    //! to an external observer.
    //!
    //!
    //! Laws
    //! ----
    //! For any Sequence data type `S`, the `to<Tuple>` conversion from `S`
    //! (as a Foldable, see below) must be a natural isomorphism. Furthermore,
    //! it must be the unique (up to implementation) bijective natural
    //! MonadPlus and Iterable isomorphism between `Tuple` and `S`.
    //! Intuitively, this means that all Sequences act exactly like `Tuple`s,
    //! but their implementation may differ. This is ensured by stating that
    //! conversion to and from a `Tuple` preserves both information quantity
    //! and organization.
    //!
    //! First, information quantity is preserved by requiring `to<Tuple>`
    //! to be bijective. Hence, the `S` and `Tuple` data types contain the
    //! same amount of objects, and information quantity is preserved. Note
    //! that we explicitly require the isomorphism to be bijective because
    //! [not all isomorphisms are bijective][1].
    //!
    //! Then, information organization is preserved by requiring `to<Tuple>`
    //! to be the unique natural MonadPlus isomorphism between `Tuple` and
    //! `S`. Since everything in Sequence is implemented in terms of MonadPlus,
    //! Iterable and other superclasses, this effectively gives us laws that
    //! must be respected for the methods of Sequence. The result is that for
    //! any Sequence `xs` of data type `S` and any n-ary function `f` (suppose
    //! without loss of generality that `f` takes its Sequence argument in the
    //! first parameter),
    //! @code
    //!     to<Tuple>(f(xs, -, ..., -)) == f(to<Tuple>(xs), -, ..., -)
    //! @endcode
    //!
    //! If `f` does not return a Sequence, then simply change the above for
    //! @code
    //!     f(xs, -, ..., -) == f(to<Tuple>(xs), -, ..., -)
    //! @endcode
    //!
    //! Here, the notation `f(xs, -, ..., -)` denotes the partial application
    //! of the function `f` to its `xs` argument, with all the other arguments
    //! left unbound. Hence, these comparisons are comparisons between
    //! functions, and they express the fact that any Sequence is just
    //! as good as a `Tuple` for an external observer (the function `f`).
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! 1. `Iterable`, `empty`, `prepend`, `models`\n
    //! The `Sequence` concept does not provide basic methods that could be
    //! used as a minimal complete definition; instead, it borrows methods from
    //! other concepts and add laws to them. For this reason, it is necessary
    //! to specialize the `models` metafunction in the `boost::hana` namespace
    //! in addition to defining the above methods. Explicitly specializing the
    //! `models` metafunction can be seen like a seal saying "this data type
    //! satisfies the additional laws of a `Sequence`", since those can't be
    //! checked by Hana automatically.
    //!
    //!
    //! Superclasses
    //! ------------
    //! 1. `Comparable` (definition provided automatically)\n
    //! Two `Sequence`s are equal if and only if they contain the same number
    //! of elements and their elements at any given index are equal.
    //! @snippet example/sequence.cpp Comparable
    //!
    //! 2. `Orderable` (definition provided automatically)\n
    //! `Sequence`s are ordered using the traditional lexicographical ordering.
    //! @snippet example/sequence.cpp Orderable
    //!
    //! 3. `Functor` (definition provided automatically)\n
    //! `Sequence`s implement `transform` as the mapping of a function over
    //! each element of the sequence. This is somewhat equivalent to what
    //! `std::transform` does to ranges of iterators. Also note that mapping
    //! a function over an empty sequence returns an empty sequence and never
    //! applies the function, as would be expected.
    //! @snippet example/sequence.cpp Functor
    //!
    //! 4. `Applicative` (definition provided automatically)\n
    //! First, `lift`ing a value into a `Sequence` is the same as creating a
    //! singleton sequence containing that value. Second, applying a sequence
    //! of functions to a sequence of values will apply each function to
    //! all the values in the sequence, and then return a list of all the
    //! results. In other words,
    //! @code
    //!     ap([f1, ..., fN], [x1, ..., xM]) == [
    //!         f1(x1), ..., f1(xM),
    //!         ...
    //!         fN(x1), ..., fN(xM)
    //!     ]
    //! @endcode
    //! Example:
    //! @snippet example/sequence.cpp Applicative
    //!
    //! 5. `Monad` (definition provided automatically)\n
    //! First, `flaten`ning a `Sequence` takes a sequence of sequences and
    //! concatenates them to get a larger sequence. In other words,
    //! @code
    //!     flatten([[a1, ..., aN], ..., [z1, ..., zM]]) == [
    //!         a1, ..., aN, ..., z1, ..., zM
    //!     ]
    //! @endcode
    //! This acts like a `std::tuple_cat` function, except it receives a
    //! sequence of sequences instead of a variadic pack of sequences to
    //! flatten.\n
    //! __Example__:
    //! @snippet example/sequence.cpp Monad.ints
    //!
    //! Also note that the model of `Monad` for `Sequence`s can be seen as
    //! modeling nondeterminism. A nondeterministic computation can be
    //! modeled as a function which returns a sequence of possible results.
    //! In this line of thought, `bind`ing such a function to a sequence
    //! of values will return a sequence of all the possible output values,
    //! i.e. a sequence of all the values applied to all the functions in
    //! the sequences.\n
    //! __Example__:
    //! @snippet example/sequence.cpp Monad.types
    //!
    //! 6. `MonadPlus` (minimal complete definition modified)\n
    //! `Sequence`s are models of the `MonadPlus` concept by considering the
    //! empty sequence as the unit of `concat`, and sequence concatenation
    //! as the combining operation. Note that the minimal complete definition
    //! of `Sequence` includes the `prepend` and the `empty` methods, which
    //! are part of the `MonadPlus` concept. When those methods are provided,
    //! the `concat` method is automatically defined and hence it is not
    //! strictly required to implement it in order to model `MonadPlus`.
    //! @snippet example/sequence.cpp MonadPlus
    //!
    //! 7. `Foldable` (definition provided automatically)\n
    //! The model of `Foldable` for `Sequence`s is uniquely determined by the
    //! model of `Iterable`. The automatically provided model is the one
    //! provided by the `Iterable` concept.
    //! @snippet example/sequence.cpp Foldable
    //!
    //! 8. `Iterable`\n
    //! The model of `Iterable` for `Sequence`s corresponds to iteration over
    //! each element of the sequence, in order.
    //! @snippet example/sequence.cpp Iterable
    //!
    //! 9. `Searchable` (definition provided automatically)\n
    //! Searching through a `Sequence` is equivalent to just searching through
    //! a list of the values it contains. The keys and the values on which
    //! the search is performed are both the elements of the sequence.
    //! @snippet example/sequence.cpp Searchable
    //!
    //! 10. `Traversable` (definition provided automatically)\n
    //! The model of `Traversable` for `Sequence`s allows turning a sequence
    //! of `Applicative`s into an `Applicative` containing a sequence.
    //! @snippet example/sequence.cpp Traversable
    //!
    //!
    //! Provided `make`
    //! ---------------
    //! For any `Sequence` `S`, the `make<S>` method is defined
    //! automatically as
    //! @code
    //!     make<S>(x1, ..., xn) == fold.right(make<Tuple>(x1, ..., xn), prepend, empty<S>())
    //!                          == [x1, ..., xn] // of data type S
    //! @endcode
    //!
    //! While this definition is correct, it can be compile-time inefficient.
    //! Hence, implementers of new sequences are encouraged to override this
    //! default definition.
    //!
    //! [1]: http://en.wikipedia.org/wiki/Isomorphism#Isomorphism_vs._bijective_morphism
    struct Sequence {
        template <typename T, typename U> struct equal_impl;
        template <typename T, typename U> struct less_impl;

        template <typename S> struct transform_impl;
        template <typename S> struct lift_impl;
        template <typename S> struct ap_impl;
        template <typename S> struct flatten_impl;
        template <typename S> struct concat_impl;
        template <typename S> struct traverse_impl;
    };

    //! Group adjacent elements of a sequence that all respect a binary
    //! predicate, by default equality.
    //! @relates Sequence
    //!
    //! Given a Sequence and an optional predicate (by default `equal`),
    //! `group` returns a sequence of subsequences representing groups of
    //! adjacent elements that are "equal" with respect to the predicate.
    //! In other words, the groups are such that the predicate is satisfied
    //! when it is applied to any two adjacent elements in that group.
    //! The sequence returned by `group` is such that the concatenation of
    //! its elements is equal to the original sequence, which is equivalent
    //! to saying that the order of the elements is not changed.
    //!
    //! If no predicate is provided, adjacent elements in the sequence must
    //! all be compile-time `Comparable`.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Sequence `s` of data type `S(T)`, a Logical `Bool` and a
    //! predicate \f$ pred : T \times T \to Bool \f$, `group` has the
    //! following signatures. For the variant with a provided predicate,
    //! \f[
    //!     \mathrm{group} : S(T) \times (T \times T \to Bool) \to S(S(T))
    //! \f]
    //!
    //! for the variant without a custom predicate, the `T` data type is
    //! required to be Comparable. The signature is then
    //! \f[
    //!     \mathrm{group} : S(T) \to S(S(T))
    //! \f]
    //!
    //! @param xs
    //! The sequence to split into groups.
    //!
    //! @param predicate
    //! A binary function called as `predicate(x, y)`, where `x` and `y`
    //! are _adjacent_ elements in the sequence, and returning a `Logical`
    //! representing whether both elements should be in the same group
    //! (subsequence) of the result. The result returned by `predicate` has
    //! to be a `Constant Logical`. Also, `predicate` has to define an
    //! equivalence relation as defined by the `Comparable` concept.
    //! When this predicate is not provided, it defaults to `equal`.
    //!
    //!
    //! Syntactic sugar (`group.by`)
    //! ----------------------------
    //! `group` can be called in a third way, which provides a nice syntax
    //! especially when working with the `comparing` combinator:
    //! @code
    //!     group.by(predicate, xs) == group(xs, predicate)
    //!     group.by(predicate) == group(-, predicate)
    //! @endcode
    //!
    //! where `group(-, predicate)` denotes the partial application of
    //! `group` to `predicate`.
    //!
    //!
    //! Tag dispatching
    //! ---------------
    //! Both the non-predicated version and the predicated versions of `group`
    //! are tag-dispatched methods, and hence they can be customized
    //! independently. Here is how the different versions of `group`
    //! are dispatched:
    //! @code
    //!     group(xs) -> group_impl<data type of xs>::apply(xs)
    //!     group(xs, pred) -> group_pred_impl<data type of xs>::apply(xs, pred)
    //! @endcode
    //!
    //! Also note that `group.by` is not tag-dispatched on its own, since it
    //! is just syntactic sugar for calling the corresponding `group`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp group
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto group = see documentation;
#else
    template <typename S, typename = void>
    struct group_impl;

    template <typename S, typename = void>
    struct group_pred_impl;

    struct _group_by {
        template <typename Predicate, typename Xs>
        constexpr decltype(auto) operator()(Predicate&&, Xs&&) const;

        template <typename Predicate>
        constexpr decltype(auto) operator()(Predicate&&) const;
    };

    struct _group {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::group(xs) requires xs to be a Sequence");
        #endif
            return group_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }

        template <typename Xs, typename Predicate>
        constexpr decltype(auto) operator()(Xs&& xs, Predicate&& pred) const {
        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::group(xs, predicate) requires xs to be a Sequence");
        #endif
            return group_pred_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Predicate&&>(pred)
            );
        }

        static constexpr _group_by by{};
    };
    constexpr _group_by _group::by;

    constexpr _group group{};
#endif

    //! Remove the last element of a non-empty sequence.
    //! @relates Sequence
    //!
    //! If the sequence is empty, a compile-time assertion is triggered.
    //! Otherwise, a copy of the original sequence with its last element
    //! removed is returned.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp init
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto init = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct init_impl;

    struct _init {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::init(xs) requires xs to be a Sequence");
#endif
            return init_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _init init{};
#endif

    //! Insert a value between each pair of elements in a sequence.
    //! @relates Sequence
    //!
    //! Specifically, `intersperse([x1, x2, ..., xn], z)` is a sequence
    //! equivalent to `[x1, z, x2, z, x3, ..., xn-1, z, xn]`. If the sequence
    //! is empty or has a single element, then `intersperse` returns the
    //! sequence as-is.
    //!
    //!
    //! @param xs
    //! The sequence in which a value is interspersed.
    //!
    //! @param z
    //! The value to be interspersed in the sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp intersperse
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto intersperse = [](auto&& xs, auto&& z) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct intersperse_impl;

    struct _intersperse {
        template <typename Xs, typename Z>
        constexpr decltype(auto) operator()(Xs&& xs, Z&& z) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::intersperse(xs, z) requires xs to be a Sequence");
#endif
            return intersperse_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Z&&>(z)
            );
        }
    };

    constexpr _intersperse intersperse{};
#endif

    //! Partition a sequence based on a `predicate`.
    //! @relates Sequence
    //!
    //! Specifically, returns an unspecified `Product` whose first element is
    //! a sequence of the elements satisfying the predicate, and whose second
    //! element is a sequence of the elements that do not satisfy the predicate.
    //!
    //!
    //! @param xs
    //! The sequence to be partitioned.
    //!
    //! @param predicate
    //! A function called as `predicate(x)` for each element `x` in the
    //! sequence and returning a `Constant Logical`. If the result of
    //! `predicate` is true, then `x` is added to the sequence in the first
    //! component of the resulting `Product`. Otherwise, `x` is added to the
    //! sequence in the second component.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp partition
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto partition = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct partition_impl;

    struct _partition {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::partition(xs, pred) requires xs to be a Sequence");
#endif
            return partition_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
        }
    };

    constexpr _partition partition{};
#endif

    //! Return a sequence of all the permutations of the given sequence.
    //! @relates Sequence
    //!
    //! Specifically, `permutations(xs)` is a sequence whose elements are
    //! permutations of the original sequence `xs`. The permutations are not
    //! guaranteed to be in any specific order. Also note that the number
    //! of permutations grows very rapidly as the length of the original
    //! sequence increases. The growth rate is `O(length(xs)!)`; with a
    //! sequence `xs` of length only 8, `permutations(xs)` contains over
    //! 40 000 elements!
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp permutations
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/permutations.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto permutations = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct permutations_impl;

    struct _permutations {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::permutations(xs) requires xs to be a Sequence");
#endif
            return permutations_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _permutations permutations{};
#endif

    //! Remove the element at a given index from a sequence.
    //! @relates Sequence
    //!
    //! `remove_at` returns a new sequence identical to the original, except
    //! that the element at the given index is removed. Specifically,
    //! `remove_at(n, [x0, ..., xn-1, xn, xn+1, ..., xm])` is a new
    //! sequence equivalent to `[x0, ..., xn-1, xn+1, ..., xm]`.
    //!
    //!
    //! @param n
    //! An non-negative `Constant` of an unsigned integral type representing
    //! the index of the element to be removed from the sequence.
    //!
    //! @param xs
    //! A sequence from which an element is to be removed.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp remove_at
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/remove_at.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto remove_at = [](auto&& n, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct remove_at_impl;

    struct _remove_at {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::remove_at(n, xs) requires xs to be a Sequence");
#endif
            return remove_at_impl<typename datatype<Xs>::type>::apply(
                static_cast<N&&>(n),
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _remove_at remove_at{};
#endif

    //! Equivalent to `remove_at`; provided for convenience.
    //! @relates Sequence
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp remove_at_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto remove_at_c = [](auto&& xs) -> decltype(auto) {
        return remove_at(size_t<n>, forwarded(xs));
    };
#else
    template <detail::std::size_t n>
    struct _remove_at_c;

    template <detail::std::size_t n>
    constexpr _remove_at_c<n> remove_at_c{};
#endif

    //! Reverse a sequence.
    //! @relates Sequence
    //!
    //! Specifically, `reverse(xs)` is a new sequence containing the same
    //! elements as `xs`, except in reverse order.
    //!
    //!
    //! @param xs
    //! The sequence to reverse.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp reverse
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto reverse = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct reverse_impl;

    struct _reverse {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::reverse(xs) requires xs to be a Sequence");
#endif
            return reverse_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _reverse reverse{};
#endif

    //! Fold a Sequence and return a list containing the successive reduction
    //! states.
    //! @relates Sequence
    //!
    //! Like `fold`, `scan` reduces a sequence to a single value. However,
    //! unlike `fold`, it builds up a sequence of the intermediary results
    //! computed along the way and returns that instead. In the same way as
    //! `fold` comes with several variants (left/right folds, with/without
    //! an initial state), `scan` offers the same 4 flavors. This method
    //! may be used to access all of these variants by using the different
    //! syntaxes documented below. Here is an overview:
    //! @code
    //!     scan.left(xs, state, f) = see below
    //!     scan.left(xs, f) = see below
    //!     scan = scan.left
    //!
    //!     scan.right(xs, state, f) = see below
    //!     scan.right(xs, f) = see below
    //! @endcode
    //!
    //! In the above, `xs` is the sequence to be folded, `state` is the
    //! optional initial accumulation state that can be provided, and
    //! `f` is a binary operation used to reduce the sequence.
    //!
    //! When the sequence is empty, two things may arise. If an initial state
    //! was provided, a singleton list containing that state is returned.
    //! Otherwise, if no initial state was provided, an empty list is
    //! returned. In particular, unlike for `fold`, using `scan` on an
    //! empty sequence without an initial state is not an error.
    //!
    //!
    //! ## Left scans (`scan.left`)
    //! `scan.left` is a left associative scan of a sequence. Specifically,
    //! the `i`th element of `scan.left([x1, ..., xn], state, f)` is
    //! equivalent to `fold.left([x1, ..., xi], state, f)`, with the
    //! no-state variant handled analogously. For example, consider this
    //! left fold on a short sequence:
    //! @code
    //!     fold.left([x1, x2, x3], state, f) == f(f(f(state, x1), x2), x3)
    //! @endcode
    //!
    //! The analogous sequence generated with `scan.left` will be
    //! @code
    //!     scan.left([x1, x2, x3], state, f) == [
    //!         state,
    //!         f(state, x1),
    //!         f(f(state, x1), x2),
    //!         f(f(f(state, x1), x2), x3)
    //!     ]
    //! @endcode
    //!
    //! Similarly, consider this left fold (without an initial state) on
    //! a short sequence:
    //! @code
    //!     fold.left([x1, x2, x3, x4], f) == f(f(f(x1, x2), x3), x4)
    //! @endcode
    //!
    //! The analogous sequence generated with `scan.left` will be
    //! @code
    //!     scan.left([x1, x2, x3, x4], f) == [
    //!         x1,
    //!         f(x1, x2),
    //!         f(f(x1, x2), x3),
    //!         f(f(f(x1, x2), x3), x4)
    //!     ]
    //! @endcode
    //!
    //! @param xs
    //! The sequence to fold from the left.
    //!
    //! @param state
    //! The initial value used for folding.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the
    //! result accumulated so far and `x` is an element in the sequence.
    //! If no initial state is provided, `f` is called as `f(x1, x2)`,
    //! where `x1` and `x2` are both elements of the sequence.
    //!
    //! @note
    //! `scan` is equivalent to `scan.left`, so `scan(xs, f)` and
    //! `scan(xs, state, f)` are equivalent to `scan.left(xs, f)` and
    //! `scan.left(xs, state, f)` respectively. This is provided solely
    //! for convenience.
    //!
    //! ### Example
    //! @snippet example/sequence.cpp scan.left
    //!
    //!
    //! ## Right scans (`scan.right`)
    //! `scan.right` is a right associative scan of a sequence. Specifically,
    //! the `i`th element of `scan.right([x1, ..., xn], state, f)` is
    //! equivalent to `fold.right([xi, ..., xn], state, f)`, with the
    //! no-state variant handled analogously. For example, consider this
    //! right fold on a short sequence:
    //! @code
    //!     fold.right([x1, x2, x3], state, f) == f(x1, f(x2, f(x3, state)))
    //! @endcode
    //!
    //! The analogous sequence generated with `scan.right` will be
    //! @code
    //!     scan.right([x1, x2, x3], state, f) == [
    //!         f(x1, f(x2, f(x3, state))),
    //!               f(x2, f(x3, state)),
    //!                     f(x3, state),
    //!                           state
    //!     ]
    //! @endcode
    //!
    //! Similarly, consider this right fold (without an initial state) on
    //! a short sequence:
    //! @code
    //!     fold.right([x1, x2, x3, x4], f) == f(x1, f(x2, f(x3, x4)))
    //! @endcode
    //!
    //! The analogous sequence generated with `scan.left` will be
    //! @code
    //!     scan.right([x1, x2, x3, x4], f) == [
    //!         f(x1, f(x2, f(x3, x4))),
    //!               f(x2, f(x3, x4)),
    //!                     f(x3, x4),
    //!                           x4
    //!     ]
    //! @endcode
    //!
    //! @param xs
    //! The sequence to fold from the right.
    //!
    //! @param state
    //! The initial value used for folding.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the
    //! result accumulated so far and `x` is an element in the sequence.
    //! When no initial state is provided, `f` is called as `f(x1, x2)`,
    //! where `x1` and `x2` are elements of the sequence.
    //!
    //! ### Example
    //! @snippet example/sequence.cpp scan.right
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto scan = see documentation;
#else
    template <typename Xs, typename = void>
    struct scan_left_impl;

    template <typename Xs, typename = void>
    struct scan_left_nostate_impl;

    template <typename Xs, typename = void>
    struct scan_right_impl;

    template <typename Xs, typename = void>
    struct scan_right_nostate_impl;


    struct _scan_left {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::scan.left(xs, state, f) requires xs to be a Sequence");
        #endif
            return scan_left_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<State&&>(state),
                static_cast<F&&>(f)
            );
        }

        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::scan.left(xs, f) requires xs to be a Sequence");
        #endif
            return scan_left_nostate_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<F&&>(f)
            );
        }
    };

    struct _scan_right {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::scan.right(xs, state, f) requires xs to be a Sequence");
        #endif
            return scan_right_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<State&&>(state),
                static_cast<F&&>(f)
            );
        }

        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::scan.right(xs, f) requires xs to be a Sequence");
        #endif
            return scan_right_nostate_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<F&&>(f)
            );
        }
    };

    struct _scan : _scan_left {
        static constexpr _scan_left left{};
        static constexpr _scan_right right{};
    };
    constexpr _scan_left _scan::left;
    constexpr _scan_right _scan::right;

    constexpr _scan scan{};
#endif

    //! Extract a subsequence delimited by the given indices.
    //! @relates Sequence
    //!
    //! Specifically, `slice(xs, from, to)` is a sequence containing all the
    //! elements of `xs` at indices in the half-open interval delimited by
    //! [`from`, `to`). Note that the indices are 0-based. For this operation
    //! to be valid, `xs` must contain at least `to + 1` elements, and it must
    //! be true that `from <= to`.
    //!
    //!
    //! @param xs
    //! The sequence to slice.
    //!
    //! @param from
    //! The index of the first element in the slice. `from` must be a
    //! non-negative `Constant` of an unsigned integral type.
    //!
    //! @param to
    //! One-past the index of the last element in the slice. `to` must be
    //! a non-negative `Constant` of an unsigned integral type such that
    //! `from <= to`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp slice
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto slice = [](auto&& xs, auto&& from, auto&& to) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct slice_impl;

    struct _slice {
        template <typename Xs, typename From, typename To>
        constexpr decltype(auto) operator()(Xs&& xs, From&& from, To&& to) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::slice(xs, from, to) requires xs to be a Sequence");
#endif
            return slice_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<From&&>(from),
                static_cast<To&&>(to)
            );
        }
    };

    constexpr _slice slice{};
#endif

    //! Equivalent to `slice`; provided for convenience.
    //! @relates Sequence
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp slice_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t from, std::size_t to>
    constexpr auto slice_c = [](auto&& xs) -> decltype(auto) {
        return slice(forwarded(xs), size_t<from>, size_t<to>);
    };
#else
    template <detail::std::size_t from, detail::std::size_t to>
    struct _slice_c;

    template <detail::std::size_t from, detail::std::size_t to>
    constexpr _slice_c<from, to> slice_c{};
#endif

    //! Sort a sequence, optionally based on a custom `predicate`.
    //! @relates Sequence
    //!
    //! Given a Sequence and an optional predicate (by default `less`), `sort`
    //! returns a new sequence containing the same elements as the original,
    //! except they are ordered in such a way that if `x` comes before `y` in
    //! the sequence, then either `predicate(x, y)` is true, or both
    //! `predicate(x, y)` and `predicate(y, x)` are false.
    //!
    //! Also note that the sort is guaranteed to be stable. Hence, if `x`
    //! comes before `y` in the original sequence and both `predicate(x, y)`
    //! and `predicate(y, x)` are false, then `x` will come before `y` in the
    //! resulting sequence.
    //!
    //! If no predicate is provided, the elements in the sequence must all be
    //! compile-time `Orderable`.
    //!
    //! Signature
    //! ---------
    //! Given a Sequence `s` of data type `S(T)`, a Logical `Bool` and a
    //! predicate \f$ pred : T \times T \to Bool \f$, `sort` has the following
    //! signatures. For the variant with a provided predicate,
    //! \f[
    //!     \mathrm{sort} : S(T) \times (T \times T \to Bool) \to S(T)
    //! \f]
    //!
    //! for the variant without a custom predicate, the `T` data type is
    //! required to be Orderable. The signature is then
    //! \f[
    //!     \mathrm{sort} : S(T) \to S(T)
    //! \f]
    //!
    //! @param xs
    //! The sequence to sort.
    //!
    //! @param predicate
    //! A function called as `predicate(x, y)` for two elements `x` and `y` of
    //! the sequence, and returning a `Logical` representing whether `x` is to
    //! be considered _less_ than `y`, i.e. whether `x` should appear _before_
    //! `y` in the resulting sequence. More specifically, `predicate` must
    //! define a [strict weak ordering][1] on the elements of the sequence.
    //! In the current version of the library, also note that `predicate` must
    //! return a `Constant Logical` when called with any two elements of the
    //! sequence. When the predicate is not specified, this defaults to `less`.
    //!
    //!
    //! Syntactic sugar (`sort.by`)
    //! ---------------------------
    //! `sort` can be called in a third way, which provides a nice syntax
    //! especially when working with the `ordering` combinator:
    //! @code
    //!     sort.by(predicate, xs) == sort(xs, predicate)
    //!     sort.by(predicate) == sort(-, predicate)
    //! @endcode
    //!
    //! where `sort(-, predicate)` denotes the partial application of
    //! `sort` to `predicate`.
    //!
    //!
    //! Tag dispatching
    //! ---------------
    //! Both the non-predicated version and the predicated versions of `sort`
    //! are tag-dispatched methods, and hence they can be customized
    //! independently. One reason for this is that some structures are
    //! able to provide a much more efficient implementation of `sort`
    //! when the `less` predicate is used. Here is how the different
    //! versions of `sort` are dispatched:
    //! @code
    //!     sort(xs) -> sort_impl<data type of xs>::apply(xs)
    //!     sort(xs, pred) -> sort_pred_impl<data type of xs>::apply(xs, pred)
    //! @endcode
    //!
    //! Also note that `sort.by` is not tag-dispatched on its own, since it
    //! is just syntactic sugar for calling the corresponding `sort`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp sort
    //!
    //! [1]: http://en.wikipedia.org/wiki/Strict_weak_ordering
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto sort = see documentation;
#else
    template <typename S, typename = void>
    struct sort_impl;

    template <typename S, typename = void>
    struct sort_pred_impl;

    struct _sort_by {
        template <typename Predicate, typename Xs>
        constexpr decltype(auto) operator()(Predicate&&, Xs&&) const;

        template <typename Predicate>
        constexpr decltype(auto) operator()(Predicate&&) const;
    };

    struct _sort {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::sort(xs) requires xs to be a Sequence");
        #endif
            return sort_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }

        template <typename Xs, typename Predicate>
        constexpr decltype(auto) operator()(Xs&& xs, Predicate&& pred) const {
        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::sort(xs, predicate) requires xs to be a Sequence");
        #endif
            return sort_pred_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Predicate&&>(pred)
            );
        }

        static constexpr _sort_by by{};
    };
    constexpr _sort_by _sort::by;

    constexpr _sort sort{};
#endif

    //! Returns a `Product` containing the longest prefix of a sequence
    //! satisfying a predicate, and the rest of the sequence.
    //! @relates Sequence
    //!
    //! The first component of the returned `Product` is a sequence for which
    //! all elements satisfy the given predicate. The second component of the
    //! returned `Product` is a sequence containing the remainder of the
    //! argument. Both or either sequences may be empty, depending on the
    //! input argument. More specifically,
    //! @code
    //!     span(xs, predicate) == make_pair(take_while(xs, predicate),
    //!                                      drop_while(xs, predicate))
    //! @endcode
    //! except that `make_pair` may be an arbitrary `Product`.
    //!
    //!
    //! @param xs
    //! The sequence to break into two parts.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! sequence, and returning a `Logical. In the current implementation of
    //! the library, `predicate` has to return a `Constant Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp span
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto span = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct span_impl;

    struct _span {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::span(xs, pred) requires xs to be a Sequence");
#endif
            return span_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
        }
    };

    constexpr _span span{};
#endif

    //! Returns the elements at the given indices of a sequence.
    //! @relates Sequence
    //!
    //! Given a (finite) and compile-time `Foldable` containing indices,
    //! `subsequence` returns a new `Sequence` of the elements of the
    //! original sequence that appear at those indices. The indices must
    //! be `Constant`s of an unsigned integral type. In other words,
    //! @code
    //!     subsequence([x1, ..., xn], [i1, ..., ik]) == [xi1, ..., xik]
    //! @endcode
    //!
    //! In particular, note that indices do not have to be ordered or
    //! sequential in any particular way, and they may contain duplicates.
    //!
    //!
    //! @param xs
    //! The sequence from which a subsequence is extracted.
    //!
    //! @param indices
    //! A compile-time `Foldable` holding `Constant`s of an unsigned integral
    //! type, and whose linearization represents the indices of the elements
    //! in the returned sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp subsequence
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto subsequence = [](auto&& xs, auto&& indices) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct subsequence_impl;

    struct _subsequence {
        template <typename Xs, typename Indices>
        constexpr decltype(auto) operator()(Xs&& xs, Indices&& indices) const {
            using S = typename datatype<Xs>::type;
            using Subsequence = subsequence_impl<S>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, S>{},
            "hana::subsequence(xs, indices) requires xs to be a Sequence");

            static_assert(_models<Foldable, typename datatype<Indices>::type>{},
            "hana::subsequence(xs, indices) requires indices to be Foldable");
        #endif

            return Subsequence::apply(static_cast<Xs&&>(xs),
                                      static_cast<Indices&&>(indices));
        }
    };

    static constexpr _subsequence subsequence{};
#endif

    //! Returns the first `n` elements of a sequence.
    //! @relates Sequence
    //!
    //! Broadly speaking, `take(n, xs)` is a new sequence containing the first
    //! `n` elements of `xs`, in the same order. `n` must be a Constant of an
    //! unsigned integral type. However, there are different ways of calling
    //! `take`, which correspond to different policies in case the length of
    //! the sequence is less than `n`:
    //! @code
    //!     take(n, xs)         = take.at_most(n, xs)
    //!     take.at_most(n, xs) = see below
    //!     take.exactly(n, xs) = see below
    //! @endcode
    //!
    //! In case `length(xs) < n`, the `take.at_most` variant will simply take
    //! the whole sequence, without failing. In contrast, the `take.exactly`
    //! variant assumes that `length(xs) >= n`, which makes it possible to
    //! perform some optimizations.
    //!
    //! All of the different variants are tag-dispatched methods that
    //! can be overriden. Here is how each variant is tag-dispatched:
    //! @code
    //!     take.at_most       ->  take_at_most_impl
    //!     take.exactly       ->  take_exactly_impl
    //! @endcode
    //! `take` is not tag dispatched, because it is just an alias to
    //! `take.at_most`.
    //!
    //!
    //! @param n
    //! A non-negative `Constant` of an unsigned integral type representing
    //! the number of elements to keep in the resulting sequence. If
    //! `length(xs) < n`, the exact behavior is determined by the chosen
    //! policy (either `take.at_most` or `take.exactly`).
    //!
    //! @param xs
    //! The sequence to take the elements from.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp take
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/take.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take = see documentation;
#else
    template <typename S, typename = void>
    struct take_exactly_impl;
    struct _take_exactly {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::take.exactly(n, xs) requires xs to be a Sequence");
#endif
            return take_exactly_impl<typename datatype<Xs>::type>::apply(
                static_cast<N&&>(n),
                static_cast<Xs&&>(xs)
            );
        }
    };

    template <typename S, typename = void>
    struct take_at_most_impl;
    struct _take_at_most {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::take.at_most(n, xs) requires xs to be a Sequence");
#endif
            return take_at_most_impl<typename datatype<Xs>::type>::apply(
                static_cast<N&&>(n),
                static_cast<Xs&&>(xs)
            );
        }
    };

    struct _take : _take_at_most {
        static constexpr _take_exactly exactly{};
        static constexpr _take_at_most at_most{};
    };
    constexpr _take_exactly _take::exactly;
    constexpr _take_at_most _take::at_most;
    constexpr _take take{};
#endif

    //! Equivalent to `take`; provided for convenience.
    //! @relates Sequence
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp take_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto take_c = [](auto&& xs) -> decltype(auto) {
        return take(size_t<n>, forwarded(xs));
    };
#else
    template <detail::std::size_t n>
    struct _take_c;

    template <detail::std::size_t n>
    constexpr _take_c<n> take_c{};
#endif

    //! Take elements from a sequence until the `predicate` is satisfied.
    //! @relates Sequence
    //!
    //! Specifically, `take_until` returns a new sequence containing the
    //! longest prefix of `xs` in which all elements do not satisfy the
    //! predicate. This is effectively equivalent to `take_while` with a
    //! negated predicate.
    //!
    //!
    //! @param xs
    //! The sequence to take the elements from.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! sequence, and returning a `Logical` representing whether the resulting
    //! sequence should stop at the element before `x`. In the current version
    //! of the library, `predicate` has to return a `Constant Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp take_until
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/take_until.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take_until = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct take_until_impl;

    struct _take_until {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::take_until(xs, pred) requires xs to be a Sequence");
#endif
            return take_until_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
        }
    };

    constexpr _take_until take_until{};
#endif

    //! Take elements from a sequence while the `predicate` is satisfied.
    //! @relates Sequence
    //!
    //! Specifically, `take_while` returns a new sequence containing the
    //! longest prefix of `xs` in which all the elements satisfy the given
    //! predicate.
    //!
    //!
    //! @param xs
    //! The sequence to take elements from.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! sequence, and returning a `Logical` representing whether `x` should be
    //! included in the resulting sequence. In the current version of the
    //! library, `predicate` has to return a `Constant Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp take_while
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/take_while.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take_while = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct take_while_impl;

    struct _take_while {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::take_while(xs, pred) requires xs to be a Sequence");
#endif
            return take_while_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
        }
    };

    constexpr _take_while take_while{};
#endif

    //! Dual operation to `fold` for sequences.
    //! @relates Sequence
    //!
    //! While `fold` reduces a structure to a summary value, `unfold` builds
    //! a sequence from a seed value and a function. As explained in the
    //! documentation for `fold`, there are several different flavors of
    //! folds. In particular, there are folds which are left associative
    //! and folds which are right associative. Similarly, there are two
    //! versions of `unfold`; one which builds the sequence from the left,
    //! and another one which builds the sequence from the right. Those
    //! two variants are accessible through `unfold<S>.left` and
    //! `unfold<S>.right`, respectively. Also note that for convenience,
    //! `unfold<S>` is an alias to `unfold<S>.left`.
    //!
    //!
    //! ### Fun fact
    //! In some cases, `unfold` can undo a `fold` operation:
    //! @code
    //!     unfold<S>.left(fold.left(xs, z, f), g) == xs
    //!     unfold<S>.right(fold.right(xs, z, f), g) == xs
    //! @endcode
    //!
    //! if the following holds
    //! @code
    //!     g(f(x, y)) == just(make_pair(x, y))
    //!     g(z) == nothing
    //! @endcode
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Sequence data type `S`, an initial value `init` of data type
    //! `I`, an arbitrary Product `P` and a function \f$ f : I \to P(I, T) \f$,
    //! `unfold<S>.left` has the following signature:
    //! \f[
    //!     \mathrm{unfold}_S.\mathrm{left} : I \times (I \to P(I, T)) \to S(T)
    //! \f]
    //!
    //! Given a function \f$ f : I \to P(T, I) \f$ instead, `unfold<S>.right`
    //! has the following signature:
    //! \f[
    //!     \mathrm{unfold}_S.\mathrm{right} : I \times (I \to P(T, I)) \to S(T)
    //! \f]
    //!
    //! @tparam S
    //! The data type of the sequence to build up.
    //!
    //! @param init
    //! An initial value to build the sequence from.
    //!
    //! @param f
    //! A function called as `f(init)`, where `init` is an initial value,
    //! and returning
    //! 1. `nothing` if it is done producing the sequence.
    //! 2. otherwise, `just(make_pair(init, x))` for `unfold.left` and
    //!    `just(make_pair(x, init))` for `unfold.right`, where `init` is the
    //!    new initial value used in the next call to `f` and `x` is an
    //!    element to be appended to the resulting sequence. Also note that
    //!    `make_pair` may actually be replaced by any `Product`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp unfold
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename S>
    constexpr auto unfold = see documentation;
#else
    template <typename S, typename = void>
    struct unfold_left_impl;

    template <typename S, typename = void>
    struct unfold_right_impl;

    template <typename S>
    struct _unfold_left {
        template <typename Initial, typename F>
        constexpr decltype(auto) operator()(Initial&& initial, F&& f) const {
            return unfold_left_impl<S>::apply(
                static_cast<Initial&&>(initial),
                static_cast<F&&>(f)
            );
        }
    };

    template <typename S>
    struct _unfold_right {
        template <typename Initial, typename F>
        constexpr decltype(auto) operator()(Initial&& initial, F&& f) const {
            return unfold_right_impl<S>::apply(
                static_cast<Initial&&>(initial),
                static_cast<F&&>(f)
            );
        }
    };

    template <typename S>
    struct _unfold : _unfold_left<S> {
    #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(_models<Sequence, S>{},
        "hana::unfold<S> requires S to be a Sequence");
    #endif

        static constexpr _unfold_left<S> left{};
        static constexpr _unfold_right<S> right{};
    };
    template <typename S>
    constexpr _unfold_left<S> _unfold<S>::left;
    template <typename S>
    constexpr _unfold_right<S> _unfold<S>::right;

    template <typename S>
    constexpr _unfold<S> unfold{};
#endif

    //! Unzip a sequence of sequences.
    //! @relates Sequence
    //!
    //! `unzip` can undo a `zip` operation. Specifically, it takes a sequence
    //! of the form
    //! @code
    //!     [s1, s2, ..., sn]
    //! @endcode
    //! where each `si` is a sequence, and returns a sequence equivalent to
    //! `zip(s1, s2, ..., sn)`.
    //!
    //!
    //! @param xs
    //! A sequence of sequences to unzip.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp unzip
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto unzip = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct unzip_impl;

    struct _unzip {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Sequence, typename datatype<Xs>::type>{},
            "hana::unzip(xs) requires xs to be a Sequence");
#endif
            return unzip_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    constexpr _unzip unzip{};
#endif

    //! Zip one sequence or more, either with a given function or into a Tuple.
    //! @relates Sequence
    //!
    //! In a general setting, zipping several sequences with a function refers
    //! to the following operation. Given `n` sequences `s1, ..., sn` and a
    //! function `f` that takes `n` arguments, zipping produces a sequence
    //! whose i-th element is `f(s1[i], ..., sn[i])`, where `sk[i]` denotes
    //! the i-th element of the k-th sequence passed as an argument. In other
    //! words, it produces a sequence of the form
    //! @code
    //!     [
    //!         f(s1[0], ..., sn[0]),
    //!         f(s1[1], ..., sn[1]),
    //!         ...
    //!         f(s1[M], ..., sn[M])
    //!     ]
    //! @endcode
    //! , where `M` is usually the length of the shortest sequence. As this
    //! suggests, there are several different ways of zipping that one might
    //! want; zip with a function, zip without a function, zip up to the
    //! shortest sequence, zip up to the longest sequence or assume all the
    //! sequences are of the same size. In Hana, all these different ways of
    //! zipping are provided, except for the "up to the longest sequence" one.
    //!
    //! > #### Rationale for not providing a `zip.longest` variant
    //! > It would require either (1) padding the shortest sequences with
    //! > an arbitrary object, or (2) pad the shortest sequences with an
    //! > object provided by the user when calling `zip.longest`. Since there
    //! > is no requirement that all the zipped sequences have elements of
    //! > similar types, there is no way to provide a single consistent
    //! > padding object in all cases. A tuple of padding objects should
    //! > be provided, but I find it perhaps too complicated to be worth
    //! > it for now. If you need this functionality, open a GitHub issue.
    //!
    //!
    //! The `zip` method is actually a function object that can be called in
    //! several different ways, each of them providing a slightly different
    //! kind of zipping. Here are the different ways of calling `zip`:
    //! @code
    //!     zip(s1, ..., sn)         = zip.shortest(s1, ..., sn)
    //!     zip.with(f, s1, ..., sn) = zip.shortest.with(f, s1, ..., sn)
    //!
    //!     zip.shortest(s1, ..., sn)         = see below
    //!     zip.shortest.with(f, s1, ..., sn) = see below
    //!
    //!     zip.unsafe(s1, ..., sn)         = see below
    //!     zip.unsafe.with(f, s1, ..., sn) = see below
    //! @endcode
    //!
    //! First, one sees that calling `zip` or `zip.with` is equivalent to
    //! calling `zip.shortest` or `zip.shortest.with`, respectively. The
    //! `shortest` variant signifies that the returned sequence should stop
    //! when the shortest input sequence is exhausted, which is the usual
    //! behavior for `zip` operations.
    //!
    //! Then, there are also the `zip.unsafe` and `zip.unsafe.with` variants,
    //! which both assume that all the sequences are of the same size. This
    //! allows the library to perform some optimizations. If you know that
    //! all the sequences you are about to zip are of the same length, you
    //! should use these variants.
    //!
    //! Finally, the `with` variants offer the possibility of providing a
    //! custom function to do the zipping, as was explained above. The
    //! non-`with` variants, which do not accept a custom function, will
    //! zip using a tuple. In other words,
    //! @code
    //!     zip.*(s1, ..., sn) == zip.*.with(make<Tuple>, s1, ..., sn)
    //!                        == [
    //!                             make<Tuple>(s1[0], ..., sn[0]),
    //!                             make<Tuple>(s1[1], ..., sn[1]),
    //!                             ...
    //!                             make<Tuple>(s1[M], ..., sn[M])
    //!                        ]
    //! @endcode
    //!
    //! All of the different zipping variants are tag-dispatched methods that
    //! can be overridden. Here is how each variant is tag-dispatched:
    //! @code
    //!     zip.shortest       ->  zip_shortest_impl
    //!     zip.shortest.with  ->  zip_shortest_with_impl
    //!     zip.unsafe         ->  zip_unsafe_impl
    //!     zip.unsafe.with    ->  zip_unsafe_with_impl
    //! @endcode
    //! `zip` and `zip.with` are not tag dispatched, because they are
    //! just aliases to `zip.shortest` and `zip.shortest.with`, respectively.
    //! Also note that all the sequences must have the same data type, and
    //! only the data type of the first one is used for tag-dispatching.
    //!
    //! @note
    //! At least one sequence must be provided. Otherwise, it is an error.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp zip
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/sequence/zip_with.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto zip = see documentation;
#else
    template <typename S, typename = void>
    struct zip_unsafe_with_impl;
    struct _zip_unsafe_with {
        template <typename F, typename Xs, typename ...Ys>
        constexpr decltype(auto) operator()(F&& f, Xs&& xs, Ys&& ...ys) const;
    };


    template <typename S, typename = void>
    struct zip_unsafe_impl;
    struct _zip_unsafe {
        static constexpr _zip_unsafe_with with{};

        template <typename Xs, typename ...Ys>
        constexpr decltype(auto) operator()(Xs&& xs, Ys&& ...ys) const;
    };
    constexpr _zip_unsafe_with _zip_unsafe::with;


    template <typename S, typename = void>
    struct zip_shortest_with_impl;
    struct _zip_shortest_with {
        template <typename F, typename Xs, typename ...Ys>
        constexpr decltype(auto) operator()(F&& f, Xs&& xs, Ys&& ...ys) const;
    };


    template <typename S, typename = void>
    struct zip_shortest_impl;
    struct _zip_shortest {
        static constexpr _zip_shortest_with with{};
        template <typename Xs, typename ...Ys>
        constexpr decltype(auto) operator()(Xs&& xs, Ys&& ...ys) const;
    };
    constexpr _zip_shortest_with _zip_shortest::with;


    struct _zip : _zip_shortest {
        static constexpr _zip_shortest shortest{};
        static constexpr _zip_unsafe unsafe{};
    };
    constexpr _zip_shortest _zip::shortest;
    constexpr _zip_unsafe _zip::unsafe;

    constexpr _zip zip{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SEQUENCE_HPP

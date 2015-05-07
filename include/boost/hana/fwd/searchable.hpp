/*!
@file
Forward declares `boost::hana::Searchable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_SEARCHABLE_HPP
#define BOOST_HANA_FWD_SEARCHABLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/infix.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/default.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/fwd/core/operators.hpp>


namespace boost { namespace hana {
    namespace operators {
        template <typename Derived>
        struct Searchable_ops;
    }

    //! @ingroup group-concepts
    //! The `Searchable` concept represents structures that can be searched.
    //!
    //! Intuitively, a `Searchable` is any structure, finite or infinite,
    //! containing elements that can be searched using a predicate. Sometimes,
    //! `Searchable`s will associate keys to values; one can search for a key
    //! with a predicate, and the value associated to it is returned. This
    //! gives rise to map-like data structures. Other times, the elements of
    //! the structure that are searched (i.e. those to which the predicate is
    //! applied) are the same that are returned, which gives rise to set-like
    //! data structures. In general, we will refer to the _keys_ of a
    //! `Searchable` structure as those elements that are used for searching,
    //! and to the _values_ of a `Searchable` as those elements that are
    //! returned when a search is successful. As was explained, there is no
    //! requirement that both notions differ, and it is often useful to have
    //! keys and values coincide (think about `std::set`).
    //!
    //! Some methods like `any_of`, `all_of` and `none_of` allow simple queries
    //! to be performed on the keys of the structure, while other methods like
    //! `find` and `find_if` make it possible to find the value associated
    //! to a key. The most specific method should always be used if one
    //! cares about performance, because it is usually the case that heavy
    //! optimizations can be performed in more specific methods. For example,
    //! an associative data structure implemented as a hash table will be much
    //! faster to access using `find` than `find_if`, because in the second
    //! case it will have to do a linear search through all the entries.
    //! Similarly, using `contains` will likely be much faster than `any_of`
    //! with an equivalent predicate.
    //!
    //! > __Insight__\n
    //! > In a lazy evaluation context, any `Foldable` can also become a model
    //! > of `Searchable` because we can search lazily through the structure
    //! > with `fold.right`. However, in the context of C++, some `Searchable`s
    //! > can not be folded; think for example of an infinite set.
    //!
    //!
    //! Laws
    //! ----
    //! In order for the semantics of the methods to be consistent, some
    //! properties must be satisfied by any model of the `Searchable` concept.
    //! Rigorously, for any `Searchable`s  `xs` and `ys` and any predicate `p`,
    //! the following laws should be satisfied:
    //! @code
    //!     any_of(xs, p) <=> !all_of(xs, negated p)
    //!                   <=> !none_of(xs, p)
    //!
    //!     contains(xs, x) <=> any_of(xs, equal.to(x))
    //!
    //!     find(xs, x) == find_if(xs, equal.to(x))
    //!     find_if(xs, always(false_)) == nothing
    //!
    //!     is_subset(xs, ys) <=> all_of(xs, [](auto x) { return contains(ys, x); })
    //! @endcode
    //!
    //! Additionally, if all the keys of the `Searchable` are `Logical`s,
    //! the following laws should be satisfied:
    //! @code
    //!     any(xs)  <=> any_of(xs, id)
    //!     all(xs)  <=> all_of(xs, id)
    //!     none(xs) <=> none_of(xs, id)
    //! @endcode
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `find_if` and `any_of`\n
    //! According to the above laws, it is possible to implement all the
    //! methods pertaining to this concept using only `find_if` and `any_of`.
    //! When `find_if` and `any_of` are provided, the other methods are
    //! implemented according to the laws above.
    //!
    //! @note
    //! We could implement `any_of(xs, pred)` as `is_just(find_if(xs, pred))`,
    //! and then reduce the minimal complete definition to `find_if`. However,
    //! this is not done because that implementation requires the predicate
    //! of `any_of` to return a compile-time `Logical`, which is more
    //! restrictive than what we have right now.
    //!
    //!
    //! Free model for builtin arrays
    //! -----------------------------
    //! Builtin arrays whose size is known can be searched as-if they were
    //! homogeneous tuples. However, since arrays can only hold objects of
    //! a single type and the predicate to `find_if` must return a compile-time
    //! `Logical`, the `find_if` method is fairly useless. For similar reasons,
    //! the `find` method is also fairly useless. This model is provided mainly
    //! because of the `any_of` method & friends, which are both useful and
    //! compile-time efficient.
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `Map`, `Maybe`, `Range`, `Set`, `String`, `Tuple`
    //!
    //!
    //! Structure preserving functions
    //! ------------------------------
    //! Given two `Searchables` `S1` and `S2`, a function
    //! @f$ f : S_1(X) \to S_2(X) @f$ is said to preserve the `Searchable`
    //! structure if for all `xs` of data type `S1(X)` and predicates
    //! @f$ pred : X \to Bool @f$ (for a `Logical` `Bool`),
    //! @code
    //!     any_of(xs, pred)  if and only if  any_of(f(xs), pred)
    //!     find_if(xs, pred) == find_if(f(xs), pred)
    //! @endcode
    //!
    //! This is really just a generalization of the following, more intuitive
    //! requirements. For all `xs` of data type `S1(X)` and `x` of data type
    //! `X`,
    //! @code
    //!     x ^in^ xs  if and only if  x ^in^ f(xs)
    //!     find(xs, x) == find(f(xs), x)
    //! @endcode
    //!
    //! These requirements can be understood as saying that `f` does not
    //! change the content of `xs`, although it may reorder elements.
    //! As usual, such a structure-preserving transformation is said to
    //! be an embedding if it is also injective, i.e. if it is a lossless
    //! transformation.
    struct Searchable { };

    //! Returns whether any key of the structure satisfies the `predicate`.
    //! @relates Searchable
    //!
    //! If the structure is not finite, `predicate` has to be satisfied
    //! after looking at a finite number of keys for this method to finish.
    //!
    //!
    //! @param xs
    //! The structure to search.
    //!
    //! @param predicate
    //! A function called as `predicate(k)`, where `k` is a key of the
    //! structure, and returning a `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp any_of
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto any_of = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct any_of_impl;

    struct _any_of {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            using S = typename datatype<Xs>::type;
            using AnyOf = any_of_impl<S>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, S>{},
            "hana::any_of(xs, pred) requires xs to be a Searchable");
        #endif

            return AnyOf::apply(static_cast<Xs&&>(xs), static_cast<Pred&&>(pred));
        }
    };

    constexpr _any_of any_of{};
#endif

    //! Returns whether any key of the structure is true-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a true-valued key must appear at a finite "index" in order for
    //! this method to finish.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp any
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto any = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct any_impl;

    struct _any {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using S = typename datatype<Xs>::type;
            using Any = any_impl<S>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, S>{},
            "hana::any(xs) requires xs to be a Searchable");
        #endif

            return Any::apply(static_cast<Xs&&>(xs));
        }
    };

    constexpr _any any{};
#endif

    //! Returns whether all the keys of the structure satisfy the `predicate`.
    //! @relates Searchable
    //!
    //! If the structure is not finite, `predicate` has to return a false-
    //! valued `Logical` after looking at a finite number of keys for this
    //! method to finish.
    //!
    //!
    //! @param xs
    //! The structure to search.
    //!
    //! @param predicate
    //! A function called as `predicate(k)`, where `k` is a key of the
    //! structure, and returning a `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp all_of
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto all_of = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct all_of_impl;

    struct _all_of {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            using S = typename datatype<Xs>::type;
            using AllOf = all_of_impl<S>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, S>{},
            "hana::all_of(xs, pred) requires xs to be a Searchable");
        #endif

            return AllOf::apply(static_cast<Xs&&>(xs), static_cast<Pred&&>(pred));
        }
    };

    constexpr _all_of all_of{};
#endif

    //! Returns whether all the keys of the structure are true-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a false-valued key must appear at a finite "index" in order
    //! for this method to finish.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp all
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto all = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct all_impl;

    struct _all {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using S = typename datatype<Xs>::type;
            using All = all_impl<S>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, S>{},
            "hana::all(xs) requires xs to be a Searchable");
        #endif

            return All::apply(static_cast<Xs&&>(xs));
        }
    };

    constexpr _all all{};
#endif

    //! Returns whether none of the keys of the structure satisfy the
    //! `predicate`.
    //! @relates Searchable
    //!
    //! If the structure is not finite, `predicate` has to return a true-
    //! valued `Logical` after looking at a finite number of keys for this
    //! method to finish.
    //!
    //!
    //! @param xs
    //! The structure to search.
    //!
    //! @param predicate
    //! A function called as `predicate(k)`, where `k` is a key of the
    //! structure, and returning a `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp none_of
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto none_of = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct none_of_impl;

    struct _none_of {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            using S = typename datatype<Xs>::type;
            using NoneOf = none_of_impl<S>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, S>{},
            "hana::none_of(xs, pred) requires xs to be a Searchable");
        #endif

            return NoneOf::apply(static_cast<Xs&&>(xs), static_cast<Pred&&>(pred));
        }
    };

    constexpr _none_of none_of{};
#endif

    //! Returns whether all of the keys of the structure are false-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a true-valued key must appear at a finite "index" in order
    //! for this method to finish.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp none
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto none = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct none_impl;

    struct _none {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using S = typename datatype<Xs>::type;
            using None = none_impl<S>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, S>{},
            "hana::none(xs) requires xs to be a Searchable");
        #endif

            return None::apply(static_cast<Xs&&>(xs));
        }
    };

    constexpr _none none{};
#endif

    //! Returns whether the key occurs in the structure.
    //! @relates Searchable
    //!
    //! Specifically, returns whether any of the keys of the structure is
    //! equal to the given `key`. If the structure is not finite, an equal
    //! key has to appear at a finite "index" in the structure for this
    //! method to finish.
    //!
    //! @note
    //! For convenience, `contains` can also be applied in infix notation.
    //! @snippet example/searchable.cpp contains.infix
    //!
    //!
    //! @param xs
    //! The structure to search.
    //!
    //! @param key
    //! A key to be searched for in the structure. The key has to be
    //! `Comparable` with the other keys of the structure.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp contains
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto contains = [](auto&& xs, auto&& key) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct contains_impl;

    struct _contains {
        template <typename Xs, typename Key>
        constexpr decltype(auto) operator()(Xs&& xs, Key&& key) const {
            using S = typename datatype<Xs>::type;
            using Contains = contains_impl<S>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, S>{}(),
            "hana::contains(xs, key) requires xs to be a Searchable");
        #endif

            return Contains::apply(static_cast<Xs&&>(xs),
                                   static_cast<Key&&>(key));
        }
    };

    constexpr auto contains = infix(_contains{});
#endif

    //! Return whether the key occurs in the structure.
    //! @relates Searchable
    //!
    //! Specifically, this is equivalent to `contains`, except `in` takes its
    //! arguments in reverse order. Like `contains`, `in` can also be applied
    //! in infix notation for increased expressiveness. This function is not a
    //! method that can be overriden; it is just a convenience function
    //! provided with the concept.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp in
    constexpr auto in = infix(flip(contains));

    //! Finds the value associated to the first key satisfying a predicate.
    //! @relates Searchable
    //!
    //! Specifically, returns `just` the first value whose key satisfies the
    //! `predicate`, or `nothing` if there is no such key.
    //!
    //!
    //! @param xs
    //! The structure to be searched.
    //!
    //! @param predicate
    //! A function called as `predicate(k)`, where `k` is a key of the
    //! structure, and returning a `Logical`. Note that in the current
    //! version of the library, the `predicate` has to return a compile-time
    //! `Logical` because `find_if` returns either a `just(...)` or `nothing`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp find_if
    //!
    //! Benchmarks
    //! ----------
    //! <div class="benchmark-chart"
    //!      style="min-width: 310px; height: 400px; margin: 0 auto"
    //!      data-dataset="benchmark.find_if.compile.json">
    //! </div>
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto find_if = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct find_if_impl;

    struct _find_if {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            using S = typename datatype<Xs>::type;
            using FindIf = find_if_impl<S>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, S>{}(),
            "hana::find_if(xs, pred) requires xs to be a Searchable");
        #endif

            return FindIf::apply(static_cast<Xs&&>(xs), static_cast<Pred&&>(pred));
        }
    };

    constexpr _find_if find_if{};
#endif

    //! Finds the value associated to the given key in a structure.
    //! @relates Searchable
    //!
    //! Given a `key` and a `Searchable` structure, `find` returns the `just`
    //! the first value whose key is equal to the given `key`, or `nothing` if
    //! there is no such key. Comparison is done with `equal`. `find` satisfies
    //! the following:
    //! @code
    //!     find(xs, key) == find_if(xs, equal.to(key))
    //! @endcode
    //!
    //!
    //! @param xs
    //! The structure to be searched.
    //!
    //! @param key
    //! A key to be searched for in the structure. The key has to be
    //! `Comparable` with the other keys of the structure. In the current
    //! version of the library, the comparison of `key` with any other key
    //! of the structure must return a compile-time `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp find
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto find = [](auto&& xs, auto&& key) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct find_impl;

    struct _find {
        template <typename Xs, typename Key>
        constexpr decltype(auto) operator()(Xs&& xs, Key&& key) const {
            using S = typename datatype<Xs>::type;
            using Find = find_impl<S>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, S>{}(),
            "hana::find(xs, key) requires xs to be Searchable");
        #endif

            return Find::apply(static_cast<Xs&&>(xs), static_cast<Key&&>(key));
        }
    };

    constexpr _find find{};
#endif

    //! Returns the value associated to the given key in a structure, or fail.
    //! @relates Searchable
    //!
    //! Given a `key` and a `Searchable` structure, `at_key` returns the first
    //! value whose key is equal to the given `key`, and fails at compile-time
    //! if no such key exists. This requires the `key` to be compile-time
    //! `Comparable`, exactly like for `find`. `at_key` satisfies the following:
    //! @code
    //!     at_key(xs, key) == from_just(find(xs, key))
    //! @endcode
    //!
    //!
    //! @param xs
    //! The structure to be searched.
    //!
    //! @param key
    //! A key to be searched for in the structure. The key has to be
    //! `Comparable` with the other keys of the structure. In the current
    //! version of the library, the comparison of `key` with any other key
    //! of the structure must return a compile-time `Logical`.
    //!
    //!
    //! Operator-form
    //! -------------
    //! For convenience, the `at_key` method can be applied to `Searchable`s
    //! that support it by using the `[]` operator. Hence, if `xs` supports
    //! the operator,
    //! @code
    //!     xs[k] == at_key(xs, k)
    //! @endcode
    //!
    //! To take advantage of this operator for a type `T`, `T` must inherit
    //! `hana::operators::Searchable_ops<T>`.
    //!
    //! @note
    //! The same operator is provided for the `at` method of the `Iterable`
    //! concept. When a data type is a model of both `Searchable` and
    //! `Iterable`, which operator is used should be documented properly.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp at_key
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto at_key = [](auto&& xs, auto&& key) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct at_key_impl;

    struct _at_key {
        template <typename Xs, typename Key>
        constexpr decltype(auto) operator()(Xs&& xs, Key&& key) const {
            using S = typename datatype<Xs>::type;
            using AtKey = at_key_impl<S>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, S>{}(),
            "hana::at_key(xs, key) requires xs to be Searchable");
        #endif

            return AtKey::apply(static_cast<Xs&&>(xs), static_cast<Key&&>(key));
        }
    };

    constexpr _at_key at_key{};
#endif

    //! Returns whether a structure contains a subset of the keys of
    //! another structure.
    //! @relates Searchable
    //!
    //! Given two `Searchable`s `xs` and `ys`, `is_subset` returns a `Logical`
    //! representing whether `xs` is a subset of `ys`. In other words, it
    //! returns whether all the keys of `xs` are also present in `ys`. This
    //! method does not return whether `xs` is a _strict_ subset of `ys`; if
    //! `xs` and `ys` are equal, all the keys of `xs` are also present in
    //! `ys`, and `is_subset` returns true.
    //!
    //! @note
    //! For convenience, `is_subset` can also be applied in infix notation.
    //! @snippet example/searchable.cpp is_subset.infix
    //!
    //!
    //! Cross-type version of the method
    //! --------------------------------
    //! This method is tag-dispatched using the data type of both arguments.
    //! It can be called with any two `Searchable`s sharing a common
    //! `Searchable` embedding, as defined in the main documentation
    //! of the `Searchable` concept. When `Searchable`s of two different
    //! data types but sharing a common embedding are sent to `is_subset`,
    //! they are first converted to this common `Searchable` and the
    //! `is_subset` method of the common embedding is then used. Of course,
    //! the method can be overriden for custom `Searchable`s for efficieny.
    //!
    //! @note
    //! While cross-type dispatching for `is_subset` is supported, it is
    //! not currently used by the library because there are no models
    //! of `Searchable` with a common embedding.
    //!
    //!
    //! @param xs
    //! The structure to check whether it is a subset of `ys`.
    //!
    //! @param ys
    //! The structure to check whether it is a superset of `xs`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp is_subset
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto is_subset = [](auto&& xs, auto&& ys) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S1, typename S2, typename = void>
    struct is_subset_impl;

    struct _is_subset {
        template <typename Xs, typename Ys>
        constexpr decltype(auto) operator()(Xs&& xs, Ys&& ys) const {
            using S1 = typename datatype<Xs>::type;
            using S2 = typename datatype<Ys>::type;
            using IsSubset = is_subset_impl<S1, S2>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, S1>{}(),
            "hana::is_subset(xs, ys) requires xs to be Searchable");

            static_assert(_models<Searchable, S2>{}(),
            "hana::is_subset(xs, ys) requires ys to be Searchable");

            static_assert(!is_default<is_subset_impl<S1, S2>>{}(),
            "hana::is_subset(xs, ys) requires xs and ys to be embeddable "
            "in a common Searchable");
        #endif

            return IsSubset::apply(static_cast<Xs&&>(xs), static_cast<Ys&&>(ys));
        }
    };

    constexpr auto is_subset = infix(_is_subset{});
#endif

    template <>
    struct operators::of<Searchable>
        : decltype(at_key)
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SEARCHABLE_HPP

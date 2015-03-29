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
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/infix.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Searchable` concept represents structures that can be searched.
    //!
    //! Intuitively, a Searchable is any structure, finite or infinite,
    //! containing elements that can be searched using a predicate. Sometimes,
    //! Searchables will associate keys to values; one can search for a key
    //! with a predicate, and the value associated to it is returned. This
    //! gives rise to map-like data structures. Other times, the elements of
    //! the structure that are searched (i.e. those to which the predicate is
    //! applied) are the same that are returned, which gives rise to set-like
    //! data structures. In general, we will refer to the _keys_ of a
    //! Searchable structure as those elements that are used for searching,
    //! and to the _values_ of a Searchable as those elements that are
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
    //! Similarly, using `elem` will likely be much faster than `any_of`
    //! with an equivalent predicate.
    //!
    //! > #### Insight
    //! > In a lazy evaluation context, any Foldable can also become a model
    //! > of Searchable because we can search lazily through the structure
    //! > with `foldr`. However, in the context of C++, some Searchables
    //! > can not be folded; think for example of an infinite set.
    //!
    //!
    //! Laws
    //! ----
    //! In order for the semantics of the methods to be consistent, some
    //! properties must be satisfied by any model of the Searchable concept.
    //! Rigorously, for any `Searchable xs, ys` and predicate `p`, the
    //! following laws should be satisfied:
    //! @code
    //!     any_of(xs, p) <=> !all_of(xs, negated p)
    //!                   <=> !none_of(xs, p)
    //!
    //!     elem(xs, x) <=> any_of(xs, [](auto y) { return y == x; })
    //!
    //!     find(xs, x) == find_if(xs, [](auto y) { return y == x; })
    //!     find_if(xs, always(false_)) == nothing
    //!
    //!     subset(xs, ys) <=> all_of(xs, [](auto x) { return elem(ys, x); })
    //! @endcode
    //!
    //! Additionally, if all the keys of the Searchable are `Logical`s,
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
    //! 1. `find_if` and `any_of`\n
    //! According to the above laws, it is possible to implement all the
    //! methods pertaining to this concept using only `find_if` and `any_of`.
    //! When `find_if` and `any_of` are provided, the other methods are
    //! implemented according to the laws above.
    //!
    //!
    //! Provided models
    //! ---------------
    //! 1. For builtin arrays\n
    //! Builtin arrays whose size is known can be searched as-if they were
    //! homogeneous tuples. However, since arrays can only hold objects of
    //! a single type and the predicate to `find_if` must return a compile-time
    //! Logical, the `find_if` method is fairly useless. For similar reasons,
    //! the `find` method is also fairly useless.
    //!
    //!
    //! @note
    //! We could implement `any_of(xs, pred)` as `is_just(find_if(xs, pred))`,
    //! and then reduce the minimal complete definition to `find_if`. However,
    //! this is not done because that implementation requires the predicate
    //! of `any_of` to return a compile-time `Logical`, which is more
    //! restrictive than what we have right now.
    //!
    //!
    //! @todo
    //! We should provide a member `operator[]` equivalent to `find`.
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
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/any_of.ctime.png
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, typename datatype<Xs>::type>{},
            "hana::any_of(xs, pred) requires xs to be a Searchable");
#endif
            return any_of_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, typename datatype<Xs>::type>{},
            "hana::any(xs) requires xs to be a Searchable");
#endif
            return any_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
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
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/all_of.ctime.png
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, typename datatype<Xs>::type>{},
            "hana::all_of(xs, pred) requires xs to be a Searchable");
#endif
            return all_of_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, typename datatype<Xs>::type>{},
            "hana::all(xs) requires xs to be a Searchable");
#endif
            return all_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
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
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/none_of.ctime.png
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, typename datatype<Xs>::type>{},
            "hana::none_of(xs, pred) requires xs to be a Searchable");
#endif
            return none_of_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, typename datatype<Xs>::type>{},
            "hana::none(xs) requires xs to be a Searchable");
#endif
            return none_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs)
            );
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
    //! @snippet example/searchable.cpp elem
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/elem.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto elem = [](auto&& xs, auto&& key) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct elem_impl;

    struct _elem {
        template <typename Xs, typename Key>
        constexpr decltype(auto) operator()(Xs&& xs, Key&& key) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, typename datatype<Xs>::type>{},
            "hana::elem(xs, key) requires xs to be a Searchable");
#endif
            return elem_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Key&&>(key)
            );
        }
    };

    constexpr _elem elem{};
#endif

    //! Return whether the key occurs in the structure.
    //! @relates Searchable
    //!
    //! Specifically, this is equivalent to `flip(elem)`, except that `in`
    //! can also be used in infix notation for increased expressiveness.
    //! This function is not a method that can be overriden; it is just a
    //! convenience function provided with the concept.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp in
    constexpr auto in = infix(flip(elem));

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
    //! @image html benchmark/searchable/find_if.ctime.png
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, typename datatype<Xs>::type>{},
            "hana::find_if(xs, pred) requires xs to be a Searchable");
#endif
            return find_if_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred)
            );
        }
    };

    constexpr _find_if find_if{};
#endif

    //! Finds the value associated to the given key in a structure.
    //! @relates Searchable
    //!
    //! Specifically, returns `just` the first value whose key is equal to
    //! the given `key`, or `nothing` if there is no such key. Comparison
    //! is done with `equal`.
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
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/find.ctime.png
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
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, typename datatype<Xs>::type>{},
            "hana::find(xs, key) requires xs to be a Searchable");
#endif
            return find_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Key&&>(key)
            );
        }
    };

    constexpr _find find{};
#endif

    //! Returns whether a structure contains a subset of the keys of
    //! another structure.
    //! @relates Searchable
    //!
    //! Specifically, `subset(xs, ys)` is a `Logical` representing whether all
    //! of the keys of `xs` are also keys of `ys`. In particular, this method
    //! does _not_ check whether `xs` is a strict subset of `ys`, i.e. a
    //! subset that is not equal.
    //!
    //! @note
    //! This method is tag-dispatched using the data type of the first
    //! argument only.
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
    //! @snippet example/searchable.cpp subset
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/subset.ctime.png
    //!
    //! @todo
    //! Consider using the data type of both arguments for tag-dispatching.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto subset = [](auto&& xs, auto&& ys) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct subset_impl;

    struct _subset {
        template <typename Xs, typename Ys>
        constexpr decltype(auto) operator()(Xs&& xs, Ys&& ys) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Searchable, typename datatype<Xs>::type>{},
            "hana::subset(xs, ys) requires xs to be a Searchable");

            static_assert(_models<Searchable, typename datatype<Ys>::type>{},
            "hana::subset(xs, ys) requires ys to be a Searchable");
#endif
            return subset_impl<typename datatype<Xs>::type>::apply(
                static_cast<Xs&&>(xs),
                static_cast<Ys&&>(ys)
            );
        }
    };

    constexpr _subset subset{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SEARCHABLE_HPP

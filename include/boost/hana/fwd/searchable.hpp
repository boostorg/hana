/*!
@file
Forward declares `boost::hana::Searchable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_SEARCHABLE_HPP
#define BOOST_HANA_FWD_SEARCHABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/infix.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
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
    //! Some methods like `any`, `all` and `none` allow simple queries to be
    //! performed on the keys of the structure, while other methods like
    //! `lookup` and `find` make it possible to find the value associated
    //! to a key. The most specific method should always be used if one
    //! cares about performance, because it is usually the case that heavy
    //! optimizations can be performed in more specific methods. For example,
    //! an associative data structure implemented as a hash table will be much
    //! faster to access using `lookup` than `find`, because in the second
    //! case it will have to do a linear search through all the entries.
    //! Similarly, using `elem` will likely be much faster than `any` with
    //! an equivalent predicate.
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
    //!     any(xs, p) <=> !all(xs, negated p)
    //!                <=> !none(xs, p)
    //!
    //!     elem(xs, x) <=> any(xs, [](auto y) { return y == x; })
    //!
    //!     lookup(xs, x) == find(xs, [](auto y) { return y == x; })
    //!     find(xs, always(false_)) == nothing
    //!
    //!     subset(xs, ys) <=> all(xs, [](auto x) { return elem(ys, x); })
    //! @endcode
    //!
    //! Additionally, if all the keys of the Searchable are `Logical`s,
    //! the following laws should be satisfied:
    //! @code
    //!     any_of(xs)  <=> any(xs, id)
    //!     all_of(xs)  <=> all(xs, id)
    //!     none_of(xs) <=> none(xs, id)
    //! @endcode
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! 1. `find` and `any`\n
    //! According to the above laws, it is possible to implement all the
    //! methods pertaining to this concept using only `find` and `any`.
    //! When `find` and `any` are provided, the other methods are implemented
    //! according to the laws above.
    //!
    //! @note
    //! We could implement `any(xs, pred)` as `is_just(find(xs, pred))`, and
    //! then reduce the minimal complete definition to `find`. However, this
    //! is not done because that implementation requires the predicate to
    //! return a compile-time `Logical`, which is too restrictive.
    //!
    //!
    //! @todo
    //! - We should provide a member `operator[]` equivalent to `lookup`.
    //! - Use perfect forwarding in the MCD once Clang bug #20619 is fixed.
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
    //! @snippet example/searchable.cpp any
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/any.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto any = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct any_impl;

    struct _any {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return any_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _any any{};
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
    //! @snippet example/searchable.cpp any_of
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto any_of = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct any_of_impl;

    struct _any_of {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return any_of_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _any_of any_of{};
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
    //! @snippet example/searchable.cpp all
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/all.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto all = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct all_impl;

    struct _all {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return all_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _all all{};
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
    //! @snippet example/searchable.cpp all_of
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto all_of = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct all_of_impl;

    struct _all_of {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return all_of_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _all_of all_of{};
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
    //! @snippet example/searchable.cpp none
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/none.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto none = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct none_impl;

    struct _none {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return none_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _none none{};
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
    //! @snippet example/searchable.cpp none_of
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto none_of = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct none_of_impl;

    struct _none_of {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            return none_of_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _none_of none_of{};
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
            return elem_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Key>(key)
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
    //! `Logical`. This is because `find` returns a `Maybe`, which is an
    //! heterogeneous data type.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/searchable.cpp find
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/find.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto find = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct find_impl;

    struct _find {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return find_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _find find{};
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
    //! @snippet example/searchable.cpp lookup
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/lookup.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto lookup = [](auto&& xs, auto&& key) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct lookup_impl;

    struct _lookup {
        template <typename Xs, typename Key>
        constexpr decltype(auto) operator()(Xs&& xs, Key&& key) const {
            return lookup_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Key>(key)
            );
        }
    };

    constexpr _lookup lookup{};
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
    //! @todo
    //! Consider using the data type of both arguments for tag-dispatching.
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
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/searchable/subset.ctime.png
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
            return subset_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Ys>(ys)
            );
        }
    };

    constexpr _subset subset{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SEARCHABLE_HPP

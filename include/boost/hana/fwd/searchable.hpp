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
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/infix.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! Data structures that can be searched.
    //!
    //! `Searchable`s are structures associating keys to values. Some methods
    //! like `any`, `all` and `none` allow simple queries to be performed
    //! on the keys of the structure, while other methods like `lookup` and
    //! `find` make it possible to find the value associated to a key. Unlike
    //! usual associative containers, `Searchable` does not require the
    //! structure to be finite; its specification is voluntarily left very
    //! general to allow infinite data structures. Also, there is no
    //! requirement that the keys and values be different, and indeed it
    //! can be useful to have identical keys and values.
    //!
    //! Also note that the most specific method should always be used if one
    //! cares about performance. For example, an associative data structure
    //! implemented as a hash table will be much faster to access using
    //! `lookup` than `find`. Similarly, using `elem` will likely be much
    //! faster than `any` with an equivalent predicate.
    //!
    //!
    //! ### Laws
    //! For any `Searchable xs, ys` and predicate `p`, the following laws
    //! should be satisfied:
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
    //! Additionally, if all the elements of the `Searchable` are `Logical`s,
    //! the following law should be satisfied:
    //! @code
    //!     {any,all,none}_of(xs) <=> {any,all,none}(xs, id)
    //! @endcode
    //!
    //! These laws make sure that `Searchable` instances are unsurprising
    //! and they also provide a default implementation in terms of the `any`
    //! and `find` methods.
    //!
    //! @todo
    //! We should provide a member `operator[]` equivalent to `lookup`.
    struct Searchable {
        BOOST_HANA_TYPECLASS(Searchable);
        struct mcd;
        template <typename R>
        struct record_mcd;
        struct iterable_mcd;
    };

    //! Return whether any key of the structure satisfies the `predicate`.
    //! @relates Searchable
    //!
    //! If the structure is not finite, `predicate` has to be satisfied
    //! after looking at a finite number of keys for this method to finish.
    //!
    //!
    //! @param searchable
    //! The structure to search.
    //!
    //! @param predicate
    //! A function called as `predicate(k)`, where `k` is a key of the
    //! structure, and returning a `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/searchable.cpp any
    //!
    //! ### Benchmarks
    //! @image html benchmark/searchable/any.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto any = [](auto&& searchable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _any {
        template <typename S, typename Pred>
        constexpr decltype(auto) operator()(S&& searchable, Pred&& pred) const {
            return Searchable::instance<
                datatype_t<S>
            >::any_impl(
                detail::std::forward<S>(searchable),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _any any{};
#endif

    //! Return whether any key of the structure is true-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a true-valued key must appear at a finite "index" in order for
    //! this method to finish.
    //!
    //! ### Example
    //! @snippet example/searchable.cpp any_of
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto any_of = [](auto&& searchable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _any_of {
        template <typename S>
        constexpr decltype(auto) operator()(S&& searchable) const {
            return Searchable::instance<
                datatype_t<S>
            >::any_of_impl(
                detail::std::forward<S>(searchable)
            );
        }
    };

    constexpr _any_of any_of{};
#endif

    //! Return whether all the keys of the structure satisfy the `predicate`.
    //! @relates Searchable
    //!
    //! If the structure is not finite, `predicate` has to return a false-
    //! valued `Logical` after looking at a finite number of keys for this
    //! method to finish.
    //!
    //!
    //! @param searchable
    //! The structure to search.
    //!
    //! @param predicate
    //! A function called as `predicate(k)`, where `k` is a key of the
    //! structure, and returning a `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/searchable.cpp all
    //!
    //! ### Benchmarks
    //! @image html benchmark/searchable/all.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto all = [](auto&& searchable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _all {
        template <typename S, typename Pred>
        constexpr decltype(auto) operator()(S&& searchable, Pred&& pred) const {
            return Searchable::instance<
                datatype_t<S>
            >::all_impl(
                detail::std::forward<S>(searchable),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _all all{};
#endif

    //! Return whether all the keys of the structure are true-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a false-valued key must appear at a finite "index" in order
    //! for this method to finish.
    //!
    //! ### Example
    //! @snippet example/searchable.cpp all_of
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto all_of = [](auto&& searchable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _all_of {
        template <typename S>
        constexpr decltype(auto) operator()(S&& searchable) const {
            return Searchable::instance<
                datatype_t<S>
            >::all_of_impl(
                detail::std::forward<S>(searchable)
            );
        }
    };

    constexpr _all_of all_of{};
#endif

    //! Return whether none of the keys of the structure satisfy the
    //! `predicate`.
    //! @relates Searchable
    //!
    //! If the structure is not finite, `predicate` has to return a true-
    //! valued `Logical` after looking at a finite number of keys for this
    //! method to finish.
    //!
    //!
    //! @param searchable
    //! The structure to search.
    //!
    //! @param predicate
    //! A function called as `predicate(k)`, where `k` is a key of the
    //! structure, and returning a `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/searchable.cpp none
    //!
    //! ### Benchmarks
    //! @image html benchmark/searchable/none.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto none = [](auto&& searchable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _none {
        template <typename S, typename Pred>
        constexpr decltype(auto) operator()(S&& searchable, Pred&& pred) const {
            return Searchable::instance<
                datatype_t<S>
            >::none_impl(
                detail::std::forward<S>(searchable),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _none none{};
#endif

    //! Return whether all of the keys of the structure are false-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a true-valued key must appear at a finite "index" in order
    //! for this method to finish.
    //!
    //! ### Example
    //! @snippet example/searchable.cpp none_of
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto none_of = [](auto&& searchable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _none_of {
        template <typename S>
        constexpr decltype(auto) operator()(S&& searchable) const {
            return Searchable::instance<
                datatype_t<S>
            >::none_of_impl(
                detail::std::forward<S>(searchable)
            );
        }
    };

    constexpr _none_of none_of{};
#endif

    //! Return whether the key occurs in the structure.
    //! @relates Searchable
    //!
    //! Specifically, returns whether any of the keys of the structure is
    //! equal to the given `key`. If the structure is not finite, an equal
    //! key has to appear at a finite "index" in the structure for this
    //! method to finish.
    //!
    //!
    //! @param searchable
    //! The structure to search.
    //!
    //! @param key
    //! A key to be searched for in the structure. The key has to be
    //! `Comparable` with the other keys of the structure.
    //!
    //!
    //! ### Example
    //! @snippet example/searchable.cpp elem
    //!
    //! ### Benchmarks
    //! @image html benchmark/searchable/elem.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto elem = [](auto&& searchable, auto&& key) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _elem {
        template <typename S, typename Key>
        constexpr decltype(auto) operator()(S&& searchable, Key&& key) const {
            return Searchable::instance<
                datatype_t<S>
            >::elem_impl(
                detail::std::forward<S>(searchable),
                detail::std::forward<Key>(key)
            );
        }
    };

    constexpr _elem elem{};
#endif

    //! Find the value associated to the first key satisfying a predicate.
    //! @relates Searchable
    //!
    //! Specifically, returns `just` the first value whose key satisfies the
    //! `predicate`, or `nothing` if there is no such key.
    //!
    //!
    //! @param searchable
    //! The structure to be searched.
    //!
    //! @param predicate
    //! A function called as `predicate(k)`, where `k` is a key of the
    //! structure, and returning a `Logical`. Note that in the current
    //! version of the library, the `predicate` has to return a compile-time
    //! `Logical`, i.e. one allowing values of different types in `if_`. This
    //! is because `find` returns a `Maybe`, which is an heterogeneous data
    //! type.
    //!
    //!
    //! ### Example
    //! @snippet example/searchable.cpp find
    //!
    //! ### Benchmarks
    //! @image html benchmark/searchable/find.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto find = [](auto&& searchable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _find {
        template <typename S, typename Pred>
        constexpr decltype(auto) operator()(S&& searchable, Pred&& pred) const {
            return Searchable::instance<
                datatype_t<S>
            >::find_impl(
                detail::std::forward<S>(searchable),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _find find{};
#endif

    //! Find the value associated to the given key in a structure.
    //! @relates Searchable
    //!
    //! Specifically, returns `just` the first value whose key is equal to
    //! the given `key`, or `nothing` if there is no such key. Comparison
    //! is done with `equal`.
    //!
    //!
    //! @param searchable
    //! The structure to be searched.
    //!
    //! @param key
    //! A key to be searched for in the structure. The key has to be
    //! `Comparable` with the other keys of the structure. In the current
    //! version of the library, the comparison of `key` with any other key
    //! of the structure must return a compile-time `Logical`, i.e. one
    //! allowing values of different types in `if_`. This is because `lookup`
    //! returns a `Maybe`, which is an heterogeneous data type.
    //!
    //!
    //! ### Example
    //! @snippet example/searchable.cpp lookup
    //!
    //! ### Benchmarks
    //! @image html benchmark/searchable/lookup.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto lookup = [](auto&& searchable, auto&& key) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _lookup {
        template <typename S, typename Key>
        constexpr decltype(auto) operator()(S&& searchable, Key&& key) const {
            return Searchable::instance<
                datatype_t<S>
            >::lookup_impl(
                detail::std::forward<S>(searchable),
                detail::std::forward<Key>(key)
            );
        }
    };

    constexpr _lookup lookup{};
#endif

    //! Return whether the key occurs in the structure.
    //! @relates Searchable
    //!
    //! Specifically, this is equivalent to `flip(elem)`, except that `in`
    //! can also be used in infix notation for increased expressiveness.
    //! This function is not a method that can be overriden; it is just a
    //! convenience function provided with the type class.
    //!
    //! ### Example
    //! @snippet example/searchable.cpp in
    constexpr auto in = infix(flip(elem));

    //! Return whether a structure contains a subset of the keys of
    //! another structure.
    //! @relates Searchable
    //!
    //! Specifically, `subset(xs, ys)` is a `Logical` representing whether all
    //! of the keys of `xs` are also keys of `ys`. In particular, this method
    //! does _not_ check whether `xs` is a strict subset of `ys`, i.e. a
    //! subset that is not equal. Both arguments may have different data
    //! types, but only the data type of the first argument is used for
    //! method dispatching.
    //!
    //!
    //! @param xs
    //! The structure to check whether it is a subset of `ys`.
    //!
    //! @param ys
    //! The structure to check whether it is a superset of `xs`.
    //!
    //!
    //! ### Example
    //! @snippet example/searchable.cpp subset
    //!
    //! ### Benchmarks
    //! @image html benchmark/searchable/subset.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto subset = [](auto&& xs, auto&& ys) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    struct _subset {
        template <typename Xs, typename Ys>
        constexpr decltype(auto) operator()(Xs&& xs, Ys&& ys) const {
            return Searchable::instance<
                datatype_t<Xs>
            >::subset_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Ys>(ys)
            );
        }
    };

    constexpr _subset subset{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SEARCHABLE_HPP

/*!
@file
Forward declares `boost::hana::Searchable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SEARCHABLE_SEARCHABLE_HPP
#define BOOST_HANA_SEARCHABLE_SEARCHABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
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
    //! is often useful to have identical keys and values.
    //!
    //! Also note that the most specific method should always be used if one
    //! cares about performance. For example, an associative data structure
    //! implemented as a hash table will be much faster to access using
    //! `lookup` than `find`. Similarly, using `elem` will likely be much
    //! faster than `any` with an equivalent predicate.
    //!
    //! @todo
    //! We should provide a member `operator[]` equivalent to `lookup`.
    struct Searchable {
        BOOST_HANA_TYPECLASS(Searchable);
        struct mcd;
        template <typename R>
        struct record_mcd;
        struct list_mcd;
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
    //! @image html benchmark/searchable/any.time.png
    constexpr auto any = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& searchable, auto&& predicate) -> decltype(auto) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::any_impl(
            detail::std::forward<decltype(searchable)>(searchable),
            detail::std::forward<decltype(predicate)>(predicate)
        );
    };

    //! Return whether any key of the structure is true-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a true-valued key must appear at a finite "index" in order for
    //! this method to finish.
    //!
    //! ### Example
    //! @snippet example/searchable.cpp any_of
    constexpr auto any_of = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& searchable) -> decltype(auto) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::any_of_impl(detail::std::forward<decltype(searchable)>(searchable));
    };

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
    //! @image html benchmark/searchable/all.time.png
    constexpr auto all = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& searchable, auto&& predicate) -> decltype(auto) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::all_impl(
            detail::std::forward<decltype(searchable)>(searchable),
            detail::std::forward<decltype(predicate)>(predicate)
        );
    };

    //! Return whether all the keys of the structure are true-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a false-valued key must appear at a finite "index" in order
    //! for this method to finish.
    //!
    //! ### Example
    //! @snippet example/searchable.cpp all_of
    constexpr auto all_of = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& searchable) -> decltype(auto) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::all_of_impl(detail::std::forward<decltype(searchable)>(searchable));
    };

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
    //! @image html benchmark/searchable/none.time.png
    constexpr auto none = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& searchable, auto&& predicate) -> decltype(auto) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::none_impl(
            detail::std::forward<decltype(searchable)>(searchable),
            detail::std::forward<decltype(predicate)>(predicate)
        );
    };

    //! Return whether all of the keys of the structure are false-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a true-valued key must appear at a finite "index" in order
    //! for this method to finish.
    //!
    //! ### Example
    //! @snippet example/searchable.cpp none_of
    constexpr auto none_of = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& searchable) -> decltype(auto) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::none_of_impl(detail::std::forward<decltype(searchable)>(searchable));
    };

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
    //! @image html benchmark/searchable/elem.time.png
    constexpr auto elem = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& searchable, auto&& key) -> decltype(auto) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::elem_impl(
            detail::std::forward<decltype(searchable)>(searchable),
            detail::std::forward<decltype(key)>(key)
        );
    };

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
    //! @image html benchmark/searchable/find.time.png
    constexpr auto find = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& searchable, auto&& predicate) -> decltype(auto) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::find_impl(
            detail::std::forward<decltype(searchable)>(searchable),
            detail::std::forward<decltype(predicate)>(predicate)
        );
    };

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
    //! @image html benchmark/searchable/lookup.time.png
    constexpr auto lookup = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& searchable, auto&& key) -> decltype(auto) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::lookup_impl(
            detail::std::forward<decltype(searchable)>(searchable),
            detail::std::forward<decltype(key)>(key)
        );
    };

    //! Return whether the key occurs in the structure.
    //! @relates Searchable
    //!
    //! Specifically, this is equivalent to `flip(elem)`, except that `in`
    //! can also be used in infix notation for increased expressiveness.
    //! This function is not a method that can be overriden; it is just a
    //! convenience function provided with the type class.
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
    //! @snippet example/searchable.cpp in
    BOOST_HANA_CONSTEXPR_LAMBDA auto in = infix([](auto&& key, auto&& searchable) -> decltype(auto) {
        return elem(
            detail::std::forward<decltype(searchable)>(searchable),
            detail::std::forward<decltype(key)>(key)
        );
    });

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
    //! @image html benchmark/searchable/subset.time.png
    constexpr auto subset = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& xs, auto&& ys) -> decltype(auto) {
        return Searchable::instance<
            datatype_t<decltype(xs)>
        >::subset_impl(
            detail::std::forward<decltype(xs)>(xs),
            detail::std::forward<decltype(ys)>(ys)
        );
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_SEARCHABLE_HPP

/*!
@file
Forward declares `boost::hana::Searchable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SEARCHABLE_SEARCHABLE_HPP
#define BOOST_HANA_SEARCHABLE_SEARCHABLE_HPP

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! Data structures that can be searched.
    //!
    //! `Searchable`s have a concept of keys and values. Searching is always
    //! done on the keys and the result is always the associated value. There
    //! is no requirement that the keys and values be different.
    struct Searchable {
        BOOST_HANA_TYPECLASS(Searchable);
        struct mcd;
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
    //! @snippet example/searchable/any.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto any = [](auto searchable, auto predicate) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::any_impl(searchable, predicate);
    };

    //! Return whether any key of the structure is true-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a true-valued key must appear at a finite "index" in order for
    //! this method to finish.
    //!
    //! ### Example
    //! @snippet example/searchable/any_of.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto any_of = [](auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::any_of_impl(searchable);
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
    //! @snippet example/searchable/all.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto all = [](auto searchable, auto predicate) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::all_impl(searchable, predicate);
    };

    //! Return whether all the keys of the structure are true-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a false-valued key must appear at a finite "index" in order
    //! for this method to finish.
    //!
    //! ### Example
    //! @snippet example/searchable/all_of.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto all_of = [](auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::all_of_impl(searchable);
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
    //! @snippet example/searchable/none.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto none = [](auto searchable, auto predicate) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::none_impl(searchable, predicate);
    };

    //! Return whether all of the keys of the structure are false-valued.
    //! @relates Searchable
    //!
    //! The keys of the structure must be `Logical`s. If the structure is not
    //! finite, a true-valued key must appear at a finite "index" in order
    //! for this method to finish.
    //!
    //! ### Example
    //! @snippet example/searchable/none_of.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto none_of = [](auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::none_of_impl(searchable);
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
    //! @snippet example/searchable/elem.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto elem = [](auto searchable, auto key) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::elem_impl(searchable, key);
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
    //! @snippet example/searchable/find.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto find = [](auto searchable, auto predicate) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::find_impl(searchable, predicate);
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
    //! @snippet example/searchable/lookup.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto lookup = [](auto searchable, auto key) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::lookup_impl(searchable, key);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_SEARCHABLE_HPP

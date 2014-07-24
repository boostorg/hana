/*!
@file
Forward declares `boost::hana::Searchable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SEARCHABLE_SEARCHABLE_HPP
#define BOOST_HANA_SEARCHABLE_SEARCHABLE_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! Data structures that can be searched.
    //!
    //! `Searchable`s have a concept of keys and values. Basically, searching
    //! is always done on the keys and the result is always the associated
    //! value. There is no requirement that the keys and values be different.
    struct Searchable {
        BOOST_HANA_TYPECLASS(Searchable);
        struct find_mcd;
    };

    //! Return whether any key of the structure satisfies the `predicate`.
    //! @method{Searchable}
    //!
    //! ### Fusion example
    //! @snippet example/list/searchable/any.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/searchable/any.cpp mpl
    BOOST_HANA_CONSTEXPR_LAMBDA auto any = [](auto predicate, auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::any_impl(predicate, searchable);
    };

    //! Return whether any key of the structure is true-valued.
    //! @method{Searchable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto any_of = [](auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::any_of_impl(searchable);
    };

    //! Return whether all the keys of the structure satisfy the `predicate`.
    //! @method{Searchable}
    //!
    //! ### Fusion example
    //! @snippet example/list/searchable/all.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/searchable/all.cpp mpl
    BOOST_HANA_CONSTEXPR_LAMBDA auto all = [](auto predicate, auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::all_impl(predicate, searchable);
    };

    //! Return whether all the keys of the structure are true-valued.
    //! @method{Searchable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto all_of = [](auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::all_of_impl(searchable);
    };

    //! Return whether none of the keys of the structure satisfy the
    //! `predicate`.
    //! @method{Searchable}
    //!
    //! ### Fusion example
    //! @snippet example/list/searchable/none.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/searchable/none.cpp mpl
    BOOST_HANA_CONSTEXPR_LAMBDA auto none = [](auto predicate, auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::none_impl(predicate, searchable);
    };

    //! Return whether none of the keys of the structure is true-valued.
    //! @method{Searchable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto none_of = [](auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::none_of_impl(searchable);
    };

    //! Return whether the key occurs in the structure.
    //! @method{Searchable}
    //!
    //! ### Example
    //! @snippet example/list/searchable/elem.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto elem = [](auto key, auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::elem_impl(key, searchable);
    };

    //! Find the value associated to a key satisfying a predicate.
    //! @method{Searchable}
    //!
    //! Specifically, returns `just` the first value whose key satisfies the
    //! `predicate`, or `nothing` if there is no such key.
    //!
    //! ### Example 1
    //! @snippet example/list/searchable/find.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/searchable/find.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto find = [](auto predicate, auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::find_impl(predicate, searchable);
    };

    //! Find the value associated to the given key.
    //! @method{Searchable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto lookup = [](auto key, auto searchable) {
        return Searchable::instance<
            datatype_t<decltype(searchable)>
        >::lookup_impl(key, searchable);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_SEARCHABLE_HPP

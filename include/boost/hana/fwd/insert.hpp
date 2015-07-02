/*!
@file
Forward declares `boost::hana::insert`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_INSERT_HPP
#define BOOST_HANA_FWD_INSERT_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Insert a key in a structure.
    //!
    //! This function is not associated to a concept right now, so its
    //! behavior can't be described generally. The `Map` and `Set` data
    //! types and the `Sequence` concept all implement this function in
    //! different ways, so you should refer to the documentation of `insert`
    //! for these containers instead.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto insert = [](auto&& xs, auto&& ...args) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct insert_impl : insert_impl<T, when<true>> { };

    struct insert_t {
        template <typename Set, typename ...Args>
        constexpr decltype(auto) operator()(Set&& set, Args&& ...args) const;
    };

    constexpr insert_t insert{};
#endif

    //! Insert a value at a given index in a sequence.
    //! @relates Sequence
    //!
    //! Given a sequence, an index and an element to insert, `insert` inserts
    //! the element at the given index.
    //!
    //! @param xs
    //! The sequence in which a value should be inserted.
    //!
    //! @param n
    //! The index at which an element should be inserted. This must be a
    //! non-negative `Constant` of an integral type, and it must also be
    //! true that `n < length(xs)` if `xs` is a finite sequence.
    //!
    //! @param element
    //! The element to insert in the sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp insert
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto insert = [](auto&& xs, auto&& n, auto&& element) {
        return tag-dispatched;
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_INSERT_HPP

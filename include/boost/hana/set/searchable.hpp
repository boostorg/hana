/*!
@file
Defines the instance of `boost::hana::Searchable` for `boost::hana::Set`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SET_SEARCHABLE_HPP
#define BOOST_HANA_SET_SEARCHABLE_HPP

#include <boost/hana/searchable/mcd.hpp>
#include <boost/hana/set/set.hpp>


namespace boost { namespace hana {
    //! The keys and the values of a `Set` are its elements; the `Searchable` 
    //! instance follows naturally from that.
    //!
    //! ### Example
    //! @snippet example/set.cpp searchable
    template <>
    struct Searchable::instance<Set> : Searchable::mcd {
        template <typename Set, typename Pred>
        static constexpr auto find_impl(Set set, Pred pred)
        { return find(set.storage, pred); }

        template <typename Set, typename Pred>
        static constexpr auto any_impl(Set set, Pred pred)
        { return any(set.storage, pred); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_SEARCHABLE_HPP

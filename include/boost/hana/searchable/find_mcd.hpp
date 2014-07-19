/*!
@file
Defines `boost::hana::Searchable::find_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SEARCHABLE_FIND_MCD_HPP
#define BOOST_HANA_SEARCHABLE_FIND_MCD_HPP

#include <boost/hana/searchable/searchable.hpp>

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `find` and `any`
    //!
    //! @note
    //! We could implement `any(pred, xs)` as `is_just(find(pred, xs))`, and
    //! then reduce the MCD to `find`. However, this is not done because that
    //! implementation requires the predicate to be compile-time, which is
    //! more restrictive than the original `any` in `Foldable`.
    struct Searchable::find_mcd {
        template <typename X, typename Srch>
        static constexpr auto elem_impl(X x, Srch srch)
        { return any([=](auto y) { return equal(x, y); }, srch); }

        template <typename Pred, typename Srch>
        static constexpr auto all_impl(Pred pred, Srch srch)
        { return not_(any([=](auto x) { return not_(pred(x)); }, srch)); }

        template <typename Pred, typename Srch>
        static constexpr auto none_impl(Pred pred, Srch srch)
        { return not_(any(pred, srch)); }

        template <typename Srch>
        static constexpr auto any_of_impl(Srch srch)
        { return any([](auto x) { return x; }, srch); }

        template <typename Srch>
        static constexpr auto all_of_impl(Srch srch)
        { return all([](auto x) { return x; }, srch); }

        template <typename Srch>
        static constexpr auto none_of_impl(Srch srch)
        { return none([](auto x) { return x; }, srch); }

        template <typename Key, typename Srch>
        static constexpr auto find_key_impl(Key key, Srch srch)
        { return find([=](auto k) { return equal(key, k); }, srch); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_FIND_MCD_HPP

/*!
@file
Defines `boost::hana::Searchable::mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SEARCHABLE_MCD_HPP
#define BOOST_HANA_SEARCHABLE_MCD_HPP

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
    struct Searchable::mcd {
        template <typename Srch, typename X>
        static constexpr auto elem_impl(Srch srch, X x)
        { return any(srch, [=](auto y) { return equal(x, y); }); }

        template <typename Srch, typename Pred>
        static constexpr auto all_impl(Srch srch, Pred pred)
        { return not_(any(srch, [=](auto x) { return not_(pred(x)); })); }

        template <typename Srch, typename Pred>
        static constexpr auto none_impl(Srch srch, Pred pred)
        { return not_(any(srch, pred)); }

        template <typename Srch>
        static constexpr auto any_of_impl(Srch srch)
        { return any(srch, [](auto x) { return x; }); }

        template <typename Srch>
        static constexpr auto all_of_impl(Srch srch)
        { return all(srch, [](auto x) { return x; }); }

        template <typename Srch>
        static constexpr auto none_of_impl(Srch srch)
        { return none(srch, [](auto x) { return x; }); }

        template <typename Srch, typename Key>
        static constexpr auto lookup_impl(Srch srch, Key key)
        { return find(srch, [=](auto k) { return equal(key, k); }); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_MCD_HPP

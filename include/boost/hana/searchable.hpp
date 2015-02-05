/*!
@file
Defines `boost::hana::Searchable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SEARCHABLE_HPP
#define BOOST_HANA_SEARCHABLE_HPP

#include <boost/hana/fwd/searchable.hpp>

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `find` and `any`
    //!
    //! @note
    //! We could implement `any(pred, xs)` as `is_just(find(pred, xs))`, and
    //! then reduce the MCD to `find`. However, this is not done because that
    //! implementation requires the predicate to be compile-time, which is
    //! more restrictive than the original `any` in `Foldable`.
    //!
    //! @todo
    //! Use perfect forwarding once bug
    //! http://llvm.org/bugs/show_bug.cgi?id=20619
    //! is fixed.
    struct Searchable::mcd {
        template <typename Srch, typename X>
        static constexpr auto elem_impl(Srch srch, X x)
        { return any(srch, partial(equal, x)); }

        template <typename Srch, typename Pred>
        static constexpr auto all_impl(Srch srch, Pred pred)
        { return not_(any(srch, compose(not_, pred))); }

        template <typename Srch, typename Pred>
        static constexpr auto none_impl(Srch srch, Pred pred)
        { return not_(any(srch, pred)); }

        template <typename Srch>
        static constexpr auto any_of_impl(Srch srch)
        { return any(srch, id); }

        template <typename Srch>
        static constexpr auto all_of_impl(Srch srch)
        { return all(srch, id); }

        template <typename Srch>
        static constexpr auto none_of_impl(Srch srch)
        { return none(srch, id); }

        template <typename Srch, typename Key>
        static constexpr auto lookup_impl(Srch srch, Key key)
        { return find(srch, partial(equal, key)); }

        template <typename Xs, typename Ys>
        static constexpr auto subset_impl(Xs xs, Ys ys)
        { return all(xs, partial(elem, ys)); }
    };

    template <typename T>
    struct Searchable::instance<T, when<models<Searchable(T)>{}>>
        : Searchable::mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_HPP

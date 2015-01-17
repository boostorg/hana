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
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    template <typename Xs, typename _>
    struct elem_impl<Xs, when<is_implemented<any_impl<Xs>, _>>, _> {
        template <typename Srch, typename X>
        static constexpr auto apply(Srch srch, X x)
        { return any(srch, partial(equal, x)); }
    };

    template <typename Xs, typename _>
    struct all_impl<Xs, when<is_implemented<any_impl<Xs>, _>>, _> {
        template <typename Srch, typename Pred>
        static constexpr auto apply(Srch srch, Pred pred)
        { return not_(any(srch, compose(not_, pred))); }
    };

    template <typename Xs, typename _>
    struct none_impl<Xs, when<is_implemented<any_impl<Xs>, _>>, _> {
        template <typename Srch, typename Pred>
        static constexpr auto apply(Srch srch, Pred pred)
        { return not_(any(srch, pred)); }
    };

    template <typename Xs, typename _>
    struct any_of_impl<Xs, when<is_implemented<any_impl<Xs>, _>>, _> {
        template <typename Srch>
        static constexpr auto apply(Srch srch)
        { return any(srch, id); }
    };

    template <typename Xs, typename _>
    struct all_of_impl<Xs, when<is_implemented<all_impl<Xs>, _>>, _> {
        template <typename Srch>
        static constexpr auto apply(Srch srch)
        { return all(srch, id); }
    };

    template <typename Xs, typename _>
    struct none_of_impl<Xs, when<is_implemented<none_impl<Xs>, _>>, _> {
        template <typename Srch>
        static constexpr auto apply(Srch srch)
        { return none(srch, id); }
    };

    template <typename Xs, typename _>
    struct lookup_impl<Xs, when<is_implemented<find_impl<Xs>, _>>, _> {
        template <typename Srch, typename Key>
        static constexpr auto apply(Srch srch, Key key)
        { return find(srch, partial(equal, key)); }
    };

    template <typename Xs, typename _>
    struct subset_impl<Xs, when<is_implemented<all_impl<Xs>, _>>, _> {
        template <typename Xs_, typename Ys>
        static constexpr auto apply(Xs_ xs, Ys ys)
        { return all(xs, partial(elem, ys)); }
    };

    template <>
    struct models_impl<Searchable> {
        template <typename S, typename Context>
        static constexpr auto apply =
            is_implemented<find_impl<S>, Context> &&
            is_implemented<any_impl<S>, Context>
        ;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_HPP

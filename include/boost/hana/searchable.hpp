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
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    template <typename Xs, typename>
    struct elem_impl : elem_impl<Xs, when<true>> { };

    template <typename Xs, bool condition>
    struct elem_impl<Xs, when<condition>> {
        template <typename Srch, typename X>
        static constexpr auto apply(Srch srch, X x)
        { return any(srch, partial(equal, x)); }
    };

    template <typename Xs, typename>
    struct all_impl : all_impl<Xs, when<true>> { };

    template <typename Xs, bool condition>
    struct all_impl<Xs, when<condition>> {
        template <typename Srch, typename Pred>
        static constexpr auto apply(Srch srch, Pred pred)
        { return not_(any(srch, compose(not_, pred))); }
    };

    template <typename Xs, typename>
    struct none_impl : none_impl<Xs, when<true>> { };

    template <typename Xs, bool condition>
    struct none_impl<Xs, when<condition>> {
        template <typename Srch, typename Pred>
        static constexpr auto apply(Srch srch, Pred pred)
        { return not_(any(srch, pred)); }
    };

    template <typename Xs, typename>
    struct any_of_impl : any_of_impl<Xs, when<true>> { };

    template <typename Xs, bool condition>
    struct any_of_impl<Xs, when<condition>> {
        template <typename Srch>
        static constexpr auto apply(Srch srch)
        { return any(srch, id); }
    };

    template <typename Xs, typename>
    struct all_of_impl : all_of_impl<Xs, when<true>> { };

    template <typename Xs, bool condition>
    struct all_of_impl<Xs, when<condition>> {
        template <typename Srch>
        static constexpr auto apply(Srch srch)
        { return all(srch, id); }
    };

    template <typename Xs, typename>
    struct none_of_impl : none_of_impl<Xs, when<true>> { };

    template <typename Xs, bool condition>
    struct none_of_impl<Xs, when<condition>> {
        template <typename Srch>
        static constexpr auto apply(Srch srch)
        { return none(srch, id); }
    };

    template <typename Xs, typename>
    struct lookup_impl : lookup_impl<Xs, when<true>> { };

    template <typename Xs, bool condition>
    struct lookup_impl<Xs, when<condition>> {
        template <typename Srch, typename Key>
        static constexpr auto apply(Srch srch, Key key)
        { return find(srch, partial(equal, key)); }
    };

    template <typename Xs, typename>
    struct subset_impl : subset_impl<Xs, when<true>> { };

    template <typename Xs, bool condition>
    struct subset_impl<Xs, when<condition>> {
        template <typename Xs_, typename Ys>
        static constexpr auto apply(Xs_ xs, Ys ys)
        { return all(xs, partial(elem, ys)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_HPP

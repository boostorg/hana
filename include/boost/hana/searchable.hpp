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
    template <typename Xs>
    struct default_<elem_impl<Xs>> {
        template <typename Srch, typename X>
        static constexpr auto apply(Srch srch, X x)
        { return any(srch, partial(equal, x)); }
    };

    template <typename Xs>
    struct default_<all_impl<Xs>> {
        template <typename Srch, typename Pred>
        static constexpr auto apply(Srch srch, Pred pred)
        { return not_(any(srch, compose(not_, pred))); }
    };

    template <typename Xs>
    struct default_<none_impl<Xs>> {
        template <typename Srch, typename Pred>
        static constexpr auto apply(Srch srch, Pred pred)
        { return not_(any(srch, pred)); }
    };

    template <typename Xs>
    struct default_<any_of_impl<Xs>> {
        template <typename Srch>
        static constexpr auto apply(Srch srch)
        { return any(srch, id); }
    };

    template <typename Xs>
    struct default_<all_of_impl<Xs>> {
        template <typename Srch>
        static constexpr auto apply(Srch srch)
        { return all(srch, id); }
    };

    template <typename Xs>
    struct default_<none_of_impl<Xs>> {
        template <typename Srch>
        static constexpr auto apply(Srch srch)
        { return none(srch, id); }
    };

    template <typename Xs>
    struct default_<lookup_impl<Xs>> {
        template <typename Srch, typename Key>
        static constexpr auto apply(Srch srch, Key key)
        { return find(srch, partial(equal, key)); }
    };

    template <typename Xs>
    struct default_<subset_impl<Xs>> {
        template <typename Xs_, typename Ys>
        static constexpr auto apply(Xs_ xs, Ys ys)
        { return all(xs, partial(elem, ys)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_HPP

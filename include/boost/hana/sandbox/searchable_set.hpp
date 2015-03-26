/*!
@file
Defines `boost::hana::SearchableSet`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SANDBOX_SEARCHABLE_SET_HPP
#define BOOST_HANA_SANDBOX_SEARCHABLE_SET_HPP

#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! A `Monad` for searching infinite sets in finite time.
    //!
    //! Taken from http://math.andrej.com/2008/11/21/a-haskell-monad-for-infinite-search-in-finite-time/.
    struct SearchableSet {
        struct hana {
            struct operators
                : boost::hana::operators::of<Comparable, Monad>
            { };
        };
    };

    template <typename Find, typename = operators::adl>
    struct _sset {
        Find find;
        struct hana { using datatype = SearchableSet; };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto searchable_set = [](auto pred) {
        return _sset<decltype(pred)>{pred};
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto singleton = [](auto x) {
        return searchable_set([=](auto p) { return x; });
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto doubleton = [](auto x, auto y) {
        return searchable_set([=](auto p) {
            return if_(p(x), x, y);
        });
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto union_ = [](auto xs, auto ys) {
        return flatten(doubleton(xs, ys));
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<SearchableSet, SearchableSet> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs xs, Ys ys)
        { return and_(subset(xs, ys), subset(ys, xs)); }
    };


    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct transform_impl<SearchableSet> {
        template <typename Set, typename F>
        static constexpr auto apply(Set set, F f) {
            return searchable_set([=](auto q) {
                return f(set.find(compose(q, f)));
            });
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<SearchableSet> {
        template <typename X>
        static constexpr auto apply(X x)
        { return singleton(x); }
    };

    template <>
    struct ap_impl<SearchableSet> {
        template <typename F, typename Set>
        static constexpr auto apply(F fset, Set set) {
            return flatten(transform(fset, partial(transform, set)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct flatten_impl<SearchableSet> {
        template <typename Set>
        static constexpr auto apply(Set set) {
            return searchable_set([=](auto p) {
                return set.find([=](auto set) {
                    return any_of(set, p);
                }).find(p);
            });
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_if_impl<SearchableSet> {
        template <typename Set, typename Pred>
        static constexpr auto apply(Set set, Pred p) {
            auto x = set.find(p);
            return if_(p(x), just(x), nothing);
        }
    };

    template <>
    struct any_of_impl<SearchableSet> {
        template <typename Set, typename Pred>
        static constexpr auto apply(Set set, Pred p) {
            return p(set.find(p));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SANDBOX_SEARCHABLE_SET_HPP

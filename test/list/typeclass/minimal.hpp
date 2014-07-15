/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LIST_TYPECLASS_MINIMAL_HPP
#define BOOST_HANA_TEST_LIST_TYPECLASS_MINIMAL_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/wrap.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>


struct MinimalList;

BOOST_HANA_CONSTEXPR_LAMBDA auto minimal_list = [](auto ...xs) {
    return boost::hana::detail::wrap<MinimalList>(boost::hana::list(xs...));
};

template <int ...i>
BOOST_HANA_CONSTEXPR_LAMBDA auto ilist = minimal_list(boost::hana::integral<int, i>...);

namespace boost { namespace hana {
    template <>
    struct Foldable::instance<MinimalList> : Iterable::FoldableInstance { };

    template <>
    struct Iterable::instance<MinimalList> : Iterable::mcd {
        template <typename Xs>
        static constexpr auto head_impl(Xs xs)
        { return head(detail::unwrap(xs)); }

        template <typename Xs>
        static constexpr auto tail_impl(Xs xs)
        { return detail::wrap<MinimalList>(tail(detail::unwrap(xs))); }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs)
        { return is_empty(detail::unwrap(xs)); }
    };

    template <>
    struct List::instance<MinimalList> : List::mcd<MinimalList> {
        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs)
        { return detail::wrap<MinimalList>(cons(x, detail::unwrap(xs))); }

        static constexpr auto nil_impl()
        { return detail::wrap<MinimalList>(nil<List>); }
    };

    // Comparable instance is required for the unit tests.
    template <>
    struct Comparable::instance<MinimalList, MinimalList> : Comparable::equal_mcd {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs xs, Ys ys)
        { return equal(detail::unwrap(xs), detail::unwrap(ys)); }
    };
}}

#endif // !BOOST_HANA_TEST_LIST_TYPECLASS_MINIMAL_HPP

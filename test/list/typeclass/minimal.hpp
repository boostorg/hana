/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LIST_TYPECLASS_MINIMAL_HPP
#define BOOST_HANA_TEST_LIST_TYPECLASS_MINIMAL_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>


struct MinimalList;

template <typename Datatype, typename X, typename = boost::hana::operators::enable>
struct wrapper {
    X unwrap;
    using hana_datatype = Datatype;
};

template <typename Datatype>
struct wrap_impl {
    template <typename X>
    constexpr auto operator()(X x) const {
        return wrapper<Datatype, X>{x};
    }
};

template <typename Datatype>
constexpr wrap_impl<Datatype> wrap{};

BOOST_HANA_CONSTEXPR_LAMBDA auto unwrap = [](auto x) { return x.unwrap; };

BOOST_HANA_CONSTEXPR_LAMBDA auto minimal_list = [](auto ...xs) {
    return wrap<MinimalList>(boost::hana::list(xs...));
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
        { return head(unwrap(xs)); }

        template <typename Xs>
        static constexpr auto tail_impl(Xs xs)
        { return wrap<MinimalList>(tail(unwrap(xs))); }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs)
        { return is_empty(unwrap(xs)); }
    };

    template <>
    struct List::instance<MinimalList> : List::mcd<MinimalList> {
        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs)
        { return wrap<MinimalList>(cons(x, unwrap(xs))); }

        static constexpr auto nil_impl()
        { return wrap<MinimalList>(nil<List>); }
    };

    // Comparable instance is required for the unit tests.
    template <>
    struct Comparable::instance<MinimalList, MinimalList> : Comparable::equal_mcd {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs xs, Ys ys)
        { return equal(unwrap(xs), unwrap(ys)); }
    };
}}

#endif // !BOOST_HANA_TEST_LIST_TYPECLASS_MINIMAL_HPP

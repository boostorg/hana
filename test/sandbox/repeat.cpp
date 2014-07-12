/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/lazy.hpp>
using namespace boost::hana;


struct LazyList;

template <typename X, typename Xs>
struct lazy_cons_type {
    X x;
    Xs xs;
    using hana_datatype = LazyList;
};

auto lazy_cons = [](auto x, auto xs) {
    return lazy_cons_type<decltype(x), decltype(xs)>{x, xs};
};

auto repeat = fix([](auto repeat, auto x) {
    return lazy_cons(x, lazy(repeat)(x));
});

namespace boost { namespace hana {
    template <>
    struct Iterable::instance<LazyList> : Iterable::mcd {
        template <typename Xs>
        static constexpr auto head_impl(Xs lcons)
        { return lcons.x; }

        template <typename Xs>
        static constexpr auto tail_impl(Xs lcons)
        { return eval(lcons.xs); }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs lcons)
        { return false_; }
    };

    template <>
    struct Comparable::instance<LazyList, LazyList>
        : Iterable::ComparableInstance
    { };
}}


int main() {
    BOOST_HANA_STATIC_ASSERT(!is_empty(repeat(1)));
    BOOST_HANA_STATIC_ASSERT(head(repeat(1)) == 1);
    BOOST_HANA_STATIC_ASSERT(at(int_<10>, repeat(1)) == 1);
}

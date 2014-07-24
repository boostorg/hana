/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair/instance.hpp>

#include <tuple>
using namespace boost::hana;


// stop_at must return a Maybe, so we need the comparison to be compile-time.
template <int i>
constexpr auto x = int_<i>;

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;
    using L = detail::minimal::List<mcd>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...xs) {
        return std::make_tuple(xs...);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto stop_at = [=](auto stop) {
        return [=](auto x) {
            return if_(equal(stop, x),
                nothing,
                just(pair(f(x), x + int_<1>))
            );
        };
    };

    BOOST_HANA_STATIC_ASSERT(unfoldr<L>(stop_at(x<0>), x<0>) == list());
    BOOST_HANA_STATIC_ASSERT(unfoldr<L>(stop_at(x<1>), x<0>) == list(f(x<0>)));
    BOOST_HANA_STATIC_ASSERT(unfoldr<L>(stop_at(x<2>), x<0>) == list(f(x<0>), f(x<1>)));
    BOOST_HANA_STATIC_ASSERT(unfoldr<L>(stop_at(x<3>), x<0>) == list(f(x<0>), f(x<1>), f(x<2>)));
    BOOST_HANA_STATIC_ASSERT(unfoldr<L>(stop_at(x<4>), x<0>) == list(f(x<0>), f(x<1>), f(x<2>), f(x<3>)));
}

// Make sure it can revert foldr under some conditions
template <typename mcd>
void test_revert() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;
    using L = detail::minimal::List<mcd>;
    static constexpr auto z = x<999>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = pair;
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = [=](auto k) {
        return if_(equal(k, z), nothing, just(k));
    };

    // Make sure the special conditions are met
    BOOST_HANA_STATIC_ASSERT(g(z) == nothing);
    BOOST_HANA_STATIC_ASSERT(g(f(x<0>, z)) == just(pair(x<0>, z)));

    // Make sure the reversing works
    auto lists = list(
        list(), list(x<0>), list(x<0>, x<1>), list(x<0>, x<1>, x<2>)
    );
    for_each(lists, [=](auto xs) {
        BOOST_HANA_STATIC_ASSERT(unfoldr<L>(g, foldr(f, z, xs)) == xs);
    });
}

int main() {
    test<List::mcd<void>>();
    test_revert<List::mcd<void>>();
}

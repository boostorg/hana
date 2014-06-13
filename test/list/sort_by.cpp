/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


namespace test {
    template <int equivalence_class> struct X { };
    template <int equivalence_class> struct Y { };

    template <int i, int j, template <int> class X, template <int> class Y>
    constexpr auto operator==(X<i>, Y<j>)
    { return type<X<i>> == type<Y<j>>; }
}

template <int equivalence_class> constexpr test::X<equivalence_class> x{};
template <int equivalence_class> constexpr test::Y<equivalence_class> y{};

constexpr struct _predicate {
    template <int i, int j, template <int> class X, template <int> class Y>
    constexpr auto operator()(X<i>, Y<j>) const
    { return bool_<(i < j)>; }
} pred{};

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto check = [](auto sorted_list) {
        auto perms = fmap(partial(sort_by, pred), permutations(sorted_list));
        BOOST_HANA_STATIC_ASSERT(all(_ == sorted_list, perms));
    };
    check(list());
    check(list(x<1>));
    check(list(x<1>, x<2>));
    check(list(x<1>, x<2>, x<3>));

    // check stability
    BOOST_HANA_STATIC_ASSERT(sort_by(pred, list(x<1>, y<1>)) == list(x<1>, y<1>));
    BOOST_HANA_STATIC_ASSERT(sort_by(pred, list(y<1>, x<1>)) == list(y<1>, x<1>));

    BOOST_HANA_STATIC_ASSERT(sort_by(pred, list(x<1>, y<1>, x<2>, y<2>)) == list(x<1>, y<1>, x<2>, y<2>));
    BOOST_HANA_STATIC_ASSERT(sort_by(pred, list(x<1>, x<2>, y<1>, y<2>)) == list(x<1>, y<1>, x<2>, y<2>));
    BOOST_HANA_STATIC_ASSERT(sort_by(pred, list(y<1>, x<1>, x<2>, y<2>)) == list(y<1>, x<1>, x<2>, y<2>));
    BOOST_HANA_STATIC_ASSERT(sort_by(pred, list(x<2>, y<1>, y<2>, x<1>)) == list(y<1>, x<1>, x<2>, y<2>));

    BOOST_HANA_STATIC_ASSERT(sort_by(pred, list(x<1>, x<3>, y<1>, x<2>, y<3>)) == list(x<1>, y<1>, x<2>, x<3>, y<3>));
}

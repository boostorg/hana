/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <int ...i>
BOOST_HANA_CONSTEXPR_LAMBDA auto ilist = list_c<int, i...>;

BOOST_HANA_CONSTEXPR_LAMBDA auto plus = [](auto x, auto y) { return x + y; };
BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) { return x + int_<1>; };

int main() {
    BOOST_HANA_STATIC_ASSERT(zip_with(inc) == list());
    BOOST_HANA_STATIC_ASSERT(zip_with(plus) == list());

    BOOST_HANA_STATIC_ASSERT(zip_with(inc, ilist<>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(zip_with(inc, ilist<0>) == ilist<1>);
    BOOST_HANA_STATIC_ASSERT(zip_with(inc, ilist<0, 1>) == ilist<1, 2>);
    BOOST_HANA_STATIC_ASSERT(zip_with(inc, ilist<0, 1, 2>) == ilist<1, 2, 3>);
    BOOST_HANA_STATIC_ASSERT(zip_with(inc, ilist<>, ilist<>) == ilist<>);

    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<>, ilist<>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<1>, ilist<>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<>, ilist<3>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<1>, ilist<3>) == ilist<1 + 3>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<1, 2>, ilist<3, 4>) == ilist<1 + 3, 2 + 4>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<1, 2, 3, 4>, ilist<5, 6, 7>) == ilist<1 + 5, 2 + 6, 3 + 7>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<>, ilist<>, ilist<>) == ilist<>);

    BOOST_HANA_STATIC_ASSERT(
        zip_with(list,
            list('1', '2', '3', '4'),
            list("1", "2", "3"),
            list(1, 2, 3, 4),
            list(list(), list(list()), list(list(list())))
        )
        ==
        list(
            list('1', "1", 1, list()),
            list('2', "2", 2, list(list())),
            list('3', "3", 3, list(list(list())))
        )
    );
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/sandbox/set.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>

#include <cassert>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto check_fold = [](auto ...elems) {
    auto result = lazy_foldr(on(cons, apply), list(), set(elems...));
    assert(elem(result, permutations(list(elems...))));
};

int main() {
    check_fold();
    check_fold(int_<1>);
    check_fold(int_<1>, int_<2>);
    check_fold(int_<1>, int_<2>, int_<3>);
    check_fold(int_<1>, int_<2>, int_<3>, int_<4>);
}

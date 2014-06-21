/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/sandbox/map.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>

#include <cassert>
using namespace boost::hana;
using sandbox::map;
using sandbox::pair;
using sandbox::second;


BOOST_HANA_CONSTEXPR_LAMBDA auto check_fold = [](auto ...pairs) {
    auto values = fmap(second, list(pairs...));
    auto result = lazy_foldr(on(cons, apply), list(), map(pairs...));
    assert(elem(result, permutations(values)));
};

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto k1 = int_<1>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto k2 = int_<2>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto k3 = int_<3>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto k4 = int_<4>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto v1 = char_<'1'>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto v2 = char_<'2'>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto v3 = char_<'3'>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto v4 = char_<'4'>;

    check_fold();
    check_fold(pair(k1, v1));
    check_fold(pair(k1, v1), pair(k2, v2));
    check_fold(pair(k1, v1), pair(k2, v2), pair(k3, v3));
    check_fold(pair(k1, v1), pair(k2, v2), pair(k3, v3), pair(k4, v4));
}

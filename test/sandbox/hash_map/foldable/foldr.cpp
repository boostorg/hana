/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/sandbox/hash_map.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/pair.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto check_fold = [](auto ...pairs) {
    auto values = fmap(second, list(pairs...));
    auto result = foldr(hash_map(pairs...), list(), cons);
    BOOST_HANA_CONSTANT_ASSERT(elem(permutations(values), result));
};

template <int k, int v>
BOOST_HANA_CONSTEXPR_LAMBDA auto p = pair(int_<k>, int_<v>);

int main() {
    check_fold();
    check_fold(p<1, 1>);
    check_fold(p<1, 1>, p<2, 2>);
    check_fold(p<1, 1>, p<2, 2>, p<3, 3>);
    check_fold(p<1, 1>, p<2, 2>, p<3, 3>, p<4, 4>);
}

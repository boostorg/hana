/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/hash_map.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/pair.hpp>

#include <boost/hana/tuple.hpp>
using namespace boost::hana;


template <int k, int v>
BOOST_HANA_CONSTEXPR_LAMBDA auto p = pair(int_<k>, int_<v>);

int main() {
    // foldr
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto check_fold = [](auto ...pairs) {
            auto values = tuple(second(pairs)...);
            auto result = foldr(hash_map(pairs...), tuple(), cons);
            BOOST_HANA_CONSTANT_CHECK(elem(permutations(values), result));
        };

        check_fold();
        check_fold(p<1, 1>);
        check_fold(p<1, 1>, p<2, 2>);
        check_fold(p<1, 1>, p<2, 2>, p<3, 3>);
        check_fold(p<1, 1>, p<2, 2>, p<3, 3>, p<4, 4>);
    }
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_permutation_of = curry<2>([](auto xs, auto perm) {
        return any(_ == perm, permutations(xs));
    });

    BOOST_HANA_STATIC_ASSERT(
        all(is_permutation_of(list('1', 2, 3.0)),
            list(
                list('1', 2, 3.0),
                list('1', 3.0, 2),
                list(2, '1', 3.0),
                list(2, 3.0, '1'),
                list(3.0, '1', 2),
                list(3.0, 2, '1')
            )
        )
    );
    //! [main]
}

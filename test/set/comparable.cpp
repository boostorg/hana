/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <test/injection.hpp>
#include <test/laws/comparable.hpp>
using namespace boost::hana;


int main() {
    using test::x;

    // equal
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto check = [](auto ...keys) {
            return and_(
                all(permutations(tuple(keys...)), [=](auto perm) {
                    return equal(to<Set>(perm), set(keys...));
                }),
                not_(equal(set(keys...), set(keys..., x<999>)))
            );
        };

        BOOST_HANA_CONSTANT_ASSERT(check());
        BOOST_HANA_CONSTANT_ASSERT(check(x<0>));
        BOOST_HANA_CONSTANT_ASSERT(check(x<0>, x<1>));
        BOOST_HANA_CONSTANT_ASSERT(check(x<0>, x<1>, x<2>));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable_laws(
            set(),
            set(x<0>),
            set(x<0>, x<1>),
            set(x<1>, x<0>),
            set(x<0>, x<1>, x<2>)
        ));
    }
}

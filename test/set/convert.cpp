/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set/comparable.hpp>
#include <boost/hana/set/convert.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
#include <test/minimal_list.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = test::minimal_list<>;
    using L = test::List<>;
    using test::x;

    // Set -> List
    {
        auto check = [=](auto ...xs) {
            BOOST_HANA_CONSTANT_ASSERT(
                elem(permutations(list(xs...)), to<L>(set(xs...)))
            );
        };
        check();
        check(x<1>);
        check(x<1>, x<2>);
        check(x<1>, x<2>, x<3>);
        check(x<1>, x<2>, x<3>, x<4>);
    }

    // List -> Set
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(list()),
            set()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(list(x<1>)),
            set(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(list(x<1>, x<2>)),
            set(x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(list(x<1>, x<2>, x<3>)),
            set(x<1>, x<2>, x<3>)
        ));
    }
}

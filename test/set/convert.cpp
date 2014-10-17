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
#include <test/seq/foldable.hpp>
#include <test/seq/list.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = test::seq;
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldable = test::seq;
    using L = test::Seq;
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

    // Foldable -> Set
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(foldable()),
            set()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(foldable(x<1>)),
            set(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(foldable(x<1>, x<1>)),
            set(x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(foldable(x<1>, x<2>)),
            set(x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(foldable(x<1>, x<2>, x<1>)),
            set(x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(foldable(x<1>, x<2>, x<2>)),
            set(x<1>, x<2>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(foldable(x<1>, x<2>, x<3>)),
            set(x<1>, x<2>, x<3>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Set>(foldable(x<1>, x<2>, x<3>, x<2>, x<1>)),
            set(x<1>, x<2>, x<3>)
        ));
    }
}

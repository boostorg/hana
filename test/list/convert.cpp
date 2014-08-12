/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
#include <test/seq/comparable.hpp>
#include <test/seq/convert.hpp>
#include <test/seq/foldable.hpp>
#include <test/seq/list.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldable = test::seq;
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = test::seq;
    using L = test::Seq;
    using test::x;

    // Foldable -> List
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<L>(foldable()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<L>(foldable(x<0>)),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<L>(foldable(x<0>, x<1>)),
            list(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<L>(foldable(x<0>, x<1>, x<2>)),
            list(x<0>, x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<L>(foldable(x<0>, x<1>, x<2>, x<3>)),
            list(x<0>, x<1>, x<2>, x<3>)
        ));
    }
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <test/injection.hpp>

#include <test/laws/foldable.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
    using test::x;

    // unpack
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(tuple(), f),
            f()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(tuple(x<0>), f),
            f(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(tuple(x<0>, x<1>), f),
            f(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(tuple(x<0>, x<1>, x<2>), f),
            f(x<0>, x<1>, x<2>)
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Foldable_laws(
            tuple(),
            tuple(x<0>),
            tuple(x<0>, x<1>),
            tuple(x<0>, x<1>, x<2>)
        ));
    }
}

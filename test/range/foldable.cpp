/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>

#include <test/injection.hpp>
#include <test/laws/foldable.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    // unpack
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(range(int_<0>, int_<0>), f),
            f()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(range(int_<0>, int_<1>), f),
            f(int_<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(range(int_<0>, int_<2>), f),
            f(int_<0>, int_<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(range(int_<0>, int_<3>), f),
            f(int_<0>, int_<1>, int_<2>)
        ));
    }

    // length
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(length(range(int_<0>, int_<0>)), int_<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(length(range(int_<0>, int_<1>)), int_<1>));
        BOOST_HANA_CONSTANT_ASSERT(equal(length(range(int_<0>, int_<2>)), int_<2>));
        BOOST_HANA_CONSTANT_ASSERT(equal(length(range(int_<4>, int_<4>)), int_<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(length(range(int_<4>, int_<10>)), int_<6>));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Foldable_laws(
            range(int_<0>, int_<0>),
            range(int_<0>, int_<1>),
            range(int_<3>, int_<10>),
            range(int_<20>, int_<20>),
            range(int_<4>, int_<8>)
        ));
    }
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>

#include <test/laws/comparable.hpp>
using namespace boost::hana;


template <typename T, typename U>
void tests() {
    // equal
    {
        // Integral == Integral
        BOOST_HANA_CONSTANT_ASSERT(equal(integral<T, 0>, integral<U, 0>));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(integral<T, 0>, integral<U, 1>)));


        // Integral == other
        BOOST_HANA_CONSTEXPR_ASSERT(equal(integral<T, 0>, U{0}));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(integral<T, 0>, U{1})));


        // other == Integral
        BOOST_HANA_CONSTEXPR_ASSERT(equal(U{0}, integral<T, 0>));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(U{1}, integral<T, 0>)));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Comparable_laws(
            integral<T, 0>, integral<U, 0>,
            integral<T, 1>, integral<U, 1>,
            T{0}, T{1}, U{0}, U{1}
        ));
    }
}

int main() {
    tests<int, int>();
    tests<int, long>();
}

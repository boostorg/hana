/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>

#include <test/laws/orderable.hpp>
using namespace boost::hana;


template <typename T, typename U>
void tests() {
    // less
    {
        // Integral < Integral
        BOOST_HANA_CONSTANT_ASSERT(less(integral<T, 0>, integral<U, 1>));
        BOOST_HANA_CONSTANT_ASSERT(not_(less(integral<T, 0>, integral<U, 0>)));
        BOOST_HANA_CONSTANT_ASSERT(not_(less(integral<T, 1>, integral<U, 0>)));

        // Integral < other
        BOOST_HANA_CONSTEXPR_ASSERT(less(integral<T, 0>, U{1}));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(less(integral<T, 0>, U{0})));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(less(integral<T, 1>, U{0})));

        // other < Integral
        BOOST_HANA_CONSTEXPR_ASSERT(less(T{0}, integral<U, 1>));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(less(T{0}, integral<U, 0>)));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(less(T{1}, integral<U, 0>)));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Orderable_laws(
            integral<T, 0>, integral<U, 0>,
            integral<T, 1>, integral<U, 1>,
            integral<T, 2>, integral<U, 2>
        ));
    }
}

int main() {
    tests<int, int>();
    tests<int, long long>();
}

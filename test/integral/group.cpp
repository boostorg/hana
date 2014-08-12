/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>

#include <test/laws/group.hpp>
#include <type_traits>
using namespace boost::hana;


template <typename T, typename U>
void tests() {
    // negate
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            negate(integral<T, 1>),
            integral<T, -1>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            negate(integral<U, 3>),
            integral<U, -3>
        ));
    }

    // minus
    {
        using C = std::common_type_t<T, U>;

        // Integral - Integral
        BOOST_HANA_CONSTANT_ASSERT(equal(
            minus(integral<T, 1>, integral<U, 3>),
            integral<C, 1 - 3>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            minus(integral<T, 4>, integral<U, 2>),
            integral<C, 4 - 2>
        ));

        // Integral - arithmetic
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            minus(integral<T, 1>, U{3}),
            C{1 - 3}
        ));

        // arithmetic - Integral
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            minus(T{1}, integral<U, 3>),
            C{1 - 3}
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Group_laws<Integral>(
            integral<T, 0>, integral<T, 1>, integral<T, 2>,
            integral<U, 0>, integral<U, 1>, integral<U, 2>
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(Group_laws<Integral>(
            integral<T, 0>, integral<T, 1>, integral<T, 2>,
            U{0}, U{1}, U{3}
        ));
    }
}

int main() {
    tests<int, short>();
    tests<int, int>();
    tests<int, long>();
}

/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


constexpr struct {
    template <typename I>
    constexpr auto operator()(I i) const { return i + int_<1>; }
} f{};

int main() {
    BOOST_HANA_STATIC_ASSERT(fmap(f, list()) == list());
    BOOST_HANA_STATIC_ASSERT(fmap(f, list(int_<1>)) == list(f(int_<1>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, list(int_<1>, int_<2>)) == list(f(int_<1>), f(int_<2>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, list(int_<1>, int_<2>, int_<3>)) == list(f(int_<1>), f(int_<2>), f(int_<3>)));
}

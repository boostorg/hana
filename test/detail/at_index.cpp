/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/at_index/overload_resolution.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <typename At>
constexpr void test_at(At at) {
    BOOST_HANA_STATIC_ASSERT(at(size_t<0>, int_<0>) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(at(size_t<0>, int_<0>, int_<1>) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(at(size_t<0>, int_<0>, int_<1>, int_<2>) == int_<0>);

    BOOST_HANA_STATIC_ASSERT(at(size_t<1>, int_<0>, int_<1>) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(at(size_t<1>, int_<0>, int_<1>, int_<2>) == int_<1>);

    BOOST_HANA_STATIC_ASSERT(at(size_t<2>, int_<0>, int_<1>, int_<2>) == int_<2>);

    BOOST_HANA_STATIC_ASSERT(at(size_t<0>, 0) == 0);
    BOOST_HANA_STATIC_ASSERT(at(size_t<1>, 0, '1') == '1');
    BOOST_HANA_STATIC_ASSERT(at(size_t<2>, 0, '1', "2") == "2");
}

int main() {
    test_at(detail::at_index::overload_resolution);
}

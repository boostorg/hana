/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/at_index/overload_resolution.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


struct non_pod { virtual ~non_pod() { } };

template <typename At>
constexpr void test_at(At at) {
    BOOST_HANA_CONSTANT_ASSERT(at(size_t<0>, int_<0>) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(at(size_t<0>, int_<0>, int_<1>) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(at(size_t<0>, int_<0>, int_<1>, int_<2>) == int_<0>);

    BOOST_HANA_CONSTANT_ASSERT(at(size_t<1>, int_<0>, int_<1>) == int_<1>);
    BOOST_HANA_CONSTANT_ASSERT(at(size_t<1>, int_<0>, int_<1>, int_<2>) == int_<1>);

    BOOST_HANA_CONSTANT_ASSERT(at(size_t<2>, int_<0>, int_<1>, int_<2>) == int_<2>);

    BOOST_HANA_CONSTEXPR_ASSERT(at(size_t<0>, 0) == 0);
    BOOST_HANA_CONSTEXPR_ASSERT(at(size_t<1>, 0, '1') == '1');
    BOOST_HANA_CONSTEXPR_ASSERT(at(size_t<2>, 0, '1', 2.2) == 2.2);

    BOOST_HANA_CONSTANT_ASSERT(at(size_t<0>, int_<0>, non_pod{}) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(at(size_t<1>, non_pod{}, int_<1>) == int_<1>);
}

int main() {
    test_at([](auto n, auto ...xs) {
        return detail::at_index::overload_resolution<n>(xs...);
    });
}

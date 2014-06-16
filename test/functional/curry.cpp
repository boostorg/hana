/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...xs) {
    return sizeof...(xs);
};

int main() {
    BOOST_HANA_STATIC_ASSERT(curry<0>(f)() == f());

    BOOST_HANA_STATIC_ASSERT(curry<1>(f)(1) == f(1));

    BOOST_HANA_STATIC_ASSERT(curry<2>(f)(1)('2') == f(1, '2'));
    BOOST_HANA_STATIC_ASSERT(curry<2>(f)(1, '2') == f(1, '2'));

    BOOST_HANA_STATIC_ASSERT(curry<3>(f)(1)('2')(3.3) == f(1, '2', 3.3));
    BOOST_HANA_STATIC_ASSERT(curry<3>(f)(1)('2', 3.3) == f(1, '2', 3.3));
    BOOST_HANA_STATIC_ASSERT(curry<3>(f)(1, '2')(3.3) == f(1, '2', 3.3));
    BOOST_HANA_STATIC_ASSERT(curry<3>(f)(1, '2', 3.3) == f(1, '2', 3.3));
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});

int main() {
    BOOST_HANA_CONSTANT_ASSERT(equal(curry<0>(f)(), f()));

    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<1>(f)(1), f(1)));

    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<2>(f)(1)('2'), f(1, '2')));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<2>(f)(1, '2'), f(1, '2')));

    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<3>(f)(1)('2')(3.3), f(1, '2', 3.3)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<3>(f)(1)('2', 3.3), f(1, '2', 3.3)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<3>(f)(1, '2')(3.3), f(1, '2', 3.3)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<3>(f)(1, '2', 3.3), f(1, '2', 3.3)));


    // Make sure curry is idempotent; this is important because it allows
    // currying a function in generic contexts where it is unknown whether
    // the function is already curried.
    BOOST_HANA_CONSTANT_ASSERT(equal(curry<0>(curry<0>(f))(), f()));

    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<1>(curry<1>(f))(1), f(1)));

    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<2>(curry<2>(f))(1)('2'), f(1, '2')));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<2>(curry<2>(f))(1, '2'), f(1, '2')));

    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<3>(curry<3>(f))(1)('2')(3.3), f(1, '2', 3.3)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<3>(curry<3>(f))(1)('2', 3.3), f(1, '2', 3.3)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<3>(curry<3>(f))(1, '2')(3.3), f(1, '2', 3.3)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal(curry<3>(curry<3>(f))(1, '2', 3.3), f(1, '2', 3.3)));
}

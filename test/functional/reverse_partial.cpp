/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional/reverse_partial.hpp>

#include <boost/hana/assert.hpp>

#include <laws/base.hpp>
using namespace boost::hana;


using test::ct_eq;

int main() {
    constexpr auto rp = reverse_partial;
    test::_injection<0> f{};

    BOOST_HANA_CONSTANT_CHECK(equal(
        rp(f)(),
        f()
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        rp(f)(ct_eq<1>{}),
        f(ct_eq<1>{})
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        rp(f)(ct_eq<1>{}, ct_eq<2>{}),
        f(ct_eq<1>{}, ct_eq<2>{})
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        rp(f)(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}),
        f(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        rp(f, ct_eq<1>{})(),
        f(ct_eq<1>{})
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        rp(f, ct_eq<1>{})(ct_eq<2>{}),
        f(ct_eq<2>{}, ct_eq<1>{})
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        rp(f, ct_eq<1>{})(ct_eq<2>{}, ct_eq<3>{}),
        f(ct_eq<2>{}, ct_eq<3>{}, ct_eq<1>{})
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        rp(f, ct_eq<1>{}, ct_eq<2>{})(),
        f(ct_eq<1>{}, ct_eq<2>{})
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        rp(f, ct_eq<1>{}, ct_eq<2>{})(ct_eq<3>{}),
        f(ct_eq<3>{}, ct_eq<1>{}, ct_eq<2>{})
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        rp(f, ct_eq<1>{}, ct_eq<2>{})(ct_eq<3>{}, ct_eq<4>{}),
        f(ct_eq<3>{}, ct_eq<4>{}, ct_eq<1>{}, ct_eq<2>{})
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        rp(f, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})(),
        f(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
    ));
}

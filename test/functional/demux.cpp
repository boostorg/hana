/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/functional/demux.hpp>

#include <boost/hana/assert.hpp>

#include <laws/base.hpp>
using namespace boost::hana;


using test::ct_eq;

int main() {
    test::_injection<0> f{};
    test::_injection<1> g{};
    test::_injection<2> h{};

    BOOST_HANA_CONSTANT_CHECK(equal(
        demux(f)()(),
        f()
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        demux(f)(g)(ct_eq<1>{}),
        f(g(ct_eq<1>{}))
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        demux(f)(g)(ct_eq<1>{}, ct_eq<2>{}),
        f(g(ct_eq<1>{}, ct_eq<2>{}))
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        demux(f)(g)(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}),
        f(g(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        demux(f)(g, h)(ct_eq<1>{}),
        f(g(ct_eq<1>{}), h(ct_eq<1>{}))
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        demux(f)(g, h)(ct_eq<1>{}, ct_eq<2>{}),
        f(g(ct_eq<1>{}, ct_eq<2>{}), h(ct_eq<1>{}, ct_eq<2>{}))
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        demux(f)(g, h)(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}),
        f(g(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}), h(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}))
    ));

    // Make sure we can pass move-only types when a single function
    // is demux'ed. In other words, make sure the arguments are
    // perfect-forwarded when a single function is demux'ed.
    {
        using test::move_only;
        demux(f)(g)(move_only{});
        demux(f)(g)(move_only{}, move_only{});
        demux(f)(g)(move_only{}, move_only{}, move_only{});
    }
}

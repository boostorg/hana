/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>

#include <test/cnumeric.hpp>
using namespace boost::hana;


template <int ...>
auto constant_true() { return test::make_cnumeric<bool, true>(); }

template <int ...>
constexpr bool constexpr_true() { return true; }

template <int ...>
bool runtime_true() { return true; }


BOOST_HANA_CONSTANT_ASSERT(constant_true<>());
BOOST_HANA_CONSTANT_ASSERT(constant_true<0, 1>());

BOOST_HANA_CONSTANT_CHECK(constant_true<>());
BOOST_HANA_CONSTANT_CHECK(constant_true<0, 1>());

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(constexpr_true<>());
    BOOST_HANA_CONSTEXPR_ASSERT(constexpr_true<0, 1>());

    BOOST_HANA_RUNTIME_ASSERT(runtime_true<>());
    BOOST_HANA_RUNTIME_ASSERT(runtime_true<0, 1>());

    BOOST_HANA_ASSERT(runtime_true<>());
    BOOST_HANA_ASSERT(runtime_true<0, 1>());
    BOOST_HANA_ASSERT(constant_true<>());
    BOOST_HANA_ASSERT(constant_true<0, 1>());


    BOOST_HANA_CONSTEXPR_CHECK(constexpr_true<>());
    BOOST_HANA_CONSTEXPR_CHECK(constexpr_true<0, 1>());

    BOOST_HANA_RUNTIME_CHECK(runtime_true<>());
    BOOST_HANA_RUNTIME_CHECK(runtime_true<0, 1>());

    BOOST_HANA_CHECK(runtime_true<>());
    BOOST_HANA_CHECK(runtime_true<0, 1>());
    BOOST_HANA_CHECK(constant_true<>());
    BOOST_HANA_CHECK(constant_true<0, 1>());

    // make sure we can put lambdas in the assertions
    {
        BOOST_HANA_CONSTANT_ASSERT([]{ return constant_true<>(); }());
        BOOST_HANA_CONSTEXPR_ASSERT([]{ return constexpr_true<>(); }());
        BOOST_HANA_RUNTIME_ASSERT([]{ return runtime_true<>(); }());
        BOOST_HANA_ASSERT([] { return constant_true<>(); }());
        BOOST_HANA_ASSERT([] { return runtime_true<>(); }());

        BOOST_HANA_CONSTANT_CHECK([]{ return constant_true<>(); }());
        BOOST_HANA_CONSTEXPR_CHECK([]{ return constexpr_true<>(); }());
        BOOST_HANA_RUNTIME_CHECK([]{ return runtime_true<>(); }());
        BOOST_HANA_CHECK([] { return constant_true<>(); }());
        BOOST_HANA_CHECK([] { return runtime_true<>(); }());
    }
}

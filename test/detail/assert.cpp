/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
using namespace boost::hana;


template <int ...>
auto constant_true = true_;

template <int ...>
BOOST_HANA_CONSTEXPR_LAMBDA bool constexpr_true = []{ return true; }();

template <int ...>
bool runtime_true = true;


BOOST_HANA_CONSTANT_ASSERT(constant_true<>);
BOOST_HANA_CONSTANT_ASSERT(constant_true<0, 1>);

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(constexpr_true<>);
    BOOST_HANA_CONSTEXPR_ASSERT(constexpr_true<0, 1>);

    BOOST_HANA_RUNTIME_ASSERT(runtime_true<>);
    BOOST_HANA_RUNTIME_ASSERT(runtime_true<0, 1>);
}

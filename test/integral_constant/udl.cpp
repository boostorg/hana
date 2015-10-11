/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/less.hpp>
#include <boost/hana/negate.hpp>

#include <type_traits>
namespace hana = boost::hana;
using namespace hana::literals;


BOOST_HANA_CONSTANT_CHECK(0_c == hana::llong_c<0>);
BOOST_HANA_CONSTANT_CHECK(1_c == hana::llong_c<1>);
BOOST_HANA_CONSTANT_CHECK(12_c == hana::llong_c<12>);
BOOST_HANA_CONSTANT_CHECK(123_c == hana::llong_c<123>);
BOOST_HANA_CONSTANT_CHECK(1234567_c == hana::llong_c<1234567>);
BOOST_HANA_CONSTANT_CHECK(-34_c == hana::llong_c<-34>);

static_assert(std::is_same<
    decltype(-1234_c)::value_type,
    long long
>{}, "");
static_assert(-1234_c == -1234ll, "");
BOOST_HANA_CONSTANT_CHECK(-12_c < 0_c);

int main() { }

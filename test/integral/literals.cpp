/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;
using namespace literals;


int main() {
    BOOST_HANA_CONSTANT_ASSERT(0_c == llong<0>);
    BOOST_HANA_CONSTANT_ASSERT(1_c == llong<1>);
    BOOST_HANA_CONSTANT_ASSERT(12_c == llong<12>);
    BOOST_HANA_CONSTANT_ASSERT(123_c == llong<123>);
    BOOST_HANA_CONSTANT_ASSERT(1234567_c == llong<1234567>);
    BOOST_HANA_CONSTANT_ASSERT(-34_c == llong<-34>);

    BOOST_HANA_CONSTANT_ASSERT(decltype_(-1234_c) == decltype_(llong<-1234>));
    BOOST_HANA_CONSTANT_ASSERT(-12_c < 0_c);
}

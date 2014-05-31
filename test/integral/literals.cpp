/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;
using namespace literals;


int main() {
    BOOST_HANA_STATIC_ASSERT(0_c == long_<0>);
    BOOST_HANA_STATIC_ASSERT(1_c == long_<1>);
    BOOST_HANA_STATIC_ASSERT(12_c == long_<12>);
    BOOST_HANA_STATIC_ASSERT(123_c == long_<123>);
    BOOST_HANA_STATIC_ASSERT(1234567_c == long_<1234567>);
}

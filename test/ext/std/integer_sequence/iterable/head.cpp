/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integer_sequence.hpp>

#include <boost/hana/detail/assert.hpp>

#include <utility>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(head(std::index_sequence<0>{}) == 0);
    BOOST_HANA_CONSTEXPR_ASSERT(head(std::index_sequence<0, 1>{}) == 0);
}

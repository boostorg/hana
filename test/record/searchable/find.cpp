/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include "../udt.hpp"
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto k) {
    return [=](auto key) { return equal(k, key); };
};

int main() {
    BOOST_HANA_STATIC_ASSERT(find(is(member1), udt{0, '0'}) == just(0));
    BOOST_HANA_STATIC_ASSERT(find(is(member2), udt{0, '0'}) == just('0'));
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % int_<2> != int_<0>;
};

int main() {
    BOOST_HANA_STATIC_ASSERT(decltype_(any(odd, list(int_<1>, int_<2>))) == decltype_(true_));
    BOOST_HANA_STATIC_ASSERT(decltype_(any(odd, list(int_<2>, int_<4>))) == decltype_(false_));
}

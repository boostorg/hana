/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>

#include <type_traits>
using namespace boost::hana;


//! @todo Fix this.
BOOST_HANA_CONSTEXPR_LAMBDA auto is_integral = [](auto x) {
    return bool_<std::is_integral<decltype(x)>::value>;
};

int main() {
    BOOST_HANA_STATIC_ASSERT(filter(is_integral, list(1, 2.0, 3, 4.0)) == list(1, 3));
}

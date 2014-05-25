/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/maybe.hpp>

#include <type_traits>
using namespace boost::hana;


//! @todo
//! This is unsatisfying. We should be able to say something
//! like `lift<std::is_integral>`.
BOOST_HANA_CONSTEXPR_LAMBDA auto is_integral = [](auto x) {
    return bool_<std::is_integral<decltype(x)>::value>;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto is_class = [](auto x) {
    return bool_<std::is_class<decltype(x)>::value>;
};

int main() {
    BOOST_HANA_STATIC_ASSERT(find(is_integral, list(1.0, 2)) == just(2));
    BOOST_HANA_STATIC_ASSERT(find(is_class, list(1.0, 2)) == nothing);
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;
using namespace literals;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
        return x % 2_c != 0_c;
    };

    BOOST_HANA_CONSTEXPR_ASSERT(all(list(1, 3), odd));
    BOOST_HANA_CONSTANT_ASSERT(!all(list(3_c, 4_c), odd));

    BOOST_HANA_CONSTANT_ASSERT(
        !all(list(type<void>, type<char&>), trait<std::is_void>)
    );
    BOOST_HANA_CONSTANT_ASSERT(
        all(list(type<int>, type<char>), trait<std::is_integral>)
    );
    //! [main]
}

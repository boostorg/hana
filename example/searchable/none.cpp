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

    BOOST_HANA_CONSTANT_ASSERT(none(odd, list(2_c, 4_c)));
    BOOST_HANA_CONSTEXPR_ASSERT(!none(odd, list(1, 2)));

    BOOST_HANA_CONSTANT_ASSERT(
        !none(trait<std::is_void>, list(type<void>, type<char&>))
    );
    BOOST_HANA_CONSTANT_ASSERT(
        none(trait<std::is_integral>, list(type<void>, type<char&>))
    );
    //! [main]
}

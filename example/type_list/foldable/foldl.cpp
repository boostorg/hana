/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_integral_constant.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>
using namespace boost::hana;
using namespace literals;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto types = type_list<long, float, short, double, float, long, long double>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto count_if_float = [](auto n, auto type) {
        return if_(trait<std::is_floating_point>(type), n + 1_c, n);
    };

    BOOST_HANA_STATIC_ASSERT(foldl(count_if_float, 0_c, types) == 4_c);
    //! [main]
}

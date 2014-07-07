/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

#include <type_traits>
using namespace boost::hana;
using namespace literals;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto types = type_list<int, char, long, short, char, long, double, long>;
    BOOST_HANA_STATIC_ASSERT(count(trait<std::is_floating_point>, types) == 1_c);
    BOOST_HANA_STATIC_ASSERT(count(_ == type<char>, types) == 2_c);
    BOOST_HANA_STATIC_ASSERT(count(_ == type<void>, types) == 0_c);
    //! [main]
}

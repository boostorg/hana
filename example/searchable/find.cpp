/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(find(list(1.0, 2, '3'), trait_<std::is_integral>) == just(2));
    BOOST_HANA_CONSTANT_ASSERT(find(list(1.0, 2, '3'), trait_<std::is_class>) == nothing);

    constexpr auto types = type_list<char, int, unsigned, long, unsigned long>;
    BOOST_HANA_CONSTANT_ASSERT(find(types, _ == type<unsigned>) == just(type<unsigned>));
    BOOST_HANA_CONSTANT_ASSERT(find(types, _ == type<void>) == nothing);
    //! [main]
}

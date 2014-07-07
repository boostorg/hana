/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(find(trait_<std::is_integral>, list(1.0, 2, '3')) == just(2));
    BOOST_HANA_STATIC_ASSERT(find(trait_<std::is_class>, list(1.0, 2, '3')) == nothing);
    //! [main]
}

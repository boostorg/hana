/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto types = type_list<char, int, unsigned, long, unsigned long>;
    BOOST_HANA_STATIC_ASSERT(find(_ == type<unsigned>, types) == just(type<unsigned>));
    BOOST_HANA_STATIC_ASSERT(find(_ == type<void>, types) == nothing);
    //! [main]
}

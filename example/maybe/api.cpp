/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


int main() {
    //! [from_maybe]
    BOOST_HANA_STATIC_ASSERT(from_maybe(0, just(1)) == 1);
    BOOST_HANA_STATIC_ASSERT(from_maybe(0, nothing) == 0);
    //! [from_maybe]

    //! [maybe]
    BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) { return x + 1; };
    BOOST_HANA_STATIC_ASSERT(maybe(0, inc, just(1)) == 2);
    BOOST_HANA_STATIC_ASSERT(maybe(0, inc, nothing) == 0);
    //! [maybe]
}

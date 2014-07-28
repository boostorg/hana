/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    auto m = map(
        pair(type<int>, 'i'),
        pair(type<float>, 'f')
    );
    BOOST_HANA_STATIC_ASSERT(lookup(type<int>, m) == just('i'));
    BOOST_HANA_STATIC_ASSERT(lookup(type<float>, m) == just('f'));
    BOOST_HANA_STATIC_ASSERT(lookup(type<void>, m) == nothing);
    BOOST_HANA_STATIC_ASSERT(lookup(int_<3>, m) == nothing);
    //! [main]
}

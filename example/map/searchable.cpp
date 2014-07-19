/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair/instance.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    auto m = map(
        pair(type<int>, 'i'),
        pair(type<float>, 'f')
    );
    BOOST_HANA_STATIC_ASSERT(find_key(type<int>, m) == just('i'));
    BOOST_HANA_STATIC_ASSERT(find_key(type<float>, m) == just('f'));
    BOOST_HANA_STATIC_ASSERT(find_key(type<void>, m) == nothing);
    //! [main]
}

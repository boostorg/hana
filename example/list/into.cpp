/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/maybe.hpp>

#include <tuple>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(unpack(into<List>, just(3)) == list(3));
    BOOST_HANA_STATIC_ASSERT(unpack(into<List>, nothing) == list());
    BOOST_HANA_STATIC_ASSERT(unpack(into<List>, std::make_tuple(1, '2', 3.3)) == list(1, '2', 3.3));
    //! [main]
}

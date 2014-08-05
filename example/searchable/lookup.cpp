/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(lookup(list(int_<1>, type<int>, '3'), type<int>) == just(type<int>));
    BOOST_HANA_CONSTANT_ASSERT(lookup(list(int_<1>, type<int>, '3'), type<void>) == nothing);

    BOOST_HANA_CONSTEXPR_LAMBDA auto m = map(
        pair(1, 'x'),
        pair(type<float>, 3.3),
        pair(type<char>, type<int>)
    );
    BOOST_HANA_CONSTEXPR_ASSERT(lookup(m, type<float>) == just(3.3));
    //! [main]
}

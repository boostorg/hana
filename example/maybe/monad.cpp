/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) {
        return just(x + 1);
    };

    BOOST_HANA_CONSTEXPR_CHECK(bind(just(1), inc) == just(2));
    BOOST_HANA_CONSTANT_CHECK(bind(nothing, inc) == nothing);

    BOOST_HANA_CONSTEXPR_CHECK(flatten(just(just(2))) == just(2));
    //! [main]
}

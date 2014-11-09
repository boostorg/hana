/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
        return x % int_<2> != int_<0>;
    };

    BOOST_HANA_CONSTANT_CHECK(find(just(int_<3>), odd) == just(int_<3>));
    BOOST_HANA_CONSTANT_CHECK(find(just(int_<2>), odd) == nothing);
    BOOST_HANA_CONSTANT_CHECK(find(nothing, odd) == nothing);

    BOOST_HANA_CONSTANT_CHECK(all(just(int_<3>), odd));
    BOOST_HANA_CONSTANT_CHECK(all(nothing, odd));
    //! [main]
}

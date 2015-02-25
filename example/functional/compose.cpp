/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto to_char = [](int x) {
        return static_cast<char>(x + 48);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto increment = [](auto x) {
        return x + 1;
    };

    BOOST_HANA_CONSTEXPR_CHECK(compose(to_char, increment)(3) == '4');
    //! [main]
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>

#include <tuple>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto make_tuple = [](auto ...xs) {
        return std::make_tuple(xs...);
    };

    BOOST_HANA_CONSTEXPR_ASSERT(
        demux(make_tuple, _ + _, _ - _, _ * _, _ / _)(10, 4)
        ==
        make_tuple(10 + 4, 10 - 4, 10 * 4, 10 / 4)
    );
    //! [main]
}

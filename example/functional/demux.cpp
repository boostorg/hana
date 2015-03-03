/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_CHECK(
        demux(make<Tuple>)(
            _ + _,
            _ - _,
            _ * _,
            _ / _
        )(10, 4)
        ==
        make<Tuple>(10 + 4, 10 - 4, 10 * 4, 10 / 4)
    );
    //! [main]
}

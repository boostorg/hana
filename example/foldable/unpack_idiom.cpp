/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto add = [](auto x, auto y) {
        return x + y;
    };

    // Would be `boost::fusion::make_fused(add)` in Boost.Fusion.
    BOOST_HANA_CONSTEXPR_LAMBDA auto add_seq = partial(flip(unpack), add);

    BOOST_HANA_CONSTEXPR_ASSERT(add_seq(list(1, 2)) == add(1, 2));
    //! [main]
}

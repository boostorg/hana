/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/range.hpp>
using namespace boost::hana;
using namespace literals;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto positive = [](auto x) {
        return x > int_<0>;
    };

    BOOST_HANA_STATIC_ASSERT(
        drop_until(positive, range(int_<-3>, int_<6>)) == range(int_<1>, int_<6>)
    );

    BOOST_HANA_STATIC_ASSERT(
        drop_until(positive, list(1_c, -2_c, 4_c, 5_c)) == list(1_c, -2_c, 4_c, 5_c)
    );
    //! [main]
}

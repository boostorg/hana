/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/at_key.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
namespace hana = boost::hana;


int main() {
    // using at_key
    {
        auto xs = hana::make_map(
            hana::make_pair(hana::int_c<0>, 0),
            hana::make_pair(hana::int_c<1>, '1'),
            hana::make_pair(hana::int_c<2>, 2.2)
        );

        BOOST_HANA_RUNTIME_CHECK(hana::at_key(xs, hana::int_c<0>) == 0);
        BOOST_HANA_RUNTIME_CHECK(hana::at_key(xs, hana::int_c<1>) == '1');
        BOOST_HANA_RUNTIME_CHECK(hana::at_key(xs, hana::int_c<2>) == 2.2);

        hana::at_key(xs, hana::int_c<0>) = 9;
        hana::at_key(xs, hana::int_c<1>) = '9';
        hana::at_key(xs, hana::int_c<2>) = 9.9;

        BOOST_HANA_RUNTIME_CHECK(hana::at_key(xs, hana::int_c<0>) == 9);
        BOOST_HANA_RUNTIME_CHECK(hana::at_key(xs, hana::int_c<1>) == '9');
        BOOST_HANA_RUNTIME_CHECK(hana::at_key(xs, hana::int_c<2>) == 9.9);
    }

    // using operator[]
    {
        auto xs = hana::make_map(
            hana::make_pair(hana::int_c<0>, 0),
            hana::make_pair(hana::int_c<1>, '1'),
            hana::make_pair(hana::int_c<2>, 2.2)
        );

        BOOST_HANA_RUNTIME_CHECK(xs[hana::int_c<0>] == 0);
        BOOST_HANA_RUNTIME_CHECK(xs[hana::int_c<1>] == '1');
        BOOST_HANA_RUNTIME_CHECK(xs[hana::int_c<2>] == 2.2);

        xs[hana::int_c<0>] = 9;
        xs[hana::int_c<1>] = '9';
        xs[hana::int_c<2>] = 9.9;

        BOOST_HANA_RUNTIME_CHECK(xs[hana::int_c<0>] == 9);
        BOOST_HANA_RUNTIME_CHECK(xs[hana::int_c<1>] == '9');
        BOOST_HANA_RUNTIME_CHECK(xs[hana::int_c<2>] == 9.9);
    }
}

/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/tuple.hpp>

#include <functional>
namespace hana = boost::hana;


int main() {
    int i = 0;
    char c = 'x';
    hana::tuple<int&, char const&> refs = hana::make_tuple(std::ref(i), std::cref(c));

    int& i_ref = hana::at_c<0>(refs);
    BOOST_HANA_RUNTIME_CHECK(&i_ref == &i);

    i_ref = 3;
    BOOST_HANA_RUNTIME_CHECK(i == 3);

    char const& c_ref = hana::at_c<1>(refs);
    BOOST_HANA_RUNTIME_CHECK(&c_ref == &c);
    BOOST_HANA_RUNTIME_CHECK(c == 'x');
}

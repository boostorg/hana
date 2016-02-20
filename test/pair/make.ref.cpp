/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/second.hpp>

#include <functional>
namespace hana = boost::hana;


int main() {
    int i = 0;
    char c = 'x';
    hana::pair<int&, char const&> refs = hana::make_pair(std::ref(i), std::cref(c));

    int& i_ref = hana::first(refs);
    BOOST_HANA_RUNTIME_CHECK(&i_ref == &i);

    i_ref = 3;
    BOOST_HANA_RUNTIME_CHECK(i == 3);

    char const& c_ref = hana::second(refs);
    BOOST_HANA_RUNTIME_CHECK(&c_ref == &c);
    BOOST_HANA_RUNTIME_CHECK(c == 'x');
}

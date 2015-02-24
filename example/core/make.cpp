/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
using namespace boost::hana;


int main() {

{

//! [make]
_tuple<int, char, double, std::string> ts{1, '2', 3.3, "abcd"};
BOOST_HANA_RUNTIME_CHECK(ts == make<Tuple>(1, '2', 3.3, std::string{"abcd"}));

std::string foo{"foo"}; // std::string has no notion of data type
BOOST_HANA_RUNTIME_CHECK(make<std::string>("foo") == foo);
//! [make]

}

}

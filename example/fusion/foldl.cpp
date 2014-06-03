/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/list.hpp>

#include <boost/lexical_cast.hpp>
#include <cassert>
#include <string>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto make_string = [](std::string str, auto x)
{ return str + boost::lexical_cast<std::string>(x); };

int main() {
    assert(foldl(make_string, "1", list(2, "3", '4')) == "1234");
}

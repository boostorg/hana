/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/list.hpp>

#include <boost/lexical_cast.hpp>
#include <cassert>
#include <string>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto make_string = [](auto x, std::string str)
{ return boost::lexical_cast<std::string>(x) + str; };

int main() {
    assert(foldr(make_string, "4", list(1, "2", '3')) == "1234");
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>

#include <cassert>
#include <string>
using namespace boost::hana;


int main() {
    //! [main]
    assert(
        map(
            pair(char_<'a'>, std::string{"foobar"}),
            pair(type<int&&>, nullptr)
        )
        ==
        map(
            pair(type<int&&>, (void*)0),
            pair(char_<'a'>, "foobar")
        )
    );
    //! [main]
}

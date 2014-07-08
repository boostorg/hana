/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/tuple.hpp>

#include <boost/tuple/tuple.hpp>
#include <cassert>
using namespace boost::hana;


int main() {
    assert(head(boost::make_tuple(1)) == 1);
    assert(head(boost::make_tuple(1, '2')) == 1);
    assert(head(boost::make_tuple(1, '2', 3.3)) == 1);
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <cassert>
#include <vector>
using namespace boost::hana;


int main() {
    //! [main]
    std::vector<int> v{1, 2, 3};
    assert(type<std::vector<int>>({1, 2, 3}) == v);
    //! [main]
}

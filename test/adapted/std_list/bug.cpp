/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_list.hpp>

#include <boost/hana/integral.hpp>

#include <cassert>
#include <list>
using namespace boost::hana;


int main() {
    using std_list = std::list<int>;
    assert(at(int_<2>, std_list{0, 1, 2, 3}) == 2);

#if 0
    assert(at(2, std_list{0, 1, 2, 3}) == 2);
    assert(last(std_list{0, 1, 2}) == 2);


    assert(length(std_list{}) == 0);
    assert(length(std_list{1}) == 1);
    assert(length(std_list{1, 2}) == 2);
#endif
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_list.hpp>

#include <cassert>
#include <list>
using namespace boost::hana;


int main() {
    using std_list = std::list<int>;
    assert(is_empty(std_list{}));
    assert(!is_empty(std_list{1}));
    assert(!is_empty(std_list{1, 2}));

    assert(head(std_list{1}) == 1);
    assert(head(std_list{1, 2}) == 1);
    assert(head(std_list{1, 2, 3}) == 1);

    assert(tail(std_list{1}) == std_list{});
    assert(tail(std_list{1, 2}) == std_list{2});
    assert(tail(std_list{1, 2, 3}) == (std_list{2, 3}));
}

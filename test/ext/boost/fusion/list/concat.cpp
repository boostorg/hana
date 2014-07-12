/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion.hpp>

#include "../helper.hpp"
#include <cassert>
using namespace boost::hana;


int main() {
    with_nonassociative_forward_sequences([](auto container) {
        assert(concat(container(), container()) == container());
        assert(concat(container(1), container()) == container(1));
        assert(concat(container(), container(1)) == container(1));
        assert(concat(container(1), container('2')) == container(1, '2'));
        assert(concat(container(1, '2'), container(3.3)) == container(1, '2', 3.3));
    });
}

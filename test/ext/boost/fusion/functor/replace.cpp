/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion.hpp>

#include <boost/hana/detail/constexpr.hpp>

#include "../helper.hpp"
#include <cassert>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto is_even = [](auto x) {
    return x % 2 == 0;
};

int main() {
    with_nonassociative_forward_sequences([](auto container) {
        assert(replace(is_even, 'x', container()) == container());
        assert(replace(is_even, 'x', container(0)) == container('x'));
        assert(replace(is_even, 'x', container(0, 1)) == container('x', 1));
        assert(replace(is_even, 'x', container(0, 1, 2)) == container('x', 1, 'x'));
        assert(replace(is_even, 'x', container(0, 1, 2, 3)) == container('x', 1, 'x', 3));
    });
}

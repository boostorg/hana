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
    with_nonassociative_forward_sequences([=](auto container) {
        assert(count(is_even, container()) == 0);
        assert(count(is_even, container(1)) == 0);
        assert(count(is_even, container(2)) == 1);
        assert(count(is_even, container(1, 2)) == 1);
        assert(count(is_even, container(1, 2, 3)) == 1);
        assert(count(is_even, container(1, 2, 3, 4)) == 2);
    });
}

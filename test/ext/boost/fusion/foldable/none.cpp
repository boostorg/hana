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
        assert( none(is_even, container()));
        assert( none(is_even, container(1)));
        assert(!none(is_even, container(2)));
        assert(!none(is_even, container(1, 2)));
        assert( none(is_even, container(1, 3)));
        assert(!none(is_even, container(1, 3, 4)));
    });
}

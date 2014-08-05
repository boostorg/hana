/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>

#include "../helper.hpp"
using namespace boost::hana;


int main() {
    with_nonassociative_forward_sequences([](auto container) {
        BOOST_HANA_CONSTANT_ASSERT(length(container()) == size_t<0>);
        BOOST_HANA_CONSTANT_ASSERT(length(container(1)) == size_t<1>);
        BOOST_HANA_CONSTANT_ASSERT(length(container(1, '2')) == size_t<2>);
        BOOST_HANA_CONSTANT_ASSERT(length(container(1, '2', 3.3)) == size_t<3>);
    });
}

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion/sequence.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical/logical.hpp>

#include "helper.hpp"
using namespace boost::hana;


int main() {
    with_nonassociative_forward_sequences([](auto container) {
        // at
        {
            BOOST_HANA_RUNTIME_ASSERT(at(size_t<0>, container(1)) == 1);
            BOOST_HANA_RUNTIME_ASSERT(at(size_t<0>, container(1, '2')) == 1);
            BOOST_HANA_RUNTIME_ASSERT(at(size_t<0>, container(1, '2', 3.3)) == 1);

            BOOST_HANA_RUNTIME_ASSERT(at(size_t<1>, container(1, '2')) == '2');
            BOOST_HANA_RUNTIME_ASSERT(at(size_t<1>, container(1, '2', 3.3)) == '2');

            BOOST_HANA_RUNTIME_ASSERT(at(size_t<2>, container(1, '2', 3.3)) == 3.3);
        }

        // head
        {
            BOOST_HANA_RUNTIME_ASSERT(head(container(1)) == 1);
            BOOST_HANA_RUNTIME_ASSERT(head(container(1, '2')) == 1);
            BOOST_HANA_RUNTIME_ASSERT(head(container(1, '2', 3.3)) == 1);
        }

        // is_empty
        {
            BOOST_HANA_CONSTANT_ASSERT(is_empty(container()));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(container(1))));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(container(1, '2'))));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(container(1, '2', 3.3))));
        }

        // last
        {
            BOOST_HANA_RUNTIME_ASSERT(last(container(1)) == 1);
            BOOST_HANA_RUNTIME_ASSERT(last(container(1, '2')) == '2');
            BOOST_HANA_RUNTIME_ASSERT(last(container(1, '2', 3.3)) == 3.3);
        }

        // tail
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(tail(container(1)), container()));
            BOOST_HANA_RUNTIME_ASSERT(equal(tail(container(1, '2')), container('2')));
            BOOST_HANA_RUNTIME_ASSERT(equal(tail(container(1, '2', 3.3)), container('2', 3.3)));
        }
    });
}

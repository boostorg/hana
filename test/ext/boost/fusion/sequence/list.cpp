/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion/sequence.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/foreign.hpp>

#include "helper.hpp"
using namespace boost::hana;


int main() {
    with_nonassociative_forward_sequences([](auto container) {
        // init
        {
            BOOST_HANA_CONSTANT_CHECK(equal(init(container(1)), container()));
            BOOST_HANA_RUNTIME_CHECK(equal(init(container(1, '2')), container(1)));
            BOOST_HANA_RUNTIME_CHECK(equal(init(container(1, '2', 3.3)), container(1, '2')));
        }

        // concat
        {
            BOOST_HANA_CONSTANT_CHECK(equal(concat(container(), container()), container()));
            BOOST_HANA_RUNTIME_CHECK(equal(concat(container(1), container()), container(1)));
            BOOST_HANA_RUNTIME_CHECK(equal(concat(container(), container(1)), container(1)));
            BOOST_HANA_RUNTIME_CHECK(equal(concat(container(1), container('2')), container(1, '2')));
            BOOST_HANA_RUNTIME_CHECK(equal(concat(container(1, '2'), container(3.3)), container(1, '2', 3.3)));
        }

        // cons
        {
            BOOST_HANA_RUNTIME_CHECK(equal(cons(1, container()), container(1)));
            BOOST_HANA_RUNTIME_CHECK(equal(cons(1, container('2')), container(1, '2')));
            BOOST_HANA_RUNTIME_CHECK(equal(cons(1, container('2', 3.3)), container(1, '2', 3.3)));
        }

        // nil
        {
            BOOST_HANA_CONSTANT_CHECK(equal(nil<ext::boost::fusion::Sequence>, container()));
        }

        // reverse
        {
            BOOST_HANA_CONSTANT_CHECK(equal(reverse(container()), container()));
            BOOST_HANA_RUNTIME_CHECK(equal(reverse(container(1)), container(1)));
            BOOST_HANA_RUNTIME_CHECK(equal(reverse(container(1, '2')), container('2', 1)));
            BOOST_HANA_RUNTIME_CHECK(equal(reverse(container(1, '2', 3.3)), container(3.3, '2', 1)));
        }

        // snoc
        {
            BOOST_HANA_RUNTIME_CHECK(equal(snoc(container(), 1), container(1)));
            BOOST_HANA_RUNTIME_CHECK(equal(snoc(container(1), '2'), container(1, '2')));
            BOOST_HANA_RUNTIME_CHECK(equal(snoc(container(1, '2'), 3.3), container(1, '2', 3.3)));
        }
    });
}

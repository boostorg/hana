/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion/sequence.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp>

#include "helper.hpp"
#include <test/injection.hpp>
#include <test/laws/functor.hpp>
#include <test/numeric/comparable.hpp>
using namespace boost::hana;


int main() {
    with_nonassociative_forward_sequences([](auto container) {
        // fmap
        {
            BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

            BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, container()), container()));
            BOOST_HANA_RUNTIME_ASSERT(equal(fmap(f, container(1)), container(f(1))));
            BOOST_HANA_RUNTIME_ASSERT(equal(fmap(f, container(1, '2')), container(f(1), f('2'))));
            BOOST_HANA_RUNTIME_ASSERT(equal(fmap(f, container(1, '2', 3.3)), container(f(1), f('2'), f(3.3))));
        }

        // replace
        {
            BOOST_HANA_CONSTEXPR_LAMBDA auto is_even = [](auto x) {
                return x % 2 == 0;
            };

            BOOST_HANA_CONSTANT_ASSERT(equal(replace(is_even, 'x', container()), container()));
            BOOST_HANA_RUNTIME_ASSERT( equal(replace(is_even, 'x', container(0)), container('x')));
            BOOST_HANA_RUNTIME_ASSERT( equal(replace(is_even, 'x', container(0, 1)), container('x', 1)));
            BOOST_HANA_RUNTIME_ASSERT( equal(replace(is_even, 'x', container(0, 1, 2)), container('x', 1, 'x')));
            BOOST_HANA_RUNTIME_ASSERT( equal(replace(is_even, 'x', container(0, 1, 2, 3)), container('x', 1, 'x', 3)));
        }

        // laws
        {
            BOOST_HANA_CONSTEXPR_LAMBDA auto x = test::numeric;
            BOOST_HANA_RUNTIME_ASSERT(Functor_laws(
                container(),
                container(x(0)),
                container(x(0), x(1)),
                container(x(0), x(1), x(2))
            ));
        }
    });
}

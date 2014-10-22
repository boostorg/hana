/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional/compose.hpp>

#include <test/injection.hpp>
#include <test/laws/monad.hpp>
#include <test/lazy_comparable.hpp>

#include <array>
#include <iostream>
using namespace boost::hana;


int main() {
    using test::x;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = compose(lazy, test::injection([]{}));

    // bind
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(bind(lazy(x<0>), f), f(x<0>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(bind(lazy(x<1>), f), f(x<1>)));
    }

    // flatten
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(flatten(lazy(lazy(x<0>))), lazy(x<0>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(flatten(lazy(lazy(x<1>))), lazy(x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(flatten(lazy(lazy(lazy(x<1>)))), lazy(lazy(x<1>))));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Monad_laws<Lazy>
            // monads
            (
                lazy(x<0>), lazy(x<1>), lazy(x<2>)
            )

            // objects
            (
                x<0>, x<1>, x<2>, x<3>
            )
        );
    }

    // Make sure the monadic chain is evaluated in the right order.
    {
        std::array<bool, 3> executed = {{false, false, false}};
        int dummy = 0;

        std::cerr << "creating the monadic chain...\n";
        auto chain = lazy(dummy)
            | [&](int dummy) {
                std::cerr << "executing the first computation...\n";
                executed[0] = true;
                BOOST_HANA_RUNTIME_ASSERT(executed == std::array<bool, 3>{{true, false, false}});
                return lazy(dummy);
            }
            | [&](int dummy) {
                std::cerr << "executing the second computation...\n";
                executed[1] = true;
                BOOST_HANA_RUNTIME_ASSERT(executed == std::array<bool, 3>{{true, true, false}});
                return lazy(dummy);
            }
            | [&](int dummy) {
                std::cerr << "executing the third computation...\n";
                executed[2] = true;
                BOOST_HANA_RUNTIME_ASSERT(executed == std::array<bool, 3>{{true, true, true}});
                return lazy(dummy);
            };

        BOOST_HANA_RUNTIME_ASSERT(executed == std::array<bool, 3>{{false, false, false}});

        std::cerr << "evaluating the chain...\n";
        eval(chain);
    }
}

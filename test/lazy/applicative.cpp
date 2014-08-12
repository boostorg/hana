/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
#include <test/lazy_comparable.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    BOOST_HANA_CONSTEXPR_LAMBDA auto invalid = [](auto x) {
        return x.this_function_must_not_be_instantiated;
    };

    using test::x;

    // ap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(lazy(f), lazy(x<0>)),
            lazy(f(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(lazy(f), lazy(x<0>), lazy(x<1>)),
            lazy(f(x<0>, x<1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(lazy(f), lazy(x<0>), lazy(x<1>), lazy(x<2>)),
            lazy(f(x<0>, x<1>, x<2>))
        ));

        // The function is not applied.
        ap(lazy(invalid), lazy(x<0>));
        ap(lazy(invalid), lazy(x<0>), lazy(x<1>));
        ap(lazy(invalid), lazy(x<0>), lazy(x<1>), lazy(x<2>));
    }

    // lift
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(lift<Lazy>(x<0>), lazy(x<0>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(lift<Lazy>(x<1>), lazy(x<1>)));
    }
}

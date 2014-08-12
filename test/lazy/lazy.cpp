/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <test/lazy_comparable.hpp>
using namespace boost::hana;


int main() {
    auto f = test::injection([]{});
    auto invalid = [](auto x) { return x.this_function_must_not_be_instantiated; };
    using test::x;

    // lazy
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(lazy(f)(), lazy(f())));
        BOOST_HANA_CONSTANT_ASSERT(equal(lazy(f)(x<0>), lazy(f(x<0>))));
        BOOST_HANA_CONSTANT_ASSERT(equal(lazy(f)(x<0>, x<1>), lazy(f(x<0>, x<1>))));
        BOOST_HANA_CONSTANT_ASSERT(equal(lazy(f)(x<0>, x<1>, x<2>), lazy(f(x<0>, x<1>, x<2>))));

        // The function is not applied.
        lazy(invalid)();
        lazy(invalid)(x<0>);
        lazy(invalid)(x<0>, x<1>);
        lazy(invalid)(x<0>, x<1>, x<2>);
    }

    // eval
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(eval(lazy(x<0>)), x<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(eval(lazy(x<1>)), x<1>));
    }
}

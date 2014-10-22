/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <test/laws/functor.hpp>
using namespace boost::hana;


int main() {
    auto f = test::injection([]{});
    auto x = test::x<0>;
    auto y = test::x<1>;
    constexpr struct { } undefined{};

    // fmap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(left(x), undefined), left(x)));
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(right(x), f), right(f(x))));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Functor_laws(
            left(x), left(y),
            right(x), right(y)
        ));
    }
}

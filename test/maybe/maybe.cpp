/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/functional/always.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    auto f = test::injection([]{});
    using test::x;
    constexpr struct { } undefined{};

    // maybe
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(maybe(x<0>, undefined, nothing), x<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(maybe(undefined, f, just(x<0>)), f(x<0>)));
    }

    // is_nothing
    {
        BOOST_HANA_CONSTANT_ASSERT(is_nothing(nothing));
        BOOST_HANA_CONSTANT_ASSERT(not_(is_nothing(just(undefined))));
    }

    // is_just
    {
        BOOST_HANA_CONSTANT_ASSERT(is_just(just(undefined)));
        BOOST_HANA_CONSTANT_ASSERT(not_(is_just(nothing)));
    }

    // from_just
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(from_just(just(x<0>)), x<0>));
        // from_just(nothing);
    }

    // from_maybe
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(from_maybe(x<0>, nothing), x<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(from_maybe(undefined, just(x<1>)), x<1>));
    }

    // only_when
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            only_when(always(true_), f, x<0>),
            just(f(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            only_when(always(false_), f, x<0>),
            nothing
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            only_when(always(false_), undefined, x<0>),
            nothing
        ));
    }
}

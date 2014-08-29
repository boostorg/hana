/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable/mcd.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/integral.hpp>

#include <test/injection.hpp>
#include <test/laws/iterable.hpp>
#include <test/seq/comparable.hpp>
#include <test/seq/iterable.hpp>

#include <vector>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto iterable = test::seq;
    constexpr struct { } undefined{};
    using test::x;

    // at
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(size_t<0>, iterable(x<0>)),
            x<0>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(size_t<0>, iterable(x<0>, undefined)),
            x<0>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(size_t<1>, iterable(undefined, x<1>)),
            x<1>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(size_t<0>, iterable(x<0>, undefined, undefined)),
            x<0>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(size_t<1>, iterable(undefined, x<1>, undefined)),
            x<1>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(size_t<2>, iterable(undefined, undefined, x<2>)),
            x<2>
        ));
    }

    // drop
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(size_t<0>, iterable()),
            iterable()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(size_t<1>, iterable()),
            iterable()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(size_t<2>, iterable()),
            iterable()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(size_t<0>, iterable(x<0>)),
            iterable(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(size_t<1>, iterable(x<0>)),
            iterable()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(size_t<2>, iterable(x<0>)),
            iterable()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(size_t<0>, iterable(x<0>, x<1>)),
            iterable(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(size_t<1>, iterable(x<0>, x<1>)),
            iterable(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop(size_t<2>, iterable(x<0>, x<1>)),
            iterable()
        ));
    }

    // drop_until
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(iterable(), id),
            iterable()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(iterable(true_), id),
            iterable(true_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(iterable(false_), id),
            iterable()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(iterable(true_, true_), id),
            iterable(true_, true_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(iterable(true_, false_), id),
            iterable(true_, false_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(iterable(false_, true_), id),
            iterable(true_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(iterable(false_, false_), id),
            iterable()
        ));
    }

    // drop_while
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(iterable(), id),
            iterable()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(iterable(true_), id),
            iterable()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(iterable(false_), id),
            iterable(false_)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(iterable(true_, true_), id),
            iterable()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(iterable(true_, false_), id),
            iterable(false_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(iterable(false_, true_), id),
            iterable(false_, true_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(iterable(false_, false_), id),
            iterable(false_, false_)
        ));
    }

    // last
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(last(iterable(x<0>)), x<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(last(iterable(undefined, x<1>)), x<1>));
        BOOST_HANA_CONSTANT_ASSERT(equal(last(iterable(undefined, undefined, x<2>)), x<2>));
    }
}

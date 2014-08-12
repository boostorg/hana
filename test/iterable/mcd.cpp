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
            drop_until(id, iterable()),
            iterable()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(id, iterable(true_)),
            iterable(true_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(id, iterable(false_)),
            iterable()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(id, iterable(true_, true_)),
            iterable(true_, true_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(id, iterable(true_, false_)),
            iterable(true_, false_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(id, iterable(false_, true_)),
            iterable(true_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_until(id, iterable(false_, false_)),
            iterable()
        ));
    }

    // drop_while
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(id, iterable()),
            iterable()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(id, iterable(true_)),
            iterable()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(id, iterable(false_)),
            iterable(false_)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(id, iterable(true_, true_)),
            iterable()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(id, iterable(true_, false_)),
            iterable(false_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(id, iterable(false_, true_)),
            iterable(false_, true_)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            drop_while(id, iterable(false_, false_)),
            iterable(false_, false_)
        ));
    }

    // for_each
    {
        auto check = [=](auto ...xs) {
            std::vector<int> seen{};
            for_each(iterable(xs...), [&](int x) {
                seen.push_back(x);
            });
            BOOST_HANA_RUNTIME_ASSERT(seen == std::vector<int>{xs...});
        };
        check();
        check(0);
        check(0, 1);
        check(0, 1, 2);
        check(0, 1, 2, 3);
        check(0, 1, 2, 3, 4);
    }

    // last
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(last(iterable(x<0>)), x<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(last(iterable(undefined, x<1>)), x<1>));
        BOOST_HANA_CONSTANT_ASSERT(equal(last(iterable(undefined, undefined, x<2>)), x<2>));
    }
}

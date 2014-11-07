/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_ENUMERABLE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_ENUMERABLE_HPP

#include <boost/hana/enumerable.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename E>
    auto laws<Enumerable, E> = [] {
        for_each(objects<E>, [](auto x) {
            BOOST_HANA_ASSERT(equal(
                succ(pred(x)),
                x
            ));

            BOOST_HANA_ASSERT(equal(
                pred(succ(x)),
                x
            ));
        });
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_ENUMERABLE_HPP

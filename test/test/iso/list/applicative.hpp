/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_LIST_APPLICATIVE_HPP
#define BOOST_HANA_TEST_TEST_ISO_LIST_APPLICATIVE_HPP

#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/list.hpp>

#include <test/injection.hpp>


template <typename L>
void List_applicative() {
    using namespace boost::hana;

    BOOST_HANA_CONSTEXPR_LAMBDA auto list = make<L>;
    using test::x;

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = test::injection([]{});

    // ap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(), list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(), list(x<0>)),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(), list(x<0>, x<1>)),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(), list(x<0>, x<1>, x<2>)),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(f), list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(f), list(x<0>)),
            list(f(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(f), list(x<0>, x<1>)),
            list(f(x<0>), f(x<1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(f), list(x<0>, x<1>, x<2>)),
            list(f(x<0>), f(x<1>), f(x<2>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(f, g), list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(f, g), list(x<0>)),
            list(f(x<0>), g(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(f, g), list(x<0>, x<1>)),
            list(f(x<0>), f(x<1>), g(x<0>), g(x<1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(list(f, g), list(x<0>, x<1>, x<2>)),
            list(f(x<0>), f(x<1>), f(x<2>), g(x<0>), g(x<1>), g(x<2>))
        ));
    }

    // lift
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(lift<L>(x<0>), list(x<0>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(lift<L>(x<1>), list(x<1>)));
    }
}

#endif // !BOOST_HANA_TEST_TEST_ISO_LIST_APPLICATIVE_HPP

/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_LIST_MONAD_HPP
#define BOOST_HANA_TEST_TEST_ISO_LIST_MONAD_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/monad/monad.hpp>
#include <boost/hana/list/list.hpp>

#include <test/injection.hpp>
#include <test/laws/monad.hpp>


template <typename L>
void List_monad() {
    using namespace boost::hana;

    BOOST_HANA_CONSTEXPR_LAMBDA auto list = make<L>;
    using test::x;

    // flatten
    {
        BOOST_HANA_CONSTANT_ASSERT(
            equal(flatten(list(list(), list())),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(
            equal(flatten(list(list(x<0>), list())),
            list(x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(
            equal(flatten(list(list(), list(x<0>))),
            list(x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(
            equal(flatten(list(list(x<0>), list(x<1>))),
            list(x<0>, x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            flatten(list(list(x<0>, x<1>), list(), list(x<2>, x<3>), list(x<4>))),
            list(x<0>, x<1>, x<2>, x<3>, x<4>)
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Monad_laws<L>
            // monads
            (
                list(),
                list(x<0>),
                list(x<0>, x<1>),
                list(x<0>, x<1>, x<2>)
            )

            // objects
            (
                x<0>, x<1>, x<2>, x<3>
            )
        );
    }
}

#endif // !BOOST_HANA_TEST_TEST_ISO_LIST_MONAD_HPP

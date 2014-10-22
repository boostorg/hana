/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_LIST_FUNCTOR_HPP
#define BOOST_HANA_TEST_TEST_ISO_LIST_FUNCTOR_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/list.hpp>

#include <test/injection.hpp>
#include <test/laws/functor.hpp>


template <typename L>
void List_functor() {
    using namespace boost::hana;

    BOOST_HANA_CONSTEXPR_LAMBDA auto list = make<L>;
    using test::x;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    // fmap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            fmap(list(), f),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            fmap(list(x<1>), f),
            list(f(x<1>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            fmap(list(x<1>, x<2>), f),
            list(f(x<1>), f(x<2>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            fmap(list(x<1>, x<2>, x<3>), f),
            list(f(x<1>), f(x<2>), f(x<3>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            fmap(list(x<1>, x<2>, x<3>, x<4>), f),
            list(f(x<1>), f(x<2>), f(x<3>), f(x<4>))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Functor_laws(
            list(),
            list(x<0>),
            list(x<0>, x<1>),
            list(x<0>, x<1>, x<2>)
        ));
    }
}

#endif // !BOOST_HANA_TEST_TEST_ISO_LIST_FUNCTOR_HPP

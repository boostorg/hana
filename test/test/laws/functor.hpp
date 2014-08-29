/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_FUNCTOR_HPP
#define BOOST_HANA_TEST_TEST_LAWS_FUNCTOR_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/logical/logical.hpp>

#include <test/injection.hpp>


template <typename ...Functors>
constexpr auto Functor_laws(Functors ...x) {
    using namespace boost::hana;
    auto f = test::injection([]{});
    auto g = test::injection([]{});

    return and_(
        and_(
            equal(fmap(x, id), x),
            equal(
                fmap(x, compose(f, g)),
                fmap(fmap(x, g), f)
            )
        )...
    );
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_FUNCTOR_HPP

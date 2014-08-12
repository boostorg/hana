/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_MONAD_HPP
#define BOOST_HANA_TEST_TEST_LAWS_MONAD_HPP

#include <boost/hana/applicative/applicative.hpp>
#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monad/monad.hpp>

#include <test/injection.hpp>


template <typename M, typename ...Monads>
constexpr auto Monad_laws(Monads ...m) {
    using namespace boost::hana;
    auto f = compose(lift<M>, test::injection([]{}));
    auto g = compose(lift<M>, test::injection([]{}));

    auto check = [=](auto x) {
        return and_(
            and_(
                equal(bind(lift<M>(x), f), f(x)),
                equal(bind(m, lift<M>), m),
                equal(
                    bind(m, [=](auto x) { return bind(f(x), g); }),
                    bind(bind(m, f), g)
                ),
                equal(fmap(f, m), bind(m, compose(lift<M>, f)))
            )...
        );
    };

    return [=](auto ...x) { return and_(check(x)...); };
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_MONAD_HPP
